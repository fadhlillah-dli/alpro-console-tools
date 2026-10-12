# Changelog

Format mengikuti [Keep a Changelog](https://keepachangelog.com/id/1.1.0/).

## [Unreleased]

### Ditambahkan
- `coba.sh` dan `coba.ps1`: mencoba program sekali jalan tanpa memasang apa pun (unduh ke folder sementara, verifikasi
  SHA-256, jalankan, hapus foldernya saat program ditutup atau dihentikan); diuji otomatis di CI ketiga OS.
- README: cara menghapus pemasangan di macOS/Linux, Homebrew, dan Windows (termasuk entri PATH).

## [1.0.4] - 2026-10-12

### Diperbaiki
- Paket Linux ditautkan statis penuh (termasuk glibc) agar jalan di distro dengan glibc lebih lama dari runner rilis
  (Ubuntu 22.04, Debian 12, WSL bawaan); sebelumnya meminta `GLIBC_2.38`. CI Linux kini juga menguji biner statis.

## [1.0.3] - 2026-10-12

### Ditambahkan
- `install.ps1`: pemasang satu perintah untuk Windows lewat PowerShell (`irm … | iex`, atau `powershell -c "…"` dari
  Command Prompt), dengan verifikasi SHA-256, tanpa hak Administrator dan tanpa layar SmartScreen; diuji otomatis di CI
  pada Windows PowerShell 5.1 dan PowerShell 7.
- Uji asap CI juga dijalankan di Windows.
- `CLAUDE.md` (aturan kerja, perintah build dan uji asap) dan `.claude/settings.json` (tanpa atribusi AI pada commit/PR).

### Diubah
- Di Windows, program mengatur konsol ke UTF-8 (code page 65001) sendiri saat dimulai, sehingga simbol π, ², ³, garis
  kotak, dan sketsa Braille tampil benar tanpa `chcp 65001` manual.

### Diperbaiki
- Langkah formula Homebrew di workflow rilis: `pipefail` agar unduhan tarball yang gagal menghentikan langkah, `desc`
  dipendekkan ke batas 80 karakter `brew audit`, `brew test` memeriksa exit code, dan secret yang belum ada atau tap
  yang tidak bisa diakses dicatat sebagai peringatan pada ringkasan run.

## [1.0.2] - 2026-10-12

### Ditambahkan
- `install.sh`: pemasang satu perintah untuk macOS/Linux lewat curl (tanpa dialog Gatekeeper), dengan verifikasi SHA-256.
- Formula Homebrew (`brew install fadhlillah-dli/tap/alpro-console-tools`) di repositori `fadhlillah-dli/homebrew-tap`;
  untuk 1.0.2 diterbitkan manual, rilis berikutnya memperbaruinya otomatis lewat workflow bila secret `HOMEBREW_TAP_TOKEN` tersedia.
- Berkas `SHA256SUMS` pada setiap rilis.

### Diperbaiki
- Peluncur Windows melepas Mark of the Web dari `tugas_alpro.exe` (Unblock-File) agar SmartScreen hanya bertanya
  sekali, pada peluncurnya.

## [1.0.1] - 2026-10-12

### Diperbaiki
- Build Windows gagal di CI: `NOMINMAX` sudah didefinisikan MinGW, kini dijaga `#ifndef`.
- Peluncur macOS melepas tanda karantina dari `tugas_alpro` sehingga paket unduhan bisa dijalankan lewat
  klik kanan → Open tanpa membuka System Settings.
- Workflow rilis memakai satu job publish agar catatan rilis tidak tergandakan.
- README: bagian unduh siap pakai dan badge CI.

## [1.0.0] - 2026-10-12

### Ditambahkan
- Program bermenu yang menggabungkan tiga tugas Modul 1 (Input dan Output).
- Tools Nilai Mahasiswa: jumlah mahasiswa bebas, nama lengkap tervalidasi, nilai desimal (titik/koma),
  rekap dalam kotak dengan total dan rata-rata.
- Volume Tabung: pilihan satuan (mm/cm/dm/m), nilai π (3.14 atau 22/7), jari-jari atau diameter, padanan liter.
- Luas Bangun yang Diarsir: pilihan satuan dan nilai π, hasil persegi − lingkaran.
- Sketsa Braille terhitung untuk tabung dan persegi–lingkaran; tata letak menyesuaikan lebar terminal.
- Validasi ketat semua input (termasuk pemeriksaan UTF-8 pada nama); pesan kesalahan berwarna; task VS Code,
  Makefile, peluncur macOS/Windows; CI dan rilis biner lewat GitHub Actions.
