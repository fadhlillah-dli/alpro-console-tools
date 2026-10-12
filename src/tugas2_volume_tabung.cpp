/*
 * Tugas Praktikum 2 — Modul 1: Input dan Output
 * Program 2 : Volume Tabung
 * Alur : mendefinisikan π = 3.14 (modul menulis "phi"; simbol konstanta lingkaran yang benar adalah π / pi),
 *        pengguna memilih satuan panjang (mm/cm/dm/m), nilai π (3.14 sesuai modul atau 22/7 seperti pada
 *        soal dengan kelipatan 7), dan ukuran yang diketahui (jari-jari atau diameter), lalu memasukkan
 *        ukuran dan tinggi. Program menampilkan luas alas, volume V = π x r x r x t, dan padanan liter.
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas2 tugas2_volume_tabung.cpp && ./tugas2
 * Arti perintah di atas: g++ = compiler C++ (pengubah kode menjadi program); -std=c++17 = memakai aturan bahasa
 * C++ versi 2017; -Wall -Wextra = menyalakan peringatan tambahan agar kesalahan kecil ketahuan; -o tugas2 = nama
 * program hasil kompilasi; && ./tugas2 = bila kompilasi berhasil, langsung jalankan programnya.
 * Bila digabung ke program bermenu (berkas main.cpp di folder yang sama), kompilasi dengan tambahan
 * -DBUILD_WITH_MENU agar main() di bagian paling bawah berkas ini tidak ikut (lihat BAGIAN 7). -D berfungsi untuk
 * mendefinisikan makro BUILD_WITH_MENU dari perintah kompilasi, sama seperti menulis #define di awal berkas.
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Teks setelah tanda // sampai akhir baris adalah komentar: diabaikan compiler, hanya untuk dibaca manusia.
// Menggunakan #include berfungsi untuk menyalin isi sebuah header (berkas berisi daftar fungsi dan tipe data milik
// sebuah pustaka) ke berkas ini sebelum dikompilasi, dikarenakan fungsi pustaka baru bisa dipakai setelah dikenalkan
// lewat header-nya. Tanda <...> berarti header bawaan compiler. Baris yang diawali # diproses lebih dulu oleh
// preprocessor (tahap persiapan sebelum kompilasi). Nama dari pustaka standar diawali std:: (std adalah namespace,
// yaitu "wadah nama", milik pustaka standar C++; :: berarti "nama di dalam wadah").

// <algorithm> menyediakan std::max (nilai terbesar dari dua nilai) dan std::min (nilai terkecil); berfungsi untuk
// membatasi lebar kotak dan ukuran sketsa (dipakai di terminalWidth(), drawBox(), dan printSketch() di bawah).
#include <algorithm>
// <cctype> menyediakan std::isdigit, yang berfungsi memeriksa apakah sebuah karakter adalah angka 0-9
// (dipakai di isAllDigits() dan readChoice(), BAGIAN 4).
#include <cctype>
// <cmath> menyediakan fungsi matematika std::cos dan std::sin; berfungsi untuk menghitung titik-titik elips
// saat menggambar sketsa tabung (dipakai di printSketch(), BAGIAN 5 lanjutan).
#include <cmath>
// <cstddef> menyediakan std::size_t, yaitu tipe bilangan bulat tanpa tanda (tidak bisa negatif) yang dipakai C++
// untuk ukuran dan posisi; berfungsi untuk menyimpan lebar teks dan posisi karakter di dalam teks.
#include <cstddef>
// <cstdlib> menyediakan std::exit dan EXIT_FAILURE (menghentikan program dengan kode gagal, dipakai di readLine()),
// std::getenv (membaca variabel lingkungan, yaitu pengaturan bernama milik sistem operasi, mis. NO_COLOR=1;
// dipakai di colorsEnabled() dan terminalWidth()), serta std::atoi (mengubah teks menjadi bilangan bulat,
// dipakai di terminalWidth()); semua fungsi itu ada di bawah.
#include <cstdlib>
// <iomanip> menyediakan pengatur format keluaran std::setw (lebar kolom), std::left (rata kiri), std::fixed
// (jumlah desimal tetap), dan std::setprecision (jumlah angka di belakang titik desimal); dipakai di
// formatResultLine() (BAGIAN 5 lanjutan).
#include <iomanip>
// <initializer_list> menyediakan std::initializer_list, yaitu daftar nilai yang ditulis langsung di dalam kurung
// kurawal { ... }; berfungsi untuk menerima daftar baris opsi pada printOptions() (BAGIAN 5).
#include <initializer_list>
// <iostream> menyediakan std::cin (membaca ketikan dari keyboard), std::cout (mencetak ke layar), dan std::cerr
// (mencetak pesan kesalahan).
#include <iostream>
// <sstream> menyediakan std::istringstream (membaca angka dari sebuah teks, dipakai di parseDecimal(), BAGIAN 4)
// dan std::ostringstream (menyusun teks berformat, dipakai di formatResultLine(), BAGIAN 5 lanjutan).
#include <sstream>
// <string> menyediakan std::string, yaitu tipe data teks yang panjangnya bisa berubah; berfungsi untuk menyimpan
// baris ketikan pengguna dan baris tampilan.
#include <string>
// <string_view> menyediakan std::string_view, yaitu "jendela baca" ke sebuah teks tanpa menyalinnya; berfungsi
// untuk teks prompt dan judul yang hanya dibaca, dikarenakan menyalin teks yang tidak diubah hanya membuang waktu.
#include <string_view>
// <vector> menyediakan std::vector, yaitu daftar (seperti array) yang jumlah isinya bisa bertambah; berfungsi
// untuk menyimpan baris-baris yang digambar di dalam kotak dan titik-titik sketsa.
#include <vector>
// Pustaka khusus sistem operasi untuk mendeteksi terminal, membaca lebar jendela, mengaktifkan warna, dan UTF-8.
// Menggunakan #ifdef _WIN32 berfungsi untuk memilih pustaka sesuai sistem operasi saat kompilasi: #ifdef adalah
// pertanyaan ke preprocessor "apakah makro ini sudah didefinisikan?", dan _WIN32 adalah penanda yang otomatis
// didefinisikan compiler di Windows. Baris sampai #else hanya ikut dikompilasi di Windows, baris setelah #else
// hanya di macOS/Linux, dan #endif menutup pilihan ini (disebut kompilasi bersyarat). Dikarenakan Windows dan
// macOS/Linux menyediakan fungsi yang berbeda untuk keperluan yang sama.
#ifdef _WIN32
    // Menggunakan #ifndef NOMINMAX ("if not defined") berfungsi untuk mengecek apakah makro NOMINMAX belum pernah
    // didefinisikan; #define di bawahnya hanya dijalankan bila belum ada, dikarenakan MinGW (g++ untuk Windows)
    // sudah mendefinisikannya lewat pustaka standar dan mendefinisikan ulang makro yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    // Menggunakan #define NOMINMAX 1 berfungsi untuk membuat makro (nama pengganti teks) NOMINMAX bernilai 1. Bila
    // makro ini ada, windows.h tidak membuat makro min dan max, dikarenakan makro itu akan mengganti tulisan max dan
    // min di dalam std::max dan std::min di berkas ini sehingga kompilasi rusak.
    #define NOMINMAX 1
    #endif  // akhir pengecekan NOMINMAX
    // <windows.h> adalah pustaka bawaan Windows; berfungsi untuk menyediakan fungsi-fungsi Windows yang dipanggil di
    // fungsi colorsEnabled(), terminalWidth(), dan enableUtf8Console() di bawah: GetStdHandle, GetConsoleMode dan
    // SetConsoleMode (warna), SetConsoleOutputCP dan SetConsoleCP (UTF-8), GetConsoleScreenBufferInfo (lebar jendela).
    #include <windows.h>
#else  // bukan Windows: macOS atau Linux
    // <cstdio> menyediakan fileno dan stdout; dipakai bersama isatty di colorsEnabled() di bawah untuk mendapatkan
    // nomor berkas keluaran standar (layar).
    #include <cstdio>
    // <sys/ioctl.h> menyediakan ioctl, TIOCGWINSZ, dan struct winsize; berfungsi untuk membaca lebar jendela
    // terminal di terminalWidth() di bawah.
    #include <sys/ioctl.h>
    // <unistd.h> menyediakan isatty (memeriksa apakah keluaran adalah terminal, bukan berkas; dipakai di
    // colorsEnabled()) dan STDOUT_FILENO (nomor 1 = keluaran standar; dipakai di terminalWidth()), keduanya di bawah.
    #include <unistd.h>
#endif  // akhir pemilihan pustaka sesuai sistem operasi

// Menggunakan namespace tanpa nama (namespace { ... }) berfungsi untuk menjadikan konstanta dan fungsi bantu di
// bawah milik berkas ini saja (tidak terlihat dari berkas lain). Namespace adalah "wadah nama" untuk mengelompokkan
// nama-nama. Dikarenakan berkas tugas lain punya fungsi bantu bernama sama; tanpa ini, penggabungan ke program
// bermenu gagal dengan error "multiple definition". Wadah ini ditutup tepat sebelum BAGIAN 6.
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // Mendefinisikan π = 3.14 sesuai ketentuan tugas (modul menulis "phi"; yang dimaksud adalah π / pi,
    // konstanta keliling lingkaran dibagi diameter; φ / phi adalah rasio emas 1,618 dan bukan konstanta ini).
    // Menggunakan tipe data double (bilangan desimal) dikarenakan π bilangan desimal; constexpr berfungsi untuk
    // menjadikannya konstanta: nilai tetap yang sudah pasti sejak kompilasi dan tidak bisa diubah saat program
    // berjalan (pengganti modern #define, lebih aman dikarenakan punya tipe data).
    // PI_MODULE = nilai bawaan 3.14, sesuai modul (pilihan [1] di choosePi(), BAGIAN 5).
    constexpr double PI_MODULE   = 3.14;
    // PI_FRACTION = 22/7, nilai yang dipakai soal sekolah bila jari-jari atau diameter kelipatan 7 (pilihan [2]).
    // Ditulis 22.0 / 7.0 dikarenakan pembagian dua bilangan bulat (22 / 7) di C++ membuang sisanya dan hasilnya 3.
    constexpr double PI_FRACTION = 22.0 / 7.0;
    // TITLE_WIDTH = lebar garis judul dalam karakter (dipakai printRule(), BAGIAN 3); int = tipe bilangan bulat.
    constexpr int    TITLE_WIDTH = 60;
    // LABEL_WIDTH = lebar kolom label pertanyaan pilihan, agar semua opsi sejajar (dipakai printOptions(), BAGIAN 5).
    constexpr int    LABEL_WIDTH = 24;
    // DIMENSION_MAX = batas atas ukuran; berfungsi untuk menolak salah ketik yang tidak masuk akal
    // (dipakai readPositiveDecimal(), BAGIAN 4).
    constexpr double DIMENSION_MAX = 1000000.0;
    // ANSI_SURFACE = kode warna cyan biasa untuk arsiran permukaan tutup tabung pada sketsa (printSketch(), BAGIAN 5
    // lanjutan). const char* adalah penunjuk ke teks tetap (teks gaya bahasa C). "\033[36m" adalah kode warna ANSI:
    // terminal tidak mencetaknya, melainkan mengganti warna teks sesudahnya (penjelasan lengkap di BAGIAN 3).
    constexpr const char* ANSI_SURFACE = "\033[36m";

    // Menggunakan struct berfungsi untuk mengemas nama satuan dan faktor konversinya ke liter menjadi satu
    // kesatuan, dikarenakan keduanya selalu dipakai berpasangan. struct adalah tipe data buatan sendiri yang
    // mengelompokkan beberapa data (disebut anggota) di bawah satu nama; anggotanya dibaca dengan titik,
    // mis. unit.symbol.
    struct LengthUnit {
        const char* symbol;   // simbol satuan panjang, mis. "cm"; const char* dikarenakan teks tetap, tidak diubah
        // isi 1 satuan kubik dalam liter, dikarenakan 1 L = 1 dm³ (satuan SI turunan); double = bilangan desimal
        double      litersPerCubicUnit;
    };  // akhir struct LengthUnit (titik koma setelah } penutup struct wajib ditulis)
    // Daftar satuan yang lazim pada soal; urutannya = nomor pilihan 1-4. 1 dm³ = 1 L, maka 1 cm³ = 0,001 L
    // (= 1 mL), 1 mm³ = 0,000001 L, dan 1 m³ = 1000 L. UNITS[] adalah array: deretan data bertipe sama (di sini
    // LengthUnit) yang diakses dengan nomor urut (indeks) mulai dari 0, jadi UNITS[0] = mm dan UNITS[3] = m.
    // Kurung siku kosong [] berarti jumlah isinya dihitung compiler dari daftar { ... }; setiap {"cm", 0.001}
    // mengisi anggota symbol dan litersPerCubicUnit sesuai urutan.
    constexpr LengthUnit UNITS[] = { {"mm", 0.000001}, {"cm", 0.001}, {"dm", 1.0}, {"m", 1000.0} };
    // UNIT_COUNT = banyaknya satuan di UNITS, dipakai sebagai nomor pilihan terbesar di chooseUnit() (BAGIAN 5).
    constexpr int    UNIT_COUNT   = 4;
    // DEFAULT_UNIT_INDEX = nomor pilihan bawaan bila pengguna hanya menekan Enter: 2 = cm, dikarenakan hampir semua
    // soal tabung memakai cm. Ini nomor pilihan (mulai 1), bukan indeks array: chooseUnit() mengambil UNITS[2 - 1].
    constexpr int    DEFAULT_UNIT_INDEX   = 2;

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, pilihan yang ditegaskan, hasil akhir, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    // Kode warna ANSI adalah teks khusus yang diawali karakter ESC ("\033" = karakter nomor 27, ditulis dalam
    // bilangan oktal), lalu "[", angka, dan huruf "m". Terminal tidak mencetaknya, melainkan mengganti warna teks
    // sesudahnya. Angka: 0 = normal, 1 = tebal, 31 = merah, 32 = hijau, 33 = kuning, 36 = cyan, 90 = abu-abu;
    // "1;36" = tebal sekaligus cyan.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // cyan tebal: judul program, judul kotak, dan garis sketsa
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // merah tebal: pesan kesalahan
    // kuning: keterangan nilai bawaan [Enter = ...] serta tanda R, panah T, dan keterangan di samping sketsa
    constexpr const char* ANSI_HINT = "\033[33m";
    // abu-abu: garis kotak, garis pembatas, serta tepi alas putus-putus dan keterangannya pada sketsa
    constexpr const char* ANSI_DIM    = "\033[90m";

    // Berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001; code page = tabel yang dipakai konsol
    // untuk menerjemahkan nomor byte menjadi huruf) sebelum keluaran pertama, dikarenakan simbol π, ², ³, garis
    // kotak, dan sketsa Braille dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan masih
    // memakai code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku untuk
    // jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur jalankan.bat (di folder
    // proyek). Di macOS/Linux terminal sudah UTF-8, sehingga fungsi ini tidak melakukan apa-apa. UTF-8 adalah
    // cara menyimpan setiap huruf/simbol sebagai 1 sampai 4 byte (huruf biasa cukup 1 byte).
    // void berarti fungsi ini tidak mengembalikan nilai; () kosong berarti fungsi ini tidak menerima masukan.
    void enableUtf8Console() {
    #ifdef _WIN32  // hanya ikut dikompilasi di Windows (lihat penjelasan #ifdef _WIN32 di BAGIAN 1)
        // SetConsoleOutputCP adalah fungsi Windows (dari windows.h, BAGIAN 1) yang mengatur code page KELUARAN
        // konsol; CP_UTF8 = nomor code page 65001, sehingga byte UTF-8 ditampilkan sebagai karakter yang benar.
        SetConsoleOutputCP(CP_UTF8);
        // SetConsoleCP adalah pasangannya untuk MASUKAN: ketikan dengan huruf non-ASCII dibaca sebagai UTF-8, sama
        // dengan keluarannya (ASCII = tabel 128 karakter dasar, antara lain huruf Latin tanpa aksen, angka 0-9,
        // tanda baca, dan spasi; π, ², dan ³ bukan ASCII).
        SetConsoleCP(CP_UTF8);
    #endif  // akhir bagian khusus Windows
    }

    // Berfungsi untuk menentukan apakah warna boleh dipakai. Warna dimatikan bila variabel lingkungan NO_COLOR
    // ada (kesepakatan umum program CLI, yaitu program baris perintah) atau keluaran bukan terminal (mis. dialihkan
    // ke berkas), dan dipaksa aktif bila FORCE_COLOR ada. Di Windows, mode "virtual terminal" diaktifkan agar
    // Command Prompt memahami kode ANSI. Menggunakan static dikarenakan pemeriksaan cukup dilakukan sekali untuk
    // seluruh program. bool adalah tipe data yang hanya bernilai true (benar) atau false (salah). Variabel
    // lingkungan adalah pengaturan bernama milik sistem operasi yang bisa dibaca program, mis. NO_COLOR=1.
    bool colorsEnabled() {
        // static pada variabel di dalam fungsi berarti variabel itu dibuat dan diisi hanya sekali, saat fungsi
        // pertama kali dipanggil; panggilan berikutnya langsung memakai nilai yang sama. const = tidak bisa diubah.
        // Nilai awalnya dihitung oleh lambda: []() { ... } adalah fungsi kecil tanpa nama yang ditulis di tempat,
        // dan tanda () setelah } penutupnya (baris "}();" di bawah) langsung menjalankannya satu kali.
        static const bool enabled = []() {
            // std::getenv mengembalikan nullptr ("alamat kosong") bila variabel lingkungan itu tidak ada, jadi
            // != nullptr berarti variabelnya ada. return mengakhiri lambda dan menyerahkan nilainya.
            if (std::getenv("NO_COLOR") != nullptr) return false;
            if (std::getenv("FORCE_COLOR") != nullptr) return true;
        #ifdef _WIN32  // Windows: aktifkan pemahaman kode ANSI pada konsol
            // GetStdHandle(STD_OUTPUT_HANDLE) adalah fungsi Windows yang meminta HANDLE, yaitu "pegangan" ke
            // konsol keluaran standar (layar). DWORD = bilangan bulat 32 bit milik Windows, di sini untuk
            // menyimpan mode konsol (nilai awal 0).
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;
            // output == INVALID_HANDLE_VALUE berarti GetStdHandle di atas gagal memberi pegangan (tidak ada konsol).
            // || berarti "atau"; ! berarti "tidak". GetConsoleMode adalah fungsi Windows yang membaca pengaturan
            // konsol ke variabel mode; ia gagal bila keluaran bukan konsol (mis. dialihkan ke berkas), sehingga
            // warna dimatikan (return false). &mode = alamat variabel mode, agar fungsi itu bisa mengisinya.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING = mode lama ditambah tanda "pahami kode ANSI" (| = OR per
            // bit). SetConsoleMode mengembalikan 0 bila gagal, maka != 0 berarti berhasil dan warna boleh dipakai.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // fileno(stdout) = nomor berkas keluaran standar; isatty mengembalikan bukan-0 bila nomor itu terminal.
            return isatty(fileno(stdout)) != 0;
        #endif  // akhir pemeriksaan sesuai sistem operasi
        }();  // () di sini langsung menjalankan lambda di atas; hasilnya disimpan ke enabled
        return enabled;  // mengembalikan hasil pemeriksaan yang sudah tersimpan
    }

    // Berfungsi untuk membungkus teks dengan kode warna bila warna aktif; bila tidak, teks dikembalikan apa adanya.
    // code = kode warna (mis. ANSI_TITLE); text = teks yang diwarnai; hasilnya std::string baru.
    std::string colorize(const char* code, std::string_view text) {
        if (!colorsEnabled()) return std::string(text);  // warna mati: teks disalin apa adanya tanpa kode warna
        // + pada std::string berarti menyambung teks: kode warna + teks + ANSI_RESET (kembali ke warna normal).
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Berfungsi untuk menentukan apakah sebuah code point Unicode memakai dua kolom di terminal (aksara CJK, Hangul,
    // emoji), dikarenakan tanpa ini lebar kotak dan pembungkusan baris salah bila teks memuat aksara lebar atau emoji.
    // Code point adalah nomor urut sebuah karakter di tabel Unicode, mis. π = 0x3C0. unsigned = bilangan bulat
    // tanpa tanda (tidak bisa negatif).
    bool isWideCodePoint(unsigned codePoint) {
        // Angka berawalan 0x ditulis dalam heksadesimal (basis 16). && berarti "dan", || berarti "atau": hasilnya
        // true bila codePoint ada di salah satu rentang berikut: 0x1100-0x115F (Hangul Jamo), 0x2E80-0xA4CF (aksara
        // CJK Cina/Jepang/Korea), 0xAC00-0xD7A3 (suku kata Hangul), 0xF900-0xFAFF dan 0xFE30-0xFE4F (bentuk
        // kompatibilitas CJK), 0xFF00-0xFF60 dan 0xFFE0-0xFFE6 (huruf dan tanda lebar penuh), 0x1F300-0x1F64F dan
        // 0x1F900-0x1F9FF (emoji).
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berfungsi untuk menghitung lebar teks di layar: karakter UTF-8 multibyte (π, ², ³, garis kotak) dihitung
    // satu kolom, aksara lebar (CJK, emoji) dua kolom, dan kode warna ANSI nol kolom, dikarenakan ketiganya
    // tidak memakai ruang sebanyak byte-nya. Byte awal UTF-8 menentukan jumlah byte lanjutan yang dirangkai.
    // Hasilnya bertipe std::size_t (bilangan bulat tanpa tanda untuk ukuran, lihat BAGIAN 1).
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah dihitung
        bool inEscape = false;  // true selama berada di dalam urutan kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point tersebut
        // Perulangan for berbasis rentang (range-based for): c berisi setiap byte dari text satu per satu, dari
        // awal sampai akhir. unsigned char = satu byte bernilai 0-255; const = c tidak diubah di dalam perulangan.
        for (const unsigned char c : text) {
            // Di dalam kode warna: lewati byte sampai huruf 'm' penutupnya. continue = lompat ke byte berikutnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC ("\033") pembuka kode warna
            // Byte lanjutan UTF-8 berpola bit 10xxxxxx: & (AND per bit) dengan 0xC0 mengambil 2 bit teratas, dan
            // hasil 0x80 berarti polanya 10. Byte ini tidak menambah kolom; 6 bit isinya (c & 0x3F) disambung ke
            // codePoint dengan menggeser codePoint 6 bit ke kiri (<< 6) lalu OR (|). --pending mengurangi sisa
            // byte; bila habis (0) dan code point itu aksara lebar, lebarnya ditambah 1 kolom lagi (jadi 2 kolom).
            if ((c & 0xC0) == 0x80) {
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            // Byte awal menentukan panjang karakter: 110xxxxx = 2 byte (1 lanjutan), 1110xxxx = 3 byte (2 lanjutan),
            // 11110xxx = 4 byte (3 lanjutan); bit x-nya menjadi awal codePoint. Selain itu (huruf biasa/ASCII,
            // 1 byte) tidak ada byte lanjutan yang ditunggu.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }  // akhir perulangan setiap byte
        return width;
    }

    // Berfungsi membaca lebar terminal saat ini (jumlah kolom) setiap kali dipanggil, sehingga tampilan ikut
    // menyesuaikan bila jendela diubah ukurannya. Bila tidak diketahui (keluaran bukan terminal), dipakai variabel
    // lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan terminal pada umumnya. Menggunakan batas bawah 50
    // dikarenakan di bawah itu kotak dan sketsa tidak bisa rapi.
    int terminalWidth() {
        int cols = 0;  // 0 = lebar belum diketahui
    #ifdef _WIN32  // Windows: tanya ukuran jendela konsol
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows untuk menampung keadaan konsol.
        // GetConsoleScreenBufferInfo adalah fungsi Windows yang mengisi info (&info = alamat info) dan mengembalikan
        // bukan-0 bila berhasil; GetStdHandle(STD_OUTPUT_HANDLE) memberi pegangan ke konsol keluaran (lihat
        // colorsEnabled() di atas). srWindow berisi tepi kiri (Left) dan kanan (Right) bagian jendela yang
        // terlihat, maka lebarnya = Right - Left + 1.
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // struct winsize menampung ukuran jendela. ioctl TIOCGWINSZ = tanya ukuran jendela; ioctl mengisi ws
        // (&ws = alamat ws) dan mengembalikan 0 bila berhasil; ws.ws_col = jumlah kolom.
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // Cadangan 1: variabel lingkungan COLUMNS; std::atoi mengubah teksnya menjadi int (mis. "100" menjadi 100).
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // cadangan 2: 80 kolom, ukuran bawaan terminal pada umumnya
        return std::max(50, cols);  // std::max memilih yang lebih besar: hasilnya tidak pernah kurang dari 50 kolom
    }

    // Berfungsi memotong satu baris teks yang lebih panjang dari 'maxWidth' kolom menjadi beberapa baris, memotong
    // pada spasi, dan memberi baris lanjutan indentasi 'gantung' (sebanyak 'hangingIndent' spasi) agar tetap
    // sejajar dengan isi setelah label. Dipakai drawBox(), printError() (keduanya di bawah), dan printOptions()
    // (BAGIAN 5) dikarenakan teks tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
    // Parameter const std::string& (disebut const&) berarti fungsi memakai teks aslinya langsung tanpa menyalin
    // (& = reference, nama lain untuk variabel yang sama) dan berjanji tidak mengubahnya (const). Hasilnya
    // std::vector berisi std::string, yaitu daftar baris hasil potongan.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil, awalnya kosong
        // indentasi terlalu besar untuk jendela sempit: pakai 2 spasi saja
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // sisa teks yang belum dipotong
        while (displayWidth(remaining) > maxWidth) {  // while = ulangi selama sisa teks lebih lebar dari batas
            // col = kolom yang sedang diperiksa; cutPos = posisi spasi tempat memotong. std::string::npos adalah
            // nilai khusus yang berarti "posisi tidak ada", di sini = belum menemukan spasi.
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama berada di dalam kode warna (tidak dihitung sebagai kolom)
            // Cari spasi terakhir sebelum batas lebar (dihitung per kolom); i = posisi byte, ++i = tambah 1.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char>(...) berfungsi mengubah tipe char menjadi unsigned char secara tegas,
                // dikarenakan char bisa bernilai negatif untuk byte UTF-8, sedangkan pemeriksaan di bawah
                // ditulis untuk nilai byte 0-255.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // lewati isi kode warna
                if (c == 0x1B) { inEscape = true; continue; }  // ESC: awal kode warna
                // (c & 0xC0) == 0x80 berarti byte lanjutan UTF-8 (pola bit 10xxxxxx): bukan kolom baru, jadi
                // dilewati dengan continue (lihat penjelasan lengkapnya di displayWidth() di atas)
                if ((c & 0xC0) == 0x80) continue;
                if (col >= maxWidth) break;  // break = keluar dari perulangan for: batas lebar sudah tercapai
                // ingat posisi spasi ini, asal letaknya sudah melewati indentasi gantung
                if (c == ' ' && col > hangingIndent) cutPos = i;
                ++col;
            }
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            // push_back menambahkan satu isi di akhir vector; substr(0, cutPos) = teks dari awal sampai sebelum spasi.
            result.push_back(remaining.substr(0, cutPos));
            // Sisa teks = indentasi gantung (std::string(n, ' ') = n buah spasi) + teks setelah spasi pemotong.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }  // akhir while
        result.push_back(remaining);  // potongan terakhir (atau seluruh teks bila sejak awal sudah muat)
        return result;
    }

    // Berfungsi untuk menggambar kotak berjudul berisi baris-baris teks. Lebar kotak mengikuti baris terpanjang,
    // tetapi tidak pernah melebihi lebar terminal: baris yang terlalu panjang dibungkus lebih dulu (wrapText() di
    // atas), dengan indentasi gantung sejajar label bila baris berpola "Label : isi". Menggunakan kotak dikarenakan
    // informasi (petunjuk, menu, hasil) jadi terpisah jelas dari pertanyaan.
    // Parameter: title = judul kotak; lines = daftar baris isi (const& agar tidak disalin, lihat wrapText() di atas).
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Dikurangi 2 tepi kotak + 2 margin + 1 kolom cadangan, dikarenakan sebagian terminal menambah baris kosong
        // bila sebuah baris persis selebar jendela. static_cast<std::size_t> mengubah hasil int ke std::size_t.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris isi setelah dibungkus
        for (const std::string& b : lines) {  // b = setiap baris masukan, dibaca lewat const& agar tidak disalin
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan: 2 spasi
            const std::size_t labelPos = b.find(" : ");  // find mencari posisi " : "; std::string::npos bila tidak ada
            // Bila ada label dan lebarnya paling banyak 28 kolom, baris lanjutan menjorok sejajar isi setelah label.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // sejajar dengan isi setelah " : "
            }
            // Setiap potongan hasil wrapText() ditambahkan ke contentLines.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Lebar dalam mengikuti baris terpanjang; + 2 = margin kiri dan kanan.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min: lebar tidak boleh melewati batas jendela
        // filler = garis ─ sebanyak sisa lebar setelah "┌─ " + judul + " ". "\u2500" adalah cara menulis karakter
        // Unicode dengan nomornya: \u2500 = ─, \u250c = ┌, \u2510 = ┐, \u2502 = │, \u2514 = └, \u2518 = ┘.
        std::string filler;
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // Cetak tepi atas: "┌─ " abu-abu, judul cyan tebal, lalu " " + filler + "┐" abu-abu; '\n' = pindah baris.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Cetak setiap baris isi: "│" + spasi + teks + spasi pengisi + "│". Operator ternary (kondisi ? a : b)
        // memilih a bila kondisi benar dan b bila salah: jumlah spasi pengisi = width - contentWidth, atau 0 bila
        // baris sudah selebar kotak (std::string(n, ' ') = n buah spasi).
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;
            const std::size_t contentWidth = displayWidth(rowText);
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }
        // Cetak tepi bawah: "└" + garis ─ sebanyak width + "┘".
        std::string bottomLine;
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }

    // Berfungsi mencetak pesan kesalahan (merah) yang dibungkus sesuai lebar terminal, dengan baris lanjutan
    // menjorok sejajar teks setelah tanda "[!]", dikarenakan pesan panjang tidak boleh terpotong jendela sempit.
    void printError(const std::string& text) {
        // Lebar = lebar terminal - 1 kolom cadangan; indentasi gantung 6 = lebar awalan "  [!] " pada setiap pesan.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';  // setiap potongan dicetak merah tebal
        }
    }

    // Berfungsi untuk mencetak satu baris garis pembatas abu-abu sepanjang TITLE_WIDTH (BAGIAN 2).
    // char = tipe untuk satu karakter, mis. '='; std::string(TITLE_WIDTH, ch) = karakter ch diulang TITLE_WIDTH kali.
    void printRule(char ch) {
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Fungsi-fungsi untuk membaca ketikan pengguna dan memeriksa apakah ketikan itu sah.

    // Berfungsi untuk menampilkan prompt (pertanyaan, dicetak tebal) lalu membaca satu baris ketikan.
    // Hasilnya std::string berisi ketikan pengguna tanpa tombol Enter-nya.
    std::string readLine(std::string_view prompt) {
        std::string line;                   // menggunakan std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt); // tebal berfungsi agar pertanyaan mudah dibedakan dari keterangan
        // Menggunakan std::getline berfungsi untuk membaca satu baris penuh dari std::cin (keyboard), termasuk spasi,
        // sampai tombol Enter. Pemeriksaan ! (NOT) berfungsi untuk menangkap kegagalan getline, dikarenakan getline
        // gagal bila input ditutup (Ctrl+D/Ctrl+Z).
        if (!std::getline(std::cin, line)) {
            // std::cerr dikarenakan ini pesan error; "\n" di awal teks = pindah baris lebih dulu.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            // std::exit menghentikan program saat itu juga; EXIT_FAILURE = kode gagal, dikarenakan program berhenti
            // tanpa data lengkap.
            std::exit(EXIT_FAILURE);
        }
        return line;  // mengembalikan baris yang dibaca
    }

    // Berfungsi untuk membuang spasi/tab di awal dan akhir teks, dan merapatkan spasi beruntun menjadi satu,
    // mis. "  7   5 " menjadi "7 5". Parameter const& dikarenakan teks hanya dibaca dan tidak perlu disalin.
    std::string normalizeSpaces(const std::string& text) {
        // menggunakan std::string berfungsi untuk menampung teks yang sudah dirapikan
        std::string result;
        bool pendingSpace = false;         // menggunakan bool berfungsi untuk menandai ada spasi yang belum ditulis
        for (const char c : text) {         // memeriksa setiap karakter satu per satu (range-based for)
            // Spasi, tab ('\t'), dan CR ('\r' = sisa akhir baris Windows) tidak langsung ditulis, hanya ditandai.
            if (c == ' ' || c == '\t' || c == '\r') {
                pendingSpace = !result.empty();  // spasi di awal diabaikan dikarenakan hasil masih kosong
            } else {  // karakter selain spasi
                if (pendingSpace) result += ' ';  // beberapa spasi beruntun ditulis sebagai satu spasi saja
                result += c;  // += = tambahkan karakter ini di akhir hasil
                pendingSpace = false;
            }
        }
        return result;                       // spasi di akhir otomatis terbuang dikarenakan tidak pernah ditulis
    }

    // Berfungsi untuk memeriksa apakah teks hanya berisi angka 0-9 (tanpa tanda, titik, huruf, atau spasi).
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;     // teks kosong bukan bilangan; empty() = true bila teks tidak berisi apa pun
        for (const char c : text) {         // setiap karakter harus digit
            // static_cast<unsigned char> dikarenakan std::isdigit hanya aman untuk nilai 0-255, sedangkan char bisa
            // bernilai negatif untuk huruf non-ASCII. Bila satu saja bukan digit, langsung false.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;  // semua karakter digit
    }

    // Berfungsi untuk memeriksa format bilangan desimal: angka, boleh diikuti SATU pemisah titik atau koma lalu
    // angka lagi (7, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, dikarenakan
    // bukan cara menulis ukuran yang diharapkan dari pengguna.
    bool isDecimalFormat(const std::string& text) {
        // find_first_of(".,") = posisi titik/koma pertama; std::string::npos bila tidak ada. auto = tipe variabel
        // ditentukan compiler dari nilainya (di sini tipe posisi teks).
        const auto separator = text.find_first_of(".,");
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        // substr(0, separator) = teks sebelum pemisah; substr(separator + 1) = teks sesudah pemisah sampai akhir.
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // bagian sesudahnya juga; pemisah kedua tertolak
    }

    // Berfungsi untuk mengubah teks menjadi bilangan desimal; mengembalikan true bila teks sah.
    // text ditulis tanpa & sehingga berupa salinan yang boleh diubah tanpa mengganggu teks asli pemanggil.
    // double& result = reference: angka hasilnya ditulis langsung ke variabel milik pemanggil.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang diterima
        // menggunakan reference (char&) berfungsi untuk mengubah karakter langsung pada teksnya
        for (char& c : text) {
            if (c == ',') c = '.';    // koma diganti titik dikarenakan C++ hanya mengenal penulisan 7.5
        }
        std::istringstream parser(text);  // menggunakan istringstream berfungsi untuk membaca angka dari teks
        // parser >> result membaca angka ke result (true bila berhasil); parser >> std::ws melewati spasi, lalu
        // .eof() memastikan sudah sampai akhir teks. Sah bila terbaca sebagai angka tanpa sisa karakter.
        return (parser >> result) && (parser >> std::ws).eof();
    }

    // Berfungsi untuk meminta bilangan desimal yang harus lebih besar dari 0 dan paling besar DIMENSION_MAX
    // (BAGIAN 2); diulang sampai sah. prompt = teks pertanyaan yang ditampilkan.
    double readPositiveDecimal(std::string_view prompt) {
        // menggunakan while (true) berfungsi untuk mengulang permintaan sampai input sah; keluar lewat return
        while (true) {
            // menggunakan double dikarenakan ukuran boleh desimal; 0.0 sebagai nilai awal yang jelas
            double value = 0.0;
            // Baca baris (readLine()), rapikan spasi (normalizeSpaces()), lalu ubah ke angka (parseDecimal()); ketiga
            // fungsi itu ada di atas. Syarat nilai > 0.0 berfungsi untuk menolak nol dan negatif, dikarenakan ukuran
            // bangun tidak mungkin nol atau negatif; value <= DIMENSION_MAX menolak angka yang terlalu besar.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            // Dua teks "..." "..." yang ditulis berurutan otomatis digabung compiler menjadi satu teks.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }  // akhir while: kembali meminta input
    }

    // Berfungsi untuk meminta pilihan berupa satu digit 1..maxValue; Enter tanpa isi berarti pilihan bawaan
    // (defaultValue). Menggunakan nilai bawaan dikarenakan pilihan paling umum (cm, π = 3.14, jari-jari) cukup
    // ditekan Enter.
    int readChoice(std::string_view prompt, int maxValue, int defaultValue) {
        while (true) {  // diulang dikarenakan pilihan salah harus diminta lagi
            const std::string text = normalizeSpaces(readLine(prompt));  // baca lalu rapikan spasinya
            if (text.empty()) return defaultValue;  // Enter saja berfungsi untuk memilih bawaan
            // Sah bila tepat satu digit (size() == 1) dan berada di rentang; text[0] = karakter pertama.
            // text[0] - '0' berfungsi mengubah karakter digit menjadi angka, dikarenakan kode karakter '0' sampai
            // '9' berurutan, mis. '7' - '0' = 7.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) {
                const int choice = text[0] - '0';
                if (choice >= 1 && choice <= maxValue) return choice;  // di rentang 1 sampai maxValue: sah
            }
            // std::to_string mengubah angka maxValue menjadi teks agar bisa disambung ke pesan dengan +.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue)
                                        + ", atau tekan Enter untuk memakai bawaan.");
        }
    }

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // Berfungsi untuk menampilkan sebuah pertanyaan pilihan dengan daftar opsinya rapi: baris pertama berisi
    // label (tebal) dan opsi pertama, baris berikutnya opsi lain yang disejajarkan di bawahnya, baris terakhir
    // nilai bawaan (kuning). Menggunakan fungsi ini dikarenakan semua pertanyaan pilihan harus tampil dengan
    // bentuk yang sama agar pengguna langsung mengenali pola "[nomor] pilihan" dan "[Enter = bawaan]".
    // Parameter line bertipe std::initializer_list: daftar teks yang ditulis langsung dalam { ... } saat fungsi
    // dipanggil, mis. printOptions("Pilih Nilai Pi", { "[1] ...", "[2] ...", "[Enter = 3.14]" }).
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) {
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // lebar terminal - 1 cadangan
        bool isFirst = true;  // menggunakan bool berfungsi menandai baris pertama (yang memuat label)
        for (const std::string_view b : line) {  // b = setiap teks opsi, berurutan
            // substr(0, 6) = 6 karakter pertama; baris "[Enter = ...]" (diawali "[Enter") diberi warna kuning.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter");
            // operator ternary: teks kuning bila baris bawaan, selain itu teks apa adanya
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b);
            // Baris pertama memuat label (tebal) lalu ": "; baris berikutnya diberi spasi selebar label + ": " agar
            // sejajar. Keduanya dibungkus (wrapText(), BAGIAN 3) agar opsi yang panjang tidak terpotong di jendela
            // sempit. Label yang lebih pendek dari LABEL_WIDTH ditambah spasi sampai lebarnya LABEL_WIDTH.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            // Cetak setiap potongan hasil wrapText(); baris lanjutan menjorok LABEL_WIDTH + 2 spasi.
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;  // baris berikutnya bukan lagi baris pertama
        }
    }

    // Berfungsi untuk menampilkan daftar satuan, membaca pilihan, lalu menegaskan satuan yang terpilih.
    // Hasilnya const LengthUnit& (reference ke salah satu isi UNITS di BAGIAN 2), sehingga tidak perlu disalin.
    const LengthUnit& chooseUnit() {
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        // nomor 1 sampai UNIT_COUNT (4); Enter saja = DEFAULT_UNIT_INDEX (2 = cm)
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        // - 1 dikarenakan nomor pilihan mulai dari 1, indeks array mulai dari 0
        const LengthUnit& unit = UNITS[selection - 1];
        // penegasan (hijau tebal) agar pilihan pasti terbaca; "\n\n" = pindah baris dua kali (satu baris kosong)
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit.symbol) << "\n\n";
        return unit;
    }

    // Berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    // Parameter std::string& legend (reference) berfungsi untuk mengirim teks nilai π kembali ke pemanggil: fungsi
    // mengisinya langsung ke variabel milik pemanggil. Nilai kembaliannya (double) adalah π untuk perhitungan.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan jari-jari atau diameter kelipatan 7)",
                                      "[Enter = 3.14]" });
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);  // nomor 1-2; Enter saja = 1 (3.14)
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // operator ternary berfungsi memilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // \u03c0 = π
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // pilihan 2 = 22/7, selain itu 3.14 (BAGIAN 2)
    }

    // Berfungsi untuk menanyakan ukuran mana yang diketahui soal (jari-jari atau diameter) dan menegaskannya.
    // Hasilnya true bila diameter yang diketahui, false bila jari-jari.
    bool chooseDiameterInput() {
        printOptions("Ukuran yang Diketahui", { "[1] Jari-jari (R)",
                                             "[2] Diameter (D), program menghitung R = D / 2",
                                             "[Enter = Jari-jari]" });
        const bool diameter = (readChoice("Pilihan Anda            : ", 2, 1) == 2);  // true hanya bila pilihan 2
        // Penegasan hijau tebal; operator ternary memilih teks "Diameter" atau "Jari-jari".
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Ukuran         : ") + (diameter ? "Diameter" : "Jari-jari")) << "\n\n";
        return diameter;
    }

    // Berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong sebelum judul; '\n' = karakter pindah baris
        printRule('=');  // garis "=" sepanjang TITLE_WIDTH (printRule() di BAGIAN 3)
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // judul menjorok 2 spasi
        printRule('=');
    }

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // Menggunakan karakter Braille Unicode (code point U+2800 s.d. U+28FF; "U+" = penulisan nomor Unicode dalam
    // heksadesimal, sama dengan 0x2800 s.d. 0x28FF) berfungsi untuk menggambar dengan resolusi 2 x 4 titik
    // per sel terminal, delapan kali lebih halus daripada satu karakter blok, dikarenakan setiap karakter Braille
    // mewakili kombinasi 8 titik yang dapat dinyalakan satu per satu (teknik yang dipakai pustaka seperti drawille).
    // struct BrailleCanvas adalah "kanvas" gambar: menyimpan titik-titik yang menyala, lalu mengubahnya menjadi
    // baris teks. Selain data, struct ini berisi fungsi anggota (dipanggil dengan titik, mis. canvas.setPixel(...)).
    struct BrailleCanvas {
        int cols, rows;                 // ukuran kanvas dalam sel terminal (kolom dan baris karakter)
        // layers: untuk tiap titik, 0 = kosong, lebih dari 0 = nomor lapisan gambar (menentukan warna).
        // Titik disimpan berderet baris demi baris: titik (x, y) ada di indeks y * pixelWidth() + x
        // (pixelWidth() = fungsi anggota beberapa baris di bawah, yaitu lebar kanvas dalam titik).
        std::vector<unsigned char> layers;

        // Constructor: fungsi bernama sama dengan struct yang otomatis dijalankan saat kanvas dibuat, mis.
        // BrailleCanvas canvas(46, 21). Bagian setelah titik dua mengisi anggota: cols = k, rows = row, dan layers
        // berisi (2 x k) x (4 x row) titik yang semuanya bernilai awal 0 (kosong).
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {}
        // const setelah () berarti fungsi anggota ini hanya membaca, tidak mengubah isi kanvas.
        int pixelWidth()  const { return 2 * cols; }    // lebar dalam titik: 2 titik per sel ke samping
        int pixelHeight() const { return 4 * rows; } // tinggi dalam titik: 4 titik per sel ke bawah

        // Berfungsi menyalakan satu titik; lapisan bernomor lebih besar menimpa yang kecil (garis di atas arsiran).
        // x = posisi mendatar, y = posisi tegak (0 = paling atas), layer = nomor lapisan.
        void setPixel(int x, int y, unsigned char layer) {
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // di luar kanvas diabaikan
            // ref adalah reference (nama lain) untuk isi layers di posisi titik (x, y), sehingga mengubah ref
            // berarti mengubah isi layers itu sendiri.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x];
            if (layer > ref) ref = layer;  // simpan hanya bila lapisannya lebih tinggi dari yang sudah ada
        }

        // Berfungsi mengubah 8 titik di satu sel menjadi satu karakter Braille UTF-8 berwarna sesuai lapisannya.
        // cx, cy = posisi sel (kolom, baris); layerColors = daftar kode warna, indeksnya = nomor lapisan.
        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const {
            // Peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40; kolom kanan 0x08, 0x10, 0x20,
            // 0x80. bit[4][2] adalah array dua dimensi: 4 baris titik x 2 kolom titik. static agar tabel ini dibuat
            // sekali saja, tidak setiap kali fungsi dipanggil.
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} };
            // value = gabungan bit titik yang menyala; maxLayer = lapisan tertinggi di sel ini (menentukan warna).
            unsigned char value = 0, maxLayer = 0;
            for (int dy = 0; dy < 4; ++dy) {  // dy = baris titik di dalam sel (0-3)
                for (int dx = 0; dx < 2; ++dx) {  // dx = kolom titik di dalam sel (0-1)
                    // ambil nomor lapisan titik (cx * 2 + dx, cy * 4 + dy) dari layers
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx];
                    // Bila titik menyala (layer bukan 0): nyalakan bit-nya (|= berarti OR lalu simpan) dan catat
                    // lapisan tertinggi.
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); }
                }
            }
            if (!value) return " ";                       // sel kosong = spasi biasa
            // kode Unicode karakter Braille yang dimaksud; akhiran u pada 0x2800u = bilangan tanpa tanda (unsigned)
            const unsigned codePoint = 0x2800u + value;
            // Disandikan ke UTF-8 tiga byte berpola 1110xxxx 10xxxxxx 10xxxxxx: >> menggeser bit ke kanan, & 0x3F
            // mengambil 6 bit terbawah, | menambahkan pola awal byte; static_cast<char> menjadikannya satu byte.
            std::string s;
            s += static_cast<char>(0xE0 | (codePoint >> 12));
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
            s += static_cast<char>(0x80 | (codePoint & 0x3F));
            return colorize(layerColors[maxLayer], s);  // beri warna sesuai lapisan tertinggi
        }

        // Berfungsi mengubah seluruh kanvas menjadi baris-baris teks siap cetak (satu std::string per baris sel).
        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const {
            std::vector<std::string> lines;
            for (int cy = 0; cy < rows; ++cy) {  // setiap baris sel, dari atas ke bawah
                std::string row;
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);  // sambung sel kiri ke kanan
                lines.push_back(row);
            }
            return lines;
        }
    };  // akhir struct BrailleCanvas

    // Berfungsi menggambar sketsa tabung pada kanvas Braille: tutup dan alas berupa elips yang dihitung dari
    // persamaannya (x = a cos t, y = b sin t), dinding berupa dua garis tegak, permukaan tutup diarsir tipis, dan
    // tepi alas yang tertutup badan tabung digambar putus-putus seperti pada buku cetak. Garis jari-jari R dan panah
    // tinggi T dibubuhkan sebagai teks berwarna kuning agar langsung terbaca.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal: kanvas maksimal 46 sel dan minimal 24 sel; tinggi mengikuti
        // perbandingan gambar aslinya (21 baris untuk 46 sel) agar bentuk tabung tidak gepeng.
        // available = lebar terminal dikurangi 8 (tepi dan margin kotak 5 + indentasi gambar 3).
        const int available = terminalWidth() - 8;
        // canvasCols = lebar tersedia dikurangi 6 (ruang panah T di kanan), dibatasi std::min paling besar 46 dan
        // std::max paling kecil 24.
        int canvasCols = std::max(24, std::min(46, available - 6));
        if (canvasCols % 2 != 0) --canvasCols;  // % = sisa bagi: bila ganjil, dikurangi 1 agar genap
        const int canvasRows = std::max(12, canvasCols * 21 / 46);  // tinggi sebanding 21/46, minimal 12 baris
        // legend = daftar 4 baris keterangan gambar, ditulis langsung dengan { ... } (indeks 0 sampai 3).
        const std::vector<std::string> legend = {
            "R = jari-jari (pusat ke tepi), D = diameter = 2 x R",
            "T = tinggi tabung (jarak alas ke tutup)",
            "V = Luas alas x T = Pi x R x R x T",
            "garis putus-putus = tepi alas yang tersembunyi di belakang badan tabung",
        };
        // Lebar keterangan terpanjang di antara legend[0] sampai legend[2] (hanya ketiganya yang bisa diletakkan di
        // samping; legend[3] selalu di bawah gambar).
        std::size_t longestLegend = 0;
        for (int i = 0; i < 3; ++i) longestLegend = std::max(longestLegend, displayWidth(legend[static_cast<std::size_t>(i)]));
        // Keterangan diletakkan di samping hanya bila baris terlebar (gambar + panah T + keterangan terpanjang) masih
        // muat di dalam kotak (lebar terminal - 5), dikarenakan drawBox() (BAGIAN 3) akan membungkus baris yang lebih
        // panjang dan itu merusak gambar. 3 = indentasi, 5 = kolom panah "    │", 6 = jarak ke keterangan.
        const bool legendOnRight = (3 + canvasCols + 5 + 6 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);
        // π yang lebih teliti, hanya untuk menggambar; volume tetap memakai π pilihan pengguna
        constexpr double PI_DRAWING = 3.14159265358979;

        BrailleCanvas canvas(canvasCols, canvasRows);  // membuat kanvas kosong (constructor BrailleCanvas di atas)
        // Ukuran elips memakai perbandingan gambar asli setinggi 84 titik (21 baris x 4 titik), sehingga bentuknya
        // sama untuk ukuran kanvas berapa pun: setengah sumbu tegak 9/84, pusat tutup 11/84, jarak tutup-alas 58/84.
        const double semiMajor = canvas.pixelWidth() / 2.0 - 2.0;        // setengah sumbu mendatar elips (titik)
        const double semiMinor = canvas.pixelHeight() * 9.0 / 84.0;      // setengah sumbu tegak elips (titik)
        const double cx = canvas.pixelWidth() / 2.0;             // pusat mendatar
        // yTop = posisi tegak pusat elips tutup; yBottom = posisi tegak pusat elips alas.
        const double yTop = canvas.pixelHeight() * 11.0 / 84.0, yBottom = yTop + canvas.pixelHeight() * 58.0 / 84.0;

        // Berfungsi menggambar satu elips; bila dashedBack, bagian belakang (atas) digambar putus-putus abu-abu.
        // auto drawEllipse = [&](...) { ... }; menyimpan lambda (fungsi kecil tanpa nama) ke variabel drawEllipse;
        // auto = tipenya ditentukan compiler. [&] berarti lambda boleh memakai variabel di sekitarnya (canvas, cx,
        // semiMajor, semiMinor, PI_DRAWING) secara langsung tanpa disalin.
        auto drawEllipse = [&](double yCenter, unsigned char frontLayer, bool dashedBack) {
            const int N = 720;  // 720 langkah sudut agar garis tersambung rapat
            for (int i = 0; i < N; ++i) {
                const double t = 2.0 * PI_DRAWING * i / N;  // sudut t dari 0 sampai hampir 2π (satu putaran penuh)
                // Titik elips: x = cx + a cos t, y = yCenter + b sin t; static_cast<int> membuang angka desimal,
                // dikarenakan posisi titik pada kanvas harus bilangan bulat.
                const int x = static_cast<int>(cx + semiMajor * std::cos(t)), y = static_cast<int>(yCenter + semiMinor * std::sin(t));
                const bool isBack = std::sin(t) < 0;  // y lebih kecil = tepi belakang, tertutup badan tabung
                // Tepi belakang yang diminta putus-putus hanya digambar pada kelompok 18 langkah yang berselang-seling
                // ((i / 18) % 2 == 0), dengan lapisan 4 (abu-abu); selain itu digambar utuh dengan frontLayer.
                if (isBack && dashedBack) { if ((i / 18) % 2 == 0) canvas.setPixel(x, y, 4); }
                else canvas.setPixel(x, y, frontLayer);
            }
        };  // akhir lambda drawEllipse
        // Arsiran tipis pada permukaan tutup: ex dan ey = posisi titik relatif terhadap pusat tutup dibagi setengah
        // sumbu; ex * ex + ey * ey < 0.9 berarti titik ada di dalam elips (sedikit ke dalam dari tepinya), dan
        // (x + y) % 8 == 0 hanya memilih titik pada garis-garis miring berjarak 8 agar arsirannya tipis (lapisan 1).
        for (int y = 0; y < canvas.pixelHeight(); ++y) {           // setiap baris titik kanvas
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double ex = (x + 0.5 - cx) / semiMajor, ey = (y + 0.5 - yTop) / semiMinor;
                if (ex * ex + ey * ey < 0.9 && (x + y) % 8 == 0) canvas.setPixel(x, y, 1);
            }
        }
        drawEllipse(yTop, 2, false);                                      // tutup: elips penuh, lapisan 2
        // xLeft dan xRight = posisi mendatar dinding kiri dan kanan (tepi kiri dan kanan elips).
        const int xLeft = static_cast<int>(cx - semiMajor), xRight = static_cast<int>(cx + semiMajor);
        // dinding kiri dan kanan dari tutup sampai alas, 2 titik tebal, lapisan 3
        for (int y = static_cast<int>(yTop); y <= static_cast<int>(yBottom); ++y) {
            canvas.setPixel(xLeft, y, 3); canvas.setPixel(xLeft + 1, y, 3);
            canvas.setPixel(xRight, y, 3); canvas.setPixel(xRight + 1, y, 3);
        }
        drawEllipse(yBottom, 2, true);                                        // alas: depan utuh, belakang putus-putus
        // Warna tiap lapisan, indeks = nomor lapisan: 0 = kosong (tanpa warna), 1 = arsiran (ANSI_SURFACE),
        // 2 = elips tutup dan alas, 3 = dinding (keduanya ANSI_TITLE), 4 = tepi alas putus-putus (ANSI_DIM).
        const std::vector<const char*> layerColors = { "", ANSI_SURFACE, ANSI_TITLE, ANSI_TITLE, ANSI_DIM };
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // gambar sebagai baris-baris teks

        // baris sel tutup dan alas: posisi titik dibagi 4, dikarenakan satu sel tingginya 4 titik
        const int rowTop = static_cast<int>(yTop) / 4, rowBottom = static_cast<int>(yBottom) / 4;
        // Kurung kurawal { ... } tanpa if/for membuat blok tersendiri, agar variabel di dalamnya (mis. row) hanya
        // berlaku di blok itu dan namanya boleh dipakai lagi di blok berikutnya.
        {   // garis jari-jari "●──R──►" pada baris tengah tutup
            const int centerCol = canvasCols / 2;  // kolom tengah kanvas = pusat tutup
            std::string radiusLine = "\u25cf";  // \u25cf = ● (titik pusat)
            // Garis ─ dari kolom setelah pusat sampai sebelum ujung panah, dengan huruf R di tengahnya.
            for (int c = centerCol + 1; c < canvasCols - 2; ++c) radiusLine += (c == (centerCol + canvasCols - 2) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";  // \u25ba = ► (ujung panah di tepi tutup)
            std::string row;
            // Baris tutup = sel kanvas di kiri pusat + garis jari-jari kuning + sel paling kanan.
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, rowTop, layerColors);
            figureLines[rowTop] = row + colorize(ANSI_HINT, radiusLine) + canvas.cellString(canvasCols - 1, rowTop, layerColors);
        }
        {   // tulisan TABUNG di tengah badan
            const int r = (rowTop + rowBottom) / 2;  // baris tengah antara tutup dan alas
            std::string row;
            for (int c = 0; c < canvasCols; ++c) {
                // Pada kolom tengah - 3, sisipkan "TABUNG" (6 huruf) lalu lompati 5 kolom berikutnya (ditambah ++c
                // dari for = 6 kolom), agar lebar baris tetap sama; kolom lain diisi sel kanvas biasa.
                if (c == canvasCols / 2 - 3) { row += colorize(ANSI_TITLE, "TABUNG"); c += 5; }
                else row += canvas.cellString(c, r, layerColors);
            }
            figureLines[r] = row;  // ganti baris tengah gambar dengan baris bertulisan
        }
        // Panah tinggi T selalu di samping kanan; keterangan di samping bila jendela lebar, di bawah bila sempit.
        // sideNotes = teks di kanan setiap baris gambar; (canvasRows, "") membuat canvasRows buah teks kosong.
        std::vector<std::string> sideNotes(canvasRows, "");
        // Baris tutup "  ──▲", baris alas "  ──▼", dan di antaranya garis tegak "    │"; operator
        // ternary bersusun: bila baris tutup pilih ▲, bila baris alas pilih ▼, selain itu │.
        for (int r = rowTop; r <= rowBottom; ++r) {
            sideNotes[r] = (r == rowTop) ? "  \u2500\u2500\u25b2" : (r == rowBottom) ? "  \u2500\u2500\u25bc" : "    \u2502";
        }
        sideNotes[(rowTop + rowBottom) / 2] += "  T";  // huruf T di tengah panah
        if (legendOnRight) {  // jendela lebar: tiga keterangan pertama ditaruh di samping panah
            sideNotes[rowTop + 1]              += "      " + legend[0];
            sideNotes[(rowTop + rowBottom) / 2]    += "   " + legend[1];
            sideNotes[rowBottom - 1]               += "      " + legend[2];
        }

        // Susun isi kotak: 3 spasi indentasi + baris gambar + catatan samping berwarna kuning.
        std::vector<std::string> lines;
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + figureLines[r] + colorize(ANSI_HINT, sideNotes[r]));
        // jendela sempit: tiga keterangan pertama diletakkan di bawah gambar
        if (!legendOnRight) for (int i = 0; i < 3; ++i) lines.push_back("   " + legend[i]);
        lines.push_back("   " + colorize(ANSI_DIM, legend[3]));  // keterangan garis putus-putus, selalu di bawah
        drawBox("Sketsa Tabung", lines);  // gambar semuanya di dalam kotak berjudul (drawBox(), BAGIAN 3)
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Berfungsi untuk menampilkan petunjuk pengisian dalam kotak sebelum pertanyaan pertama, dikarenakan pengguna
    // perlu tahu bentuk input yang diterima (pilihan, angka > 0, cara menulis desimal) agar tidak salah ketik.
    // Daftar teks di dalam { ... } otomatis menjadi std::vector<std::string> untuk drawBox() (BAGIAN 3); di dalam
    // teks, \" berarti tanda kutip yang ikut tercetak dan \u03c0 tercetak sebagai π.
    void printInstructions() {
        drawBox("Petunjuk Pengisian", {
            "- Satuan Panjang     : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)",
            "                       (contoh: Pilih [2] untuk satuan \"cm\")",
            "- Nilai Pi (\u03c0)       : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila jari-jari atau diameter kelipatan 7",
            "                       (contoh: Pilih [2] untuk nilai \"22/7\")",
            "- Ukuran Diketahui   : Nomor pilihan 1-2; 1 = jari-jari, 2 = diameter (program menghitung R = D / 2)",
            "                       (contoh: Pilih [2] bila soal menyebut \"diameter\")",
            "- Jari-Jari/Diameter : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                       (contoh: 7, 7,5 atau 7.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)",
            "- Tinggi Tabung      : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                       (contoh: 10, 10,5 atau 10.5)",
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang."
        });
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Berfungsi untuk membuat satu baris hasil "Label : nilai satuan" dengan 'decimals' angka desimal (bawaan 2),
    // siap dimasukkan ke kotak. int decimals = 2 adalah argumen bawaan: bila pemanggil tidak mengisinya, nilainya 2.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit, int decimals = 2) {
        std::ostringstream line;  // menggunakan ostringstream berfungsi menyusun teks tanpa mengubah format std::cout
        // std::left = rata kiri; std::setw(22) = label diberi lebar 22 kolom agar titik dua semua baris sejajar;
        // std::fixed dan std::setprecision(decimals) = angka ditulis dengan tepat 'decimals' angka di belakang titik.
        line << std::left << std::setw(22) << label << ": " << std::fixed << std::setprecision(decimals) << value;
        if (!unit.empty()) line << ' ' << unit;  // satuan ditambahkan hanya bila tidak kosong
        return line.str();  // str() mengambil teks yang sudah tersusun
    }

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 2 =====
// Menggunakan namespace task2 berfungsi untuk memberi nama khas pada run(), dikarenakan program bermenu
// (berkas main.cpp) memanggilnya sebagai task2::run(). Tanda :: berarti "nama di dalam wadah": run milik task2.
namespace task2 {

    // Berfungsi menjalankan seluruh Program 2: tampilan awal, tahap pengaturan, input, perhitungan, dan output.
    // Fungsi ini sengaja tidak berada di namespace tanpa nama, dikarenakan harus bisa dipanggil dari main.cpp.
    void run() {
        // Mengatur konsol Windows ke UTF-8 (enableUtf8Console(), BAGIAN 3); dipanggil di sini agar program mandiri
        // (tanpa menu) juga benar; aman dipanggil berulang.
        enableUtf8Console();
        printTitle("PROGRAM 2 : VOLUME TABUNG");  // judul program (printTitle(), BAGIAN 5)
        // Mencetak rumus; \u03c0 di dalam teks tercetak sebagai π, dan \n\n = pindah baris dua kali.
        std::cout << "Rumus: V = Pi x R x R x T   (Pi = \u03c0; modul menulisnya phi)\n\n";
        printSketch();    // berfungsi menampilkan sketsa bangun agar pengguna tahu bagian mana yang dimaksud R, D, T
        printInstructions();  // kotak petunjuk pengisian (BAGIAN 5 lanjutan)

        // --- Tahap pengaturan: satuan, nilai π, dan ukuran yang diketahui ---
        // reference (&) dikarenakan cukup menunjuk ke isi daftar UNITS (BAGIAN 2), tidak perlu disalin
        const LengthUnit& unit = chooseUnit();
        std::string   piLabel;                  // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double  pi = choosePi(piLabel);    // nilai π yang dipakai perhitungan; choosePi() juga mengisi piLabel
        const bool    diameterGiven = chooseDiameterInput();  // true bila soal menyebut diameter

        // --- Tahap input ---
        // Menggunakan tipe data double berfungsi untuk menyimpan ukuran tabung, dikarenakan jari-jari dan tinggi
        // boleh desimal (mis. 7.5). Menggunakan readPositiveDecimal() (BAGIAN 4) dikarenakan ukuran bangun tidak
        // mungkin nol atau negatif. Prompt menyertakan satuan agar pengguna tahu satuan yang dipakai.
        // label = awal prompt, dipilih dengan operator ternary sesuai ukuran yang diketahui.
        const std::string label = diameterGiven ? "Masukkan Diameter Alas (" : "Masukkan Jari-Jari Alas (";
        // prompt = label + simbol satuan + ") : ", mis. "Masukkan Jari-Jari Alas (cm) : "
        const double inputSize = readPositiveDecimal(label + unit.symbol + ") : ");
        // std::string(...) diperlukan dikarenakan dua teks mentah (teks dalam tanda kutip dan unit.symbol yang
        // bertipe const char*) tidak bisa disambung langsung dengan +.
        const double height = readPositiveDecimal(std::string("Masukkan Tinggi Tabung (") + unit.symbol + ")  : ");
        // Menggunakan operator ternary (kondisi ? a : b) berfungsi memilih: bila diameterGiven, radius = inputSize / 2;
        // bila tidak, radius = inputSize.
        const double radius = diameterGiven ? inputSize / 2.0 : inputSize;

        // --- Tahap perhitungan ---
        // Menggunakan dua langkah (luas alas lalu volume) berfungsi agar rumusnya mudah dibaca:
        // luas alas lingkaran = π x r x r, volume = luas alas x tinggi. Operator * = perkalian.
        const double baseArea = pi * radius * radius;  // luas alas = π x r x r
        const double volume   = baseArea * height;  // volume = luas alas x tinggi
        // padanan liter = volume x isi 1 satuan kubik dalam liter (anggota litersPerCubicUnit, BAGIAN 2),
        // dikarenakan soal cerita sering meminta liter
        const double liters    = volume * unit.litersPerCubicUnit;

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit.symbol) + "\u00b2";  // satuan luas, mis. cm² (\u00b2 = ²)
        const std::string volumeUnit = std::string(unit.symbol) + "\u00b3";  // satuan volume, mis. cm³ (\u00b3 = ³)
        // menggunakan vector dikarenakan jumlah baris berbeda (jari-jari vs diameter); push_back = tambah di akhir
        std::vector<std::string> resultLines;
        resultLines.push_back("Pi (\u03c0) yang Digunakan : " + piLabel);
        if (diameterGiven) {  // bila diameter yang diketahui, tampilkan langkah r = d/2 seperti penyelesaian soal
            resultLines.push_back(formatResultLine("Diameter (D)", inputSize, unit.symbol));
            resultLines.push_back(formatResultLine("Jari-Jari (R) = D / 2", radius, unit.symbol));
        } else {  // jari-jari yang diketahui: cukup satu baris
            resultLines.push_back(formatResultLine("Jari-Jari (R)", radius, unit.symbol));
        }
        resultLines.push_back(formatResultLine("Tinggi (T)", height, unit.symbol));
        resultLines.push_back(formatResultLine("Luas Alas", baseArea, areaUnit));
        // hasil akhir ditonjolkan hijau tebal (ANSI_SUCCESS)
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Volume Tabung", volume, volumeUnit)));
        // padanan liter ditulis dengan 3 angka desimal (argumen terakhir = decimals)
        resultLines.push_back(formatResultLine("Setara", liters, "liter (1 liter = 1 dm\u00b3)", 3));
        std::cout << '\n';  // baris kosong sebelum kotak hasil
        drawBox("Hasil Perhitungan", resultLines);  // cetak semua baris hasil di dalam kotak (drawBox(), BAGIAN 3)
    }  // akhir run()

}  // akhir namespace task2

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU berfungsi untuk menyertakan main() hanya saat berkas ini dikompilasi sendiri,
// dikarenakan program bermenu sudah punya main() dan C++ hanya mengizinkan satu main(). #ifndef = "bila makro
// belum didefinisikan"; BUILD_WITH_MENU baru terdefinisi bila perintah kompilasi memakai -DBUILD_WITH_MENU.
#ifndef BUILD_WITH_MENU
// main() adalah titik awal: fungsi pertama yang dijalankan saat program dibuka. int = main() mengembalikan
// kode angka ke sistem operasi.
int main() {
    // cetak identitas (tebal), lalu jalankan Program 2 lewat task2::run() (BAGIAN 6)
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 2 - Fadhlillah (2531070011)") << '\n';
    task2::run();
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif  // akhir bagian yang hanya ada bila berkas dikompilasi sendiri (tanpa BUILD_WITH_MENU)
