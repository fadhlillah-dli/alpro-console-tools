#!/bin/bash
# Peluncur paket rilis macOS: klik dua kali untuk membuka Terminal dan menjalankan program yang sudah dikompilasi.
cd "$(dirname "$0")" || exit 1
./tugas_alpro
read -r -p "Program selesai. Tekan Enter untuk menutup jendela..."
