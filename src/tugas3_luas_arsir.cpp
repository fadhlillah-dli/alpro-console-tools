/*
 * Tugas Praktikum 3 — Modul 1: Input dan Output
 * Program 3 : Luas Bangun yang Diarsir
 * Alur : pengguna memilih satuan panjang (mm/cm/dm/m) dan nilai π (3.14 sesuai modul atau 22/7 seperti pada
 *        soal yang jari-jarinya kelipatan 7, yaitu sisi persegi kelipatan 14), lalu memasukkan sisi persegi
 *        (bujur sangkar). Output: luas daerah yang diarsir, yaitu persegi dikurangi lingkaran di dalamnya yang
 *        menyinggung keempat sisi (r = s / 2).
 *        Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2)); Pi = π (modul menulisnya phi).
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri (perintah di bawah menghasilkan program bernama tugas3,
 * lalu langsung menjalankannya):
 *   g++ -std=c++17 -Wall -Wextra -o tugas3 tugas3_luas_arsir.cpp && ./tugas3
 * Arti tiap bagian: g++ = compiler C++; -std=c++17 = memakai aturan bahasa C++ versi 2017; -Wall -Wextra =
 * menyalakan peringatan tambahan dari compiler; -o tugas3 = nama program hasil; && ./tugas3 = jalankan program
 * hanya bila kompilasi berhasil.
 * Bila digabung ke program bermenu (main.cpp di folder src), kompilasi dengan tambahan -DBUILD_WITH_MENU (opsi -D
 * membuat makro, yaitu nama penanda, bernama BUILD_WITH_MENU) agar main() di bagian paling bawah berkas ini tidak
 * ikut (lihat BAGIAN 7).
 *
 * Cara membaca komentar di berkas ini: tiap komentar menyebut dulu APA konstruksi C++ pada baris itu, lalu
 * "berfungsi untuk ..." = apa yang dilakukannya di sini, dan "dikarenakan ..." = alasannya dipilih.
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// #include adalah perintah preprocessor (tahap sebelum kompilasi) yang menyalin isi sebuah header (berkas pustaka)
// ke berkas ini, sehingga nama-nama seperti std::cout atau std::max dikenal compiler. Nama di dalam < > adalah
// pustaka standar C++. std:: adalah awalan untuk nama milik pustaka standar C++ (std = standard), jadi std::cout
// dibaca "cout milik pustaka standar". Setiap baris di bawah menyebut apa yang diambil dari pustaka itu dan di mana
// dipakainya.
// Menggunakan <algorithm> berfungsi untuk std::max dan std::min (memilih nilai terbesar/terkecil dari dua angka),
// dipakai di terminalWidth() dan drawBox() pada BAGIAN 3 (lebar jendela dan lebar kotak), di BrailleCanvas pada
// BAGIAN 5 (lapisan tertinggi satu sel), dan di printSketch() pada BAGIAN 5 (ukuran sketsa dan lebar keterangan).
#include <algorithm>
// Menggunakan <cctype> berfungsi untuk std::isdigit, memeriksa apakah satu karakter adalah angka 0-9;
// dipakai di isAllDigits() dan readChoice() pada BAGIAN 4.
#include <cctype>
// Menggunakan <cmath> berfungsi untuk std::hypot (panjang sisi miring = jarak dua titik) dan std::fabs (nilai
// mutlak bilangan desimal), dipakai di printSketch() pada BAGIAN 5 saat menghitung lingkaran sketsa.
#include <cmath>
// Menggunakan <cstddef> berfungsi untuk std::size_t, tipe bilangan bulat tak bertanda (tidak pernah negatif)
// yang dipakai C++ untuk ukuran dan jumlah, misalnya panjang teks dan lebar kolom di BAGIAN 3.
#include <cstddef>
// Menggunakan <cstdlib> berfungsi untuk std::exit dan EXIT_FAILURE (menghentikan program dengan kode gagal,
// dipakai di readLine() pada BAGIAN 4), std::getenv (membaca variabel lingkungan sistem, dipakai di
// colorsEnabled() dan terminalWidth() pada BAGIAN 3), dan std::atoi (teks -> bilangan bulat, di terminalWidth()).
#include <cstdlib>
// Menggunakan <iomanip> berfungsi untuk pengatur format keluaran std::setw (lebar kolom) dan std::setprecision
// (jumlah angka desimal), dipakai bersama std::left dan std::fixed di formatResultLine() pada BAGIAN 5.
#include <iomanip>
// Menggunakan <initializer_list> berfungsi untuk std::initializer_list, tipe yang menerima daftar nilai yang
// ditulis dalam kurung kurawal { ... }, dipakai printOptions() pada BAGIAN 5 untuk menerima daftar baris opsi.
#include <initializer_list>
// Menggunakan <iostream> berfungsi untuk std::cin (membaca ketikan), std::cout (mencetak ke layar), dan
// std::cerr (mencetak pesan kesalahan).
#include <iostream>
// Menggunakan <sstream> berfungsi untuk std::istringstream (membaca angka dari teks, di parseDecimal() pada
// BAGIAN 4) dan std::ostringstream (menyusun teks seperti mencetak ke layar, di formatResultLine() pada BAGIAN 5).
#include <sstream>
// Menggunakan <string> berfungsi untuk std::string, tipe teks yang panjangnya bisa berubah (baris ketikan pengguna
// dan baris tampilan), serta std::getline (membaca satu baris ketikan, di readLine() pada BAGIAN 4) dan
// std::to_string (angka -> teks, di readChoice() pada BAGIAN 4).
#include <string>
// Menggunakan <string_view> berfungsi untuk std::string_view, "jendela" ke teks yang sudah ada: hanya dibaca,
// tidak disalin; dipakai untuk teks prompt (pertanyaan) agar pemanggilan fungsi lebih ringan.
#include <string_view>
// Menggunakan <vector> berfungsi untuk std::vector, daftar (array) yang jumlah isinya bisa bertambah; tipe isinya
// ditulis di dalam < >, mis. std::vector<std::string> = daftar teks. Dipakai untuk daftar baris yang digambar di
// dalam kotak (BAGIAN 3) dan titik-titik kanvas sketsa (BAGIAN 5).
#include <vector>
// Menggunakan #ifdef _WIN32 berfungsi untuk memilih pustaka sesuai sistem operasi saat kompilasi: #ifdef adalah
// pertanyaan ke compiler "apakah makro ini sudah didefinisikan?" (makro = nama penanda atau pengganti teks, dibuat
// dengan #define atau otomatis oleh compiler), dan _WIN32 adalah penanda yang otomatis didefinisikan compiler di
// Windows. Baris sampai #else hanya ikut dikompilasi di Windows, baris setelah #else sampai #endif hanya di
// macOS/Linux; #endif menandai akhir blok bersyarat itu. Dikarenakan kedua sistem menyediakan fungsi berbeda
// untuk keperluan yang sama: mendeteksi terminal, membaca lebar jendela, mengaktifkan warna, dan UTF-8 (cara
// menyimpan huruf sebagai byte: huruf biasa 1 byte, simbol seperti π dan ² 2-3 byte), dipakai di BAGIAN 3.
#ifdef _WIN32
    // Menggunakan #ifndef NOMINMAX berfungsi untuk mengecek apakah makro (nama pengganti teks yang dibuat #define)
    // NOMINMAX belum pernah didefinisikan; #define di bawahnya hanya dijalankan bila belum ada, dikarenakan MinGW
    // (compiler g++ versi Windows, mis. dari MSYS2) sudah mendefinisikannya lewat pustaka standar, dan
    // mendefinisikan ulang makro yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    // Menggunakan #define NOMINMAX 1 berfungsi untuk membuat makro NOMINMAX (nilainya 1, yang penting adalah
    // keberadaannya) sebelum <windows.h> di-#include beberapa baris di bawah, dikarenakan tanpa itu windows.h
    // membuat makro bernama min dan max yang menimpa teks max(...) dan min(...), sehingga std::max dan std::min
    // di berkas ini gagal dikompilasi.
    #define NOMINMAX 1
    #endif  // akhir pengecekan NOMINMAX
    // Menggunakan <windows.h> berfungsi untuk fungsi-fungsi Windows yang dipanggil di BAGIAN 3: GetStdHandle
    // (pegangan ke layar, di colorsEnabled() dan terminalWidth()), GetConsoleMode dan SetConsoleMode (warna, di
    // colorsEnabled()), SetConsoleOutputCP dan SetConsoleCP (UTF-8, di enableUtf8Console()), serta
    // GetConsoleScreenBufferInfo (lebar jendela, di terminalWidth()).
    #include <windows.h>
#else  // bukan Windows: macOS atau Linux
    // Menggunakan <cstdio> berfungsi untuk stdout (keluaran standar = layar terminal tempat program mencetak) dan
    // fileno (fungsi yang memberi nomor berkas milik stdout), dipakai bersama isatty di colorsEnabled() pada
    // BAGIAN 3.
    #include <cstdio>
    // Menggunakan <sys/ioctl.h> berfungsi untuk ioctl (fungsi sistem untuk bertanya ke perangkat, di sini
    // terminal), TIOCGWINSZ (kode perintah "ambil ukuran jendela"), dan struct winsize (struct = wadah beberapa
    // variabel; winsize berisi jumlah baris dan kolom jendela). Dipakai membaca lebar jendela di terminalWidth()
    // pada BAGIAN 3.
    #include <sys/ioctl.h>
    // Menggunakan <unistd.h> berfungsi untuk isatty (memeriksa apakah keluaran adalah terminal, bukan berkas; di
    // colorsEnabled()) dan STDOUT_FILENO (nomor keluaran standar, dipakai ioctl di terminalWidth()), keduanya BAGIAN 3.
    #include <unistd.h>
#endif  // akhir pemilihan pustaka sesuai sistem operasi

// namespace adalah wadah nama: nama di dalamnya terpisah dari nama yang sama di wadah lain. Menggunakan namespace
// tanpa nama (namespace { ... }) berfungsi untuk menjadikan konstanta dan fungsi bantu di dalamnya milik berkas ini
// saja, tidak terlihat dari berkas lain; dikarenakan tugas1_nilai_mahasiswa.cpp dan tugas2_volume_tabung.cpp
// (folder src) juga punya fungsi bantu bernama sama (mis. colorsEnabled() dan drawBox()), dan tanpa ini
// penggabungan ke program bermenu gagal dengan error "multiple definition".
// Blok ini berakhir pada baris "}  // namespace" sebelum BAGIAN 6.
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // constexpr adalah kata kunci untuk nilai tetap yang sudah pasti sejak kompilasi (bukan dihitung saat program
    // jalan) dan tidak bisa diubah; double adalah tipe bilangan desimal. Menggunakan π = 3.14 sebagai nilai
    // bawaan berfungsi agar konsisten dengan program 2 (tugas2_volume_tabung.cpp di folder src) dan modul (modul
    // menulis "phi"; yang dimaksud adalah π / pi, bukan φ / phi rasio emas 1,618). Menggunakan tipe double
    // dikarenakan π bilangan desimal.
    constexpr double PI_MODULE   = 3.14;        // nilai bawaan, sesuai modul
    // 22.0 / 7.0 ditulis dengan .0 dikarenakan 22 / 7 (bilangan bulat) hasilnya 3, bukan 3.142857.
    constexpr double PI_FRACTION = 22.0 / 7.0;  // nilai soal sekolah bila jari-jari kelipatan 7 (sisi kelipatan 14)
    // int adalah tipe bilangan bulat; kedua lebar di bawah dihitung dalam jumlah karakter.
    constexpr int    TITLE_WIDTH = 60;         // lebar garis judul, dalam karakter
    constexpr int    LABEL_WIDTH = 24;         // lebar kolom label pertanyaan pilihan, agar semua opsi sejajar
    constexpr double DIMENSION_MAX = 1000000.0;  // batas atas ukuran: menolak salah ketik yang tidak masuk akal
    // const char* adalah tipe teks tetap (penunjuk ke deretan karakter yang tidak diubah). "\033[31m" adalah kode
    // warna ANSI: \033 = karakter ESC (kode 27, ditulis oktal = bilangan basis 8), lalu "[31m" = perintah warna
    // merah. Merah berfungsi untuk daerah yang diarsir pada sketsa, dikarenakan meniru warna gambar soal.
    constexpr const char* ANSI_HATCH = "\033[31m";

    // Menggunakan array (daftar bernomor tetap, tanda [] setelah nama) const char* berfungsi untuk menyimpan daftar
    // satuan panjang yang lazim pada soal; urutannya = nomor pilihan 1-4 (indeks array 0-3). Menggunakan const char*
    // dikarenakan simbol satuan adalah teks tetap yang tidak diubah.
    constexpr const char* UNITS[] = { "mm", "cm", "dm", "m" };
    constexpr int         UNIT_COUNT   = 4;  // jumlah isi UNITS di atas; batas nomor pilihan di chooseUnit() (BAGIAN 5)
    // DEFAULT_UNIT_INDEX = 2 adalah NOMOR PILIHAN untuk cm (pilihan [2]), bukan indeks array: UNITS[2] adalah "dm",
    // sedangkan cm ada di UNITS[1]; chooseUnit() di BAGIAN 5 mengurangi 1 sebelum mengambil dari UNITS. Dipilih
    // dikarenakan hampir semua soal luas daerah arsiran memakai cm.
    constexpr int         DEFAULT_UNIT_INDEX   = 2;

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Kode warna ANSI adalah teks khusus yang diawali karakter ESC ("\033") dan dibaca terminal sebagai perintah
    // ganti warna, bukan dicetak; angka di dalamnya: 0 = normal, 1 = tebal, 31 = merah, 32 = hijau, 33 = kuning,
    // 36 = cyan, 90 = abu-abu, dan dua angka dipisah ; digabung. Menggunakan kode warna berfungsi untuk membedakan
    // judul, pilihan yang ditegaskan, hasil akhir, dan pesan kesalahan secara visual, dikarenakan terminal macOS,
    // Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // 0 = kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // 1 = tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // 1;36 = cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // 1;32 = hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // 1;31 = merah tebal: pesan kesalahan
    // 33 = kuning: keterangan nilai bawaan [Enter = ...] serta garis jari-jari R pada sketsa.
    constexpr const char* ANSI_HINT = "\033[33m";
    constexpr const char* ANSI_DIM    = "\033[90m";    // 90 = abu-abu: garis kotak dan garis pembatas

    // Fungsi adalah blok perintah bernama yang bisa dipanggil berulang; void berarti fungsi ini tidak mengembalikan
    // nilai, dan () kosong berarti tanpa parameter (nilai masukan yang diterima fungsi). Fungsi ini berfungsi untuk
    // mengatur konsol Windows ke UTF-8 (code page 65001; code page = tabel Windows yang menentukan byte mana tampil
    // sebagai huruf apa) sebelum keluaran pertama, dikarenakan simbol π, ², garis kotak, dan sketsa Braille (lihat
    // BAGIAN 5 lanjutan) dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan masih memakai
    // code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku untuk jendela konsol
    // yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur jalankan.bat di folder utama proyek.
    // Di macOS/Linux terminal sudah UTF-8, sehingga fungsi ini tidak melakukan apa-apa. Dipanggil oleh run() di
    // BAGIAN 6.
    void enableUtf8Console() {
    // Menggunakan #ifdef _WIN32 di dalam fungsi berfungsi untuk menyertakan dua baris di bawah hanya saat dikompilasi
    // di Windows, dikarenakan kedua fungsi itu hanya ada di windows.h; di macOS/Linux isi fungsi ini kosong.
    #ifdef _WIN32
        // SetConsoleOutputCP dan SetConsoleCP adalah fungsi Windows untuk mengganti code page keluaran dan masukan
        // konsol; CP_UTF8 adalah konstanta Windows bernilai 65001, yaitu nomor code page UTF-8.
        SetConsoleOutputCP(CP_UTF8);  // keluaran: byte UTF-8 ditampilkan sebagai karakter yang benar
        SetConsoleCP(CP_UTF8);        // masukan: ketikan huruf non-ASCII dibaca sebagai UTF-8, sama seperti keluaran
    #endif  // akhir bagian khusus Windows
    }

    // bool adalah tipe dengan dua nilai: true (benar) atau false (salah). Fungsi ini berfungsi untuk menentukan
    // apakah warna boleh dipakai. Warna dimatikan bila variabel lingkungan NO_COLOR ada (konvensi umum program CLI,
    // yaitu program berbasis teks yang dijalankan di terminal) atau keluaran bukan terminal (mis. dialihkan ke
    // berkas), dan dipaksa aktif bila FORCE_COLOR ada. Di Windows, mode "virtual terminal" diaktifkan agar Command
    // Prompt memahami kode ANSI. Dipanggil oleh colorize() di bawah.
    bool colorsEnabled() {
        // static pada variabel lokal berarti variabel dibuat satu kali dan tetap hidup sampai program selesai;
        // nilai awalnya dihitung sekali oleh lambda []() { ... } (fungsi kecil tanpa nama) yang langsung dipanggil
        // dengan () di akhirnya. Menggunakan static dikarenakan pemeriksaan cukup dilakukan sekali untuk seluruh
        // program; const dikarenakan hasilnya tidak perlu diubah lagi.
        static const bool enabled = []() {
            // std::getenv membaca variabel lingkungan; hasilnya nullptr (penunjuk kosong) bila variabel tidak ada.
            // != nullptr berarti "variabel ada", apa pun isinya. return langsung mengakhiri lambda dengan nilai itu.
            if (std::getenv("NO_COLOR") != nullptr) return false;
            if (std::getenv("FORCE_COLOR") != nullptr) return true;
        // Menggunakan #ifdef _WIN32 berfungsi memilih cara mendeteksi terminal sesuai sistem operasi, dikarenakan
        // Windows memakai fungsi konsolnya sendiri sedangkan macOS/Linux memakai isatty.
        #ifdef _WIN32
            // HANDLE adalah tipe Windows untuk "pegangan" ke sebuah objek sistem; GetStdHandle(STD_OUTPUT_HANDLE)
            // mengambil pegangan ke keluaran standar (layar). DWORD adalah bilangan bulat 32-bit tak bertanda Windows.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;
            // || adalah "atau", ! adalah "bukan/tidak". Bila pegangan tidak sah atau mode konsol tidak bisa dibaca
            // (keluaran bukan konsol, mis. dialihkan ke berkas), warna dimatikan. &mode = alamat variabel mode agar
            // GetConsoleMode bisa mengisinya.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // ENABLE_VIRTUAL_TERMINAL_PROCESSING adalah bendera (pengaturan nyala/mati) Windows yang membuat konsol
            // memahami kode warna ANSI; | (OR bit) menambahkannya ke mode yang sudah ada tanpa menghapus pengaturan
            // lain. SetConsoleMode mengembalikan bukan-0 bila berhasil, sehingga != 0 menjadi true = warna boleh
            // dipakai.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // isatty(fileno(stdout)) bertanya ke sistem apakah keluaran standar terhubung ke terminal (bukan berkas);
            // hasilnya bukan-0 bila ya, sehingga != 0 mengubahnya menjadi true/false.
            return isatty(fileno(stdout)) != 0;
        #endif  // akhir pemilihan cara deteksi terminal
        }();  // () di sini langsung memanggil lambda di atas; hasilnya menjadi nilai awal 'enabled'
        return enabled;
    }

    // Fungsi ini mengembalikan std::string dan menerima dua parameter: code (kode warna ANSI, tipe const char*) dan
    // text (teks yang akan diwarnai, tipe std::string_view = hanya dibaca). Berfungsi untuk membungkus teks dengan
    // kode warna bila warna aktif; bila tidak, teks dikembalikan apa adanya, dikarenakan kode warna yang tercetak di
    // berkas atau terminal tanpa warna hanya menjadi sampah teks.
    std::string colorize(const char* code, std::string_view text) {
        // std::string(text) mengubah string_view menjadi std::string (menyalin teks) agar bisa dikembalikan.
        if (!colorsEnabled()) return std::string(text);
        // + pada std::string berfungsi menyambung teks: kode warna, teks, lalu ANSI_RESET agar warna tidak bocor
        // ke teks berikutnya.
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // unsigned (singkatan unsigned int) adalah bilangan bulat tak bertanda; code point adalah nomor sebuah karakter
    // dalam tabel Unicode, ditulis heksadesimal (awalan 0x). Fungsi ini berfungsi untuk menentukan apakah sebuah
    // code point memakai dua kolom di terminal (aksara CJK = Cina/Jepang/Korea, Hangul, emoji), dikarenakan tanpa ini
    // lebar kotak dan pembungkusan baris salah bila teks memuat aksara lebar atau emoji. Dipakai displayWidth()
    // di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Setiap pasangan (codePoint >= awal && codePoint <= akhir) adalah satu rentang; && = "dan", || = "atau",
        // jadi hasilnya true bila code point masuk salah satu rentang. Rentang: 1100-115F Hangul Jamo, 2E80-A4CF
        // aksara CJK dan Yi, AC00-D7A3 suku kata Hangul, F900-FAFF dan FE30-FE4F CJK kompatibilitas, FF00-FF60 dan
        // FFE0-FFE6 bentuk lebar penuh (fullwidth), 1F300-1F64F dan 1F900-1F9FF emoji.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Fungsi ini mengembalikan std::size_t (bilangan bulat tak bertanda untuk ukuran). Berfungsi untuk menghitung
    // lebar teks di layar: karakter UTF-8 multibyte (π, ², garis kotak, Braille) dihitung satu kolom, aksara lebar
    // (CJK, emoji) dua kolom, dan kode warna ANSI nol kolom, dikarenakan ketiganya tidak memakai ruang sebanyak
    // byte-nya. Pada UTF-8 satu karakter bisa terdiri dari 1-4 byte: byte awal menentukan jumlah byte lanjutan yang
    // dirangkai menjadi satu code point. Dipakai wrapText(), drawBox(), dan printSketch() di bawah.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah dihitung
        bool inEscape = false;  // true selama berada di dalam urutan kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point tersebut
        // Range-based for (for (x : wadah)) berfungsi memeriksa setiap byte teks satu per satu; unsigned char agar
        // nilai byte selalu 0-255 (char biasa bisa negatif untuk byte >= 128, yang merusak perbandingan di bawah).
        for (const unsigned char c : text) {
            // continue berarti lompat ke byte berikutnya. Di dalam kode warna, semua byte diabaikan sampai huruf 'm'.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC pembuka kode warna
            // (c & 0xC0) == 0x80: & (AND bit) mengambil 2 bit teratas; pola 10xxxxxx = byte lanjutan UTF-8 (x = bit
            // apa saja, 0 atau 1). Byte lanjutan tidak menambah kolom, hanya menambah 6 bit ke code point: << 6
            // menggeser bit yang sudah ada, | (OR bit) menempelkan 6 bit baru (c & 0x3F). --pending = kurangi pending
            // 1 lebih dulu, lalu hasilnya dibandingkan dengan 0: bila itu byte lanjutan terakhir dan code point
            // ternyata aksara lebar, ++width (tambah 1 ke width) menambah satu kolom lagi.
            if ((c & 0xC0) == 0x80) {
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            // Pola byte awal UTF-8: 110xxxxx (cek dengan & 0xE0 == 0xC0) diikuti 1 byte lanjutan, 1110xxxx diikuti 2,
            // 11110xxx diikuti 3; sisa bit awal (c & 0x1F / 0x0F / 0x07) menjadi bit tertinggi code point.
            // Byte ASCII biasa (0xxxxxxx) tidak punya lanjutan: pending = 0.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }
        return width;
    }

    // Fungsi ini mengembalikan int. Berfungsi membaca lebar terminal saat ini (jumlah kolom) setiap kali dipanggil,
    // sehingga tampilan ikut menyesuaikan bila jendela diubah ukurannya. Bila tidak diketahui (keluaran bukan
    // terminal), dipakai variabel lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan terminal pada umumnya.
    // Menggunakan batas bawah 50 dikarenakan di bawah itu kotak dan sketsa tidak bisa rapi.
    int terminalWidth() {
        int cols = 0;  // 0 berarti "belum diketahui"; setiap cara di bawah hanya mengisinya bila berhasil
    // Menggunakan #ifdef _WIN32 berfungsi memilih cara membaca ukuran jendela sesuai sistem operasi.
    #ifdef _WIN32
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct (kumpulan variabel) Windows berisi info layar konsol;
        // GetConsoleScreenBufferInfo mengisinya lewat alamat &info dan mengembalikan bukan-0 bila berhasil.
        // Titik (.) mengambil anggota sebuah struct: info.srWindow = kotak jendela di dalam info, lalu .Right dan
        // .Left = nomor kolom kanan dan kiri kotak itu. Lebar jendela = kanan - kiri + 1 (keduanya termasuk).
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // struct winsize adalah tipe dari <sys/ioctl.h> berisi ukuran jendela; ioctl dengan perintah TIOCGWINSZ
        // ("tanya ukuran jendela") mengisinya dan mengembalikan 0 bila berhasil. ws.ws_col = anggota ws_col di dalam
        // struct ws (titik = mengambil anggota struct) = jumlah kolom.
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;  // ioctl TIOCGWINSZ = tanya ukuran jendela
    #endif  // akhir pemilihan cara membaca ukuran jendela
        // Cadangan 1: variabel lingkungan COLUMNS (teks), diubah ke bilangan bulat dengan std::atoi; env == nullptr
        // berarti variabel tidak ada. Cadangan 2: 80 kolom.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;
        return std::max(50, cols);  // std::max memilih yang lebih besar: hasil tidak pernah di bawah 50
    }

    // Fungsi ini mengembalikan std::vector<std::string> (daftar baris teks). Parameter const std::string& text:
    // & = referensi (memakai teks pemanggil langsung tanpa menyalin), const = tidak boleh diubah; maxWidth dan
    // hangingIndent bertipe std::size_t dan diterima sebagai salinan, sehingga boleh diubah di dalam fungsi.
    // Berfungsi memotong satu baris teks yang lebih panjang dari 'maxWidth' kolom menjadi beberapa baris, memotong
    // pada spasi, dan memberi baris lanjutan indentasi 'gantung' (hangingIndent spasi) agar tetap sejajar dengan isi
    // setelah label. Dipakai drawBox(), printError(), dan printOptions() di bawah, dikarenakan teks tidak boleh
    // dipotong sembarangan oleh jendela terminal yang sempit.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar potongan baris yang akan dikembalikan
        // 10 = ruang minimal (kolom) untuk isi: bila indentasi + 10 melebihi lebar maksimal, indentasi jadi 2 spasi.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;  // indentasi terlalu besar untuk jendela sempit: 2 spasi
        std::string remaining = text;  // salinan teks yang belum terpotong; makin pendek tiap putaran
        // while mengulang selama sisa teks masih lebih lebar dari batas.
        while (displayWidth(remaining) > maxWidth) {
            // col = kolom layar saat ini; cutPos = posisi byte spasi terakhir yang cocok untuk memotong.
            // std::string::npos = nilai khusus "tidak ada posisi".
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama di dalam kode warna (tidak dihitung sebagai kolom)
            // for klasik (awal; syarat; langkah) berfungsi mencari spasi terakhir sebelum batas lebar, dihitung per
            // kolom seperti di displayWidth(): kode warna dan byte lanjutan UTF-8 dilewati tanpa menambah kolom.
            // remaining.size() = jumlah byte di remaining; i = posisi byte yang sedang diperiksa (mulai dari 0).
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char> mengubah tipe char menjadi unsigned char (0-255) secara eksplisit.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }
                if (c == 0x1B) { inEscape = true; continue; }
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8: bukan kolom baru
                if (col >= maxWidth) break;  // break = hentikan for: sudah melewati batas lebar
                if (c == ' ' && col > hangingIndent) cutPos = i;  // spasi setelah indentasi: calon titik potong
                ++col;
            }
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            // push_back menambahkan satu baris ke akhir daftar; substr(0, n) = potongan dari awal sepanjang n byte.
            result.push_back(remaining.substr(0, cutPos));
            // Sisa teks (setelah spasi, cutPos + 1) diawali hangingIndent spasi: std::string(n, ' ') = n spasi.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }
        result.push_back(remaining);  // potongan terakhir (atau seluruh teks bila memang sudah muat)
        return result;
    }

    // Parameter title bertipe std::string_view (hanya dibaca) dan lines bertipe const std::vector<std::string>&
    // (daftar baris milik pemanggil, dipakai langsung tanpa disalin dan tidak diubah). Berfungsi untuk menggambar
    // kotak berjudul berisi baris-baris teks. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah melebihi
    // lebar terminal: baris yang terlalu panjang dibungkus lebih dulu oleh wrapText() di atas, dengan indentasi
    // gantung sejajar label bila baris berpola "Label : isi". Menggunakan kotak dikarenakan informasi (petunjuk,
    // menu, hasil) jadi terpisah jelas dari pertanyaan.
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // static_cast<std::size_t>(...) mengubah hasil int menjadi std::size_t agar setipe dengan lebar teks.
        // Dikurangi 2 tepi kotak + 2 margin + 1 kolom cadangan (= 5), dikarenakan sebagian terminal menambah baris
        // kosong bila sebuah baris persis selebar jendela.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // semua baris setelah dibungkus; inilah yang digambar
        // Range-based for dengan const std::string& b: tiap baris dipakai langsung (referensi) tanpa disalin.
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan bila baris tidak berpola "Label : isi"
            // find(" : ") mencari posisi teks " : "; hasilnya npos bila tidak ada. Hanya label yang lebarnya <= 28
            // kolom dipakai sebagai patokan, agar baris lanjutan tidak menjorok terlalu jauh.
            const std::size_t labelPos = b.find(" : ");
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // + 3 = lebar " : ", sejajar isi setelahnya
            }
            // Setiap potongan hasil wrapText ditambahkan sebagai baris isi kotak.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // + 2 = margin kiri dan kanan; std::max menjaga lebar kotak = baris terlebar.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min memilih yang lebih kecil: kotak tidak melebihi terminal
        // "\u2500" adalah cara menulis karakter Unicode U+2500 (garis ─) di dalam string; filler diisi garis
        // sebanyak sisa lebar setelah judul (judul + 3 hiasan), sehingga garis atas kotak tertutup rapat.
        std::string filler;
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // std::cout << ... << '\n' mencetak ke layar lalu ganti baris. Garis atas: ┌─ (\u250c\u2500), judul, garis
        // pengisi, lalu ┐ (\u2510); garis diwarnai abu-abu dan judul cyan tebal.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Setiap baris isi: │ (\u2502), spasi margin + teks, spasi pengisi sampai selebar kotak, lalu │ lagi.
        // Operator ternary (syarat ? a : b) memilih a bila syarat benar, b bila salah: jumlah spasi pengisi =
        // width - contentWidth, tetapi 0 bila teks lebih lebar, dikarenakan pengurangan size_t tidak boleh negatif.
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;
            const std::size_t contentWidth = displayWidth(rowText);
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }
        // Garis bawah: └ (\u2514), garis ─ selebar kotak, lalu ┘ (\u2518).
        std::string bottomLine;
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }

    // Berfungsi mencetak pesan kesalahan (merah) yang dibungkus sesuai lebar terminal, dengan baris lanjutan
    // menjorok 6 spasi sejajar teks setelah tanda "  [!] ", dikarenakan pesan panjang tidak boleh terpotong jendela
    // sempit. Dipanggil oleh readPositiveDecimal() dan readChoice() di BAGIAN 4.
    void printError(const std::string& text) {
        // Setiap potongan hasil wrapText (lebar terminal - 1 kolom cadangan, indentasi gantung 6) dicetak merah tebal.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Parameter ch bertipe char (satu karakter). Berfungsi untuk mencetak satu baris garis pembatas abu-abu sepanjang
    // TITLE_WIDTH (60) karakter ch: std::string(n, ch) membuat teks berisi n kali karakter ch. Dipakai printTitle()
    // di BAGIAN 5.
    void printRule(char ch) {
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====

    // Fungsi ini mengembalikan std::string (teks ketikan). Berfungsi untuk menampilkan prompt (pertanyaan, dicetak
    // tebal) lalu membaca satu baris ketikan. Dipanggil oleh readPositiveDecimal() dan readChoice() di bawah,
    // dikarenakan cara membaca dan cara menangani input yang berakhir cukup ditulis satu kali.
    std::string readLine(std::string_view prompt) {
        std::string line;                   // menggunakan std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt); // tebal berfungsi agar pertanyaan mudah dibedakan dari keterangan
        // Menggunakan std::getline(std::cin, line) berfungsi untuk membaca satu baris penuh ketikan (sampai Enter) ke
        // variabel line. Pemeriksaan ! (NOT) berfungsi untuk menangkap kegagalan getline, dikarenakan getline gagal
        // bila input ditutup (Ctrl+D di macOS/Linux, Ctrl+Z di Windows) dan program tidak boleh lanjut tanpa data.
        if (!std::getline(std::cin, line)) {
            // std::cerr dipakai dikarenakan ini pesan error (saluran terpisah dari std::cout).
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            std::exit(EXIT_FAILURE);  // std::exit menghentikan program seketika; EXIT_FAILURE = kode "gagal" ke sistem
        }
        return line;
    }

    // Parameter const std::string& text: referensi (memakai teks pemanggil langsung) yang tidak boleh diubah,
    // dikarenakan teks hanya dibaca dan tidak perlu disalin. Berfungsi untuk membuang spasi/tab di awal dan akhir
    // teks, dan merapatkan spasi beruntun menjadi satu, dikarenakan salah ketik spasi tidak boleh membuat input
    // ditolak.
    std::string normalizeSpaces(const std::string& text) {
        std::string result;                  // std::string berfungsi untuk menampung teks yang sudah dirapikan
        bool pendingSpace = false;         // menggunakan bool berfungsi untuk menandai ada spasi yang belum ditulis
        for (const char c : text) {         // range-based for: memeriksa setiap karakter satu per satu
            // '\t' = tab, '\r' = CR (sisa akhir baris Windows); spasi/tab/CR tidak langsung ditulis, hanya ditandai.
            if (c == ' ' || c == '\t' || c == '\r') {
                // !result.empty() = "hasil sudah berisi"; spasi di awal diabaikan dikarenakan hasil masih kosong.
                pendingSpace = !result.empty();
            } else {
                if (pendingSpace) result += ' ';  // beberapa spasi beruntun ditulis sebagai satu spasi saja
                result += c;                      // += menambahkan karakter ke akhir teks
                pendingSpace = false;
            }
        }
        return result;                       // spasi di akhir otomatis terbuang dikarenakan tidak pernah ditulis
    }

    // Berfungsi untuk memeriksa apakah teks hanya berisi angka 0-9 (tanpa tanda, titik, huruf, atau spasi); hasilnya
    // true/false. Dipakai isDecimalFormat() di bawah untuk memeriksa tiap bagian bilangan.
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;     // teks kosong bukan bilangan
        for (const char c : text) {         // setiap karakter harus digit
            // std::isdigit menerima nilai 0-255 (atau EOF); static_cast<unsigned char> mengubah char (yang bisa
            // negatif untuk byte UTF-8) ke rentang aman itu. Satu saja bukan digit: langsung false.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    // Berfungsi untuk memeriksa format bilangan desimal: angka, boleh diikuti SATU pemisah titik atau koma lalu
    // angka lagi (14, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, dikarenakan
    // bukan cara menulis ukuran yang diharapkan dari pengguna. Dipakai parseDecimal() di bawah.
    bool isDecimalFormat(const std::string& text) {
        // auto = compiler menentukan tipenya sendiri (di sini std::size_t); find_first_of(".,") mencari posisi
        // titik atau koma pertama; hasilnya npos bila tidak ada.
        const auto separator = text.find_first_of(".,");
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        // substr(0, separator) = teks sebelum pemisah; substr(separator + 1) = teks sesudahnya sampai akhir.
        // Pemisah kedua (8.7.5) otomatis tertolak dikarenakan bagian sesudahnya memuat titik.
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // bagian sesudahnya juga
    }

    // Parameter text diterima sebagai salinan (boleh diubah di dalam), dan double& result adalah referensi ke
    // variabel pemanggil, sehingga angka hasil bacaan "dikirim balik" lewat parameter ini. Berfungsi untuk mengubah
    // teks menjadi bilangan desimal; mengembalikan true bila teks sah. Dipakai readPositiveDecimal() di bawah.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang diterima
        for (char& c : text) {        // char& = referensi: berfungsi mengubah karakter langsung pada teksnya
            if (c == ',') c = '.';    // koma diganti titik dikarenakan C++ hanya mengenal penulisan 7.5
        }
        // std::istringstream adalah "aliran masukan dari teks": membaca angka dari teks dengan operator >> persis
        // seperti std::cin membaca ketikan.
        std::istringstream parser(text);
        // (parser >> result) = berhasil membaca angka; (parser >> std::ws).eof() = setelah spasi dilewati tidak ada
        // sisa karakter lagi. Keduanya harus benar (&&) agar teks dinyatakan sah.
        return (parser >> result) && (parser >> std::ws).eof();
    }

    // Fungsi ini mengembalikan double. Berfungsi untuk meminta bilangan desimal yang harus lebih besar dari 0 dan
    // paling besar DIMENSION_MAX (1000000); diulang sampai sah. Dipanggil run() di BAGIAN 6 untuk sisi persegi.
    double readPositiveDecimal(std::string_view prompt) {
        while (true) {  // while (true) berfungsi mengulang permintaan sampai input sah; keluar lewat return
            double value = 0.0;  // double dikarenakan ukuran boleh desimal; 0.0 sebagai nilai awal yang jelas
            // Tiga langkah bersarang: readLine membaca ketikan, normalizeSpaces merapikan spasi, parseDecimal mengubah
            // ke angka (hasil ditaruh di value). Syarat value > 0.0 berfungsi untuk menolak nol dan negatif,
            // dikarenakan panjang sisi tidak mungkin nol atau negatif; value <= DIMENSION_MAX menolak angka raksasa.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            // Dua string literal yang berdampingan otomatis disambung compiler menjadi satu teks panjang.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }
    }

    // Fungsi ini mengembalikan int (nomor pilihan). Berfungsi untuk meminta pilihan berupa satu digit 1..maxValue;
    // Enter tanpa isi berarti pilihan bawaan (defaultValue). Menggunakan nilai bawaan dikarenakan pilihan paling
    // umum (cm, π = 3.14) cukup ditekan Enter. Dipakai chooseUnit() dan choosePi() di BAGIAN 5.
    int readChoice(std::string_view prompt, int maxValue, int defaultValue) {
        while (true) {  // diulang dikarenakan pilihan salah harus diminta lagi
            const std::string text = normalizeSpaces(readLine(prompt));  // ketikan yang sudah dirapikan spasinya
            if (text.empty()) return defaultValue;  // Enter saja berfungsi untuk memilih bawaan
            // Sah bila tepat satu karakter (text.size() == 1) dan karakter itu digit. text[0] = karakter pertama;
            // text[0] - '0' berfungsi mengubah karakter digit menjadi angka, dikarenakan kode karakter '0'..'9'
            // berurutan sehingga selisihnya terhadap '0' adalah nilai angkanya.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) {
                const int choice = text[0] - '0';
                if (choice >= 1 && choice <= maxValue) return choice;  // harus berada di rentang 1..maxValue
            }
            // std::to_string mengubah angka menjadi teks agar bisa disambung dengan + ke pesan.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue)
                                        + ", atau tekan Enter untuk memakai bawaan.");
        }
    }

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // Parameter line bertipe std::initializer_list<std::string_view>: daftar teks yang ditulis pemanggil dalam
    // kurung kurawal { "...", "..." }. Berfungsi untuk menampilkan sebuah pertanyaan pilihan dengan daftar opsinya
    // rapi: baris pertama berisi label (tebal) dan opsi pertama, baris berikutnya opsi lain yang disejajarkan di
    // bawahnya, baris terakhir nilai bawaan (kuning). Menggunakan fungsi ini dikarenakan semua pertanyaan pilihan
    // harus tampil dengan bentuk yang sama agar pengguna langsung mengenali pola "[nomor] pilihan" dan
    // "[Enter = bawaan]". Dipakai chooseUnit() dan choosePi() di bawah.
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) {
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // lebar maksimal satu baris
        bool isFirst = true;  // menggunakan bool berfungsi menandai baris pertama (yang memuat label)
        for (const std::string_view b : line) {  // range-based for: memproses tiap baris opsi berurutan
            // substr(0, 6) mengambil 6 karakter pertama; baris yang diawali "[Enter" adalah baris nilai bawaan.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter");
            // Operator ternary: baris bawaan diwarnai kuning, baris lain dijadikan std::string apa adanya.
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b);
            // Baris pertama memuat label (tebal) yang ditambah spasi sampai selebar LABEL_WIDTH (24) lalu ": ";
            // baris berikutnya diberi spasi selebar LABEL_WIDTH + 2 agar sejajar. Ternary di dalam std::string(n, ' ')
            // menghitung jumlah spasi tambahan: LABEL_WIDTH - panjang label, atau 0 bila label sudah lebih panjang.
            // Keduanya dibungkus (wrapText) agar opsi yang panjang tidak terpotong di jendela sempit.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;  // setelah baris pertama tercetak, baris berikutnya bukan baris label lagi
        }
    }

    // Fungsi ini mengembalikan const char* (simbol satuan, teks tetap). Berfungsi untuk menampilkan daftar satuan,
    // membaca pilihan, lalu menegaskan simbol satuan yang terpilih. Dipanggil run() di BAGIAN 6.
    const char* chooseUnit() {
        // Argumen = nilai yang dikirim ke parameter saat fungsi dipanggil. Argumen kedua { ... } adalah daftar baris
        // opsi untuk parameter initializer_list di printOptions() di atas.
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        // UNIT_COUNT (4) = nomor pilihan terbesar; DEFAULT_UNIT_INDEX (2 = cm) dipakai bila pengguna hanya menekan
        // Enter.
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        const char* unit = UNITS[selection - 1];  // - 1 dikarenakan nomor pilihan mulai dari 1, indeks array dari 0
        // std::string("...") + unit menyambung teks tetap dengan simbol satuan; penegasan hijau agar pilihan
        // pasti terbaca.
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit) << "\n\n";
        return unit;
    }

    // Parameter std::string& legend adalah referensi ke variabel pemanggil: fungsi mengisinya dengan teks nilai π
    // untuk ditampilkan di hasil, sedangkan nilai π untuk perhitungan dikembalikan lewat return (double).
    // Berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    // Dipanggil run() di BAGIAN 6.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan sisi persegi kelipatan 14)",
                                      "[Enter = 3.14]" });
        // Argumen 2 = nomor pilihan terbesar, 1 = nomor bawaan (3.14) bila pengguna hanya menekan Enter.
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // operator ternary berfungsi memilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // \u03c0 = simbol π
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // ternary lagi: nilai π sesuai nomor pilihan
    }

    // Berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas '=' sepanjang TITLE_WIDTH
    // (lihat printRule() di BAGIAN 3). Dipanggil run() di BAGIAN 6.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong sebelum judul agar tidak menempel pada teks sebelumnya
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // "  " + ... = judul menjorok 2 spasi
        printRule('=');
    }

    // Berfungsi untuk mengulang sebuah teks (termasuk karakter UTF-8 seperti ─) sebanyak n kali, dikarenakan
    // std::string(n, c) hanya bisa mengulang satu byte, bukan karakter multibyte. Dipakai printSketch() di bawah.
    std::string repeatText(std::string_view text, int n) {
        std::string result;
        for (int i = 0; i < n; ++i) result += text;  // for klasik: += menambahkan teks ke akhir, n kali
        return result;
    }

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // struct adalah tipe data buatan sendiri yang mengelompokkan beberapa variabel (anggota data) dan fungsi
    // (anggota fungsi) dalam satu wadah; di sini BrailleCanvas = "kanvas gambar" untuk sketsa. Menggunakan karakter
    // Braille Unicode (U+2800 s.d. U+28FF) berfungsi untuk menggambar dengan resolusi 2 x 4 titik per sel terminal,
    // delapan kali lebih halus daripada satu karakter blok, dikarenakan setiap karakter Braille mewakili kombinasi
    // 8 titik yang dapat dinyalakan satu per satu (teknik yang juga dipakai drawille, pustaka Python untuk
    // menggambar di terminal; tidak dipakai di program ini).
    // Blok struct ini berakhir pada "};" sebelum fungsi printSketch() di bawah.
    struct BrailleCanvas {
        int cols, rows;                 // anggota data: ukuran kanvas dalam sel terminal (kolom, baris)
        // std::vector<unsigned char> = daftar byte, satu untuk tiap titik: 0 = kosong, >0 = nomor lapisan gambar
        // (menentukan warna). Titik disimpan baris demi baris, dari kiri atas.
        std::vector<unsigned char> layers;

        // Constructor: fungsi bernama sama dengan struct yang dijalankan saat kanvas dibuat. Bagian setelah ":"
        // (daftar inisialisasi) mengisi cols = k, rows = row, dan layers dengan (2k) x (4 row) titik bernilai 0.
        // static_cast<std::size_t>(2 * k) mengubah int ke size_t, tipe ukuran yang diminta vector.
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {}
        // const setelah () berarti fungsi anggota ini tidak mengubah isi struct; keduanya mengembalikan ukuran
        // dalam titik.
        int pixelWidth()  const { return 2 * cols; }    // 2 titik per sel ke samping
        int pixelHeight() const { return 4 * rows; } // 4 titik per sel ke bawah

        // Berfungsi menyalakan satu titik pada koordinat (x, y) dengan nomor lapisan layer; lapisan bernomor lebih
        // besar menimpa yang kecil (garis di atas arsiran), dikarenakan garis lingkaran harus tetap terlihat.
        void setPixel(int x, int y, unsigned char layer) {
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // di luar kanvas diabaikan
            // unsigned char& ref = referensi (nama lain) ke byte titik di layers; indeks = y * lebar + x dikarenakan
            // titik disimpan baris demi baris. Mengubah ref berarti mengubah isi layers langsung.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x];
            if (layer > ref) ref = layer;
        }

        // Parameter cx, cy = posisi sel (kolom, baris); layerColors = daftar kode warna, indeksnya nomor lapisan.
        // Berfungsi mengubah 8 titik di satu sel menjadi satu karakter Braille UTF-8 berwarna sesuai lapisan
        // tertingginya. const di akhir: fungsi ini hanya membaca kanvas. Dipakai toLines() tepat di bawah dan
        // printSketch() setelah struct ini (untuk baris tengah sketsa).
        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const {
            // Peta bit Braille (array 2 dimensi [baris][kolom], static const = dibuat sekali dan tidak berubah):
            // kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40; kolom kanan 0x08, 0x10, 0x20, 0x80. Urutan
            // ganjil ini mengikuti standar Braille Unicode (titik 7-8 ditambahkan belakangan di baris ke-4).
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} };
            unsigned char value = 0, maxLayer = 0;  // value = gabungan bit titik menyala; maxLayer = lapisan tertinggi
            // Dua for bersarang memeriksa 4 baris (dy) x 2 kolom (dx) titik di dalam sel ini.
            for (int dy = 0; dy < 4; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    // Posisi titik di kanvas: baris cy * 4 + dy, kolom cx * 2 + dx; indeks = baris * lebar + kolom.
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx];
                    // Bila titik menyala: |= (OR bit) menambahkan bit titik itu ke value; std::max menyimpan
                    // lapisan tertinggi.
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); }
                }
            }
            if (!value) return " ";                       // sel kosong = spasi biasa
            // 0x2800u: akhiran u = bilangan unsigned (tak bertanda); U+2800 + pola bit = kode karakter Braille.
            const unsigned codePoint = 0x2800u + value;          // U+2800 + pola bit = kode Unicode karakter Braille
            // Code point 0x2800-0x28FF disandikan ke UTF-8 tiga byte dengan pola 1110xxxx 10xxxxxx 10xxxxxx:
            // >> menggeser bit ke kanan, & 0x3F mengambil 6 bit terbawah, | menempelkan awalan byte (0xE0 / 0x80).
            std::string s;
            s += static_cast<char>(0xE0 | (codePoint >> 12));
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
            s += static_cast<char>(0x80 | (codePoint & 0x3F));
            return colorize(layerColors[maxLayer], s);  // diwarnai sesuai lapisan tertinggi di sel ini
        }

        // Berfungsi mengubah seluruh kanvas menjadi baris-baris teks siap cetak (satu std::string per baris sel).
        // Dipakai printSketch() di bawah.
        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const {
            std::vector<std::string> lines;
            for (int cy = 0; cy < rows; ++cy) {  // tiap baris sel
                std::string row;
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);  // sel disambung ke kanan
                lines.push_back(row);
            }
            return lines;
        }
    };  // akhir struct BrailleCanvas

    // Berfungsi menggambar sketsa persegi dengan lingkaran di dalamnya pada kanvas Braille (struct BrailleCanvas di
    // atas): lingkaran dihitung dari persamaannya (titik berjarak R dari pusat), daerah di luar lingkaran diarsir
    // garis diagonal merah seperti pada buku cetak, dan garis jari-jari R dibubuhkan di baris tengah. Menggunakan
    // perhitungan dikarenakan hasilnya benar-benar bulat, dan Braille dikarenakan resolusinya jauh lebih halus
    // daripada karakter blok. Dipanggil run() di BAGIAN 6 sebelum pertanyaan pertama.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal: available = lebar terminal dikurangi tepi kotak, margin, dan
        // indentasi (8 kolom); lebar kanvas = available - 2 (2 = garis tepi │ kiri dan kanan persegi), maksimal
        // 48 sel dan minimal 20 sel (std::max dan std::min bersarang menjepit nilai di antara keduanya); jumlah
        // baris = setengah kolom agar bidangnya persegi (2 x 4 titik per sel).
        const int available = terminalWidth() - 8;
        int canvasCols = std::max(20, std::min(48, available - 2));
        if (canvasCols % 2 != 0) --canvasCols;  // % = sisa bagi; bila ganjil dikurangi 1 agar pusatnya tepat di tengah
        const int canvasRows = canvasCols / 2;
        // legend = daftar baris keterangan simbol (std::vector<std::string> diisi langsung lewat { ... }).
        // Keterangan diletakkan di samping hanya bila baris terlebar (gambar + keterangan terpanjang) masih muat di
        // dalam kotak (lebar terminal - 5), dikarenakan drawBox() di BAGIAN 3 akan membungkus baris yang lebih
        // panjang dan itu merusak gambar. Pada rumus di bawah: 3 = indentasi, 2 = tepi persegi, 3 = jarak ke
        // keterangan.
        const std::vector<std::string> legend = {
            "R = jari-jari lingkaran = S / 2",
            "S = sisi persegi = 2 x R (lingkaran menyinggung sisi)",
            colorize(ANSI_HATCH, "merah") + " : daerah yang diarsir (persegi - lingkaran)",
            colorize(ANSI_TITLE, "cyan") + "  : garis lingkaran",
            "Luas Arsir = (S x S) - (Pi x R x R)",
        };
        std::size_t longestLegend = 0;  // lebar keterangan terpanjang (dalam kolom)
        for (const std::string& k : legend) longestLegend = std::max(longestLegend, displayWidth(k));
        // static_cast<std::size_t> agar kedua sisi perbandingan setipe (size_t), menghindari peringatan compiler.
        const bool legendOnRight = (3 + canvasCols + 2 + 3 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);

        BrailleCanvas canvas(canvasCols, canvasRows);  // membuat kanvas kosong; memanggil constructor di struct di atas
        const double cx = canvas.pixelWidth() / 2.0, cy = canvas.pixelHeight() / 2.0;  // pusat dalam satuan titik
        const double radiusPx  = std::min(cx, cy) - 0.5;  // - 0.5 agar lingkaran menyinggung sisi persegi
        // Dua for bersarang mengunjungi setiap titik (x, y) kanvas. std::hypot(a, b) = akar(a² + b²) = jarak titik ke
        // pusat (+ 0.5 = tengah titik). std::fabs = nilai mutlak: titik yang jaraknya hampir sama dengan R (selisih
        // <= 0.9) adalah garis lingkaran (lapisan 2); titik di luar lingkaran dengan (x + y) kelipatan 6 (% = sisa
        // bagi) menjadi arsiran diagonal (lapisan 1), dikarenakan x + y tetap sepanjang satu garis miring.
        for (int y = 0; y < canvas.pixelHeight(); ++y) {
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double dist = std::hypot(x + 0.5 - cx, y + 0.5 - cy);  // jarak titik ke pusat
                if (std::fabs(dist - radiusPx) <= 0.9)          canvas.setPixel(x, y, 2);  // lapisan 2: garis lingkaran
                else if (dist > radiusPx && (x + y) % 6 == 0)   canvas.setPixel(x, y, 1);  // lapisan 1: arsiran luar
            }
        }
        // Daftar warna per lapisan: indeks 0 (kosong) = "", 1 (arsiran) = merah, 2 (garis lingkaran) = cyan tebal.
        const std::vector<const char*> layerColors = { "", ANSI_HATCH, ANSI_TITLE };
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // kanvas menjadi baris-baris teks

        // Blok { ... } tanpa kepala berfungsi membatasi umur variabel sementara di dalamnya. Garis jari-jari
        // "●──R──►" ditimpakan pada baris tengah, dari pusat ke tepi kanan lingkaran.
        {
            const int r = canvasRows / 2, centerCol = canvasCols / 2;  // r = baris tengah, centerCol = kolom pusat
            std::string radiusLine = "\u25cf";                                    // ● (U+25CF) pusat lingkaran
            // Dari kolom setelah pusat sampai sebelum tepi kanan: huruf "R" tepat di tengah ruas (ternary), selain
            // itu garis ─ (\u2500).
            for (int c = centerCol + 1; c < canvasCols - 1; ++c) radiusLine += (c == (centerCol + canvasCols - 1) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";                                               // ► (U+25BA) ujung jari-jari
            // Separuh kiri baris tengah tetap gambar Braille; separuh kanan diganti garis jari-jari berwarna kuning.
            std::string row;
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, r, layerColors);
            figureLines[r] = row + colorize(ANSI_HINT, radiusLine);
        }  // akhir blok garis jari-jari

        // Keterangan simbol: di samping kanan bila jendela lebar, di bawah gambar bila sempit.
        // sideNotes(canvasRows, "") = daftar berisi canvasRows teks kosong, satu per baris gambar.
        std::vector<std::string> sideNotes(canvasRows, "");
        if (legendOnRight) {
            // Array int berisi 5 nomor baris tempat tiap keterangan diletakkan (dihitung dari tinggi kanvas: 1/8,
            // 1/8 + 2, 1/8 + 5, 1/8 + 6, dan baris tengah), sejajar dengan isi legend di atas.
            const int positions[5] = { canvasRows / 8, canvasRows / 8 + 2, canvasRows / 8 + 5, canvasRows / 8 + 6, canvasRows / 2 };
            // Hanya bila nomor baris masih di dalam kanvas; "   " = 3 spasi jarak ke keterangan.
            for (int i = 0; i < 5; ++i) if (positions[i] < canvasRows) sideNotes[positions[i]] = "   " + legend[i];
        }

        // lines = semua baris yang akan digambar dalam kotak: panah sisi S, tepi atas persegi, baris gambar (diapit
        // │ dan ditambah keterangan samping), tepi bawah, lalu keterangan di bawah bila tidak muat di samping.
        std::vector<std::string> lines;
        // Panah ◄────► bertanda S (◄ = U+25C4, ► = U+25BA) menunjukkan panjang sisi persegi.
        lines.push_back("   \u25c4" + repeatText("\u2500", canvasCols - 2) + "\u25ba  S");
        // ┌ (\u250c) + garis ─ selebar kanvas + ┐ (\u2510) = sisi atas persegi, abu-abu.
        lines.push_back("   " + colorize(ANSI_DIM, "\u250c" + repeatText("\u2500", canvasCols) + "\u2510"));
        // Tiap baris gambar diapit │ (\u2502) kiri dan kanan, lalu keterangan samping (kosong bila tidak ada).
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + colorize(ANSI_DIM, "\u2502") + figureLines[r] + colorize(ANSI_DIM, "\u2502") + sideNotes[r]);
        // └ (\u2514) + garis ─ + ┘ (\u2518) = sisi bawah persegi.
        lines.push_back("   " + colorize(ANSI_DIM, "\u2514" + repeatText("\u2500", canvasCols) + "\u2518"));
        if (!legendOnRight) for (const std::string& k : legend) lines.push_back("   " + k);  // bila sempit: di bawah
        // drawBox() (BAGIAN 3) menggambar semua baris di atas di dalam kotak berjudul "Sketsa Bangun yang Diarsir".
        drawBox("Sketsa Bangun yang Diarsir", lines);
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Berfungsi untuk menampilkan petunjuk pengisian dalam kotak (drawBox() di BAGIAN 3) sebelum pertanyaan pertama,
    // dikarenakan pengguna perlu tahu bentuk input yang diterima (pilihan, angka > 0, cara menulis desimal) agar
    // tidak salah ketik. Dipanggil run() di BAGIAN 6.
    void printInstructions() {
        // Argumen kedua { ... } adalah daftar baris yang langsung menjadi std::vector<std::string> untuk drawBox.
        // \" di dalam string = tanda kutip yang ikut dicetak; \u03c0 = simbol π.
        drawBox("Petunjuk Pengisian", {
            "- Satuan Panjang   : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)",
            "                     (contoh: Pilih [2] untuk satuan \"cm\")",
            "- Nilai Pi (\u03c0)     : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila sisi persegi kelipatan 14",
            "                     (contoh: Pilih [2] untuk nilai \"22/7\")",
            "- Sisi Persegi     : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                     (contoh: 14, 14,5 atau 14.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)",
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang."
        });
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Berfungsi untuk membuat satu baris hasil "Label : nilai satuan" dengan 2 desimal, siap dimasukkan ke kotak.
    // Dipanggil run() di BAGIAN 6 untuk tiap baris angka hasil.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit) {
        // std::ostringstream = "aliran keluaran ke teks": menerima << seperti std::cout, tetapi hasilnya ditampung
        // sebagai teks. Berfungsi menyusun baris tanpa mengubah format std::cout.
        std::ostringstream line;
        // std::left = rata kiri; std::setw(25) = lebar kolom label 25 karakter (sisanya diisi spasi);
        // std::fixed = desimal biasa (bukan notasi ilmiah); std::setprecision(2) = tepat 2 angka di belakang koma.
        line << std::left << std::setw(25) << label << ": " << std::fixed << std::setprecision(2) << value << ' ' << unit;
        return line.str();  // .str() mengambil teks yang sudah tersusun
    }

}  // namespace (akhir namespace tanpa nama yang dibuka setelah BAGIAN 1)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 3 =====
// namespace task3 { ... } adalah ruang nama bernama: fungsi di dalamnya dipanggil dengan awalan task3::.
// Menggunakan namespace task3 berfungsi untuk memberi nama khas pada run(), dikarenakan program bermenu
// (main.cpp di folder src) memanggilnya sebagai task3::run(). Blok ini berakhir pada "}  // namespace task3".
namespace task3 {

    // Fungsi utama tugas ini: menjalankan seluruh alur (judul, sketsa, petunjuk, pilihan, input, hitung, hasil)
    // dengan memanggil fungsi-fungsi bantu di BAGIAN 3-5. Dipanggil oleh main() di BAGIAN 7 atau oleh program bermenu.
    void run() {
        enableUtf8Console();  // dipanggil di sini agar program mandiri (tanpa menu) juga benar; aman dipanggil berulang
        printTitle("PROGRAM 3 : LUAS BANGUN YANG DIARSIR (PERSEGI - LINGKARAN)");
        // Dua baris teks dicetak dengan satu std::cout (<< boleh dirangkai); \n = ganti baris, \u03c0 = simbol π.
        std::cout << "Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2))\n"
                  << "        Pi = \u03c0 (modul menulisnya phi)\n\n";
        printSketch();    // berfungsi menampilkan sketsa bangun agar pengguna tahu bagian mana yang dimaksud S dan R
        printInstructions();  // kotak petunjuk pengisian

        // --- Tahap pengaturan: satuan dan nilai π ---
        const char* unit = chooseUnit();          // simbol satuan yang dipilih, mis. "cm"; const char* = teks tetap
        std::string piLabel;                    // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double pi = choosePi(piLabel);     // nilai π yang dipakai perhitungan; piLabel diisi lewat referensi

        // --- Tahap input ---
        // Menggunakan tipe data double berfungsi untuk menyimpan sisi persegi, dikarenakan sisi boleh desimal.
        // Menggunakan readPositiveDecimal (BAGIAN 4) dikarenakan panjang sisi tidak mungkin nol atau negatif.
        // std::string("...") + unit + ") : " menyambung teks prompt dengan simbol satuan yang dipilih.
        const double side = readPositiveDecimal(std::string("Masukkan Panjang Sisi Persegi (") + unit + ") : ");

        // --- Tahap perhitungan --- (const = nilainya tidak diubah lagi setelah dihitung)
        const double radius      = side / 2.0;  // / berfungsi membagi: lingkaran menyinggung sisi, jadi r = s/2
        const double squareArea   = side * side;                  // * berfungsi mengalikan: luas persegi = s x s
        const double circleArea = pi * radius * radius;     // luas lingkaran = π x r x r
        const double shadedArea     = squareArea - circleArea;  // - berfungsi mengurangi: arsir = persegi - lingkaran

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit) + "\u00b2";  // satuan luas, mis. cm² (\u00b2 = pangkat dua)
        std::vector<std::string> resultLines;  // daftar baris hasil; push_back menambahkan satu baris ke akhir
        // Tiap push_back menambah satu baris hasil; formatResultLine() (BAGIAN 5) membuat baris
        // "Label : angka satuan" dengan 2 desimal.
        resultLines.push_back("Pi (\u03c0) yang Digunakan    : " + piLabel);
        resultLines.push_back(formatResultLine("Sisi Persegi (S)", side, unit));
        resultLines.push_back(formatResultLine("Jari-Jari Lingkaran (R)", radius, unit) + "  (R = S / 2, lingkaran menyinggung sisi)");
        resultLines.push_back(formatResultLine("Luas Persegi", squareArea, areaUnit));
        resultLines.push_back(formatResultLine("Luas Lingkaran", circleArea, areaUnit));
        // Hasil akhir ditonjolkan hijau tebal dengan colorize (BAGIAN 3).
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Luas Bangun yang Diarsir", shadedArea, areaUnit)));
        // Baris kosong, lalu drawBox() (BAGIAN 3) menggambar semua baris hasil di dalam kotak "Hasil Perhitungan".
        std::cout << '\n';
        drawBox("Hasil Perhitungan", resultLines);
    }

}  // namespace task3

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU (pertanyaan ke compiler: apakah makro BUILD_WITH_MENU BELUM didefinisikan?)
// berfungsi untuk menyertakan main() hanya saat berkas ini dikompilasi sendiri; makro itu diberikan lewat opsi
// -DBUILD_WITH_MENU saat menggabung ke program bermenu. Dikarenakan program bermenu sudah punya main() dan C++
// hanya mengizinkan satu main().
#ifndef BUILD_WITH_MENU
// int main() adalah titik awal program: fungsi pertama yang dijalankan sistem; mengembalikan int sebagai kode status.
int main() {
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 3 - Fadhlillah (2531070011)") << '\n';  // identitas, tebal
    task3::run();  // menjalankan seluruh alur tugas 3 (BAGIAN 6)
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif  // akhir bagian main() untuk kompilasi mandiri
