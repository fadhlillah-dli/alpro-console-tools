# CLAUDE.md — aturan kerja untuk Claude Code di repositori ini

Proyek C++17 open-source: program konsol bermenu untuk praktikum Algoritma & Pemrograman (Modul 1) —
rekap nilai mahasiswa, volume tabung, luas bangun yang diarsir — dengan validasi input ketat, tampilan kotak
berwarna, sketsa Braille, CI di 3 OS, dan rilis biner via GitHub Actions. Baca `README.md`, `CHANGELOG.md`,
`CONTRIBUTING.md`, dan `src/` sebelum mengubah apa pun.

## Aturan tetap (semua sesi)

1. **Identitas commit hanya pemilik repositori.** Jangan pernah menambahkan `Co-Authored-By`,
   "Generated with Claude Code", `Claude-Session`, atau atribusi AI lain ke pesan commit/PR. Setelah tiap
   commit jalankan `git log -1 --format=full` dan pastikan bersih. Hook `.githooks/commit-msg` harus tetap
   aktif: `git config core.hooksPath .githooks`.
2. **Komentar kode berbahasa Indonesia**, pola "menggunakan X berfungsi untuk Y" dan "dikarenakan Z", tanpa
   penanda `[Slide]`/`[Modul]`. Teks tampilan program tetap berbahasa Indonesia.
3. **Identifier berbahasa Inggris**: tipe `PascalCase`, fungsi/variabel `lowerCamelCase`, konstanta
   `UPPER_SNAKE_CASE`, namespace huruf kecil (`task1`, `task2`, `task3`).
4. **Kompilasi bersih** dengan `-Wall -Wextra -pedantic -Werror`: program bermenu (`-DBUILD_WITH_MENU`)
   dan tiap berkas tugas sebagai program sendiri (lihat perintah di bawah).
5. **Uji asap sebelum commit**, hasil harus persis (lihat perintah di bawah):
   - menu 2 (cm, π 22/7, diameter 14, tinggi 10) → `Volume Tabung         : 1540.00 cm³`
   - menu 3 (cm, π 22/7, sisi 14) → `Luas Bangun yang Diarsir : 42.00 cm²`
   - menu 1 (3 mahasiswa 80/60/70) → `Nilai Rata-Rata        70.00`
   - input salah ditolak dengan pesan `[!]`.
6. **Jangan mengubah perilaku, teks tampilan, atau hasil hitung tanpa diminta pemilik.** Perubahan kecil,
   selesai utuh.
7. **Tampilkan ringkasan diff dan minta konfirmasi pemilik sebelum commit dan sebelum push.**

## Perintah build

```bash
# Program bermenu (wajib bersih tanpa peringatan)
g++ -std=c++17 -Wall -Wextra -pedantic -Werror -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp

# Tiap berkas tugas sebagai program sendiri (tanpa -DBUILD_WITH_MENU); nama keluaran sama dengan Makefile dan .gitignore
g++ -std=c++17 -Wall -Wextra -pedantic -Werror -o tugas1 src/tugas1_nilai_mahasiswa.cpp
g++ -std=c++17 -Wall -Wextra -pedantic -Werror -o tugas2 src/tugas2_volume_tabung.cpp
g++ -std=c++17 -Wall -Wextra -pedantic -Werror -o tugas3 src/tugas3_luas_arsir.cpp

# Alternatif harian: make (tugas_alpro), make tugas1 / tugas2 / tugas3, make clean — Makefile hanya memakai
# -Wall -Wextra (tanpa -pedantic -Werror), jadi untuk memenuhi aturan 4 pakai perintah g++ di atas.
```

## Perintah uji asap

Urutan ketikan di bawah sama dengan langkah "Uji asap" di `.github/workflows/ci.yml`. Enter kosong memilih
nilai bawaan (satuan cm) atau kembali ke menu.

```bash
# Menu 2: cm (Enter), π 22/7 (2), diameter (2), 14, tinggi 10, Enter, keluar
printf '2\n\n2\n2\n14\n10\n\n0\n' | ./tugas_alpro | grep "Volume Tabung         : 1540.00 cm³"

# Menu 3: cm (Enter), π 22/7 (2), sisi 14, Enter, keluar
printf '3\n\n2\n14\n\n0\n' | ./tugas_alpro | grep "Luas Bangun yang Diarsir : 42.00 cm²"

# Menu 1: mata kuliah, 3 mahasiswa A 80 / B 60 / C 70, Enter, keluar
printf '1\nAlgoritma dan Pemrograman\n3\nA\n80\nB\n60\nC\n70\n\n0\n' | ./tugas_alpro | grep "Nilai Rata-Rata        70.00"

# Input salah harus ditolak dengan pesan [!]
printf '9\nabc\n0\n' | ./tugas_alpro | grep -c '\[!\]'   # harus > 0

# Ketiganya sekaligus, persis seperti CI: tiap ketidakcocokan menghentikan dengan exit 1 (jalankan sebagai skrip)
out=$(printf '2\n\n2\n2\n14\n10\n\n3\n\n2\n14\n\n1\nAlgoritma dan Pemrograman\n3\nA\n80\nB\n60\nC\n70\n\n0\n' | ./tugas_alpro)
echo "$out" | grep -q "Volume Tabung         : 1540.00" || { echo "volume salah"; exit 1; }
echo "$out" | grep -q "Luas Bangun yang Diarsir : 42.00" || { echo "luas arsir salah"; exit 1; }
echo "$out" | grep -q "Nilai Rata-Rata        70.00" || { echo "rata-rata salah"; exit 1; }
echo "Uji asap lulus"
```

Warna ANSI mati otomatis bila keluaran bukan terminal, sehingga `grep` di atas cocok tanpa kode escape.

## Alur commit dan rilis

- Sebelum commit: aturan 4 dan 5 lulus, lalu tunjukkan `git diff --stat` dan minta konfirmasi (aturan 7).
- Setelah commit: `git log -1 --format=full` harus tanpa baris atribusi AI (aturan 1).
- Push hanya setelah konfirmasi pemilik (aturan 7).
- Rilis: tag `v*` memicu `.github/workflows/release.yml` (tiga paket + `SHA256SUMS`, lalu formula Homebrew ke
  `fadhlillah-dli/homebrew-tap` bila secret `HOMEBREW_TAP_TOKEN` tersedia). `CHANGELOG.md` mengikuti
  Keep a Changelog; versi di `CHANGELOG.md` dan tag harus sama.

## Peta berkas

- `src/main.cpp` — banner, menu, pembacaan pilihan, jeda; memanggil `task1::run()`, `task2::run()`, `task3::run()`.
- `src/tugas1_nilai_mahasiswa.cpp`, `src/tugas2_volume_tabung.cpp`, `src/tugas3_luas_arsir.cpp` — tiap berkas
  lengkap dengan fungsi bantunya di namespace tanpa nama; `main()`-nya dibungkus `#ifndef BUILD_WITH_MENU`.
- `.github/workflows/ci.yml` — build 3 OS + uji asap; `release.yml` — paket rilis saat tag `v*`.
- `dist/` — peluncur dan catatan untuk paket rilis; `install.sh` — pemasang curl untuk macOS/Linux.
- `.claude/settings.json` — setelan Claude Code yang dibagikan (atribusi commit/PR kosong);
  `.claude/settings.local.json` — setelan lokal, tidak di-commit.
