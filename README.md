# alpro-console-tools

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)](#unduh-siap-pakai-tanpa-kompilasi)
[![CI](../../actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)

Program konsol C++ untuk praktikum **Algoritma & Pemrograman (Modul 1: Input dan Output)**: rekap nilai
mahasiswa, volume tabung, dan luas bangun yang diarsir, dalam satu menu interaktif dengan validasi input
yang ketat, tampilan berwarna, dan sketsa bangun yang digambar langsung di terminal.

*C++17 console tools for an introductory programming lab (student grade recap, cylinder volume, shaded
area), with strict input validation, colored boxed UI, and computed Braille sketches. UI text is in Indonesian.*

## Demo

> 📺 Video demo akan ditambahkan di sini.

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
  titik atau koma, rekap total dan rata-rata dengan kolom yang tetap sejajar untuk aksara lebar (CJK, emoji).
- **Volume Tabung**: pilihan satuan (mm/cm/dm/m), nilai π 3.14 atau 22/7 seperti pada soal sekolah,
  input jari-jari atau diameter, hasil cm³ plus padanan liter.
- **Luas Bangun yang Diarsir**: persegi dikurangi lingkaran yang menyinggung keempat sisi; hasil sama
  dengan kunci soal (sisi 14 cm, π = 22/7 → 42 cm²).
- **Validasi ketat**: setiap input diperiksa dan diminta ulang dengan pesan yang menyebut tindakan yang benar.
- **Tampilan profesional**: kotak berjudul, warna ANSI (otomatis mati bila keluaran bukan terminal atau
  `NO_COLOR` ada), petunjuk pengisian, dan sketsa bangun dari karakter Braille yang dihitung dari rumus.
- **Menyesuaikan lebar terminal** setiap kali layar digambar: teks dibungkus, menu menjadi dua baris, sketsa
  mengecil, keterangan pindah ke bawah gambar bila jendela sempit.
- **Setiap baris berkomentar** (bahasa Indonesia) menjelaskan apa yang dipakai dan mengapa, cocok untuk belajar.

## Pemasangan

### macOS dan Linux: satu perintah (disarankan)

```bash
curl -fsSL https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/install.sh | sh
```

Skrip ini mengunduh paket rilis terbaru lewat `curl`, memverifikasi SHA-256, dan memasang `tugas_alpro` ke
`~/.local/bin`. Karena tidak lewat browser, berkasnya tidak diberi tanda karantina, sehingga **tidak ada dialog
Gatekeeper** di macOS. Lalu jalankan dengan `tugas_alpro`.

### Windows: satu perintah di PowerShell (disarankan)

Gunakan **PowerShell**, bukan Command Prompt: tekan tombol Windows, ketik `PowerShell`, Enter (atau buka tab PowerShell di
Windows Terminal). Windows PowerShell 5.1 bawaan maupun PowerShell 7 sama-sama bisa. Lalu jalankan:

```powershell
irm https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/install.ps1 | iex
```

Skrip ini mengunduh paket rilis terbaru, memverifikasi SHA-256, memasang `tugas_alpro.exe` ke
`%LOCALAPPDATA%\Programs\alpro-console-tools`, melepas tanda unduhan sehingga **tidak ada layar SmartScreen**, dan
menambahkan folder itu ke PATH pengguna. Tidak perlu hak Administrator. Setelah itu jalankan `tugas_alpro` (di jendela
terminal baru bila perintah belum dikenal); perintah ini berjalan di PowerShell maupun Command Prompt.

### macOS dan Linux: Homebrew

```bash
brew install fadhlillah-dli/tap/alpro-console-tools
```

Homebrew mengambil source code rilis dan mengompilasinya di mesin Anda (beberapa detik); program yang dikompilasi
lokal juga tidak terkena Gatekeeper. Perbarui dengan `brew upgrade alpro-console-tools`.

### Paket siap pakai (zip) untuk yang memilih cara klik

Halaman [**Releases**](../../releases/latest) menyertakan `…-macos-universal.zip` (Apple Silicon dan Intel),
`…-windows-x86_64.zip`, dan `…-linux-x86_64.tar.gz`, masing-masing dengan peluncur klik-dua-kali dan berkas
`SHA256SUMS` untuk verifikasi. Program tidak ditandatangani Apple/Microsoft (keduanya berbayar), jadi berkas yang
diunduh **lewat browser** diperiksa pada pemakaian pertama:

- macOS: klik kanan `Jalankan.command` → Open → Open; bila macOS tetap menolak ("Not Opened"), buka System Settings →
  Privacy & Security → **Open Anyway** sekali. Peluncur melepas tanda karantina dari `tugas_alpro` setelah itu.
- Windows: klik dua kali `Jalankan.bat`; pada layar "Windows protected your PC" klik **More info** → **Run anyway**
  (sekali). Peluncur melepas tanda unduhan dari `tugas_alpro.exe` sehingga tidak ditanya lagi.
- Linux: `sh jalankan.sh`.

## Kompilasi sendiri

Syarat: compiler C++17 (`g++` atau `clang++`).

- macOS: `xcode-select --install`
- Linux (Debian/Ubuntu): `sudo apt install build-essential`
- Windows: pasang [MSYS2](https://www.msys2.org/), lalu `pacman -S mingw-w64-ucrt-x86_64-gcc` dan tambahkan
  `C:\msys64\ucrt64\bin` ke PATH.

```bash
git clone https://github.com/<nama-pengguna>/alpro-console-tools.git
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
├── .github/workflows/               CI (build 3 OS + uji asap) dan Release (biner siap pakai saat tag v*)
├── dist/                            peluncur dan catatan untuk paket rilis
├── .vscode/                         task build & run, pengaturan IntelliSense C++17
├── .githooks/commit-msg             hook: pesan commit bebas atribusi alat AI
├── .claude/settings.json            setelan Claude Code yang dibagikan (tanpa atribusi AI pada commit/PR)
├── CLAUDE.md                        aturan kerja untuk Claude Code: konvensi, perintah build dan uji asap
├── install.sh                       pemasang satu perintah untuk macOS/Linux (unduh rilis + verifikasi SHA-256)
├── install.ps1                      pemasang satu perintah untuk Windows lewat PowerShell (unduh rilis + verifikasi SHA-256)
├── Makefile · jalankan.command · jalankan.bat
├── CONTRIBUTING.md · CHANGELOG.md · LICENSE
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

Identifier berbahasa Inggris; komentar dan teks tampilan berbahasa Indonesia. Kompilasi dijaga bersih dengan
`-Wall -Wextra -pedantic -Werror` (GCC 13 dan Clang).

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
