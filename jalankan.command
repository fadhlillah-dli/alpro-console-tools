#!/bin/bash
# jalankan.command — peluncur untuk macOS: klik dua kali di Finder, Terminal terbuka, program langsung jalan.
# Skrip ini mengompilasi ulang bila perlu (g++ di macOS = clang++ dari Command Line Tools), lalu menjalankan program.

cd "$(dirname "$0")" || exit 1          # pindah ke folder tempat skrip ini berada, agar src/ ditemukan

if ! command -v g++ >/dev/null 2>&1; then
    echo "Compiler belum terpasang. Jalankan dulu:  xcode-select --install"
    read -r -p "Tekan Enter untuk menutup..."
    exit 1
fi

echo "Mengompilasi program..."
if ! g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp; then
    echo "Kompilasi gagal. Periksa pesan error di atas."
    read -r -p "Tekan Enter untuk menutup..."
    exit 1
fi

./tugas_alpro                              # menjalankan program bermenu
read -r -p "Program selesai. Tekan Enter untuk menutup jendela..."
