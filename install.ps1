# Pemasang alpro-console-tools untuk Windows (Windows PowerShell 5.1 atau PowerShell 7).
#   irm https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/install.ps1 | iex
# Mengunduh paket rilis Windows terbaru dari GitHub, memverifikasi SHA-256, memasang tugas_alpro.exe ke
# %LOCALAPPDATA%\Programs\alpro-console-tools, melepas tanda unduhan (Mark of the Web) agar SmartScreen tidak bertanya,
# memastikan program bisa dijalankan, lalu menambahkan folder itu ke PATH pengguna. Tidak membutuhkan hak Administrator.
# Menggunakan blok & { ... } berfungsi untuk menjalankan seluruh skrip dalam lingkup sendiri, dikarenakan `iex` menjalankan
# teks ini di sesi pengguna dan variabel atau $ErrorActionPreference tidak boleh tertinggal di sana.
& {
    $ErrorActionPreference = 'Stop'
    $ProgressPreference    = 'SilentlyContinue'  # bilah progres Invoke-WebRequest di PowerShell 5.1 memperlambat unduhan

    $repo   = 'fadhlillah-dli/alpro-console-tools'
    $binDir = if ($env:ALPRO_BIN_DIR) { $env:ALPRO_BIN_DIR } else { Join-Path $env:LOCALAPPDATA 'Programs\alpro-console-tools' }
    $agent  = @{ 'User-Agent' = 'alpro-console-tools-install' }

    function Say($text) { Write-Host $text }
    function Fail($text) { throw "install.ps1: $text" }

    # Arsitektur dibaca dari nilai mesin (HKLM), dikarenakan proses x86/x64 yang diemulasi di Windows Arm hanya melihat
    # nilai prosesnya sendiri. Paket rilis hanya x86_64: Windows 11 Arm (build 22000 ke atas) menjalankannya lewat
    # emulasi, sedangkan Windows 10 Arm dan Windows 32-bit tidak bisa.
    $arch  = [Environment]::GetEnvironmentVariable('PROCESSOR_ARCHITECTURE', 'Machine')
    $build = [int](Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion').CurrentBuildNumber
    if (-not ($arch -eq 'AMD64' -or ($arch -eq 'ARM64' -and $build -ge 22000))) {
        Fail "paket Windows hanya tersedia untuk x86_64 (atau Windows 11 Arm lewat emulasi); komputer ini: $arch, build $build. Kompilasi sendiri: lihat README."
    }
    # TLS 1.2 berguna bila skrip ini dijalankan dari berkas tersimpan pada Windows PowerShell 5.1 di sistem lama; pada
    # pemakaian `irm | iex` unduhan skrip ini sendiri sudah berhasil, sehingga baris ini tinggal pengaman.
    if ($PSVersionTable.PSVersion.Major -lt 6) {
        [Net.ServicePointManager]::SecurityProtocol = [Net.ServicePointManager]::SecurityProtocol -bor [Net.SecurityProtocolType]::Tls12
    }

    Say "Mencari rilis terbaru $repo ..."
    # Versi terbaru dibaca dari pengalihan halaman releases/latest (tanpa API, sehingga tidak kena batas permintaan
    # API GitHub); API dipakai sebagai cadangan, dan penyebab kegagalannya ikut dilaporkan.
    $version = $null
    $reason  = $null
    try {
        $request = [System.Net.HttpWebRequest]::Create("https://github.com/$repo/releases/latest")
        $request.AllowAutoRedirect = $false
        $request.UserAgent = 'alpro-console-tools-install'
        $response = $request.GetResponse()
        $location = $response.Headers['Location']
        $response.Close()
        if ($location -match '/tag/([^/?#]+)') { $version = $Matches[1] }
    } catch { $reason = $_.Exception.Message }
    if (-not $version) {
        try {
            $version = (Invoke-RestMethod -Uri "https://api.github.com/repos/$repo/releases/latest" -Headers $agent -UseBasicParsing).tag_name
        } catch { $version = $null; $reason = $_.Exception.Message }
    }
    if (-not $version) {
        $why = if ($reason) { " Penyebab: $reason" } else { '' }
        Fail "tidak bisa membaca versi rilis terbaru (periksa koneksi internet, atau coba lagi nanti).$why"
    }
    $asset = "alpro-console-tools-$version-windows-x86_64.zip"
    $base  = "https://github.com/$repo/releases/download/$version"

    $tmp = Join-Path ([IO.Path]::GetTempPath()) ('alpro-install-' + [IO.Path]::GetRandomFileName())
    New-Item -ItemType Directory -Path $tmp | Out-Null
    try {
        Say "Mengunduh $asset ..."
        $zip = Join-Path $tmp $asset
        Invoke-WebRequest -Uri "$base/$asset" -OutFile $zip -Headers $agent -UseBasicParsing

        # SHA256SUMS wajib ada, dikarenakan pemasang ini selalu mengambil rilis terbaru dan setiap rilis sejak v1.0.1
        # menyertakannya; unduhan yang gagal karena jaringan pun dengan demikian tidak disalahartikan sebagai "tidak ada".
        $sums = Join-Path $tmp 'SHA256SUMS'
        Invoke-WebRequest -Uri "$base/SHA256SUMS" -OutFile $sums -Headers $agent -UseBasicParsing
        $expected = Get-Content -LiteralPath $sums | Where-Object { $_ -match ('\s' + [regex]::Escape($asset) + '\s*$') } |
                    ForEach-Object { ($_ -split '\s+')[0].ToLowerInvariant() } | Select-Object -First 1
        # Menggunakan kelas SHA256 .NET berfungsi untuk menghitung hash tanpa modul, dikarenakan Get-FileHash pada Windows
        # PowerShell 5.1 adalah fungsi modul yang gagal dimuat bila PSModulePath mewarisi jalur PowerShell 7.
        $hasher = [System.Security.Cryptography.SHA256]::Create()
        try {
            $stream = [IO.File]::OpenRead($zip)
            try { $actual = ([BitConverter]::ToString($hasher.ComputeHash($stream)) -replace '-', '').ToLowerInvariant() }
            finally { $stream.Dispose() }
        } finally { $hasher.Dispose() }
        if (-not $expected -or $expected -ne $actual) { Fail 'SHA-256 tidak cocok; unduhan rusak atau diubah. Dibatalkan.' }
        Say 'SHA-256 cocok.'

        Say 'Mengekstrak ...'
        # Menggunakan kelas ZipFile .NET berfungsi untuk mengambil hanya tugas_alpro.exe dari paket, dikarenakan
        # Expand-Archive adalah modul skrip yang tidak bisa dimuat bila execution policy Restricted (bawaan Windows),
        # sedangkan kelas .NET selalu tersedia di Windows PowerShell 5.1 maupun PowerShell 7.
        Add-Type -AssemblyName System.IO.Compression.FileSystem
        $exe = Join-Path $tmp 'tugas_alpro.exe'
        $archive = [IO.Compression.ZipFile]::OpenRead($zip)
        try {
            $entry = $archive.GetEntry('tugas_alpro.exe')
            if (-not $entry) { Fail 'berkas tugas_alpro.exe tidak ada di dalam paket' }
            [IO.Compression.ZipFileExtensions]::ExtractToFile($entry, $exe, $true)
        } finally { $archive.Dispose() }

        New-Item -ItemType Directory -Path $binDir -Force | Out-Null
        $target = Join-Path $binDir 'tugas_alpro.exe'
        # Sisa pemasangan sebelumnya (program lama yang masih berjalan waktu itu) dibersihkan sebisanya.
        Get-ChildItem -LiteralPath $binDir -Filter 'tugas_alpro.exe.old-*' -ErrorAction SilentlyContinue |
            Remove-Item -Force -ErrorAction SilentlyContinue
        # Menggunakan ganti nama (bukan timpa langsung) berfungsi agar pemasangan ulang tetap jalan saat tugas_alpro lama
        # masih terbuka, dikarenakan Windows menolak menimpa .exe yang sedang berjalan tetapi mengizinkan mengganti namanya.
        if (Test-Path -LiteralPath $target) {
            $old = $target + '.old-' + [IO.Path]::GetRandomFileName()
            Move-Item -LiteralPath $target -Destination $old -Force
            Remove-Item -LiteralPath $old -Force -ErrorAction SilentlyContinue
        }
        Copy-Item -LiteralPath $exe -Destination $target -Force
        # Menggunakan Unblock-File berfungsi untuk melepas Mark of the Web yang mungkin ikut dari berkas zip, dikarenakan
        # tanpa itu SmartScreen menolak program yang tidak ditandatangani Microsoft saat pertama dijalankan.
        Unblock-File -LiteralPath $target -ErrorAction SilentlyContinue

        # Uji jalan singkat (pilihan menu 0 = keluar), dikarenakan Smart App Control (Windows 11) atau antivirus bisa
        # memblokir program yang tidak ditandatangani walau berkasnya sudah terpasang; lebih baik ketahuan di sini.
        # Menggunakan Start-Process dengan masukan/keluaran dialihkan ke berkas berfungsi untuk mendapatkan kode keluar
        # dan pesan galat apa adanya, dikarenakan pipa PowerShell 5.1 mengubah keluaran stderr program menjadi error
        # PowerShell dan menyamarkan penyebab sebenarnya.
        # Encoding konsol disimpan lalu dipulihkan, dikarenakan program mengubah code page konsol ke UTF-8 dan jendela
        # PowerShell pengguna tidak boleh ikut berubah hanya karena uji ini.
        $probeIn  = Join-Path $tmp 'uji-masuk.txt'
        $probeOut = Join-Path $tmp 'uji-keluar.txt'
        $probeErr = Join-Path $tmp 'uji-galat.txt'
        [IO.File]::WriteAllText($probeIn, "0`r`n")
        $exitCode = $null
        $detail   = ''
        $savedInputEncoding = [Console]::InputEncoding; $savedOutputEncoding = [Console]::OutputEncoding
        try {
            $proc = Start-Process -FilePath $target -RedirectStandardInput $probeIn -RedirectStandardOutput $probeOut `
                                  -RedirectStandardError $probeErr -NoNewWindow -Wait -PassThru
            $exitCode = $proc.ExitCode
            if (Test-Path -LiteralPath $probeErr) { $detail = ([IO.File]::ReadAllText($probeErr)).Trim() }
        } catch { $detail = $_.Exception.Message }
        finally {
            # Pemulihan dibungkus try/catch dikarenakan tanpa konsol (mis. di CI) setter ini bisa gagal, dan itu bukan kesalahan pemasangan.
            try { [Console]::InputEncoding = $savedInputEncoding; [Console]::OutputEncoding = $savedOutputEncoding } catch { }
        }
        if ($exitCode -ne 0) {
            Fail ("program sudah disalin ke $target tetapi tidak bisa dijalankan di komputer ini (kode keluar: $exitCode). " +
                  'Kemungkinan Smart App Control (Windows 11) atau antivirus memblokir program yang tidak ditandatangani; ' +
                  "izinkan berkas itu, atau kompilasi sendiri (lihat README). Detail: $detail")
        }
        Say "Terpasang: $target ($version)"
    } finally {
        Remove-Item -LiteralPath $tmp -Recurse -Force -ErrorAction SilentlyContinue
    }

    # PATH pengguna ditulis lewat registri secara mentah, dikarenakan [Environment]::SetEnvironmentVariable mengembangkan
    # setiap %VAR% pada entri lama dan mengubah tipe REG_EXPAND_SZ menjadi REG_SZ, sehingga entri seperti %JAVA_HOME%\bin
    # membeku ke nilai saat ini. Pembandingan memakai bentuk yang sudah dikembangkan, tanpa backslash akhir.
    $want   = $binDir.TrimEnd('\')
    $envKey = [Microsoft.Win32.Registry]::CurrentUser.CreateSubKey('Environment')
    try {
        $raw     = [string]$envKey.GetValue('Path', '', [Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames)
        $entries = @($raw -split ';' | Where-Object { $_ })
        $present = @($entries | ForEach-Object { [Environment]::ExpandEnvironmentVariables($_).TrimEnd('\') }) -contains $want
        if (-not $present) {
            $kind = [Microsoft.Win32.RegistryValueKind]::ExpandString
            if (($envKey.GetValueNames() -contains 'Path') -and ($envKey.GetValueKind('Path') -eq [Microsoft.Win32.RegistryValueKind]::String)) {
                $kind = [Microsoft.Win32.RegistryValueKind]::String
            }
            $envKey.SetValue('Path', (($entries + $binDir) -join ';'), $kind)
            # Menghapus variabel yang memang tidak ada lewat [Environment] berfungsi untuk menyiarkan WM_SETTINGCHANGE tanpa
            # meninggalkan jejak, dikarenakan penulisan registri langsung tidak memberi tahu Explorer dan terminal baru.
            # NullString dipakai dikarenakan $null biasa menjadi "" saat masuk ke parameter string .NET, dan PowerShell 7.5+
            # menyimpan "" sebagai variabel kosong alih-alih menghapusnya.
            [Environment]::SetEnvironmentVariable('ALPRO_PATH_REFRESH', [System.Management.Automation.Language.NullString]::Value, 'User')
            Say "Folder $binDir ditambahkan ke PATH pengguna."
        }
    } finally { $envKey.Close() }
    if (@($env:Path -split ';' | ForEach-Object { $_.TrimEnd('\') }) -notcontains $want) { $env:Path = "$env:Path;$binDir" }
    Say 'Jalankan dengan:  tugas_alpro   (bila perintah belum dikenal, buka jendela terminal baru)'
}
