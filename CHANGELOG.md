# Changelog

Format mengikuti [Keep a Changelog](https://keepachangelog.com/id/1.1.0/).

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
