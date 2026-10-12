/*
 * Tugas Praktikum 1 — Modul 1: Input dan Output
 * Program 1 : Tools Nilai Mahasiswa
 * Alur : pengguna mengisi Nama Mata Kuliah dan jumlah mahasiswa (tidak dibatasi 3), lalu nama lengkap
 *        dan nilai (boleh desimal, 87.5 maupun 87,5) tiap mahasiswa; program menampilkan rekap nilai,
 *        nilai total, dan nilai rata-rata. Setiap input divalidasi dan diminta ulang bila salah;
 *        nama mahasiswa hanya boleh huruf/spasi/titik/tanda hubung/tanda petik, nama mata kuliah harus
 *        mengandung huruf, jumlah hanya angka 0-9, nilai hanya angka dengan satu pemisah desimal.
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas1 tugas1_nilai_mahasiswa.cpp && ./tugas1
 *   Arti perintah: g++ = compiler (pengubah kode C++ menjadi program yang bisa dijalankan); -std=c++17 = memakai
 *   aturan bahasa C++ versi 2017; -Wall -Wextra = tampilkan semua peringatan (warning) agar kesalahan kecil
 *   terlihat; -o tugas1 = nama program hasil kompilasi; && ./tugas1 = bila kompilasi berhasil, program tugas1
 *   langsung dijalankan.
 * Bila digabung ke program bermenu (main.cpp), kompilasi dengan tambahan -DBUILD_WITH_MENU
 * agar main() di bagian paling bawah berkas ini tidak ikut (lihat BAGIAN 7).
 *
 * Susunan berkas: BAGIAN 1 pustaka, BAGIAN 2 konstanta dan tipe data, BAGIAN 3 warna dan kotak tampilan,
 * BAGIAN 4 fungsi bantu input, BAGIAN 5 judul/petunjuk/tabel, BAGIAN 6 alur utama (fungsi run() di dalam
 * namespace task1, ditulis task1::run; tanda :: dijelaskan di BAGIAN 1), BAGIAN 7 main().
 * Komentar di seluruh berkas memakai pola "menggunakan X berfungsi untuk Y, dikarenakan Z": X = konstruksi yang
 * dipakai di baris itu, Y = gunanya di sini, Z = alasan memilihnya.
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// #include adalah perintah preprocessor (tahap yang berjalan sebelum kompilasi) yang menyalin isi sebuah header
// atau pustaka ke dalam berkas ini, sehingga fungsi dan tipe siap pakai di dalamnya bisa dipanggil. Header adalah
// berkas berisi daftar fungsi dan tipe yang disediakan sebuah pustaka. Nama di antara < > adalah pustaka standar
// C++. Awalan std:: pada nama seperti std::max berarti nama itu milik pustaka standar C++ (namespace std, wadah
// nama pustaka standar); tanda :: (operator cakupan) dibaca "milik". Namespace dijelaskan lebih lengkap tepat
// sebelum BAGIAN 2. Setiap #include di bawah diberi keterangan: apa yang dipakai dan di mana.
// Menggunakan #include <algorithm> berfungsi untuk memakai std::max dan std::min (mengambil yang terbesar/terkecil
// dari dua nilai), dikarenakan keduanya dipakai saat menghitung lebar kolom nama dan lebar kotak (BAGIAN 3 dan 6).
#include <algorithm>
// Menggunakan #include <cctype> berfungsi untuk memakai std::isalpha dan std::isdigit (memeriksa apakah satu
// karakter adalah huruf atau angka), dikarenakan dipakai oleh fungsi-fungsi pemeriksa input di BAGIAN 4.
#include <cctype>
// Menggunakan #include <cstddef> berfungsi untuk memakai tipe std::size_t (bilangan bulat tak bertanda = tidak bisa
// negatif, mulai dari 0; dipakai untuk ukuran dan indeks), dikarenakan panjang std::string dan jumlah elemen
// std::vector (keduanya dijelaskan pada #include <string> dan #include <vector> di bawah) bertipe ini.
#include <cstddef>
// Menggunakan #include <cstdlib> berfungsi untuk memakai std::exit dan EXIT_FAILURE (menghentikan program dengan
// kode gagal), std::getenv (membaca variabel lingkungan: pengaturan berbentuk NAMA=nilai yang diberikan sistem
// operasi ke program, mis. NO_COLOR atau COLUMNS), dan std::atoi (teks ke int), dikarenakan dipakai oleh
// colorsEnabled() dan terminalWidth() di BAGIAN 3 serta readLine() di BAGIAN 4.
#include <cstdlib>
// Menggunakan #include <iomanip> berfungsi untuk memakai pengatur format keluaran: std::setw (lebar kolom),
// std::left dan std::right (rata kiri/kanan), std::fixed dan std::setprecision (jumlah angka desimal), dikarenakan
// tabel rekap di BAGIAN 5 memerlukannya.
#include <iomanip>
// Menggunakan #include <iostream> berfungsi untuk memakai std::cin (masukan keyboard), std::cout (keluaran layar),
// dan std::cerr (keluaran pesan kesalahan), dikarenakan program ini berinteraksi lewat terminal.
#include <iostream>
// Menggunakan #include <sstream> berfungsi untuk memakai std::istringstream (membaca angka dari teks) dan
// std::ostringstream (menyusun teks dari angka), dikarenakan input dibaca sebagai teks lalu diubah ke angka
// (BAGIAN 4) dan angka disusun menjadi teks tabel (BAGIAN 5).
#include <sstream>
// Menggunakan #include <string> berfungsi untuk memakai std::string (tipe teks yang panjangnya bisa berubah),
// dikarenakan Nama Mata Kuliah dan nama mahasiswa adalah teks.
#include <string>
// Menggunakan #include <string_view> berfungsi untuk memakai std::string_view (tipe "pandangan" ke teks yang sudah
// ada: hanya dibaca, tanpa disalin), dikarenakan parameter teks seperti prompt (teks pertanyaan ke pengguna), judul
// kotak, dan isi kolom tabel di BAGIAN 3 sampai 5 cukup dibaca dan tidak perlu disalin.
#include <string_view>
// Menggunakan #include <vector> berfungsi untuk memakai std::vector (daftar/array yang ukurannya bisa ditentukan
// saat program berjalan), dikarenakan jumlah mahasiswa baru diketahui setelah pengguna mengetiknya.
#include <vector>
// Menggunakan #ifdef _WIN32 berfungsi untuk memilih pustaka sesuai sistem operasi saat kompilasi: #ifdef adalah
// pertanyaan ke preprocessor "apakah makro ini sudah didefinisikan?" (makro = nama pengganti teks yang dibuat dengan
// #define, dijelaskan pada #define NOMINMAX di bawah), dan _WIN32 adalah penanda (makro) yang otomatis
// didefinisikan compiler di Windows. Baris sampai #else hanya ikut dikompilasi di Windows, baris setelah #else hanya
// di macOS/Linux. Dikarenakan kedua sistem menyediakan fungsi berbeda untuk keperluan yang sama: mendeteksi
// terminal, membaca lebar jendela, mengaktifkan warna, dan mengatur UTF-8 (cara menyimpan huruf sebagai byte,
// dijelaskan di fungsi enableUtf8Console(), BAGIAN 3).
#ifdef _WIN32
    // Menggunakan #ifndef NOMINMAX berfungsi untuk mengecek apakah makro (nama pengganti teks) NOMINMAX belum pernah
    // didefinisikan; #define di bawahnya hanya dijalankan bila belum ada, dikarenakan MinGW (compiler g++ untuk
    // Windows) sudah mendefinisikannya lewat pustaka standar, dan mendefinisikan ulang makro yang sama menghasilkan
    // peringatan.
    #ifndef NOMINMAX
    // Menggunakan #define NOMINMAX 1 berfungsi untuk membuat makro NOMINMAX (#define = membuat nama pengganti teks;
    // nilai 1 di sini hanya penanda "ada") sebelum windows.h dibaca, dikarenakan tanpa itu windows.h mendefinisikan
    // makro min dan max yang menimpa nama std::max/std::min sehingga kode tidak bisa dikompilasi.
    #define NOMINMAX 1
    #endif  // akhir pengecekan NOMINMAX
    // Menggunakan #include <windows.h> berfungsi untuk memakai fungsi-fungsi Windows yang dipanggil di fungsi
    // enableUtf8Console(), colorsEnabled(), dan terminalWidth() di BAGIAN 3: GetStdHandle, GetConsoleMode dan
    // SetConsoleMode (mengaktifkan warna), SetConsoleOutputCP dan SetConsoleCP (UTF-8), serta
    // GetConsoleScreenBufferInfo (lebar jendela). Dikarenakan di Windows hal-hal itu hanya tersedia lewat pustaka ini.
    #include <windows.h>
#else  // bukan Windows: macOS atau Linux
    // Menggunakan #include <cstdio> berfungsi untuk memakai stdout (keluaran standar: tujuan cetak program, biasanya
    // layar, sama dengan tujuan std::cout) dan fileno (mengambil nomor saluran milik stdout), dikarenakan keduanya
    // dipakai bersama isatty di fungsi colorsEnabled() di BAGIAN 3.
    #include <cstdio>
    // Menggunakan #include <sys/ioctl.h> berfungsi untuk memakai ioctl, TIOCGWINSZ, dan struct winsize (membaca
    // lebar jendela terminal), dikarenakan dipakai oleh fungsi terminalWidth() di BAGIAN 3.
    #include <sys/ioctl.h>
    // Menggunakan #include <unistd.h> berfungsi untuk memakai isatty (memeriksa apakah keluaran adalah terminal,
    // bukan berkas) dan STDOUT_FILENO (nomor keluaran standar), dikarenakan dipakai oleh fungsi colorsEnabled()
    // dan terminalWidth() di BAGIAN 3.
    #include <unistd.h>
#endif  // akhir pemilihan pustaka sesuai sistem operasi

// namespace adalah wadah yang mengelompokkan nama (fungsi, konstanta) agar tidak bentrok dengan nama yang sama di
// tempat lain. Menggunakan namespace tanpa nama (anonymous namespace: kata kunci namespace langsung diikuti { tanpa
// nama) berfungsi untuk menjadikan semua konstanta dan fungsi bantu di dalamnya milik berkas ini saja (tidak
// terlihat dari berkas lain), dikarenakan tugas2_volume_tabung.cpp dan tugas3_luas_arsir.cpp (folder src yang sama)
// juga punya fungsi bantu bernama sama, mis. colorize() dan drawBox(); tanpa ini, penggabungan ke program bermenu
// main.cpp akan gagal dengan error "multiple definition" (satu nama didefinisikan lebih dari sekali). Blok ini
// berisi BAGIAN 2 sampai BAGIAN 5 dan ditutup oleh "}  // akhir namespace tanpa nama" tepat sebelum BAGIAN 6.
namespace {

    // ===== BAGIAN 2: KONSTANTA DAN TIPE DATA =====
    // constexpr adalah kata kunci untuk nilai tetap (konstanta) yang sudah pasti sejak kompilasi dan tidak bisa
    // diubah saat program berjalan; nama konstanta ditulis HURUF_BESAR sebagai kebiasaan. Menggunakan tipe data int
    // (bilangan bulat, 4 byte) berfungsi untuk batas jumlah mahasiswa, dikarenakan jumlah orang selalu bilangan
    // bulat. Keduanya dipakai saat memanggil readInteger() di fungsi run(), BAGIAN 6.
    constexpr int STUDENT_COUNT_MIN = 1;     // batas bawah 1 dikarenakan rekap tanpa mahasiswa tidak bermakna
    constexpr int STUDENT_COUNT_MAX = 1000;  // batas atas 1000 berfungsi untuk menolak salah ketik seperti 30000

    // Menggunakan tipe data double (bilangan desimal, 8 byte, presisi sekitar 15 digit) berfungsi untuk batas
    // nilai, dikarenakan nilai mahasiswa bisa desimal seperti 87.5; bila memakai int, 87.5 terpotong menjadi 87.
    // Keduanya dipakai saat memanggil readDecimal() di fungsi run(), BAGIAN 6.
    constexpr double SCORE_MIN = 0.0;    // batas bawah 0 dikarenakan nilai tidak mungkin negatif
    constexpr double SCORE_MAX = 100.0;  // batas atas 100 dikarenakan skala nilai mata kuliah 0-100

    // Lebar kolom tabel rekap, lebar garis judul, dan batas panjang input teks, dalam satuan karakter. Semuanya
    // constexpr (nilai tetap sejak kompilasi). Dua tipe dipakai: int untuk lebar kolom yang diberikan ke std::setw
    // (pengatur lebar kolom, hanya menerima int) dan untuk TITLE_WIDTH (jumlah karakter garis yang dibuat
    // std::string(TITLE_WIDTH, ch) di printRule(), BAGIAN 3), serta std::size_t untuk batas yang dibandingkan dengan
    // lebar teks (hasil fungsi displayWidth() di BAGIAN 3 bertipe std::size_t), dikarenakan membandingkan int dengan
    // size_t (bertanda dengan tak bertanda) menghasilkan peringatan compiler.
    constexpr int         NO_COLUMN_WIDTH    = 5;   // lebar kolom nomor urut ("No"): int untuk std::setw
    constexpr std::size_t NAME_COLUMN_MIN_WIDTH    = 20;  // lebar minimal kolom nama: size_t, lihat alasan di atas
    constexpr int         SCORE_COLUMN_WIDTH = 8;   // lebar kolom nilai ("Nilai"): int untuk std::setw
    constexpr int         TITLE_WIDTH       = 60;  // lebar garis judul (jumlah karakter '=' yang dicetak)
    constexpr std::size_t NAME_MAX_LENGTH = 60;  // batas panjang nama: menolak tempelan teks yang bukan nama
    constexpr std::size_t COURSE_NAME_MAX_LENGTH = 60;  // batas panjang nama mata kuliah, alasan yang sama

    // struct adalah tipe data buatan sendiri yang menggabungkan beberapa variabel (anggota) menjadi satu kesatuan.
    // Menggunakan struct Student berfungsi untuk mengemas nama dan nilai satu mahasiswa menjadi satu unit,
    // dikarenakan dengan begitu keduanya selalu berpasangan dan tidak tertukar antar mahasiswa. Anggotanya diakses
    // dengan tanda titik, misalnya student.name (lihat fungsi run() di BAGIAN 6).
    struct Student {
        std::string name;     // nama mahasiswa: std::string dikarenakan berisi huruf dan spasi, panjang tidak pasti
        double      score{};  // nilai: double dikarenakan boleh desimal; {} berfungsi memberi nilai awal 0
    };  // akhir struct Student

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, pilihan yang ditegaskan, hasil akhir, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    // Kode ANSI adalah urutan karakter khusus: ESC (karakter nomor 27, ditulis "\033": angka 033 dalam basis 8/oktal
    // = 27) lalu "[", angka, dan "m"; terminal tidak mencetaknya, melainkan mengubah tampilan teks berikutnya.
    // constexpr const char* berarti pointer (penunjuk: variabel yang menyimpan alamat memori, di sini alamat huruf
    // pertama teks) yang tetap, ke teks yang tetap: nilainya tidak berubah dan sudah pasti sejak kompilasi.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kode 0: kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // kode 1: tebal, untuk prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // kode 1;36: cyan tebal, untuk judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // kode 1;32: hijau tebal, untuk nilai total dan rata-rata
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // kode 1;31: merah tebal, untuk pesan kesalahan
    constexpr const char* ANSI_DIM    = "\033[90m";    // kode 90: abu-abu, untuk garis kotak dan garis pembatas

    // Fungsi enableUtf8Console(): void berarti tidak mengembalikan nilai, dan () kosong berarti tanpa parameter.
    // UTF-8 adalah aturan menyimpan huruf sebagai byte: huruf ASCII (128 karakter dasar: huruf Inggris, angka, tanda
    // baca) memakai 1 byte, huruf lain seperti é, garis kotak, atau aksara Asia memakai 2-4 byte; "non-ASCII"
    // berarti huruf di luar 128 karakter dasar itu. Code page adalah tabel yang dipakai konsol Windows untuk
    // mengubah byte menjadi huruf; 65001 adalah nomor code page UTF-8.
    // Berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama, dikarenakan garis
    // kotak dan nama berhuruf non-ASCII (é, ö) dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell
    // bawaan masih memakai code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku
    // untuk jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur jalankan.bat di
    // folder utama proyek. Di macOS/Linux terminal sudah UTF-8, sehingga fungsi ini tidak melakukan apa-apa.
    // Dipanggil di awal fungsi run(), BAGIAN 6.
    void enableUtf8Console() {
    // #ifdef _WIN32: dua baris kode di bawah hanya ikut dikompilasi di Windows (penjelasan #ifdef ada di BAGIAN 1).
    #ifdef _WIN32
        // CP_UTF8 adalah konstanta dari windows.h bernilai 65001 (nomor code page UTF-8).
        SetConsoleOutputCP(CP_UTF8);  // fungsi Windows: keluaran konsol diatur ke UTF-8 agar tampil benar
        SetConsoleCP(CP_UTF8);        // fungsi Windows: masukan (ketikan huruf non-ASCII) juga diatur ke UTF-8
    #endif  // akhir bagian khusus Windows
    }

    // Fungsi colorsEnabled(): bool berarti mengembalikan true (ya) atau false (tidak), tanpa parameter. Berfungsi
    // untuk menentukan apakah warna boleh dipakai. Warna dimatikan bila variabel lingkungan NO_COLOR ada (konvensi
    // umum program CLI, yaitu program berbasis teks di terminal) atau keluaran bukan terminal (mis. dialihkan ke
    // berkas), dan dipaksa aktif bila FORCE_COLOR ada. Di Windows, mode "virtual terminal" diaktifkan agar Command
    // Prompt memahami kode ANSI. Dipanggil oleh colorize() di bawah.
    bool colorsEnabled() {
        // static membuat variabel ini hidup selama program berjalan dan hanya diisi sekali (pemanggilan berikutnya
        // memakai nilai yang sama); const berarti tidak boleh diubah setelahnya. Nilai awalnya dihitung oleh lambda
        // []() { ... } (fungsi kecil tanpa nama) yang langsung dipanggil lewat () pada baris "}();" di bawah.
        // Dikarenakan pemeriksaan cukup dilakukan sekali untuk seluruh program.
        static const bool enabled = []() {
            // std::getenv membaca variabel lingkungan; hasilnya nullptr (pointer kosong) bila variabel tidak ada;
            // != berarti "tidak sama dengan". return di dalam lambda hanya keluar dari lambda: nilai yang
            // dikembalikan menjadi isi 'enabled', bukan langsung keluar dari colorsEnabled().
            if (std::getenv("NO_COLOR") != nullptr) return false;  // NO_COLOR ada: warna dimatikan
            if (std::getenv("FORCE_COLOR") != nullptr) return true;  // FORCE_COLOR ada: warna dipaksa aktif
        // #ifdef _WIN32: cara Windows sampai #else, cara macOS/Linux setelah #else.
        #ifdef _WIN32
            // GetStdHandle(STD_OUTPUT_HANDLE) meminta dari Windows sebuah pegangan (HANDLE) ke keluaran standar.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);  // HANDLE = pegangan Windows ke keluaran standar
            DWORD  mode     = 0;  // DWORD = bilangan bulat 32-bit Windows, menampung mode konsol saat ini
            // Gagal mendapat pegangan (INVALID_HANDLE_VALUE) atau gagal membaca mode (keluaran bukan konsol):
            // warna dimatikan. || adalah OR (salah satu benar), ! adalah NOT (membalik true/false). &mode = alamat
            // variabel mode (& di depan nama variabel), agar GetConsoleMode bisa menulis mode konsol ke dalamnya.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // Mode lama ditambah bit ENABLE_VIRTUAL_TERMINAL_PROCESSING (operator | = OR per bit) lalu dipasang
            // dengan SetConsoleMode; warna aktif bila pemasangan berhasil (nilai kembaliannya bukan 0).
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // isatty(fileno(stdout)) bernilai bukan 0 bila keluaran standar adalah terminal, 0 bila berkas/pipa.
            return isatty(fileno(stdout)) != 0;
        #endif  // akhir pemilihan cara deteksi sesuai sistem operasi
        }();  // () di sini langsung memanggil lambda di atas; hasilnya menjadi nilai awal 'enabled'
        return enabled;
    }  // akhir fungsi colorsEnabled()

    // Fungsi colorize(): mengembalikan std::string. Parameter const char* code adalah kode warna (salah satu
    // konstanta ANSI_... di atas, mis. ANSI_TITLE) dan std::string_view text adalah teks yang hanya dibaca.
    // Berfungsi untuk membungkus teks dengan kode warna bila warna aktif; bila tidak, teks dikembalikan apa adanya,
    // dikarenakan kode warna yang ikut tersimpan ke berkas hanya menjadi sampah karakter.
    std::string colorize(const char* code, std::string_view text) {
        // ! = NOT: bila warna tidak aktif, std::string(text) membuat std::string baru dari text (string_view) sebagai
        // nilai kembalian, dikarenakan fungsi ini harus mengembalikan std::string; teks dikembalikan polos.
        if (!colorsEnabled()) return std::string(text);  // ! = NOT: warna tidak aktif, kembalikan teks polos
        // Operator + pada std::string menyambung teks: kode warna + teks + kode reset (kembali normal).
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Fungsi isWideCodePoint(): bool; parameter unsigned (bilangan bulat tak bertanda) codePoint adalah nomor
    // karakter Unicode. Berfungsi untuk menentukan apakah sebuah code point Unicode memakai dua kolom di terminal
    // (aksara CJK, Hangul, emoji), dikarenakan tanpa ini kolom tabel bergeser untuk nama berhuruf
    // Jepang/Tionghoa/Korea atau beremoji. Dipakai oleh displayWidth() di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Rangkaian || (OR: cukup salah satu benar) dan && (AND: keduanya harus benar); a >= X && a <= Y berarti a
        // berada di antara X dan Y (keduanya termasuk). Hasilnya true bila codePoint berada di salah satu rentang
        // berikut (angka 0x... = heksadesimal):
        // 0x1100-0x115F Hangul Jamo, 0x2E80-0xA4CF aksara CJK dan sekitarnya, 0xAC00-0xD7A3 suku kata Hangul,
        // 0xF900-0xFAFF ideograf kompatibilitas, 0xFE30-0xFE4F bentuk CJK, 0xFF00-0xFF60 huruf lebar penuh,
        // 0xFFE0-0xFFE6 simbol lebar penuh, 0x1F300-0x1F64F dan 0x1F900-0x1F9FF emoji.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Fungsi displayWidth(): mengembalikan std::size_t (bilangan bulat tak bertanda untuk ukuran); parameter
    // std::string_view text adalah teks yang hanya dibaca. Berfungsi untuk menghitung lebar teks di layar: karakter
    // UTF-8 multibyte (é, ö, garis kotak) dihitung satu kolom, aksara lebar (CJK, emoji) dua kolom, dan kode warna
    // ANSI nol kolom, dikarenakan ketiganya tidak memakai ruang sebanyak byte-nya. Byte awal UTF-8 menentukan jumlah
    // byte lanjutan yang dirangkai. Dipakai oleh wrapText() dan drawBox() di BAGIAN 3, isValidCourseName() dan
    // isValidPersonName() di BAGIAN 4, formatTableRow() di BAGIAN 5, serta run() di BAGIAN 6.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // penampung lebar (jumlah kolom) yang sedang dihitung
        bool inEscape = false;  // true selama berada di dalam urutan kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point tersebut
        // Range-based for (for (elemen : kumpulan)) mengunjungi setiap byte teks satu per satu; const unsigned char c
        // membuat nilai byte 0-255, dikarenakan char biasa bisa negatif sehingga perbandingan bit di bawah salah.
        for (const unsigned char c : text) {
            // continue = lewati sisa badan loop, langsung ke byte berikutnya. Di dalam kode warna: tunggu 'm'.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC pembuka kode warna
            // (c & 0xC0) == 0x80: operator & (AND per bit) mengambil 2 bit teratas; 10xxxxxx = byte lanjutan UTF-8.
            if ((c & 0xC0) == 0x80) {                      // byte lanjutan (10xxxxxx): tambahkan 6 bit ke code point
                // << 6 menggeser bit ke kiri (memberi ruang 6 bit), | (OR per bit) menempelkan 6 bit baru (c & 0x3F);
                // --pending mengurangi sisa 1 (-- = kurang 1, ++ = tambah 1); bila sudah 0 dan code point-nya aksara
                // lebar, ++width menambah lebar 1 lagi.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;  // byte lanjutan tidak menambah kolom sendiri
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            // Pola bit byte awal menentukan panjang karakter: 110xxxxx = 2 byte, 1110xxxx = 3 byte, 11110xxx = 4 byte;
            // bit sisanya menjadi awal code point, pending = jumlah byte lanjutan yang ditunggu. Selain itu: ASCII.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }  // 2 byte: 5 bit awal
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }  // 3 byte: 4 bit awal
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }  // 4 byte: 3 bit awal
            else                         { pending = 0; }                        // ASCII 1 byte: tanpa lanjutan
        }  // akhir for
        return width;
    }  // akhir fungsi displayWidth()

    // Fungsi terminalWidth(): mengembalikan int (jumlah kolom), tanpa parameter. Berfungsi membaca lebar terminal
    // saat ini setiap kali dipanggil, sehingga tampilan ikut menyesuaikan bila jendela diubah ukurannya. Bila tidak
    // diketahui (keluaran bukan terminal), dipakai variabel lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan
    // terminal pada umumnya. Menggunakan batas bawah 50 dikarenakan di bawah itu tabel tidak bisa rapi.
    // Dipakai oleh drawBox() dan printError() di bawah.
    int terminalWidth() {
        int cols = 0;  // jumlah kolom; 0 berarti "belum diketahui"
    // #ifdef _WIN32: cara Windows sampai #else, cara macOS/Linux setelah #else.
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;  // struct Windows penampung informasi layar konsol, diisi fungsi di bawah
        // GetConsoleScreenBufferInfo mengisi 'info'; bila berhasil, lebar = kolom kanan - kolom kiri + 1 (keduanya
        // termasuk). &info = alamat variabel info, agar fungsi bisa menulis ke dalamnya.
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        struct winsize ws;  // struct dari <sys/ioctl.h> penampung ukuran jendela (baris dan kolom)
        // ioctl mengisi ws dengan ukuran jendela; hasil 0 berarti berhasil, lalu ws.ws_col = jumlah kolom.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;  // ioctl TIOCGWINSZ = tanya ukuran jendela
    #endif  // akhir pemilihan cara membaca lebar sesuai sistem operasi
        // Belum diketahui: coba variabel lingkungan COLUMNS; std::atoi mengubah teksnya menjadi int (0 bila bukan
        // angka). const char* env = pointer ke teks hasil getenv, nullptr bila variabel tidak ada.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // masih belum diketahui: pakai 80, lebar bawaan terminal pada umumnya
        return std::max(50, cols);  // std::max memilih yang terbesar: hasil tidak pernah kurang dari 50 kolom
    }  // akhir fungsi terminalWidth()

    // Fungsi wrapText(): mengembalikan std::vector<std::string> (vector yang tiap elemennya std::string; tipe isi
    // ditulis di antara < >), di sini daftar baris teks. Parameter: const std::string& text (teks asli; & =
    // referensi, yaitu nama lain untuk variabel milik pemanggil sehingga tidak disalin; const = tidak boleh diubah),
    // maxWidth (lebar maksimal dalam kolom), hangingIndent (jumlah spasi indentasi gantung). Berfungsi memotong satu
    // baris teks yang lebih panjang dari maxWidth kolom menjadi beberapa baris, memotong pada spasi, dan memberi
    // baris lanjutan indentasi 'gantung' agar tetap sejajar dengan isi setelah label. Dipakai drawBox() dan
    // printError() di bawah, dikarenakan teks tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil pemotongan; diisi satu per satu lewat push_back
        // Bila indentasi + 10 melebihi lebar (jendela sempit), indentasi dikecilkan menjadi 2 spasi saja.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // salinan teks yang belum terpotong; dipendekkan tiap putaran
        // while = ulangi selama kondisi benar: sisa teks masih lebih lebar dari maxWidth (dihitung per kolom).
        while (displayWidth(remaining) > maxWidth) {
            // col menghitung kolom; cutPos menyimpan posisi spasi tempat memotong; std::string::npos = "tidak ada".
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama di dalam kode warna ANSI (tidak dihitung sebagai kolom)
            // for dengan indeks i (dari 0 sampai sebelum remaining.size()): menelusuri byte demi byte, mencari spasi
            // terakhir sebelum batas lebar (dihitung per kolom, bukan per byte).
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char> mengubah char (bisa negatif) menjadi nilai 0-255 agar uji bit benar.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // di dalam kode warna: tunggu 'm'
                if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = ESC, awal kode warna
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8: bukan kolom baru
                if (col >= maxWidth) break;  // break = keluar dari loop: sudah melewati batas lebar
                if (c == ' ' && col > hangingIndent) cutPos = i;  // spasi setelah indentasi: calon titik potong
                ++col;
            }  // akhir for
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            result.push_back(remaining.substr(0, cutPos));  // substr(0, cutPos) = potongan sebelum spasi
            // Sisa teks setelah spasi (substr(cutPos + 1)) diberi awalan spasi sebanyak hangingIndent, dibuat
            // dengan std::string(jumlah, ' ').
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }  // akhir while
        result.push_back(remaining);  // sisa terakhir (sudah muat) menjadi baris penutup
        return result;
    }  // akhir fungsi wrapText()

    // Fungsi drawBox(): void (tanpa nilai kembalian). Parameter std::string_view title (judul kotak) dan
    // const std::vector<std::string>& lines (daftar baris isi, dibaca lewat referensi tanpa disalin). Berfungsi untuk
    // menggambar kotak berjudul berisi baris-baris teks. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah
    // melebihi lebar terminal: baris yang terlalu panjang dibungkus lebih dulu (wrapText() di atas), dengan
    // indentasi gantung sejajar label bila baris berpola "Label : isi". Menggunakan kotak dikarenakan informasi
    // (petunjuk, menu, hasil) jadi terpisah jelas dari pertanyaan. Dipakai printInstructions() di BAGIAN 5 dan
    // run() di BAGIAN 6.
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Lebar isi maksimal = lebar terminal dikurangi 5 (2 tepi kotak + 2 margin + 1 kolom cadangan), dikarenakan
        // sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela. static_cast<std::size_t>
        // mengubah hasil int menjadi size_t agar bisa dibandingkan dengan hasil displayWidth().
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris-baris isi setelah dibungkus; bisa lebih banyak dari lines
        // Range-based for: b adalah setiap baris dalam lines; const std::string& = dibaca lewat referensi tanpa salin.
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan: 2 spasi
            const std::size_t labelPos = b.find(" : ");  // posisi " : " pemisah label dan isi; npos bila tidak ada
            // Bila ada label dan lebarnya paling banyak 28 kolom, baris lanjutan disejajarkan dengan isi setelah " : ".
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // sejajar dengan isi setelah " : "
            }
            // Setiap potongan hasil wrapText() ditambahkan ke contentLines lewat push_back (tambah di akhir vector).
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }  // akhir for
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Lebar dalam = baris terpanjang + 2 (margin kiri dan kanan); std::max menyimpan yang terbesar.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min: tidak melebihi lebar isi maksimal + 2 margin
        std::string filler;  // garis pengisi di sebelah kanan judul pada tepi atas
        // Notasi Unicode (backslash-u lalu 4 digit heksadesimal) menulis karakter lewat nomornya: U+2500 = garis
        // mendatar (─); += menyambung satu garis ke filler tiap putaran.
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // std::cout << x mencetak x ke layar; beberapa << berurutan mencetak bagian-bagiannya berurutan, dan '\n' =
        // ganti baris (berbeda dengan << 6 di displayWidth() di atas, yang menggeser bit). Tepi atas: sudut kiri
        // atas U+250C dan garis, judul berwarna, garis pengisi, sudut kanan atas U+2510.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Setiap baris isi: tepi kiri U+2502, spasi + teks, spasi pengisi sampai selebar kotak, tepi kanan U+2502.
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;  // margin kiri 1 spasi di depan teks
            const std::size_t contentWidth = displayWidth(rowText);  // lebar tampil baris ini, tanpa kode warna
            // Operator ternary (kondisi ? nilaiJikaBenar : nilaiJikaSalah): jumlah spasi pengisi = width - contentWidth
            // bila width lebih besar, selain itu 0, dikarenakan size_t tidak boleh negatif.
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }  // akhir for
        std::string bottomLine;  // garis mendatar tepi bawah
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";  // ─ sebanyak lebar dalam kotak
        // Tepi bawah: sudut kiri bawah U+2514, garis, sudut kanan bawah U+2518, semuanya abu-abu.
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }  // akhir fungsi drawBox()

    // Fungsi printError(): void; parameter const std::string& text (pesan, dibaca lewat referensi). Berfungsi
    // mencetak pesan kesalahan (merah) yang dibungkus sesuai lebar terminal, dengan baris lanjutan menjorok 6 spasi
    // sejajar teks setelah tanda "[!]", dikarenakan pesan panjang tidak boleh terpotong jendela sempit. Dipakai
    // readCourseName(), readPersonName(), readInteger(), dan readDecimal() di BAGIAN 4 (readLine() tidak memakainya,
    // dikarenakan pesannya dikirim ke std::cerr).
    void printError(const std::string& text) {
        // Range-based for atas hasil wrapText(): lebar maksimal = lebar terminal - 1, indentasi gantung 6 kolom.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';  // tiap potongan dicetak merah tebal, lalu ganti baris
        }
    }

    // Fungsi printRule(): void; parameter char ch (satu karakter, mis. '='). Berfungsi untuk mencetak satu baris
    // garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter ch. Dipakai printTitle() di BAGIAN 5.
    void printRule(char ch) {
        // std::string(TITLE_WIDTH, ch) membuat teks berisi ch diulang TITLE_WIDTH kali; '\n' = ganti baris.
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Menggunakan fungsi-fungsi terpisah berfungsi untuk menghindari penulisan kode yang sama berulang,
    // dikarenakan pembacaan input dipakai di banyak tempat (mata kuliah, jumlah, nama, nilai).

    // Fungsi readLine(): mengembalikan std::string; parameter std::string_view prompt (teks pertanyaan, hanya
    // dibaca). Berfungsi untuk menampilkan prompt (dicetak tebal) lalu membaca satu baris ketikan pengguna. Fungsi
    // ini menjadi dasar semua fungsi pembaca input lain di bagian ini.
    std::string readLine(std::string_view prompt) {
        std::string line;                   // menggunakan std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt); // tebal berfungsi agar pertanyaan mudah dibedakan dari keterangan
        // Menggunakan std::getline berfungsi untuk membaca satu baris penuh termasuk spasi (std::cin >>
        // berhenti di spasi pertama). Pemeriksaan ! (NOT) berfungsi untuk menangkap kegagalan getline,
        // dikarenakan getline bernilai gagal bila input ditutup (Ctrl+D/Ctrl+Z), agar program tidak terjebak.
        if (!std::getline(std::cin, line)) {
            // std::cerr adalah saluran keluaran khusus pesan kesalahan (terpisah dari std::cout); "\n" = ganti baris.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            std::exit(EXIT_FAILURE);  // std::exit menghentikan program; EXIT_FAILURE = kode gagal ke sistem operasi
        }
        return line;
    }  // akhir fungsi readLine()

    // Fungsi normalizeSpaces(): mengembalikan std::string; parameter const std::string& text (const& = dibaca lewat
    // referensi tanpa disalin, tidak diubah). Berfungsi untuk membuang spasi/tab di awal dan akhir teks ("  Budi  "
    // menjadi "Budi") dan merapatkan spasi beruntun di tengah menjadi satu spasi, dikarenakan ketikan pengguna sering
    // membawa spasi tak sengaja. Dipakai readCourseName(), readPersonName(), readInteger(), dan readDecimal() di bawah.
    std::string normalizeSpaces(const std::string& text) {
        std::string result;                  // std::string penampung teks yang sudah dirapikan
        bool pendingSpace = false;         // menggunakan bool berfungsi untuk menandai ada spasi yang belum ditulis
        for (const char c : text) {         // range-based for: memeriksa setiap karakter satu per satu
            // Spasi, tab ('\t'), atau CR ('\r', sisa akhir baris Windows) tidak langsung ditulis, hanya ditandai.
            if (c == ' ' || c == '\t' || c == '\r') {
                pendingSpace = !result.empty();  // true hanya bila result sudah berisi: spasi di awal diabaikan
            } else {
                if (pendingSpace) result += ' ';  // beberapa spasi beruntun ditulis sebagai satu spasi saja
                result += c;  // += menambahkan karakter c ke akhir result
                pendingSpace = false;
            }
        }  // akhir for
        return result;                       // spasi di akhir otomatis terbuang dikarenakan tidak pernah ditulis
    }  // akhir fungsi normalizeSpaces()

    // Fungsi isAllDigits(): bool; parameter const std::string& text. Berfungsi untuk memeriksa apakah teks hanya
    // berisi angka 0-9 (tanpa tanda, titik, huruf, atau spasi). Dipakai isDecimalFormat() dan readInteger() di bawah.
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;     // teks kosong bukan bilangan (empty() = true bila panjangnya 0)
        for (const char c : text) {         // range-based for: setiap karakter harus digit
            // std::isdigit bernilai benar bila karakter adalah angka 0-9; static_cast<unsigned char> dikarenakan
            // isdigit hanya aman untuk nilai 0-255 (char bisa negatif untuk byte non-ASCII).
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;  // semua karakter lolos
    }

    // Fungsi isDecimalFormat(): bool; parameter const std::string& text. Berfungsi untuk memeriksa format bilangan
    // desimal: angka, boleh diikuti SATU pemisah titik atau koma lalu angka lagi (87, 87.5, 87,5). Bentuk lain
    // seperti 1e2, +5, .5, 5., 8.7.5, atau 87 5 ditolak, dikarenakan bukan cara menulis nilai yang diharapkan dari
    // pengguna. Dipakai parseDecimal() di bawah.
    bool isDecimalFormat(const std::string& text) {
        // auto = tipe ditentukan compiler dari nilainya (di sini std::size_t); find_first_of(".,") mencari posisi
        // titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        const auto separator = text.find_first_of(".,");         // posisi titik/koma pertama; npos bila tidak ada
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        // substr(0, separator) = bagian sebelum pemisah; substr(separator + 1) = bagian sesudahnya sampai akhir.
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // sesudahnya juga; pemisah kedua ikut tertolak
    }

    // Fungsi isValidUtf8(): bool; parameter const std::string& text. Berfungsi untuk memeriksa apakah teks adalah
    // UTF-8 yang sah: setiap karakter non-ASCII terdiri dari satu byte awal (0xC2-0xF4) diikuti 1-3 byte lanjutan
    // (0x80-0xBF). Menggunakan pemeriksaan ini dikarenakan byte acak seperti 0xFF (bukan huruf, bukan UTF-8) harus
    // ditolak, sementara huruf seperti é atau ö (UTF-8 sah) tetap diterima. Dipakai isValidCourseName() dan
    // isValidPersonName() di bawah.
    bool isValidUtf8(const std::string& text) {
        std::size_t i = 0;  // indeks byte yang sedang diperiksa
        while (i < text.size()) {  // while: ulangi selama masih ada byte yang belum diperiksa (size() = jumlah byte)
            // static_cast<unsigned char>: byte dibaca sebagai 0-255 agar perbandingan rentang di bawah benar.
            const unsigned char lead = static_cast<unsigned char>(text[i]);
            std::size_t extra = 0;                               // jumlah byte lanjutan yang harus mengikuti
            // Rantai if / else if: rentang byte awal menentukan panjang karakter (angka 0x.. = heksadesimal).
            if (lead < 0x80)                       extra = 0;    // ASCII biasa
            else if (lead >= 0xC2 && lead <= 0xDF) extra = 1;    // 2 byte, mis. é ö ü
            else if (lead >= 0xE0 && lead <= 0xEF) extra = 2;    // 3 byte, mis. aksara Asia
            else if (lead >= 0xF0 && lead <= 0xF4) extra = 3;    // 4 byte, mis. emoji
            else return false;                                   // 0x80-0xC1 dan 0xF5-0xFF bukan byte awal yang sah
            if (i + extra >= text.size()) return false;             // byte lanjutan terpotong di akhir teks
            for (std::size_t k = 1; k <= extra; ++k) {  // periksa setiap byte lanjutan ke-k
                const unsigned char cont = static_cast<unsigned char>(text[i + k]);
                if (cont < 0x80 || cont > 0xBF) return false;    // byte lanjutan harus 10xxxxxx
            }
            i += extra + 1;  // lompat ke byte awal karakter berikutnya
        }  // akhir while
        return true;
    }  // akhir fungsi isValidUtf8()

    // Fungsi isValidCourseName(): bool; parameter const std::string& text. Berfungsi untuk memeriksa apakah teks
    // layak sebagai nama mata kuliah: harus mengandung huruf, dan hanya boleh berisi huruf, angka (Kalkulus 2),
    // spasi, atau tanda . , & / ( ) - ' ; tidak boleh terlalu panjang. Dipakai readCourseName() di bawah.
    bool isValidCourseName(const std::string& text) {
        // Tiga syarat gugur dengan || (OR): kosong, lebih dari COURSE_NAME_MAX_LENGTH kolom, atau bukan UTF-8 sah.
        if (text.empty() || displayWidth(text) > COURSE_NAME_MAX_LENGTH || !isValidUtf8(text)) return false;
        bool hasLetter = false;  // menggunakan bool berfungsi untuk menandai sudah ada minimal satu huruf
        for (const char c : text) {  // range-based for: periksa setiap karakter
            // static_cast<unsigned char> mengubah c menjadi 0-255, dikarenakan isalpha/isdigit hanya aman untuk 0-255.
            const unsigned char uc = static_cast<unsigned char>(c);
            // Byte >= 0x80 adalah bagian huruf UTF-8 non-ASCII (mis. é, ö), dikarenakan std::isalpha hanya mengenal
            // ASCII. Huruf: tandai hasLetter, lalu continue (lanjut ke karakter berikutnya).
            if (std::isalpha(uc) || uc >= 0x80) { hasLetter = true; continue; }
            // Angka atau salah satu tanda yang diizinkan: lewati (continue). '\'' = karakter petik tunggal.
            if (std::isdigit(uc) || c == ' ' || c == '.' || c == ',' || c == '&' || c == '/'
                || c == '(' || c == ')' || c == '-' || c == '\'') continue;
            return false;  // karakter lain (mis. #, @, !) bukan bagian nama mata kuliah
        }  // akhir for
        return hasLetter;  // "123" ditolak dikarenakan tidak mengandung huruf sama sekali
    }  // akhir fungsi isValidCourseName()

    // Fungsi readCourseName(): mengembalikan std::string; parameter std::string_view prompt. Berfungsi untuk meminta
    // nama mata kuliah dan mengulanginya sampai lolos isValidCourseName() di atas. Dipanggil run() di BAGIAN 6.
    std::string readCourseName(std::string_view prompt) {
        while (true) {  // while (true) = ulang terus sampai input sah; keluar lewat return
            const std::string text = normalizeSpaces(readLine(prompt));  // const: hasilnya tidak diubah lagi
            if (isValidCourseName(text)) return text;  // sah: kembalikan dan keluar dari fungsi
            // printError() di BAGIAN 3 mencetak pesan merah; std::to_string mengubah angka batas menjadi teks, dan
            // operator + menyambung potongan pesan; dua teks literal (teks di antara tanda kutip ganda yang ditulis
            // langsung di kode) yang berdampingan otomatis digabung compiler menjadi satu.
            printError("  [!] Nama Mata Kuliah harus mengandung huruf dan hanya boleh berisi huruf, angka, spasi,"
                                        " atau tanda . , & / ( ) - ' (maksimal " + std::to_string(COURSE_NAME_MAX_LENGTH)
                                        + " karakter). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readCourseName()

    // Fungsi isValidPersonName(): bool; parameter const std::string& name. Berfungsi untuk memeriksa apakah teks
    // layak sebagai nama orang: hanya huruf, spasi, titik (A. Rizky), tanda hubung (Anne-Marie), atau tanda petik
    // (Nur'aini); harus diawali huruf dan tidak terlalu panjang. Dengan ini ketikan seperti "Luthfi 87.5" atau "123"
    // ditolak, dikarenakan angka dan simbol bukan bagian nama. Dipakai readPersonName() di bawah.
    bool isValidPersonName(const std::string& name) {
        // Tiga syarat gugur dengan || (OR): kosong, lebih dari NAME_MAX_LENGTH kolom, atau bukan UTF-8 sah.
        if (name.empty() || displayWidth(name) > NAME_MAX_LENGTH || !isValidUtf8(name)) return false;
        // Menggunakan static_cast<unsigned char> dikarenakan std::isalpha hanya aman untuk nilai 0-255 (char bisa
        // negatif). name.front() = karakter pertama teks.
        const unsigned char first = static_cast<unsigned char>(name.front());
        if (!std::isalpha(first) && first < 0x80) return false;  // karakter pertama harus huruf (ASCII atau UTF-8)
        for (const char c : name) {  // range-based for: memeriksa setiap karakter; const dikarenakan hanya dibaca
            const unsigned char uc = static_cast<unsigned char>(c);  // 0-255 agar aman untuk std::isalpha
            const bool isLetter = std::isalpha(uc) != 0 || uc >= 0x80;  // >= 0x80: byte huruf UTF-8 non-ASCII
            const bool isAllowedMark = (c == ' ' || c == '.' || c == '-' || c == '\'');  // tanda yang diizinkan
            if (!isLetter && !isAllowedMark) return false;  // ada angka atau simbol lain -> bukan nama
        }  // akhir for
        return true;
    }  // akhir fungsi isValidPersonName()

    // Fungsi readPersonName(): mengembalikan std::string; parameter std::string_view prompt. Berfungsi untuk meminta
    // nama mahasiswa dan mengulanginya sampai lolos isValidPersonName() di atas. Dipanggil run() di BAGIAN 6.
    std::string readPersonName(std::string_view prompt) {
        while (true) {  // while (true) = diulang terus, dikarenakan nama yang salah harus diminta lagi
            const std::string name = normalizeSpaces(readLine(prompt));  // baca satu baris, lalu rapikan spasinya
            if (isValidPersonName(name)) return name;  // sah: kembalikan dan keluar dari fungsi
            // printError() mencetak pesan merah; std::to_string mengubah batas angka menjadi teks, + menyambung pesan.
            printError("  [!] Nama hanya boleh berisi huruf, spasi, titik, tanda hubung (-), atau tanda petik ('),"
                                        " diawali huruf, maksimal " + std::to_string(NAME_MAX_LENGTH)
                                        + " karakter. Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readPersonName()

    // Fungsi readInteger(): mengembalikan int; parameter std::string_view prompt (pertanyaan), int minValue dan
    // int maxValue (batas bawah dan atas). Berfungsi untuk meminta bilangan bulat dari minValue sampai maxValue
    // (keduanya termasuk); dipakai untuk jumlah mahasiswa (dipanggil run() di BAGIAN 6).
    int readInteger(std::string_view prompt, int minValue, int maxValue) {
        while (true) {  // while (true) = diulang terus, dikarenakan input salah harus diminta lagi
            // Menggunakan pembacaan sebagai teks lebih dulu (readLine(), lalu diperiksa) berfungsi untuk menolak
            // ketikan salah (abc, 2.5) tanpa merusak kondisi std::cin: bila std::cin >> angka menerima huruf,
            // std::cin masuk keadaan gagal dan menolak semua input berikutnya sampai dipulihkan dengan
            // std::cin.clear().
            const std::string text = normalizeSpaces(readLine(prompt));
            std::istringstream parser(text);  // istringstream: aliran masukan dari teks, untuk membaca angka darinya
            int number = 0;  // int dikarenakan yang diminta bilangan bulat; 0 agar tidak berisi nilai acak
            // Lima syarat dengan && (AND) berfungsi untuk memastikan input sah: teks hanya berisi digit 0-9 (menolak
            // +3, 3.0, 1e3, "3 orang"), terbaca sebagai bilangan bulat (parser >> number: >> mengambil angka dari
            // aliran ke variabel number, gagal bila tidak bisa), tanpa sisa karakter ((parser >> std::ws).eof():
            // std::ws membuang spasi, eof() = true bila aliran sudah habis), tidak kurang dari minValue, dan tidak
            // lebih dari maxValue.
            if (isAllDigits(text) && parser >> number && (parser >> std::ws).eof() && number >= minValue && number <= maxValue) {
                return number;  // sah: kembalikan dan keluar dari fungsi
            }
            // printError() mencetak pesan merah; std::to_string mengubah batas angka menjadi teks, + menyambung pesan.
            printError("  [!] Harus bilangan bulat " + std::to_string(minValue) + "-" + std::to_string(maxValue)
                                        + " (hanya angka). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readInteger()

    // Fungsi parseDecimal(): bool (true bila teks sah). Parameter std::string text (salinan teks, boleh diubah di
    // dalam fungsi) dan double& result (referensi ke variabel milik pemanggil). Berfungsi untuk mengubah teks
    // menjadi bilangan desimal. Parameter double& result berfungsi untuk mengirim angka kembali ke pemanggil,
    // dikarenakan nilai kembalian (bool) sudah dipakai untuk status sah/tidak. Dipakai readDecimal() di bawah.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang diterima
        for (char& c : text) {        // char& (referensi): mengubah karakter langsung pada teksnya
            if (c == ',') c = '.';    // koma diganti titik dikarenakan C++ hanya mengenal penulisan 87.5
        }  // akhir for
        std::istringstream parser(text);  // menggunakan istringstream berfungsi untuk membaca angka dari teks
        // parser >> result membaca angka desimal ke result; (parser >> std::ws).eof() memastikan tidak ada sisa.
        return (parser >> result) && (parser >> std::ws).eof();  // sah bila terbaca sebagai angka tanpa sisa
    }

    // Fungsi readDecimal(): mengembalikan double; parameter std::string_view prompt, double minValue dan maxValue.
    // Berfungsi untuk meminta nilai desimal dari minValue sampai maxValue (keduanya termasuk); koma (87,5) diterima.
    // Dipanggil run() di BAGIAN 6 untuk nilai mahasiswa.
    double readDecimal(std::string_view prompt, double minValue, double maxValue) {
        while (true) {  // while (true) = diulang terus sampai nilai sah; keluar lewat return
            double score = 0.0;  // double dikarenakan nilai boleh desimal; 0.0 = nilai awal yang jelas
            // parseDecimal() mengisi score lewat referensi dan mengembalikan true bila teks sah; && (AND) lalu
            // memastikan score berada dalam rentang.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), score) && score >= minValue && score <= maxValue) {
                return score;
            }
            // Menggunakan ostringstream terpisah berfungsi agar batas tampil "0-100", bukan "0.000000-100.000000",
            // dikarenakan std::to_string untuk double selalu menulis 6 angka desimal.
            std::ostringstream range;  // std::ostringstream = aliran keluaran ke teks (menyusun teks dari angka)
            range << minValue << "-" << maxValue;  // << menulis angka dan "-" ke dalam range
            // range.str() mengambil teks hasil susunan; printError() mencetak pesan merah.
            printError("  [!] Nilai harus angka " + range.str() + ", hanya digit dengan satu pemisah desimal"
                                        " (mis. 87.5 atau 87,5). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readDecimal()

    // ===== BAGIAN 5: JUDUL, PETUNJUK, DAN TABEL REKAP =====

    // Fungsi printTitle(): void; parameter std::string_view text (judul). Berfungsi untuk mencetak judul program
    // (cyan tebal) di antara dua garis pembatas '=' (printRule() di BAGIAN 3). Dipanggil run() di BAGIAN 6.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong sebelum judul
        printRule('=');  // garis '=' sepanjang TITLE_WIDTH
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // judul cyan tebal, menjorok 2 spasi
        printRule('=');  // garis penutup
    }

    // Fungsi printInstructions(): void, tanpa parameter. Berfungsi untuk menampilkan petunjuk pengisian dalam kotak
    // sebelum pertanyaan pertama, dikarenakan pengguna perlu tahu bentuk input yang diterima (huruf/angka, rentang,
    // cara menulis desimal) agar tidak salah ketik. Dipanggil run() di BAGIAN 6.
    void printInstructions() {
        // drawBox() di BAGIAN 3 menggambar kotak; daftar baris ditulis langsung dengan { ... } (initializer list)
        // yang menjadi std::vector<std::string>. Angka 1-1000 dan 0-100 mengikuti konstanta di BAGIAN 2.
        drawBox("Petunjuk Pengisian", {
            "- Nama Mata Kuliah : Huruf, boleh Angka dan Spasi",
            "                     (contoh: Algoritma dan Pemrograman, Kalkulus 2)",
            "- Jumlah Mahasiswa : Angka bulat 1-1000",
            "                     (contoh: 3)",
            "- Nama Lengkap     : Huruf dan Spasi; tanda titik (.), tanda hubung (-), dan tanda petik (') diperbolehkan",
            "                     (contoh: Muhammad A. Rizky, Siti Nur'aini)",
            "- Nilai            : Angka 0-100; nilai desimal ditulis dengan tanda titik atau koma",
            "                     (contoh: 87, 87,5 atau 87.5)",
            "Ketik jawaban lalu tekan Enter. Input yang tidak sesuai akan diminta ulang."
        });
        std::cout << '\n';  // baris kosong setelah kotak
    }  // akhir fungsi printInstructions()

    // Fungsi formatTableRow(): mengembalikan std::string (satu baris tabel). Parameter std::string_view no, name,
    // score (teks tiap kolom, hanya dibaca) dan int nameWidth (lebar kolom nama). Berfungsi untuk membuat satu
    // baris tabel (nomor, nama, nilai) dengan lebar kolom tetap, siap dimasukkan ke kotak. Menggunakan ostringstream
    // berfungsi menyusun teks tanpa mengubah format std::cout. Dipanggil run() di BAGIAN 6.
    std::string formatTableRow(std::string_view no, std::string_view name, std::string_view score, int nameWidth) {
        // Kolom nama diisi spasi berdasarkan lebar tampil (displayWidth), bukan std::setw yang menghitung byte,
        // dikarenakan nama dengan huruf non-ASCII (é, ö) memakai 2 byte per huruf dan akan membuat kolom bergeser.
        const std::size_t shownWidth = displayWidth(name);  // lebar nama di layar (dalam kolom)
        const std::size_t targetWidth = static_cast<std::size_t>(nameWidth);  // lebar kolom diubah ke size_t
        // Operator ternary (kondisi ? benar : salah): spasi tambahan = targetWidth - shownWidth bila positif, selain
        // itu 0. std::string(n, ' ') membuat n spasi; + menyambungnya di belakang nama.
        const std::string namePadded = std::string(name) + std::string(targetWidth > shownWidth ? targetWidth - shownWidth : 0, ' ');
        std::ostringstream line;  // aliran keluaran ke teks untuk menyusun baris
        // std::left/std::right = rata kiri/kanan di dalam kolom; std::setw(n) = lebar kolom n karakter, berlaku
        // untuk item berikutnya saja; << menulis item ke dalam line.
        line << std::left  << std::setw(NO_COLUMN_WIDTH)    << no
             << namePadded
             << std::right << std::setw(SCORE_COLUMN_WIDTH) << score;
        return line.str();  // str() mengambil teks hasil susunan
    }  // akhir fungsi formatTableRow()

    // Fungsi formatTwoDecimals(): mengembalikan std::string; parameter double number. Berfungsi untuk mengubah angka
    // menjadi teks dengan tepat 2 desimal (87.5 -> "87.50"). Dipanggil run() di BAGIAN 6.
    std::string formatTwoDecimals(double number) {
        std::ostringstream text;  // aliran keluaran ke teks
        // std::fixed = tulis desimal biasa (bukan notasi ilmiah 8.75e+01); std::setprecision(2) = tepat 2 angka
        // di belakang titik desimal.
        text << std::fixed << std::setprecision(2) << number;
        return text.str();  // str() mengambil teks hasil susunan
    }

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 1 =====
// Menggunakan namespace task1 (namespace bernama: mengelompokkan nama di bawah nama "task1") berfungsi untuk memberi
// nama yang khas pada fungsi run(), dikarenakan program bermenu (main.cpp, di folder yang sama) memanggilnya sebagai
// task1::run() dan tugas lain punya run() juga. Blok ini ditutup oleh "}  // akhir namespace task1".
namespace task1 {

    // Fungsi run(): void, tanpa parameter. Berfungsi sebagai alur utama Tugas 1: judul, petunjuk, input, hitung,
    // lalu rekap. Dipanggil oleh main() di BAGIAN 7, atau oleh menu di main.cpp.
    void run() {
        // enableUtf8Console() (BAGIAN 3) dipanggil di sini agar program mandiri (tanpa menu) juga benar; aman
        // dipanggil berulang.
        enableUtf8Console();
        printTitle("PROGRAM 1 : TOOLS NILAI MAHASISWA");  // judul program (printTitle() di BAGIAN 5)
        std::cout << '\n';  // baris kosong
        printInstructions();  // kotak petunjuk (printInstructions() di BAGIAN 5)

        // --- Pengaturan awal: mata kuliah dan jumlah mahasiswa ditentukan oleh pengguna ---
        // Menggunakan std::string dikarenakan Nama Mata Kuliah adalah teks; const dikarenakan setelah diisi
        // tidak diubah lagi. readCourseName() (BAGIAN 4) mengulang pertanyaan sampai jawabannya sah.
        const std::string courseName = readCourseName("Nama Mata Kuliah : ");
        // Menggunakan int dikarenakan jumlah mahasiswa bilangan bulat; readInteger() (BAGIAN 4) membatasinya
        // STUDENT_COUNT_MIN sampai STUDENT_COUNT_MAX (1-1000, BAGIAN 2) berfungsi untuk menolak salah ketik.
        const int studentCount = readInteger("Jumlah Mahasiswa : ", STUDENT_COUNT_MIN, STUDENT_COUNT_MAX);

        // Menggunakan std::vector berfungsi untuk membuat daftar mahasiswa sebesar jumlah yang diketik,
        // dikarenakan jumlah elemen baru diketahui saat program berjalan sehingga tidak bisa memakai array
        // berukuran tetap. std::vector<Student> = vector yang tiap elemennya struct Student (BAGIAN 2); angka di
        // dalam ( ) = jumlah elemen awal. Menggunakan static_cast<std::size_t> dikarenakan ukuran vector bertipe
        // size_t, sedangkan studentCount bertipe int.
        std::vector<Student> students(static_cast<std::size_t>(studentCount));

        // --- Tahap input: isi nama dan nilai setiap mahasiswa ---
        std::cout << '\n' << colorize(ANSI_TITLE, "Input Nilai " + courseName) << '\n';  // sub-judul cyan tebal
        int rowNumber = 1;  // int dikarenakan nomor urut bulat; mulai 1 dikarenakan manusia menghitung dari 1
        // Menggunakan range-based for (for (elemen : daftar)) berfungsi untuk mengunjungi setiap mahasiswa tanpa
        // mengurus indeks; Student& (reference) berfungsi agar data asli yang terisi, bukan salinannya.
        for (Student& student : students) {
            // Label "Mahasiswa ke-N dari M" dicetak tebal; std::to_string mengubah angka menjadi teks, + menyambung.
            std::cout << colorize(ANSI_BOLD, "Mahasiswa ke-" + std::to_string(rowNumber) + " dari " + std::to_string(studentCount)) << '\n';
            // Operator . (titik) mengakses anggota struct: student.name dan student.score (struct Student, BAGIAN 2).
            // readPersonName() dikarenakan nama harus berupa huruf; readDecimal() dikarenakan nilai boleh desimal,
            // dibatasi SCORE_MIN sampai SCORE_MAX (0-100). Keduanya ada di BAGIAN 4.
            student.name  = readPersonName   ("  Nama Lengkap  : ");                       // isi nama (std::string)
            student.score = readDecimal("  Nilai (0-100) : ", SCORE_MIN, SCORE_MAX);  // isi nilai (double)
            ++rowNumber;  // menggunakan ++ berfungsi untuk menambah nomor urut sebesar 1
        }  // akhir for

        // --- Tahap perhitungan: total, rata-rata, dan lebar kolom nama ---
        double      total     = 0.0;             // double dikarenakan jumlah nilai desimal harus tetap desimal
        std::size_t nameWidth = NAME_COLUMN_MIN_WIDTH;  // size_t dikarenakan dibandingkan dengan hasil displayWidth()
        // Range-based for dengan const Student& (referensi hanya-baca): data dibaca tanpa disalin dan tidak diubah.
        for (const Student& student : students) {
            total += student.score;  // += menjumlahkan nilai ke total (singkatan total = total + student.score)
            // std::max menyimpan lebar terbesar antara lebar saat ini dan lebar nama + 2 (jarak ke kolom nilai).
            nameWidth = std::max(nameWidth, displayWidth(student.name) + 2);
        }  // akhir for
        // Menggunakan pembagian double dikarenakan hasilnya harus desimal (pada int, 11/2 = 5; pada double 5.5).
        // students.size() = jumlah elemen vector (size_t); static_cast<double> mengubahnya menjadi double dulu.
        const double average = total / static_cast<double>(students.size());

        // --- Tahap output: tabel rekap disusun ke vector, lalu digambar dalam satu kotak ---
        const int nameColumnWidth = static_cast<int>(nameWidth);  // static_cast<int>: std::setw hanya menerima int
        // std::string(n, '-') membuat garis '-' sepanjang n = jumlah lebar ketiga kolom (garis selebar tabel).
        const std::string tableRule(NO_COLUMN_WIDTH + nameColumnWidth + SCORE_COLUMN_WIDTH, '-');

        std::vector<std::string> summaryLines;  // vector dikarenakan jumlah baris mengikuti jumlah mahasiswa
        // push_back menambahkan satu baris di akhir vector; + menyambung label dengan isinya.
        summaryLines.push_back("Mata Kuliah      : " + courseName);
        summaryLines.push_back("Jumlah Mahasiswa : " + std::to_string(studentCount));  // std::to_string: int -> teks
        summaryLines.push_back("");  // baris kosong pemisah
        // Judul kolom ("No", "Nama", "Nilai") disusun formatTableRow() (BAGIAN 5) lalu dicetak tebal.
        summaryLines.push_back(colorize(ANSI_BOLD, formatTableRow("No", "Nama", "Nilai", nameColumnWidth)));
        summaryLines.push_back(tableRule);  // garis atas tabel
        rowNumber = 1;  // diisi 1 lagi dikarenakan penomoran tabel dimulai ulang dari 1
        for (const Student& student : students) {  // berfungsi untuk membuat satu baris tabel per mahasiswa
            // formatTableRow() menyusun nomor, nama, dan nilai (formatTwoDecimals() = 2 desimal) menjadi satu baris.
            summaryLines.push_back(formatTableRow(std::to_string(rowNumber), student.name, formatTwoDecimals(student.score), nameColumnWidth));
            ++rowNumber;  // nomor urut berikutnya
        }  // akhir for
        summaryLines.push_back(tableRule);  // garis bawah tabel
        // Menggunakan "" selebar kolom No berfungsi agar label total dan rata-rata tetap sejajar dengan kolom nama;
        // keduanya hijau tebal dikarenakan merupakan hasil akhir yang paling ditunggu pengguna.
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Total",     formatTwoDecimals(total),    nameColumnWidth)));
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Rata-Rata", formatTwoDecimals(average), nameColumnWidth)));
        std::cout << '\n';  // baris kosong sebelum kotak
        drawBox("Rekap Nilai Mahasiswa", summaryLines);  // gambar kotak rekap (drawBox() di BAGIAN 3)
    }  // akhir fungsi run()

}  // akhir namespace task1

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU (#ifndef = "bila makro ini BELUM didefinisikan") berfungsi untuk menyertakan
// main() hanya saat berkas ini dikompilasi sendiri, dikarenakan program bermenu (main.cpp) sudah punya main() dan
// C++ hanya mengizinkan satu main(). Saat digabung, kompilasi memakai -DBUILD_WITH_MENU (opsi -D mendefinisikan
// makro dari baris perintah) sehingga bagian ini sampai #endif dilewati oleh preprocessor.
#ifndef BUILD_WITH_MENU
// Fungsi main() adalah titik awal program: fungsi pertama yang dijalankan saat program dibuka.
int main() {  // menggunakan int main() dan return 0 dikarenakan standar C++ mewajibkan main bertipe int
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 1 - Fadhlillah (2531070011)") << '\n';  // identitas, tebal
    task1::run();  // :: (operator cakupan) memanggil fungsi run() di dalam namespace task1 (BAGIAN 6)
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif  // akhir #ifndef BUILD_WITH_MENU
