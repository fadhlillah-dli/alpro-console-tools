#!/bin/sh
# Mencoba alpro-console-tools sekali jalan, tanpa memasang apa pun (macOS dan Linux).
#   curl -fsSL https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/coba.sh | sh
# Mengunduh paket rilis terbaru ke folder sementara, memverifikasi SHA-256, menjalankan program, lalu menghapus folder
# itu begitu program ditutup (juga bila dihentikan dengan Ctrl+C). Tidak ada berkas yang tertinggal dan PATH tidak disentuh.
# Menggunakan fungsi main yang dipanggil di baris terakhir berfungsi agar shell membaca seluruh skrip sebelum menjalankan
# apa pun, dikarenakan pada `curl ... | sh` tanpa terminal program akan mewarisi stdin yang sama dan bisa membaca sisa
# teks skrip sebagai ketikan; ini juga melindungi dari unduhan skrip yang terpotong.
set -eu

main() {
  REPO="fadhlillah-dli/alpro-console-tools"
  API="https://api.github.com/repos/$REPO/releases/latest"

  say() { printf '%s\n' "$*"; }
  die() { printf 'coba.sh: %s\n' "$*" >&2; exit 1; }
  command -v curl >/dev/null 2>&1 || die "curl tidak ditemukan"

  os="$(uname -s)"
  case "$os" in
    Darwin) label="macos-universal"; ext="zip" ;;
    Linux)
      arch="$(uname -m)"
      [ "$arch" = "x86_64" ] || die "paket Linux hanya tersedia untuk x86_64 (arsitektur ini: $arch). Kompilasi sendiri: lihat README."
      label="linux-x86_64"; ext="tar.gz" ;;
    *) die "sistem $os tidak didukung oleh skrip ini" ;;
  esac

  say "Mencari rilis terbaru $REPO ..."
  # Versi terbaru dibaca dari pengalihan halaman releases/latest (tanpa API, sehingga tidak kena batas permintaan
  # API GitHub); API dipakai sebagai cadangan.
  version="$(curl -fsSI -o /dev/null -w '%{redirect_url}' "https://github.com/$REPO/releases/latest" | sed -n 's#.*/tag/##p')"
  [ -n "$version" ] || version="$(curl -fsSL "$API" | sed -n 's/.*"tag_name": *"\([^"]*\)".*/\1/p' | head -n 1)"
  [ -n "$version" ] || die "tidak bisa membaca versi rilis terbaru (periksa koneksi internet)"
  asset="alpro-console-tools-$version-$label.$ext"
  base="https://github.com/$REPO/releases/download/$version"

  # Menggunakan trap pada EXIT, INT, TERM, dan HUP yang dipasang SEBELUM folder dibuat berfungsi untuk menghapus folder
  # sementara apa pun cara skrip berakhir, dikarenakan tujuan skrip ini adalah tidak menyisakan berkas di komputer pengguna.
  tmp=""
  trap 'if [ -n "$tmp" ]; then rm -rf "$tmp"; fi' EXIT INT TERM HUP
  tmp="$(mktemp -d)"
  say "Mengunduh $asset ..."
  curl -fsSL -o "$tmp/$asset" "$base/$asset"

  # SHA256SUMS wajib ada, dikarenakan skrip ini selalu mengambil rilis terbaru dan setiap rilis sejak v1.0.1 menyertakannya.
  curl -fsSL -o "$tmp/SHA256SUMS" "$base/SHA256SUMS"
  expected="$(grep " $asset\$" "$tmp/SHA256SUMS" | cut -d' ' -f1)"
  if command -v sha256sum >/dev/null 2>&1; then actual="$(sha256sum "$tmp/$asset" | cut -d' ' -f1)"
  else actual="$(shasum -a 256 "$tmp/$asset" | cut -d' ' -f1)"; fi
  [ -n "$expected" ] && [ "$expected" = "$actual" ] || die "SHA-256 tidak cocok; unduhan rusak atau diubah. Dibatalkan."
  say "SHA-256 cocok."

  case "$ext" in
    zip)    command -v unzip >/dev/null 2>&1 || die "unzip tidak ditemukan"; unzip -q -o "$tmp/$asset" tugas_alpro -d "$tmp" ;;
    tar.gz) tar -xzf "$tmp/$asset" -C "$tmp" ./tugas_alpro 2>/dev/null || tar -xzf "$tmp/$asset" -C "$tmp" tugas_alpro ;;
  esac
  [ -f "$tmp/tugas_alpro" ] || die "berkas tugas_alpro tidak ada di dalam paket"
  chmod +x "$tmp/tugas_alpro"

  say "Menjalankan tugas_alpro ($version) dari folder sementara; folder dihapus saat program ditutup."
  say ""
  # Menggunakan /dev/tty sebagai masukan program berfungsi agar ketikan pengguna sampai ke program, dikarenakan pada
  # pemakaian `curl ... | sh` stdin sudah dipakai untuk teks skrip ini. Bila tidak ada terminal (mis. di CI), stdin dipakai
  # apa adanya. `|| status=$?` berfungsi agar kode keluar program tidak menghentikan skrip sebelum folder dibersihkan.
  status=0
  if ( : </dev/tty ) 2>/dev/null; then "$tmp/tugas_alpro" </dev/tty || status=$?
  else "$tmp/tugas_alpro" || status=$?
  fi
  rm -rf "$tmp"
  say ""
  if [ -e "$tmp" ]; then say "Peringatan: folder sementara $tmp tidak bisa dihapus; hapus manual."
  else tmp=""; say "Selesai; folder sementara dihapus. Tidak ada yang terpasang."
  fi
  exit "$status"
}

main "$@"
