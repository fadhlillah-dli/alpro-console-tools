# Changelog

Format mengikuti [Keep a Changelog](https://keepachangelog.com/id/1.1.0/).

## [Unreleased]

### Diperbaiki
- Tampilan di terminal sempit, misalnya Termux pada ponsel tegak (sekitar 46 kolom): lebar minimal kini 40 kolom
  (sebelumnya 50), dan tidak ada lagi baris buatan program yang melebihi lebar terminal. Garis judul mengikuti lebar
  terminal; judul program, judul kotak banner, baris rumus, sub-judul "Input Nilai", dan baris identitas program mandiri
  dilipat per kata; kanvas sketsa menyisakan ruang untuk panah "S" (program 3) dan catatan "T" (program 2).
- Tabel rekap program 1 tidak lagi pecah bila nama mahasiswa lebih panjang dari ruang yang tersedia (untuk nama terpanjang,
  60 karakter, terjadi di bawah 80 kolom): kolom nama dibatasi lebar kotak dan nama panjang dilipat di dalam kolomnya.
- Pembungkus baris: angka hasil yang sangat panjang setelah label pindah ke baris berikutnya, kata yang lebih panjang dari
  satu baris dipotong di batas lebar, dan lanjutan baris yang sudah menjorok (contoh di kotak petunjuk, keterangan menu)
  kini lurus di bawah barisnya, juga di terminal sampai 100 kolom.
- Diuji di pseudo-terminal 40-200 kolom untuk ketiga program (menu dan mandiri, termasuk nama dan angka panjang): tidak ada
  baris yang melebihi lebar terminal, uji asap tetap persis, dan di terminal 101 kolom ke atas tampilan sama dengan v1.0.7.
  Paket android v1.0.7 sudah dicoba pemilik di perangkat Termux dan berjalan; masalah yang terlihat hanya tata letak di atas.

## [1.0.7] - 2026-10-13

### Ditambahkan
- Paket Android aarch64 (Termux) dibangun dengan clang NDK (r30, clang 21) untuk bionic: PIE dinamis dengan interpreter
  `/system/bin/linker64`, libc++ statis, di-strip (paket sekitar 0,2 MB); `coba.sh` mendeteksi Termux (`uname -o` = Android
  atau `TERMUX_VERSION`) dan memilih paket itu. Paket glibc (`linux-*`) tidak bisa jalan di Termux ("has unexpected
  e_type: 2" pada v1.0.6): termux-exec memuatnya lewat linker Android dan seccomp aplikasi menjebak syscall mulai glibc.
  Diuji sebelum rilis di kontainer bionic (termux-docker Android 9; Android 14 lewat linker64 mode exec dengan filter
  seccomp aplikasi): ketiga uji asap lulus dan input salah ditolak; hasil uji di perangkat Termux dicatat di rilis berikutnya.
- Catatan rilis diambil dari bagian CHANGELOG versi itu; jumlah berkas (lima paket + SHA256SUMS) diperiksa sebelum rilis dibuat.

### Diubah
- Paket Linux (x86_64 dan aarch64) ditautkan `-static-pie` (tetap statis penuh, ditambah ASLR); workflow memeriksa tipe
  ELF DYN tanpa interpreter.

## [1.0.6] - 2026-10-13

### Ditambahkan
- Paket Linux aarch64 (ARM64, tautan statis penuh) dibangun di runner `ubuntu-24.04-arm`; `coba.sh` memilih paket dari
  `uname -m` (x86_64 atau aarch64/arm64) dan memberi pesan jelas bila rilis belum memuat paketnya; CI juga berjalan di
  runner ARM (build, uji asap, biner statis, coba.sh).
- Dry-run jalur rilis: `release.yml` bisa dijalankan lewat `workflow_dispatch` (membangun empat paket dan daftar SHA-256
  tanpa membuat rilis); runner Linux dipin ke `ubuntu-24.04` untuk kedua arsitektur.

### Diubah
- Komentar sumber mengikuti pola pemilik: kata kunci dijelaskan di atas kemunculan pertamanya, tiap baris kode diberi
  komentar ujung; konvensi di README dan CONTRIBUTING diperbarui.
- Komentar `coba.sh`, `coba.ps1`, dan `.github/dependabot.yml` diluruskan: `SHA256SUMS` ada sejak v1.0.2, dan PR Dependabot
  tidak di-merge melainkan diterapkan sebagai commit pemilik.

### Keamanan
- Pin action diperbarui dari PR #1 Dependabot: checkout v7.0.1, upload-artifact v7.0.2, download-artifact v8.0.2; jalur
  rilis dengan dua action terakhir baru teruji pada tag berikutnya.

## [1.0.5] - 2026-10-12

### Ditambahkan
- `coba.sh` dan `coba.ps1`: menjalankan program sekali jalan tanpa memasang apa pun (unduh ke folder sementara, verifikasi
  SHA-256, jalankan, hapus foldernya saat program ditutup atau dihentikan); diuji otomatis di CI ketiga OS.

### Diubah
- README: cara pakai utama kini satu perintah sekali jalan tanpa memasang; petunjuk pemasang satu perintah, Homebrew, dan
  paket zip dihapus dari README.
- Paket rilis kini hanya berisi program, README, dan LICENSE.

### Keamanan
- Workflow: token `GITHUB_TOKEN` hanya-baca di seluruh CI dan di job build rilis, izin tulis hanya pada job publish;
  `persist-credentials: false` pada checkout; semua action dipin ke commit SHA penuh dan diperbarui Dependabot;
  rilis dibuat dengan `gh release create` tanpa action pihak ketiga; cache toolchain MSYS2 dimatikan untuk build rilis;
  pemeriksaan bahwa sumber tidak berubah sejak checkout sebelum kompilasi.
- `SECURITY.md`: cara melaporkan masalah keamanan secara privat.

### Dihapus
- `install.sh`, `install.ps1`, peluncur paket zip (`dist/`), dan langkah formula Homebrew di workflow rilis, dikarenakan
  jalur pakai utama kini `coba.sh`/`coba.ps1` tanpa pemasangan.
- `CLAUDE.md` dan `.claude/` tidak lagi disertakan di repositori.

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
