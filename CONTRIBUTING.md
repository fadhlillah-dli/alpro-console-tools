# Berkontribusi

Terima kasih sudah tertarik ikut mengembangkan proyek ini. Alurnya singkat:

1. Fork repositori ini, lalu buat branch dari `main`, misalnya `fitur/validasi-nama` atau `perbaikan/lebar-kotak`.
2. Ikuti konvensi kode di README (identifier bahasa Inggris, komentar bahasa Indonesia dengan pola
   "menggunakan X berfungsi untuk Y / dikarenakan Z"). Komentar ditulis untuk mahasiswa semester awal: sebutkan dulu
   apa konstruksinya dengan kata sederhana, lalu apa yang dilakukannya di baris itu dan mengapa; jangan menyebut fungsi
   atau variabel yang tidak ada di baris itu tanpa penunjuk lokasinya. Nadanya seperti teman yang sabar menjelaskan:
   mulai dari gambaran besar, kalimat pendek, istilah dijelaskan sekali saat pertama muncul (contoh: blok
   `#ifdef _WIN32` dan fungsi `colorsEnabled()` di `src/main.cpp`).
3. Pastikan kompilasi bersih tanpa peringatan:
   `g++ -std=c++17 -Wall -Wextra -pedantic -Werror -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp`
   dan tiap berkas tugas juga bisa dikompilasi sendiri (tanpa `-DBUILD_WITH_MENU`).
4. Uji manual minimal: contoh modul (A 80, B 60, C 70 → 210.00 / 70.00), tabung r 7 t 10 (1538.60 cm³),
   persegi sisi 14 dengan π = 22/7 (42.00 cm²), dan input salah ditolak dengan pesan yang jelas.
5. Kirim pull request dengan penjelasan singkat: apa yang diubah dan mengapa.

Laporan bug dan ide fitur silakan lewat tab Issues; masalah keamanan dilaporkan secara privat, lihat `SECURITY.md`.
