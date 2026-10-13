# alpro-console-tools

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)](#menjalankan)
[![CI](../../actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)

Program konsol C++ untuk praktikum **Algoritma & Pemrograman (Modul 1: Input dan Output)**: rekap nilai
mahasiswa, volume tabung, dan luas bangun yang diarsir, dalam satu menu interaktif dengan validasi input
yang ketat, tampilan berwarna, dan sketsa bangun yang digambar langsung di terminal.

*C++17 console tools for an introductory programming lab (student grade recap, cylinder volume, shaded
area), with strict input validation, colored boxed UI, and computed Braille sketches. UI text is in Indonesian.*

## Demo

https://github.com/user-attachments/assets/cb81f1b3-6605-4465-a380-b10b1024aa75

```text
┌─ Menu Program ───────────────────────────────────────────────────────────────────────────────┐
│ [1] Tools Nilai Mahasiswa   Rekap Nama, Nilai, Total, dan Rata-Rata satu mata kuliah         │
│ [2] Volume Tabung           V = Pi x R x R x T; satuan, nilai Pi, jari-jari/diameter dipilih │
│ [3] Luas Bangun Diarsir     Persegi dikurangi lingkaran di dalamnya; satuan dan Pi dipilih   │
│ [0] Keluar                  Menutup program                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
Ketik nomor menu lalu tekan Enter.
Pilihan Anda [0-3] : 2

┌─ Hasil Perhitungan ───────────────────────────────────┐
│ Pi (π) yang Digunakan : 22/7 (3.142857)               │
│ Diameter (D)          : 14.00 cm                      │
│ Jari-Jari (R) = D / 2 : 7.00 cm                       │
│ Tinggi (T)            : 10.00 cm                      │
│ Luas Alas             : 154.00 cm²                    │
│ Volume Tabung         : 1540.00 cm³                   │
│ Setara                : 1.540 liter (1 liter = 1 dm³) │
└───────────────────────────────────────────────────────┘
```

## Fitur

- **Tiga program dalam satu menu**, masing-masing juga bisa dikompilasi sebagai program mandiri.
- **Tools Nilai Mahasiswa**: nama mata kuliah dan jumlah mahasiswa bebas, nama lengkap tervalidasi
  (huruf termasuk non-ASCII seperti é/ö atau aksara lain, spasi, `. - '`), nilai 0–100 boleh desimal dengan
  titik atau koma, rekap total dan rata-rata dengan kolom yang tetap sejajar untuk aksara lebar.
- **Volume Tabung**: pilihan satuan (mm/cm/dm/m), nilai π 3.14 atau 22/7, input jari-jari atau diameter,
  hasil cm³ plus padanan liter.
- **Luas Bangun yang Diarsir**: persegi dikurangi lingkaran yang menyinggung keempat sisi
  (sisi 14 cm, π = 22/7 → 42 cm²).
- **Validasi ketat**: setiap input diperiksa dan diminta ulang ketika tidak sesuai dengan pesan yang
  menyebut tindakan yang benar.
- **Tampilan profesional**: kotak berjudul, warna ANSI (otomatis mati bila keluaran bukan terminal atau
  `NO_COLOR`), petunjuk pengisian, dan sketsa bangun dari karakter Braille yang dihitung dari rumus.
- **Menyesuaikan lebar terminal** setiap kali layar digambar: teks dibungkus, menu menjadi dua baris, sketsa
  mengecil, keterangan pindah ke bawah gambar bila jendela sempit.
- **Setiap baris berkomentar** (bahasa Indonesia) menjelaskan apa yang dipakai dan mengapa, cocok untuk belajar.

## Menjalankan

Satu perintah, tanpa memasang apa pun: paket rilis terbaru diunduh ke folder sementara, diverifikasi SHA-256, dijalankan,
lalu foldernya dihapus begitu program ditutup (juga bila dihentikan dengan Ctrl+C). Tidak perlu hak Administrator, tidak
ada dialog Gatekeeper atau SmartScreen, dan PATH tidak disentuh.

macOS dan Linux (x86_64 atau aarch64/ARM64, misalnya Raspberry Pi 64-bit dan server ARM; paket aarch64 ada sejak v1.0.6), di Terminal:

```bash
curl -fsSL https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/coba.sh | sh
```

Windows 10/11 (x64), di **PowerShell** (tekan tombol Windows, ketik `PowerShell`, Enter; bukan Command Prompt):

```powershell
irm https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/coba.ps1 | iex
```

Setiap kali dijalankan paket diunduh ulang (kurang dari 1 MB), jadi perlu koneksi internet. Di Windows, Command Prompt
klasik dengan font Consolas menampilkan sketsa Braille sebagai kotak kecil; Windows Terminal menampilkannya dengan benar.

## Kompilasi sendiri

Syarat: compiler C++17 (`g++` atau `clang++`).

- macOS: `xcode-select --install`
- Linux (Debian/Ubuntu): `sudo apt install build-essential`
- Windows: pasang [MSYS2](https://www.msys2.org/), lalu `pacman -S mingw-w64-ucrt-x86_64-gcc` dan tambahkan
  `C:\msys64\ucrt64\bin` ke PATH.

```bash
git clone https://github.com/fadhlillah-dli/alpro-console-tools.git
cd alpro-console-tools
g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
```

Alternatif:

| Cara | Perintah / langkah |
| --- | --- |
| Make | `make && ./tugas_alpro` — atau `make tugas1`, `make tugas2`, `make tugas3` untuk program mandiri |
| VS Code | Open Folder → Cmd/Ctrl+Shift+B (build) → Terminal → Run Task… → "Build & Run" |
| Klik dua kali | macOS: `jalankan.command` · Windows: `jalankan.bat` (mengompilasi lalu menjalankan) |

Satu berkas tugas sebagai program sendiri, tanpa menu:

```bash
g++ -std=c++17 -Wall -Wextra -o tugas1 src/tugas1_nilai_mahasiswa.cpp && ./tugas1
```

## Cara pakai

1. Pilih program dengan mengetik nomor menu lalu Enter.
2. Baca kotak **Petunjuk Pengisian** di awal tiap program; setiap pertanyaan pilihan menampilkan opsinya
   dalam bentuk `[1] … [2] …` dan nilai bawaan `[Enter = …]`.
3. Input yang tidak sesuai ditolak dengan pesan `[!]` dan diminta ulang; Ctrl+D (macOS/Linux) atau
   Ctrl+Z (Windows) menghentikan program dengan rapi.

Data uji dan hasil yang diharapkan:

| Program | Input | Hasil |
| --- | --- | --- |
| 1 | "Algoritma dan Pemrograman", 3 mahasiswa: A 80, B 60, C 70 | Nilai Total 210.00, Rata-Rata 70.00 |
| 1 | 4 mahasiswa: 87.5, 92,25, 70, 100 | Nilai Total 349.75, Rata-Rata 87.44 |
| 2 | cm, π 3.14, jari-jari 7, tinggi 10 | Luas Alas 153.86 cm², Volume 1538.60 cm³, 1.539 liter |
| 2 | cm, π 22/7, diameter 14, tinggi 10 | R = 7 cm, Volume 1540.00 cm³ |
| 3 | cm, π 22/7, sisi 14 | Luas Lingkaran 154.00 cm², Luas Arsir 42.00 cm² |

## Struktur proyek

```text
alpro-console-tools/
├── src/
│   ├── main.cpp                     menu utama (banner, menu, pembacaan pilihan, jeda)
│   ├── tugas1_nilai_mahasiswa.cpp   Program 1 — lengkap, bisa dikompilasi sendiri
│   ├── tugas2_volume_tabung.cpp     Program 2 — lengkap, bisa dikompilasi sendiri
│   └── tugas3_luas_arsir.cpp        Program 3 — lengkap, bisa dikompilasi sendiri
├── .github/workflows/               CI (build 3 OS, uji asap, uji coba.sh/coba.ps1) dan Release (paket biner saat tag v*)
├── .github/dependabot.yml           pembaruan mingguan pin commit SHA action di workflow
├── .vscode/                         task build & run, pengaturan IntelliSense C++17
├── .githooks/commit-msg             hook: pesan commit bebas atribusi alat AI
├── coba.sh · coba.ps1               jalankan sekali tanpa memasang: unduh ke folder sementara, jalankan, hapus
├── Makefile · jalankan.command · jalankan.bat
├── CONTRIBUTING.md · SECURITY.md · CHANGELOG.md · LICENSE
└── README.md
```

Setiap berkas tugas membawa fungsi bantunya sendiri di dalam `namespace` tanpa nama, dan `main()`-nya
dibungkus `#ifndef BUILD_WITH_MENU`: dikompilasi sendiri ia menjadi program utuh; dikompilasi bersama
`main.cpp` dengan `-DBUILD_WITH_MENU`, menu yang dipakai.

## Konvensi kode

| Jenis | Gaya | Contoh |
| --- | --- | --- |
| Tipe | PascalCase | `Student`, `LengthUnit`, `BrailleCanvas` |
| Fungsi dan variabel | lowerCamelCase | `readLine()`, `drawBox()`, `studentCount` |
| Konstanta | UPPER_SNAKE_CASE | `SCORE_MAX`, `PI_MODULE`, `ANSI_ERROR` |
| Namespace | huruf kecil | `task1`, `task2`, `task3` |

Identifier berbahasa Inggris; komentar dan teks tampilan berbahasa Indonesia. Komentar ditulis untuk mahasiswa semester
awal: setiap kata kunci atau konstruksi C++ dijelaskan satu baris di atas kemunculan pertamanya di berkas itu, dan
setiap baris kode diberi komentar di ujungnya yang mengulang bagian khas baris itu lalu menjelaskan gunanya dengan kata
sederhana ("digunakan untuk", "adalah", "artinya", "berfungsi untuk"), termasuk alasan teknis yang penting.
Kompilasi dijaga bersih dengan `-Wall -Wextra -pedantic -Werror` (GCC 13 dan Clang).

## Catatan tampilan

- Simbol π, ², ³, garis kotak, dan sketsa Braille memakai UTF-8. macOS/Linux langsung mendukung; di Windows program
  mengatur konsolnya sendiri ke UTF-8 (code page 65001) saat dimulai, sehingga tidak perlu `chcp 65001` manual
  (peluncur `jalankan.bat` juga memanggilnya). Font monospace modern (Menlo, SF Mono, Cascadia Mono, DejaVu Sans
  Mono) menampilkan Braille dengan baik; Command Prompt klasik dengan font Consolas menampilkannya sebagai kotak.
- Warna mengikuti konvensi CLI: mati otomatis bila keluaran dialihkan ke berkas atau `NO_COLOR` ada; `FORCE_COLOR`
  memaksanya aktif. Di Windows, program mengaktifkan mode virtual terminal sendiri.

## Berkontribusi

Lihat [CONTRIBUTING.md](CONTRIBUTING.md). Setelah clone, aktifkan hook sekali:

```bash
git config core.hooksPath .githooks
```

## Lisensi

[MIT](LICENSE) © 2026 Fadhlillah
