#!/bin/bash
# Peluncur paket rilis macOS: klik kanan berkas ini -> Open (sekali saja), lalu program berjalan di Terminal.
# Program tidak ditandatangani Apple, sehingga macOS memberi tanda "karantina" pada berkas yang diunduh dan
# menolak menjalankannya. Baris xattr di bawah melepas tanda itu hanya dari program ini, atas persetujuan Anda
# saat membuka peluncur ini; tidak ada perubahan lain pada sistem.
cd "$(dirname "$0")" || exit 1
xattr -d com.apple.quarantine ./tugas_alpro 2>/dev/null
chmod +x ./tugas_alpro
./tugas_alpro
read -r -p "Program selesai. Tekan Enter untuk menutup jendela..."
