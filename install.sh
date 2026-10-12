#!/bin/sh
# Pemasang alpro-console-tools untuk macOS dan Linux.
#   curl -fsSL https://raw.githubusercontent.com/fadhlillah-dli/alpro-console-tools/main/install.sh | sh
# Mengunduh paket rilis terbaru dari GitHub, memverifikasi SHA-256 (bila berkas SHA256SUMS tersedia), lalu
# memasang program ke ~/.local/bin/tugas_alpro. Berkas yang diambil lewat curl tidak diberi tanda karantina
# macOS, sehingga program langsung bisa dijalankan tanpa dialog Gatekeeper.
set -eu

REPO="fadhlillah-dli/alpro-console-tools"
BIN_DIR="${ALPRO_BIN_DIR:-$HOME/.local/bin}"
API="https://api.github.com/repos/$REPO/releases/latest"

say() { printf '%s\n' "$*"; }
die() { printf 'install.sh: %s\n' "$*" >&2; exit 1; }
command -v curl >/dev/null 2>&1 || die "curl tidak ditemukan"

os="$(uname -s)"
case "$os" in
  Darwin) label="macos-universal"; ext="zip" ;;
  Linux)
    arch="$(uname -m)"
    [ "$arch" = "x86_64" ] || die "paket Linux hanya tersedia untuk x86_64 (arsitektur ini: $arch). Kompilasi sendiri: lihat README."
    label="linux-x86_64"; ext="tar.gz" ;;
  *) die "sistem $os tidak didukung oleh pemasang ini" ;;
esac

say "Mencari rilis terbaru $REPO ..."
# Versi terbaru dibaca dari pengalihan halaman releases/latest (tanpa API, sehingga tidak kena batas permintaan
# API GitHub); API dipakai sebagai cadangan.
version="$(curl -fsSI -o /dev/null -w '%{redirect_url}' "https://github.com/$REPO/releases/latest" | sed -n 's#.*/tag/##p')"
[ -n "$version" ] || version="$(curl -fsSL "$API" | sed -n 's/.*"tag_name": *"\([^"]*\)".*/\1/p' | head -n 1)"
[ -n "$version" ] || die "tidak bisa membaca versi rilis terbaru (periksa koneksi internet)"
asset="alpro-console-tools-$version-$label.$ext"
base="https://github.com/$REPO/releases/download/$version"

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT INT TERM
say "Mengunduh $asset ..."
curl -fsSL -o "$tmp/$asset" "$base/$asset"

# Verifikasi SHA-256 bila rilis menyertakan SHA256SUMS (rilis v1.0.1 ke atas).
if curl -fsSL -o "$tmp/SHA256SUMS" "$base/SHA256SUMS" 2>/dev/null; then
  expected="$(grep " $asset\$" "$tmp/SHA256SUMS" | cut -d' ' -f1)"
  if command -v sha256sum >/dev/null 2>&1; then actual="$(sha256sum "$tmp/$asset" | cut -d' ' -f1)"
  else actual="$(shasum -a 256 "$tmp/$asset" | cut -d' ' -f1)"; fi
  [ -n "$expected" ] && [ "$expected" = "$actual" ] || die "SHA-256 tidak cocok; unduhan rusak atau diubah. Dibatalkan."
  say "SHA-256 cocok."
else
  say "Catatan: rilis ini belum menyertakan SHA256SUMS; melewati verifikasi."
fi

say "Mengekstrak ..."
case "$ext" in
  zip)    command -v unzip >/dev/null 2>&1 || die "unzip tidak ditemukan"; unzip -q -o "$tmp/$asset" tugas_alpro -d "$tmp" ;;
  tar.gz) tar -xzf "$tmp/$asset" -C "$tmp" ./tugas_alpro 2>/dev/null || tar -xzf "$tmp/$asset" -C "$tmp" tugas_alpro ;;
esac
[ -f "$tmp/tugas_alpro" ] || die "berkas tugas_alpro tidak ada di dalam paket"

mkdir -p "$BIN_DIR"
install -m 755 "$tmp/tugas_alpro" "$BIN_DIR/tugas_alpro"
say "Terpasang: $BIN_DIR/tugas_alpro ($version)"

case ":$PATH:" in
  *":$BIN_DIR:"*) say "Jalankan dengan:  tugas_alpro" ;;
  *) say "Folder $BIN_DIR belum ada di PATH. Tambahkan baris ini ke ~/.zshrc atau ~/.bashrc, lalu buka terminal baru:"
     say "  export PATH=\"$BIN_DIR:\$PATH\""
     say "Untuk sekarang jalankan dengan:  $BIN_DIR/tugas_alpro" ;;
esac
