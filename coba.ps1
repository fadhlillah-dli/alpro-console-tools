# Mencoba alpro-console-tools sekali jalan, tanpa memasang apa pun (Windows PowerShell 5.1 atau PowerShell 7).
#   irm https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/coba.ps1 | iex
# Mengunduh paket rilis Windows terbaru ke folder sementara, memverifikasi SHA-256, menjalankan program, lalu menghapus
# folder itu begitu program ditutup (juga bila dihentikan dengan Ctrl+C). Tidak ada berkas yang tertinggal, PATH dan
# registri tidak disentuh, dan tidak butuh hak Administrator.
# Menggunakan blok & { ... } berfungsi untuk menjalankan seluruh skrip dalam lingkup sendiri, dikarenakan `iex` menjalankan
# teks ini di sesi pengguna dan variabel atau $ErrorActionPreference tidak boleh tertinggal di sana.
& {
    $ErrorActionPreference = 'Stop'
    $ProgressPreference    = 'SilentlyContinue'  # bilah progres Invoke-WebRequest di PowerShell 5.1 memperlambat unduhan
    # Kode keluar program (termasuk Ctrl+C) tidak boleh menjadi error PowerShell, dikarenakan profil pengguna di PowerShell
    # 7.4+ bisa menyalakan $PSNativeCommandUseErrorActionPreference; nilainya dibatasi ke lingkup blok ini saja.
    $PSNativeCommandUseErrorActionPreference = $false

    $repo  = 'fadhlillah-dli/alpro-console-tools'
    $agent = @{ 'User-Agent' = 'alpro-console-tools-coba' }

    function Say($text) { Write-Host $text }
    function Fail($text) { throw "coba.ps1: $text" }

    # Arsitektur dibaca dari nilai mesin (HKLM), dikarenakan proses x86/x64 yang diemulasi di Windows Arm hanya melihat
    # nilai prosesnya sendiri. Paket rilis hanya x86_64: Windows 11 Arm (build 22000 ke atas) menjalankannya lewat
    # emulasi, sedangkan Windows 10 Arm dan Windows 32-bit tidak bisa.
    $arch  = [Environment]::GetEnvironmentVariable('PROCESSOR_ARCHITECTURE', 'Machine')
    $build = [int](Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion').CurrentBuildNumber
    if (-not ($arch -eq 'AMD64' -or ($arch -eq 'ARM64' -and $build -ge 22000))) {
        Fail "paket Windows hanya tersedia untuk x86_64 (atau Windows 11 Arm lewat emulasi); komputer ini: $arch, build $build. Kompilasi sendiri: lihat README."
    }
    if ($PSVersionTable.PSVersion.Major -lt 6) {
        [Net.ServicePointManager]::SecurityProtocol = [Net.ServicePointManager]::SecurityProtocol -bor [Net.SecurityProtocolType]::Tls12
    }

    Say "Mencari rilis terbaru $repo ..."
    $version = $null
    $reason  = $null
    try {
        $request = [System.Net.HttpWebRequest]::Create("https://github.com/$repo/releases/latest")
        $request.AllowAutoRedirect = $false
        $request.UserAgent = 'alpro-console-tools-coba'
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

    $tmp = Join-Path ([IO.Path]::GetTempPath()) ('alpro-coba-' + [IO.Path]::GetRandomFileName())
    # Blok try/finally berfungsi agar folder sementara selalu dihapus, dikarenakan Ctrl+C maupun kegagalan apa pun tetap
    # menjalankan finally, dan tujuan skrip ini adalah tidak menyisakan berkas di komputer pengguna. Folder dibuat di dalam
    # try, dikarenakan Ctrl+C di antara pembuatan folder dan awal try akan meninggalkan folder kosong.
    $savedInputEncoding = [Console]::InputEncoding; $savedOutputEncoding = [Console]::OutputEncoding
    $exitCode = 0
    try {
        New-Item -ItemType Directory -Path $tmp | Out-Null
        Say "Mengunduh $asset ..."
        $zip = Join-Path $tmp $asset
        Invoke-WebRequest -Uri "$base/$asset" -OutFile $zip -Headers $agent -UseBasicParsing

        # SHA256SUMS wajib ada, dikarenakan skrip ini selalu mengambil rilis terbaru dan setiap rilis sejak v1.0.1 menyertakannya.
        $sums = Join-Path $tmp 'SHA256SUMS'
        Invoke-WebRequest -Uri "$base/SHA256SUMS" -OutFile $sums -Headers $agent -UseBasicParsing
        $expected = Get-Content -LiteralPath $sums | Where-Object { $_ -match ('\s' + [regex]::Escape($asset) + '\s*$') } |
                    ForEach-Object { ($_ -split '\s+')[0].ToLowerInvariant() } | Select-Object -First 1
        $hasher = [System.Security.Cryptography.SHA256]::Create()
        try {
            $stream = [IO.File]::OpenRead($zip)
            try { $actual = ([BitConverter]::ToString($hasher.ComputeHash($stream)) -replace '-', '').ToLowerInvariant() }
            finally { $stream.Dispose() }
        } finally { $hasher.Dispose() }
        if (-not $expected -or $expected -ne $actual) { Fail 'SHA-256 tidak cocok; unduhan rusak atau diubah. Dibatalkan.' }
        Say 'SHA-256 cocok.'

        # Menggunakan kelas ZipFile .NET berfungsi untuk mengambil hanya tugas_alpro.exe dari paket, dikarenakan
        # Expand-Archive adalah modul skrip yang tidak bisa dimuat bila execution policy Restricted (bawaan Windows).
        Add-Type -AssemblyName System.IO.Compression.FileSystem
        $exe = Join-Path $tmp 'tugas_alpro.exe'
        $archive = [IO.Compression.ZipFile]::OpenRead($zip)
        try {
            $entry = $archive.GetEntry('tugas_alpro.exe')
            if (-not $entry) { Fail 'berkas tugas_alpro.exe tidak ada di dalam paket' }
            [IO.Compression.ZipFileExtensions]::ExtractToFile($entry, $exe, $true)
        } finally { $archive.Dispose() }
        Unblock-File -LiteralPath $exe -ErrorAction SilentlyContinue

        Say "Menjalankan tugas_alpro ($version) dari folder sementara; folder dihapus saat program ditutup."
        Say ''
        if ($env:ALPRO_INPUT) {
            # Mode terskrip (dipakai CI): masukan dari berkas, keluaran diteruskan ke layar lewat berkas sementara.
            $outFile = Join-Path $tmp 'keluaran.txt'
            $proc = Start-Process -FilePath $exe -RedirectStandardInput $env:ALPRO_INPUT -RedirectStandardOutput $outFile -NoNewWindow -Wait -PassThru
            $exitCode = $proc.ExitCode
            if (Test-Path -LiteralPath $outFile) { Write-Host ([IO.File]::ReadAllText($outFile)) }
        } else {
            & $exe
            $exitCode = $LASTEXITCODE
        }
    } finally {
        # Program mengubah code page konsol ke UTF-8; jendela PowerShell pengguna dipulihkan seperti semula.
        try { [Console]::InputEncoding = $savedInputEncoding; [Console]::OutputEncoding = $savedOutputEncoding } catch { }
        Remove-Item -LiteralPath $tmp -Recurse -Force -ErrorAction SilentlyContinue
        if (Test-Path -LiteralPath $tmp) { Write-Warning "coba.ps1: folder sementara $tmp tidak bisa dihapus; hapus manual." }
    }
    Say ''
    if (Test-Path -LiteralPath $tmp) { Say "Selesai (kode keluar $exitCode); folder sementara masih ada, lihat peringatan di atas." }
    else { Say "Selesai (kode keluar $exitCode); folder sementara dihapus. Tidak ada yang terpasang." }
}
