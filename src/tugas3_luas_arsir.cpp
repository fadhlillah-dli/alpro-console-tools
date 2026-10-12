/*
 * Tugas Praktikum 3 — Modul 1: Input dan Output
 * Program 3 : Luas Bangun yang Diarsir
 * Alur : Pengguna memilih satuan panjang (mm/cm/dm/m) dan nilai π, lalu mengetik sisi sebuah persegi
 *        (bujur sangkar). Di dalam persegi itu ada lingkaran yang menyinggung keempat sisinya, jadi
 *        jari-jarinya r = s / 2. Keluarannya: luas daerah yang diarsir, yaitu luas persegi dikurangi
 *        luas lingkaran.
 *        Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2)); Pi = π (modul menulisnya phi).
 *        Nilai π boleh dipilih: 3.14 sesuai modul, atau 22/7 untuk soal yang jari-jarinya kelipatan 7
 *        (berarti sisi perseginya kelipatan 14).
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri. Perintah di bawah membuat program bernama tugas3,
 * lalu langsung menjalankannya:
 *   g++ -std=c++17 -Wall -Wextra -o tugas3 tugas3_luas_arsir.cpp && ./tugas3
 * Isi perintah itu, satu per satu:
 *   g++           : compiler C++, penerjemah kode C++ menjadi program yang bisa dijalankan.
 *   -std=c++17    : pakai aturan bahasa C++ versi 2017.
 *   -Wall -Wextra : nyalakan peringatan tambahan, supaya kesalahan kecil cepat ketahuan.
 *   -o tugas3     : beri nama program hasilnya tugas3.
 *   && ./tugas3   : jalankan program, tetapi hanya bila kompilasi berhasil.
 * Bila digabung ke program bermenu (main.cpp di folder src), tambahkan -DBUILD_WITH_MENU. Opsi -D
 * membuat sebuah makro bernama BUILD_WITH_MENU (apa itu makro, lihat BAGIAN 1), supaya main() di
 * bagian paling bawah berkas ini tidak ikut dikompilasi (lihat BAGIAN 7).
 *
 * Komentar di berkas ini ditulis untuk pembaca yang baru belajar C++. Tiap blok dimulai dari gambaran
 * besarnya, baru detailnya; istilah baru dijelaskan sekali saat pertama kali muncul.
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Program ini tidak menulis semuanya dari nol. Untuk mencetak ke layar, menyimpan teks, atau memilih
// nilai terbesar, kita meminjam dari pustaka standar C++. Caranya lewat #include.
// #include menyalin isi sebuah header ke berkas ini. Header adalah berkas pustaka berisi fungsi dan tipe
// siap pakai. Penyalinan itu dikerjakan preprocessor, yaitu tahap persiapan sebelum kompilasi dimulai.
// Nama di dalam < > adalah header pustaka standar. Semua nama milik pustaka standar berawalan std::
// (std = standard), jadi std::cout dibaca "cout milik pustaka standar".
// Tiap baris di bawah menyebut apa yang kita ambil dari header itu.

// std::max dan std::min: memilih yang lebih besar atau lebih kecil dari dua angka.
// Dipakai saat menghitung lebar jendela, lebar kotak, dan ukuran sketsa.
#include <algorithm>
// std::isdigit: apakah satu karakter adalah angka 0-9? Dipakai saat memeriksa ketikan di BAGIAN 4.
#include <cctype>
// std::hypot (jarak dua titik, seperti sisi miring segitiga) dan std::fabs (nilai mutlak bilangan desimal).
// Keduanya dipakai saat menghitung lingkaran sketsa di BAGIAN 5.
#include <cmath>
// std::size_t: tipe bilangan bulat tak bertanda (tidak pernah negatif). C++ memakainya untuk ukuran dan
// jumlah, misalnya panjang teks dan lebar kolom.
#include <cstddef>
// Dari header ini kita ambil tiga hal. std::exit dan EXIT_FAILURE menghentikan program dengan kode "gagal".
// std::getenv membaca variabel lingkungan, yaitu pengaturan yang diwariskan terminal ke program.
// std::atoi mengubah teks menjadi bilangan bulat.
#include <cstdlib>
// std::setw dan std::setprecision: pengatur lebar kolom dan jumlah angka desimal saat mencetak.
#include <iomanip>
// std::initializer_list: tipe yang menerima daftar nilai di dalam kurung kurawal { ... }.
// Dengan ini daftar opsi bisa ditulis langsung saat memanggil fungsi (lihat BAGIAN 5).
#include <initializer_list>
// std::cin (membaca ketikan), std::cout (mencetak ke layar), dan std::cerr (mencetak pesan kesalahan).
#include <iostream>
// std::istringstream (membaca angka dari teks) dan std::ostringstream (menyusun teks seperti mencetak).
#include <sstream>
// std::string (teks yang panjangnya bisa berubah), std::getline (membaca satu baris ketikan),
// dan std::to_string (angka menjadi teks).
#include <string>
// std::string_view: "jendela" ke teks yang sudah ada. Isinya hanya dibaca, tidak disalin, jadi lebih
// ringan untuk teks pertanyaan yang hanya perlu dicetak.
#include <string_view>
// std::vector: daftar yang isinya bisa bertambah. Tipe isinya ditulis di dalam < >, misalnya
// std::vector<std::string> = daftar teks. Dipakai untuk baris-baris kotak dan titik-titik sketsa.
#include <vector>
// Program ini harus jalan di Windows maupun macOS/Linux, padahal cara ketiganya "berbicara" dengan
// terminal berbeda: mendeteksi terminal, membaca lebar jendela, menyalakan warna, dan UTF-8 (lihat BAGIAN 3).
// Jadi di sini kita bertanya ke compiler: "ini sedang dikompilasi untuk Windows atau bukan?"
// Itulah arti #ifdef _WIN32 (if defined: apakah nama _WIN32 ada? di Windows nama itu otomatis ada).
// Baris sampai #else hanya ikut di Windows, baris setelah #else hanya di macOS/Linux.
#ifdef _WIN32
    // Sebelum memanggil windows.h, kita pastikan dulu makro NOMINMAX sudah ada.
    // Makro itu seperti catatan tempel untuk compiler: nama yang diganti teks lain.
    // NOMINMAX artinya "jangan buat min dan max", karena windows.h punya kebiasaan membuat makro
    // min/max sendiri yang merusak std::min dan std::max kita.
    // #ifndef = "if not defined": blok ini hanya jalan bila NOMINMAX belum ada, dikarenakan MinGW
    // (g++ untuk Windows, mis. dari MSYS2) sudah mendefinisikannya lebih dulu dan mendefinisikan ulang
    // nama yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    #define NOMINMAX 1  // bentuknya: #define NAMA NILAI; mulai baris ini NOMINMAX berarti 1
    #endif  // selesai mengecek NOMINMAX
    // windows.h adalah pustaka resmi Windows. Dari sini kita memakai fungsi untuk warna, UTF-8, dan
    // lebar jendela; semuanya dipanggil di fungsi-fungsi BAGIAN 3.
    #include <windows.h>
#else  // bukan Windows, berarti macOS atau Linux
    #include <cstdio>       // stdout = saluran keluaran ke layar; fileno memberi nomor saluran itu
    #include <sys/ioctl.h>  // untuk ioctl dan TIOCGWINSZ: bertanya ke terminal seberapa lebar jendelanya
    #include <unistd.h>     // untuk isatty: memeriksa apakah keluaran benar-benar terminal, bukan berkas
#endif  // selesai memilih pustaka sesuai sistem operasi

// Satu hal penting sebelum masuk ke isi. tugas1_nilai_mahasiswa.cpp dan tugas2_volume_tabung.cpp (folder
// src) juga punya fungsi bantu bernama sama, misalnya colorsEnabled() dan drawBox() (di berkas ini ada di
// BAGIAN 3). Kalau ketiganya digabung ke program bermenu, g++ bingung "yang mana?" dan gagal dengan error
// "multiple definition".
// Jalan keluarnya: namespace tanpa nama. namespace adalah wadah nama; nama di dalamnya terpisah dari nama
// yang sama di wadah lain. Bila wadahnya tidak diberi nama, isinya hanya terlihat dari berkas ini saja.
// Blok ini berakhir pada baris "}  // namespace" sebelum BAGIAN 6.
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // Angka yang tidak pernah berubah selama program jalan kita beri nama di sini, supaya tidak ada "angka
    // ajaib" tersebar di dalam fungsi. constexpr menandai nilai tetap: sudah pasti sejak kompilasi dan
    // tidak bisa diubah. double adalah tipe bilangan desimal.
    // Mengapa π bawaannya 3.14? Supaya sama dengan program 2 (tugas2_volume_tabung.cpp di folder src) dan
    // modul. Modul menulisnya "phi", tetapi yang dimaksud π (pi), bukan φ (phi) rasio emas 1,618.
    constexpr double PI_MODULE   = 3.14;        // nilai bawaan, sesuai modul
    // Ditulis 22.0 / 7.0, bukan 22 / 7: pembagian dua bilangan bulat hasilnya 3, bukan 3.142857.
    constexpr double PI_FRACTION = 22.0 / 7.0;  // nilai soal sekolah bila jari-jari kelipatan 7 (sisi kelipatan 14)
    // int adalah tipe bilangan bulat. Dua lebar di bawah dihitung dalam jumlah karakter.
    constexpr int    TITLE_WIDTH = 60;         // lebar garis judul
    constexpr int    LABEL_WIDTH = 24;         // lebar kolom label pertanyaan pilihan, agar semua opsi sejajar
    constexpr double DIMENSION_MAX = 1000000.0;  // batas atas ukuran: menolak salah ketik yang tidak masuk akal
    // Warna arsiran pada sketsa: merah, meniru gambar di soal.
    // Tipenya const char*. Tanda * berarti penunjuk (pointer): ia menyimpan alamat tempat teks itu berada.
    // const berarti teks itu tidak akan diubah.
    // Isinya kode warna ANSI: teks khusus yang dibaca terminal sebagai perintah, bukan dicetak.
    // \033 adalah karakter ESC, nomor 27 yang ditulis dalam oktal (basis 8). Lalu "[31m" berarti "merah".
    constexpr const char* ANSI_HATCH = "\033[31m";

    // Daftar satuan panjang yang lazim di soal. Tanda [] setelah nama berarti array: daftar bernomor yang
    // jumlahnya tetap; nomornya (indeks) mulai dari 0. Urutannya = nomor pilihan 1-4.
    constexpr const char* UNITS[] = { "mm", "cm", "dm", "m" };
    constexpr int         UNIT_COUNT   = 4;  // jumlah isi UNITS; batas nomor pilihan di chooseUnit() (BAGIAN 5)
    // Hati-hati: 2 di sini adalah NOMOR PILIHAN untuk cm (pilihan [2]), bukan indeks array. UNITS[2] adalah
    // "dm"; cm ada di UNITS[1]. chooseUnit() mengurangi 1 lebih dulu sebelum mengambil dari UNITS.
    // Mengapa cm? Hampir semua soal luas daerah arsiran memakai cm.
    constexpr int         DEFAULT_UNIT_INDEX   = 2;

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, pilihan yang ditegaskan, hasil akhir, dan pesan kesalahan mudah dibedakan, program
    // memakai warna. Kode warna ANSI sudah kita lihat di ANSI_HATCH; angka di dalamnya: 0 = normal,
    // 1 = tebal, 31 = merah, 32 = hijau, 33 = kuning, 36 = cyan, 90 = abu-abu. Dua angka dipisah ; berarti
    // digabung. Terminal macOS, Linux, VS Code, dan Windows Terminal semuanya memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // 0 = kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // 1 = tebal: pertanyaan dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // 1;36 = cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // 1;32 = hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // 1;31 = merah tebal: pesan kesalahan
    // 33 = kuning: keterangan nilai bawaan [Enter = ...] dan garis jari-jari R pada sketsa.
    constexpr const char* ANSI_HINT = "\033[33m";
    constexpr const char* ANSI_DIM    = "\033[90m";    // 90 = abu-abu: garis kotak dan garis pembatas

    // Fungsi pertama kita. Fungsi adalah sekumpulan perintah bernama yang bisa dipanggil berulang.
    // void berarti fungsi ini tidak mengembalikan nilai.
    // () kosong berarti ia tidak menerima parameter, yaitu nilai masukan untuk fungsi.
    // Tugasnya: menyiapkan konsol Windows supaya π, ², garis kotak, dan sketsa Braille (lihat BAGIAN 5)
    // tampil benar. Di macOS/Linux terminal sudah siap, jadi fungsi ini tidak melakukan apa-apa.
    // Dipanggil run() di BAGIAN 6.
    void enableUtf8Console() {
    // Simbol-simbol tadi dikirim sebagai UTF-8, cara umum menyimpan huruf sebagai deretan byte.
    // Huruf Latin biasa butuh 1 byte, π dan ² 2 byte, garis kotak dan Braille 3 byte.
    // Masalahnya, Command Prompt dan PowerShell bawaan masih memakai code page lama (mis. 437).
    // Code page adalah tabel Windows yang menentukan byte mana tampil sebagai huruf apa.
    // Dengan tabel yang salah, π tampil sebagai huruf acak. Jadi sebelum keluaran pertama kita pindahkan
    // konsol ke code page 65001 = UTF-8. Ini hanya berlaku untuk jendela konsol yang sedang dipakai, sama
    // seperti perintah chcp 65001 pada peluncur jalankan.bat di folder utama proyek.
    // #ifdef _WIN32 lagi: dua baris di bawah hanya ikut di Windows, karena fungsinya hanya ada di windows.h.
    #ifdef _WIN32
        // SetConsoleOutputCP mengganti code page keluaran, SetConsoleCP code page masukan.
        // CP_UTF8 adalah konstanta Windows bernilai 65001.
        SetConsoleOutputCP(CP_UTF8);  // keluaran: byte UTF-8 tampil sebagai karakter yang benar
        SetConsoleCP(CP_UTF8);        // masukan: ketikan di luar huruf Latin biasa juga dibaca sebagai UTF-8
    #endif  // akhir bagian khusus Windows
    }

    // Boleh pakai warna atau tidak? Aturannya mengikuti kebiasaan program terminal: kalau pengguna menyetel
    // NO_COLOR, warna mati; kalau FORCE_COLOR, warna dipaksa hidup; selain itu warna hanya dipakai bila
    // keluaran benar-benar terminal, bukan berkas. Di Windows ada satu langkah tambahan: menyalakan mode
    // "virtual terminal" supaya Command Prompt memahami kode ANSI. Hasilnya bool, tipe dengan dua nilai:
    // true (benar) berarti boleh berwarna, false (salah) berarti tidak. Dipakai colorize() di bawah.
    bool colorsEnabled() {
        // Pemeriksaannya cukup sekali seumur program, jadi hasilnya disimpan di variabel static (dibuat
        // sekali, diingat terus); const karena tidak perlu diubah lagi. Nilai awalnya dihitung oleh lambda,
        // yaitu fungsi kecil tanpa nama berbentuk []() { ... }, yang langsung dijalankan oleh tanda () di
        // baris "}();" paling bawah.
        static const bool enabled = []() {
            // getenv membaca variabel lingkungan. Hasil nullptr (penunjuk kosong) berarti variabel itu
            // tidak ada; != nullptr berarti "ada", apa pun isinya. return langsung mengakhiri lambda.
            if (std::getenv("NO_COLOR") != nullptr) return false;
            if (std::getenv("FORCE_COLOR") != nullptr) return true;
        // Cara mendeteksi terminal berbeda per sistem operasi, jadi #ifdef _WIN32 lagi.
        #ifdef _WIN32
            // Di Windows, kode warna ANSI baru dimengerti setelah mode "virtual terminal" dinyalakan.
            // Caranya: ambil pegangan (HANDLE) ke layar, baca mode konsol yang sedang aktif, lalu tulis
            // kembali mode itu dengan tambahan satu bendera.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;  // DWORD = bilangan bulat milik Windows, tempat mode konsol disimpan
            // Kalau pegangannya tidak sah, atau keluaran ternyata bukan konsol (misalnya dialihkan ke
            // berkas), menyerah saja: tanpa warna. Di baris bawah, || berarti "atau" dan ! berarti "tidak".
            // &mode (tanda & di depan nama variabel) memberi alamat variabel mode, supaya GetConsoleMode
            // bisa mengisinya langsung.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // ENABLE_VIRTUAL_TERMINAL_PROCESSING adalah bendera (pengaturan nyala/mati) itu. Tanda |
            // menambahkannya ke mode lama tanpa menghapus yang sudah ada. SetConsoleMode mengembalikan 0
            // bila gagal, jadi "!= 0" artinya berhasil = warna boleh dipakai.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // Di macOS/Linux cukup satu pertanyaan: apakah stdout sebuah terminal (tty)?
            // isatty mengembalikan bukan-0 bila ya, jadi != 0 mengubahnya menjadi true/false.
            return isatty(fileno(stdout)) != 0;
        #endif  // selesai memilih cara deteksi terminal
        }();  // tanda () ini menjalankan lambda sekali; hasilnya jadi isi variabel enabled
        return enabled;  // pemanggilan berikutnya tinggal memakai hasil yang tersimpan
    }

    // Pembungkus kecil: beri warna pada sepotong teks, tetapi hanya bila warna memang boleh dipakai. Kalau
    // tidak, teks dikembalikan apa adanya, karena kode warna yang ikut tercetak ke berkas atau ke terminal
    // tanpa warna hanya jadi sampah teks. Fungsi ini mengembalikan std::string dan menerima dua parameter:
    // code (kode warna ANSI) dan text (teks yang diwarnai; std::string_view berarti hanya dibaca).
    std::string colorize(const char* code, std::string_view text) {
        // std::string(text) menyalin string_view menjadi std::string supaya bisa dikembalikan.
        if (!colorsEnabled()) return std::string(text);
        // Tanda + pada std::string menyambung teks: kode warna, teks, lalu ANSI_RESET supaya warna tidak
        // bocor ke teks berikutnya.
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Tidak semua karakter sama lebarnya di terminal. Aksara CJK (Cina/Jepang/Korea), Hangul, dan emoji
    // memakai dua kolom, bukan satu. Kalau ini diabaikan, lebar kotak dan pemotongan baris jadi salah.
    // Fungsi ini menjawab: apakah sebuah code point termasuk karakter lebar? Code point adalah nomor
    // karakter dalam tabel Unicode, biasa ditulis heksadesimal (basis 16, awalan 0x). Parameternya bertipe
    // unsigned, singkatan dari unsigned int = bilangan bulat tak bertanda. Dipakai displayWidth() di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Setiap pasangan (codePoint >= awal && codePoint <= akhir) adalah satu rentang; && berarti "dan".
        // Hasilnya true bila code point masuk salah satu rentang: 1100-115F Hangul Jamo,
        // 2E80-A4CF aksara CJK dan Yi, AC00-D7A3 suku kata Hangul, F900-FAFF dan FE30-FE4F CJK
        // kompatibilitas, FF00-FF60 dan FFE0-FFE6 bentuk lebar penuh (fullwidth), 1F300-1F64F dan
        // 1F900-1F9FF emoji.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berapa kolom yang dipakai sebuah teks di layar? Jawabannya bukan sekadar jumlah byte-nya. Pada UTF-8
    // satu karakter bisa terdiri dari 1-4 byte; byte awal memberi tahu berapa byte lanjutan yang harus
    // dirangkai menjadi satu code point. Jadi: π, ², garis kotak, dan Braille dihitung satu kolom; aksara
    // lebar (CJK, emoji) dua kolom; kode warna ANSI nol kolom. Fungsi ini mengembalikan std::size_t dan
    // dipakai oleh fungsi pembungkus baris dan penggambar kotak di bawah.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah dihitung
        bool inEscape = false;  // true selama berada di dalam kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu
        // Kita periksa teks byte demi byte dengan range-based for, bentuk for (x : wadah) yang mengunjungi
        // tiap isi wadah berurutan. Tipe unsigned char dipilih agar nilai byte selalu 0-255; char biasa bisa
        // negatif untuk byte >= 128, dan itu merusak perbandingan di bawah.
        for (const unsigned char c : text) {
            // Di dalam kode warna semua byte diabaikan sampai huruf 'm'. continue = lompat ke byte berikutnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC pembuka kode warna
            // Satu byte terdiri dari 8 bit, yaitu digit biner 0 atau 1.
            // Byte lanjutan UTF-8 selalu berpola 10xxxxxx (x = bit apa saja).
            // c & 0xC0 menyisakan 2 bit teratas saja (& = AND bit). Bila hasilnya 0x80, polanya 10:
            // ini byte lanjutan. Byte lanjutan tidak menambah kolom. Ia hanya menyumbang 6 bit terbawahnya
            // (c & 0x3F) ke code point: << 6 menggeser bit lama ke kiri untuk memberi tempat, lalu | (OR bit)
            // menempelkan 6 bit baru. --pending mengurangi pending 1 lebih dulu, baru dibandingkan dengan 0.
            // Bila itu byte lanjutan terakhir dan code point-nya aksara lebar, ++width menambah satu kolom lagi.
            if ((c & 0xC0) == 0x80) {
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;  // setiap byte awal = satu karakter = minimal satu kolom
            // Byte awal UTF-8 punya pola sendiri: 110xxxxx diikuti 1 byte lanjutan, 1110xxxx diikuti 2,
            // 11110xxx diikuti 3. Polanya dicek dengan & 0xE0 / 0xF0 / 0xF8, dan sisa bit awal
            // (c & 0x1F / 0x0F / 0x07) menjadi bit tertinggi code point. Byte ASCII, yaitu huruf Latin biasa
            // berpola 0xxxxxxx, tidak punya lanjutan: pending = 0.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }
        return width;
    }

    // Seberapa lebar jendela terminal sekarang? Kita bertanya setiap kali dipanggil, bukan sekali saja,
    // supaya tampilan ikut menyesuaikan bila jendela diubah ukurannya. Bila tidak terjawab (misalnya
    // keluaran bukan terminal), kita coba variabel lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan
    // terminal pada umumnya. Hasilnya tidak pernah di bawah 50, karena di bawah itu kotak dan sketsa tidak
    // bisa rapi. Fungsi ini mengembalikan int.
    int terminalWidth() {
        int cols = 0;  // 0 berarti "belum diketahui"; setiap cara di bawah hanya mengisinya bila berhasil
    // Cara bertanya ke terminal berbeda per sistem operasi, jadi #ifdef _WIN32 lagi.
    #ifdef _WIN32
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct Windows berisi info layar konsol. struct adalah wadah
        // beberapa variabel dalam satu nama; anggotanya diambil dengan tanda titik, mis. info.srWindow.
        // GetConsoleScreenBufferInfo mengisi info lewat alamatnya (&info) dan mengembalikan bukan-0 bila
        // berhasil. srWindow adalah kotak jendela; .Right dan .Left nomor kolom kanan dan kirinya, keduanya
        // termasuk, jadi lebar = kanan - kiri + 1.
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // struct winsize (dari <sys/ioctl.h>) menampung ukuran jendela. ioctl dengan perintah TIOCGWINSZ
        // ("tanya ukuran jendela") mengisinya dan mengembalikan 0 bila berhasil; ws.ws_col = jumlah kolom.
        // STDOUT_FILENO (dari <unistd.h>) adalah nomor saluran keluaran layar, sama dengan fileno(stdout).
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // selesai memilih cara membaca ukuran jendela
        // Cadangan 1: variabel lingkungan COLUMNS, berupa teks, diubah ke bilangan bulat oleh std::atoi.
        // Cadangan 2: 80 kolom.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;
        return std::max(50, cols);  // std::max memilih yang lebih besar: hasil tidak pernah di bawah 50
    }

    // Jendela terminal bisa sempit. Daripada baris panjang dipotong sembarangan oleh terminal, kita potong
    // sendiri pada spasi menjadi beberapa baris yang masing-masing muat dalam maxWidth kolom. Baris lanjutan
    // diberi indentasi "gantung" sebanyak hangingIndent spasi, supaya tetap sejajar dengan isi setelah label.
    // Fungsi ini mengembalikan std::vector<std::string>, daftar baris hasil potongan.
    // Parameter text bertipe const std::string&. Tanda & yang ditulis setelah nama tipe berarti referensi:
    // fungsi memakai teks milik pemanggil langsung, tanpa menyalin. Ini beda dengan &mode di colorsEnabled()
    // di atas, yang ditulis di depan variabel dan memberi alamatnya. const berarti teks itu tidak boleh diubah.
    // maxWidth dan hangingIndent diterima sebagai salinan, jadi boleh diubah di dalam fungsi.
    // Dipakai fungsi-fungsi pencetak di bawah: kotak, pesan kesalahan, dan daftar pilihan.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar potongan baris yang akan dikembalikan
        // Isi butuh ruang minimal 10 kolom. Bila indentasi + 10 melebihi lebar maksimal, indentasi dipaksa 2.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // sisa teks yang belum terpotong; makin pendek tiap putaran
        // Ulangi selama sisa teks masih lebih lebar dari batas.
        while (displayWidth(remaining) > maxWidth) {
            // col = kolom layar saat ini; cutPos = posisi byte spasi terakhir yang cocok untuk memotong.
            // std::string::npos adalah nilai khusus yang berarti "tidak ada posisi".
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama di dalam kode warna
            // for klasik, bentuk (awal; syarat; langkah), mencari spasi terakhir sebelum batas lebar. Kolom
            // dihitung seperti di displayWidth(): kode warna dan byte lanjutan UTF-8 dilewati tanpa menambah
            // kolom. remaining.size() = jumlah byte; i = posisi byte yang sedang diperiksa, mulai dari 0.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<tipe>(nilai) mengubah tipe secara eksplisit; di sini char menjadi 0-255.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }
                if (c == 0x1B) { inEscape = true; continue; }
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8: bukan kolom baru
                if (col >= maxWidth) break;  // break menghentikan for: sudah melewati batas lebar
                if (c == ' ' && col > hangingIndent) cutPos = i;  // spasi setelah indentasi: calon titik potong
                ++col;
            }
            if (cutPos == std::string::npos) break;  // tidak ada spasi untuk memotong: biarkan apa adanya
            // push_back menambahkan satu baris ke akhir daftar; substr(0, n) = potongan dari awal sepanjang
            // n byte.
            result.push_back(remaining.substr(0, cutPos));
            // Sisanya dimulai setelah spasi (cutPos + 1) dan diawali hangingIndent spasi;
            // std::string(n, ' ') membuat n spasi.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }
        result.push_back(remaining);  // potongan terakhir, atau seluruh teks bila memang sudah muat
        return result;
    }

    // Menggambar kotak berjudul berisi baris-baris teks, seperti kotak petunjuk dan kotak hasil. Dengan
    // kotak, informasi terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tetapi tidak
    // pernah melebihi lebar terminal: baris yang kepanjangan dibungkus dulu oleh wrapText(), dengan
    // indentasi gantung sejajar label bila baris berpola "Label : isi".
    // Parameter lines bertipe const std::vector<std::string>&: daftar milik pemanggil, dipakai lewat
    // referensi tanpa disalin dan tidak diubah.
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Lebar maksimal isi = lebar terminal dikurangi 5: 2 tepi kotak, 2 margin, dan 1 kolom cadangan,
        // karena sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela.
        // static_cast<std::size_t> menyamakan tipe hasil int dengan tipe lebar teks.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // semua baris setelah dibungkus; inilah yang digambar
        // Tiap baris dipakai lewat referensi (const std::string& b) supaya tidak disalin.
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan bila baris tidak berpola "Label : isi"
            // find(" : ") mencari posisi teks " : "; hasilnya npos bila tidak ada. Hanya label yang lebarnya
            // <= 28 kolom dipakai sebagai patokan, agar baris lanjutan tidak menjorok terlalu jauh.
            const std::size_t labelPos = b.find(" : ");
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // + 3 = lebar " : ", sejajar isi setelahnya
            }
            // Setiap potongan hasil wrapText menjadi satu baris isi kotak.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // + 2 = margin kiri dan kanan; std::max menjaga lebar kotak = baris terlebar.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min memilih yang lebih kecil: kotak tidak melebihi terminal
        // "\u2500" adalah cara menulis karakter Unicode U+2500 (garis ─) di dalam string. filler diisi garis
        // sebanyak sisa lebar setelah judul (judul + 3 hiasan), supaya garis atas kotak tertutup rapat.
        std::string filler;
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";
        // Garis atas: ┌─ (\u250c\u2500), judul, garis pengisi, lalu ┐ (\u2510). Garis abu-abu, judul cyan
        // tebal. std::cout << ... << '\n' mencetak ke layar lalu ganti baris.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Setiap baris isi: │ (\u2502), spasi margin + teks, spasi pengisi sampai selebar kotak, lalu │ lagi.
        // Jumlah spasi pengisi dihitung dengan operator ternary (syarat ? a : b): a bila syarat benar,
        // b bila salah. Hasilnya width - contentWidth, tetapi 0 bila teks lebih lebar. Mengapa dicek dulu?
        // size_t tidak bisa negatif: 3 - 5 tidak menjadi -2, melainkan angka raksasa.
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

    // Mencetak pesan kesalahan berwarna merah, dibungkus sesuai lebar terminal supaya pesan panjang tidak
    // terpotong di jendela sempit. Baris lanjutan menjorok 6 spasi, sejajar teks setelah tanda "  [!] ".
    // Dipanggil oleh fungsi-fungsi pembaca input di BAGIAN 4.
    void printError(const std::string& text) {
        // Lebar yang dipakai = lebar terminal - 1 kolom cadangan; tiap potongan dicetak merah tebal.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Mencetak satu baris garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter ch. Parameter ch bertipe
    // char, satu karakter; std::string(n, ch) membuat teks berisi n kali karakter itu. Dipakai printTitle()
    // di BAGIAN 5.
    void printRule(char ch) {
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====

    // Semua pembacaan ketikan lewat sini. Fungsi ini mencetak prompt, yaitu teks pertanyaan, dengan huruf
    // tebal, lalu membaca satu baris ketikan dan mengembalikannya sebagai std::string. Dengan satu pintu ini,
    // cara menangani input yang tiba-tiba berakhir cukup ditulis sekali.
    std::string readLine(std::string_view prompt) {
        std::string line;  // std::string karena panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt);  // tebal supaya pertanyaan mudah dibedakan dari keterangan
        // std::getline(std::cin, line) membaca satu baris penuh (sampai Enter) ke variabel line. Ia gagal
        // bila input ditutup (Ctrl+D di macOS/Linux, Ctrl+Z di Windows), dan program tidak boleh lanjut
        // tanpa data. Tanda ! di depannya berarti "bila gagal".
        if (!std::getline(std::cin, line)) {
            // Pesan lewat std::cerr, saluran khusus pesan kesalahan yang terpisah dari std::cout.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            std::exit(EXIT_FAILURE);  // hentikan program seketika dengan kode "gagal" ke sistem
        }
        return line;
    }

    // Salah ketik spasi tidak boleh membuat input ditolak. Fungsi ini membuang spasi dan tab di awal dan
    // akhir teks, lalu merapatkan spasi beruntun menjadi satu. Parameternya referensi const karena teks
    // hanya dibaca, tidak perlu disalin.
    std::string normalizeSpaces(const std::string& text) {
        std::string result;                  // teks yang sudah dirapikan
        bool pendingSpace = false;         // ada spasi yang belum ditulis?
        for (const char c : text) {         // periksa setiap karakter satu per satu
            // '\t' = tab, '\r' = CR (sisa akhir baris Windows). Ketiganya tidak langsung ditulis, hanya ditandai.
            if (c == ' ' || c == '\t' || c == '\r') {
                // !result.empty() berarti "hasil sudah berisi"; spasi di awal diabaikan karena hasil masih kosong.
                pendingSpace = !result.empty();
            } else {
                if (pendingSpace) result += ' ';  // spasi beruntun ditulis sebagai satu spasi saja
                result += c;                      // += menambahkan karakter ke akhir teks
                pendingSpace = false;
            }
        }
        return result;                       // spasi di akhir otomatis terbuang karena tidak pernah ditulis
    }

    // Apakah teks hanya berisi angka 0-9, tanpa tanda, titik, huruf, atau spasi? Dipakai isDecimalFormat()
    // di bawah untuk memeriksa tiap bagian bilangan.
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;  // teks kosong bukan bilangan
        for (const char c : text) {
            // std::isdigit menerima nilai 0-255; static_cast<unsigned char> membawa char (yang bisa negatif
            // untuk byte UTF-8) ke rentang aman itu. Satu saja bukan digit: langsung false.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    // Apakah teks berbentuk bilangan desimal yang kita harapkan? Yang diterima: angka, boleh diikuti SATU
    // pemisah titik atau koma lalu angka lagi (14, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5,
    // atau 7 5 ditolak, karena bukan cara menulis ukuran yang wajar dari pengguna. Dipakai parseDecimal().
    bool isDecimalFormat(const std::string& text) {
        // auto = biarkan compiler menentukan tipenya (di sini std::size_t). find_first_of(".,") mencari
        // posisi titik atau koma pertama; hasilnya npos bila tidak ada.
        const auto separator = text.find_first_of(".,");
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        // substr(0, separator) = teks sebelum pemisah; substr(separator + 1) = teks sesudahnya sampai akhir.
        // Pemisah kedua (8.7.5) otomatis tertolak, karena bagian sesudahnya masih memuat titik.
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // bagian sesudahnya juga
    }

    // Mengubah teks menjadi bilangan desimal; mengembalikan true bila teksnya sah. Angkanya sendiri
    // "dikirim balik" lewat parameter double& result, referensi ke variabel milik pemanggil. Parameter text
    // diterima sebagai salinan, jadi boleh diubah di dalam. Dipakai readPositiveDecimal() di bawah.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang lolos
        for (char& c : text) {        // char& = referensi: mengubah karakter langsung pada teksnya
            if (c == ',') c = '.';    // koma diganti titik, karena C++ hanya mengenal penulisan 7.5
        }
        // std::istringstream adalah "aliran masukan dari teks": membaca angka dengan operator >> persis
        // seperti std::cin membaca ketikan.
        std::istringstream parser(text);
        // (parser >> result) = berhasil membaca angka; (parser >> std::ws).eof() = setelah spasi dilewati
        // tidak ada sisa karakter lagi. Keduanya harus benar (&&) agar teks dinyatakan sah.
        return (parser >> result) && (parser >> std::ws).eof();
    }

    // Meminta bilangan desimal yang lebih besar dari 0 dan paling besar DIMENSION_MAX (1000000). Diulang
    // sampai sah. Mengembalikan double. Dipanggil run() di BAGIAN 6 untuk sisi persegi.
    double readPositiveDecimal(std::string_view prompt) {
        while (true) {  // while (true) mengulang terus; jalan keluarnya hanya lewat return
            double value = 0.0;  // double karena ukuran boleh desimal; 0.0 sebagai nilai awal yang jelas
            // Tiga langkah bersarang: readLine membaca ketikan, normalizeSpaces merapikan spasi, parseDecimal
            // mengubahnya ke angka (hasil ditaruh di value). value > 0.0 menolak nol dan negatif, karena
            // panjang sisi tidak mungkin nol atau negatif; value <= DIMENSION_MAX menolak angka raksasa.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            // Teks di dalam tanda kutip disebut string literal. Dua string literal yang berdampingan otomatis
            // disambung compiler menjadi satu, jadi pesan panjang bisa dipecah ke dua baris kode.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }
    }

    // Meminta pilihan berupa satu digit 1..maxValue. Enter tanpa isi berarti pilihan bawaan (defaultValue),
    // supaya pilihan paling umum (cm, π = 3.14) cukup ditekan Enter. Mengembalikan int, nomor pilihan.
    // Dipakai chooseUnit() dan choosePi() di BAGIAN 5.
    int readChoice(std::string_view prompt, int maxValue, int defaultValue) {
        while (true) {  // diulang karena pilihan salah harus diminta lagi
            const std::string text = normalizeSpaces(readLine(prompt));  // ketikan yang sudah dirapikan spasinya
            if (text.empty()) return defaultValue;  // Enter saja = pilihan bawaan
            // Sah bila tepat satu karakter (text.size() == 1) dan karakter itu digit. text[0] = karakter
            // pertama; text[0] - '0' mengubah karakter digit menjadi angka, karena kode karakter '0'..'9'
            // berurutan sehingga selisihnya terhadap '0' adalah nilai angkanya.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) {
                const int choice = text[0] - '0';
                if (choice >= 1 && choice <= maxValue) return choice;  // harus berada di rentang 1..maxValue
            }
            // std::to_string mengubah angka menjadi teks supaya bisa disambung dengan + ke pesan.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue)
                                        + ", atau tekan Enter untuk memakai bawaan.");
        }
    }

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // Semua pertanyaan pilihan harus tampil dengan bentuk yang sama, supaya pengguna langsung mengenali pola
    // "[nomor] pilihan" dan "[Enter = bawaan]". Fungsi ini yang menjaganya: baris pertama berisi label
    // (tebal) dan opsi pertama, baris berikutnya opsi lain yang disejajarkan di bawahnya, baris terakhir nilai
    // bawaan (kuning). Parameter line bertipe std::initializer_list<std::string_view>: daftar teks yang
    // ditulis pemanggil dalam kurung kurawal { "...", "..." }. Dipakai chooseUnit() dan choosePi() di bawah.
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) {
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // lebar maksimal satu baris
        bool isFirst = true;  // penanda baris pertama, yang memuat label
        for (const std::string_view b : line) {  // tiap baris opsi berurutan
            // substr(0, 6) mengambil 6 karakter pertama; baris yang diawali "[Enter" adalah baris nilai bawaan.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter");
            // Ternary: baris bawaan diwarnai kuning, baris lain dijadikan std::string apa adanya.
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b);
            // Baris pertama memuat label (tebal) yang ditambah spasi sampai selebar LABEL_WIDTH (24), lalu ": ".
            // Baris berikutnya diberi spasi selebar LABEL_WIDTH + 2 agar sejajar. Ternary di dalam
            // std::string(n, ' ') menghitung spasi tambahan: LABEL_WIDTH - panjang label, atau 0 bila label
            // sudah lebih panjang. Keduanya dibungkus wrapText agar opsi panjang tidak terpotong di jendela sempit.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;  // setelah baris pertama tercetak, baris berikutnya bukan baris label lagi
        }
    }

    // Menampilkan daftar satuan, membaca pilihan, lalu menegaskan simbol satuan yang terpilih. Mengembalikan
    // const char*, teks tetap simbol satuan. Dipanggil run() di BAGIAN 6.
    const char* chooseUnit() {
        // Argumen = nilai yang dikirim ke parameter saat fungsi dipanggil. Argumen kedua { ... } adalah daftar
        // baris opsi untuk parameter initializer_list di printOptions().
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        // UNIT_COUNT (4) = nomor pilihan terbesar; DEFAULT_UNIT_INDEX (2 = cm) dipakai bila pengguna hanya
        // menekan Enter.
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        const char* unit = UNITS[selection - 1];  // - 1 karena nomor pilihan mulai dari 1, indeks array dari 0
        // std::string("...") + unit menyambung teks tetap dengan simbol satuan; penegasan hijau supaya
        // pilihan pasti terbaca.
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit) << "\n\n";
        return unit;
    }

    // Menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih. Nilai π untuk
    // perhitungan dikembalikan lewat return (double). Teks nilai π untuk ditampilkan di hasil dikirim lewat
    // parameter std::string& legend, referensi ke variabel pemanggil. Dipanggil run() di BAGIAN 6.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan sisi persegi kelipatan 14)",
                                      "[Enter = 3.14]" });
        // Argumen 2 = nomor pilihan terbesar, 1 = nomor bawaan (3.14) bila pengguna hanya menekan Enter.
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // ternary memilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // \u03c0 = simbol π
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // ternary lagi: nilai π sesuai nomor pilihan
    }

    // Mencetak judul program (cyan tebal) di antara dua garis pembatas '=' sepanjang TITLE_WIDTH, memakai
    // printRule() dari BAGIAN 3. Dipanggil run() di BAGIAN 6.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong supaya judul tidak menempel pada teks sebelumnya
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // "  " + ... = judul menjorok 2 spasi
        printRule('=');
    }

    // Mengulang sebuah teks sebanyak n kali. Mengapa tidak std::string(n, c) saja? Karena itu hanya bisa
    // mengulang satu byte, sedangkan karakter UTF-8 seperti ─ terdiri dari beberapa byte. Dipakai printSketch().
    std::string repeatText(std::string_view text, int n) {
        std::string result;
        for (int i = 0; i < n; ++i) result += text;  // += menambahkan teks ke akhir, n kali
        return result;
    }

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // Sketsa bangun digambar di terminal, yang hanya bisa menampilkan karakter. Triknya: karakter Braille
    // Unicode (U+2800 s.d. U+28FF). Setiap karakter Braille mewakili 8 titik yang bisa dinyalakan satu per
    // satu, tersusun 2 kolom x 4 baris. Jadi satu sel terminal memberi resolusi 2 x 4 titik, delapan kali
    // lebih halus daripada satu karakter blok. Teknik ini juga dipakai drawille, pustaka Python untuk
    // menggambar di terminal (tidak dipakai di program ini).
    // BrailleCanvas adalah "kanvas gambar" kita, berupa struct. Selain variabel (anggota data), struct juga
    // boleh berisi fungsi (anggota fungsi) yang bekerja pada isinya. Blok ini berakhir pada "};" sebelum
    // printSketch().
    struct BrailleCanvas {
        int cols, rows;                 // ukuran kanvas dalam sel terminal (kolom, baris)
        // Satu byte untuk tiap titik: 0 = kosong, lebih dari 0 = nomor lapisan gambar (menentukan warna).
        // Titik disimpan baris demi baris, dari kiri atas.
        std::vector<unsigned char> layers;

        // Constructor: fungsi bernama sama dengan struct, dijalankan saat kanvas dibuat. Bagian setelah ":"
        // (daftar inisialisasi) mengisi cols = k, rows = row, dan layers dengan (2k) x (4 row) titik bernilai 0.
        // static_cast<std::size_t>(2 * k) mengubah int ke size_t, tipe ukuran yang diminta vector.
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {}
        // Ukuran kanvas dalam titik. const setelah () berarti fungsi anggota ini tidak mengubah isi struct.
        int pixelWidth()  const { return 2 * cols; }    // 2 titik per sel ke samping
        int pixelHeight() const { return 4 * rows; } // 4 titik per sel ke bawah

        // Menyalakan satu titik di (x, y) dengan nomor lapisan layer. Lapisan bernomor lebih besar menimpa
        // yang kecil, supaya garis lingkaran tetap terlihat di atas arsiran.
        void setPixel(int x, int y, unsigned char layer) {
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // di luar kanvas: abaikan
            // ref adalah referensi ke byte titik di dalam layers; mengubah ref berarti mengubah layers langsung.
            // Indeksnya y * lebar + x, karena titik disimpan baris demi baris.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x];
            if (layer > ref) ref = layer;
        }

        // Mengubah 8 titik di satu sel (kolom cx, baris cy) menjadi satu karakter Braille berwarna sesuai
        // lapisan tertingginya. layerColors = daftar kode warna, indeksnya nomor lapisan. const di akhir:
        // fungsi ini hanya membaca kanvas. Dipakai toLines() di bawah dan printSketch() untuk baris tengah.
        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const {
            // Peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40; kolom kanan 0x08, 0x10,
            // 0x20, 0x80. Urutan ganjil ini mengikuti standar Braille Unicode (titik 7-8 ditambahkan
            // belakangan di baris ke-4). Array 2 dimensi [baris][kolom]; static const = dibuat sekali, tidak berubah.
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} };
            unsigned char value = 0, maxLayer = 0;  // value = gabungan bit titik menyala; maxLayer = lapisan tertinggi
            // Dua for bersarang memeriksa 4 baris (dy) x 2 kolom (dx) titik di dalam sel ini.
            for (int dy = 0; dy < 4; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    // Posisi titik di kanvas: baris cy * 4 + dy, kolom cx * 2 + dx; indeks = baris * lebar + kolom.
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx];
                    // Titik menyala: |= (OR bit) menambahkan bit titik itu ke value, dan std::max menyimpan
                    // lapisan tertinggi.
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); }
                }
            }
            if (!value) return " ";  // sel kosong = spasi biasa
            // Kode karakter Braille = U+2800 + pola bit. Akhiran u pada 0x2800u berarti bilangan unsigned.
            const unsigned codePoint = 0x2800u + value;
            // Code point 0x2800-0x28FF ditulis dalam UTF-8 sebagai tiga byte berpola
            // 1110xxxx 10xxxxxx 10xxxxxx. >> menggeser bit ke kanan (kebalikan <<), supaya bagian atas
            // code point turun ke bawah. Lalu & 0x3F menyisakan 6 bit terbawah, dan | menempelkan awalan
            // byte 0xE0 atau 0x80.
            std::string s;
            s += static_cast<char>(0xE0 | (codePoint >> 12));
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
            s += static_cast<char>(0x80 | (codePoint & 0x3F));
            return colorize(layerColors[maxLayer], s);  // diwarnai sesuai lapisan tertinggi di sel ini
        }

        // Mengubah seluruh kanvas menjadi baris-baris teks siap cetak, satu std::string per baris sel.
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

    // Inilah sketsa yang tampil sebelum pertanyaan pertama: persegi dengan lingkaran di dalamnya. Lingkaran
    // dihitung dari persamaannya (titik yang berjarak R dari pusat), jadi hasilnya benar-benar bulat. Daerah
    // di luar lingkaran diarsir garis diagonal merah seperti di buku cetak, dan garis jari-jari R dibubuhkan
    // di baris tengah. Dipanggil run() di BAGIAN 6.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal. available = lebar terminal dikurangi 8 kolom untuk tepi
        // kotak, margin, dan indentasi. Lebar kanvas = available - 2 (2 = garis tepi │ kiri dan kanan persegi),
        // dijepit antara 20 dan 48 sel oleh std::max dan std::min bersarang. Jumlah baris = setengah kolom
        // supaya bidangnya persegi (ingat: 2 x 4 titik per sel).
        const int available = terminalWidth() - 8;
        int canvasCols = std::max(20, std::min(48, available - 2));
        if (canvasCols % 2 != 0) --canvasCols;  // % = sisa bagi; bila ganjil dikurangi 1 agar pusatnya tepat di tengah
        const int canvasRows = canvasCols / 2;
        // legend = baris-baris keterangan simbol. std::vector<std::string> bisa diisi langsung lewat { ... }.
        const std::vector<std::string> legend = {
            "R = jari-jari lingkaran = S / 2",
            "S = sisi persegi = 2 x R (lingkaran menyinggung sisi)",
            colorize(ANSI_HATCH, "merah") + " : daerah yang diarsir (persegi - lingkaran)",
            colorize(ANSI_TITLE, "cyan") + "  : garis lingkaran",
            "Luas Arsir = (S x S) - (Pi x R x R)",
        };
        std::size_t longestLegend = 0;  // lebar keterangan terpanjang (dalam kolom)
        for (const std::string& k : legend) longestLegend = std::max(longestLegend, displayWidth(k));
        // Keterangan ditaruh di samping gambar hanya bila baris terlebar (gambar + keterangan terpanjang)
        // masih muat di dalam kotak (lebar terminal - 5). Kalau tidak, drawBox() akan membungkus baris itu
        // dan gambarnya rusak. Pada rumus di bawah: 3 = indentasi, 2 = tepi persegi, 3 = jarak ke keterangan.
        // static_cast<std::size_t> menyamakan tipe kedua sisi perbandingan, supaya compiler tidak memberi
        // peringatan.
        const bool legendOnRight = (3 + canvasCols + 2 + 3 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);

        BrailleCanvas canvas(canvasCols, canvasRows);  // kanvas kosong; ini memanggil constructor di struct di atas
        const double cx = canvas.pixelWidth() / 2.0, cy = canvas.pixelHeight() / 2.0;  // pusat dalam satuan titik
        const double radiusPx  = std::min(cx, cy) - 0.5;  // - 0.5 agar lingkaran menyinggung sisi persegi
        // Sekarang kita kunjungi setiap titik (x, y) kanvas dengan dua for bersarang dan bertanya: titik ini
        // bagian dari apa? std::hypot(a, b) = akar(a² + b²) = jarak titik ke pusat (+ 0.5 = tengah titik).
        // Jarak yang hampir sama dengan R (selisih <= 0.9, diperiksa dengan std::fabs = nilai mutlak) berarti
        // garis lingkaran: lapisan 2. Titik di luar lingkaran dengan (x + y) kelipatan 6 menjadi arsiran
        // diagonal: lapisan 1, karena x + y tetap sepanjang satu garis miring.
        for (int y = 0; y < canvas.pixelHeight(); ++y) {
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double dist = std::hypot(x + 0.5 - cx, y + 0.5 - cy);  // jarak titik ke pusat
                if (std::fabs(dist - radiusPx) <= 0.9)          canvas.setPixel(x, y, 2);  // lapisan 2: garis lingkaran
                else if (dist > radiusPx && (x + y) % 6 == 0)   canvas.setPixel(x, y, 1);  // lapisan 1: arsiran luar
            }
        }
        // Warna per lapisan: indeks 0 (kosong) = "", 1 (arsiran) = merah, 2 (garis lingkaran) = cyan tebal.
        const std::vector<const char*> layerColors = { "", ANSI_HATCH, ANSI_TITLE };
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // kanvas menjadi baris-baris teks

        // Garis jari-jari "●──R──►" ditimpakan pada baris tengah, dari pusat ke tepi kanan lingkaran.
        // Kurung kurawal { } di bawah berdiri sendiri, tanpa if atau for di depannya.
        // Variabel yang dibuat di dalamnya hilang setelah } penutup, jadi tidak bercampur dengan sisa fungsi.
        {
            const int r = canvasRows / 2, centerCol = canvasCols / 2;  // r = baris tengah, centerCol = kolom pusat
            std::string radiusLine = "\u25cf";  // ● (U+25CF) pusat lingkaran
            // Dari kolom setelah pusat sampai sebelum tepi kanan: huruf "R" tepat di tengah ruas (dipilih
            // ternary), selain itu garis ─ (\u2500).
            for (int c = centerCol + 1; c < canvasCols - 1; ++c) radiusLine += (c == (centerCol + canvasCols - 1) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";  // ► (U+25BA) ujung jari-jari
            // Separuh kiri baris tengah tetap gambar Braille; separuh kanan diganti garis jari-jari kuning.
            std::string row;
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, r, layerColors);
            figureLines[r] = row + colorize(ANSI_HINT, radiusLine);
        }  // akhir blok garis jari-jari

        // Keterangan simbol: di samping kanan bila jendela lebar, di bawah gambar bila sempit.
        // sideNotes(canvasRows, "") = daftar berisi canvasRows teks kosong, satu per baris gambar.
        std::vector<std::string> sideNotes(canvasRows, "");
        if (legendOnRight) {
            // Lima nomor baris tempat tiap keterangan diletakkan, dihitung dari tinggi kanvas: 1/8, 1/8 + 2,
            // 1/8 + 5, 1/8 + 6, dan baris tengah; urutannya sejajar dengan isi legend di atas.
            const int positions[5] = { canvasRows / 8, canvasRows / 8 + 2, canvasRows / 8 + 5, canvasRows / 8 + 6, canvasRows / 2 };
            // Hanya bila nomor baris masih di dalam kanvas; "   " = 3 spasi jarak ke keterangan.
            for (int i = 0; i < 5; ++i) if (positions[i] < canvasRows) sideNotes[positions[i]] = "   " + legend[i];
        }

        // Sekarang semua baris kotak disusun: panah sisi S, tepi atas persegi, baris-baris gambar (diapit │
        // dan ditambah keterangan samping), tepi bawah, lalu keterangan di bawah bila tidak muat di samping.
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
        // drawBox() menggambar semuanya di dalam kotak berjudul "Sketsa Bangun yang Diarsir".
        drawBox("Sketsa Bangun yang Diarsir", lines);
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Sebelum pertanyaan pertama, pengguna perlu tahu bentuk input yang diterima: pilihan, angka lebih dari 0,
    // dan cara menulis desimal. Fungsi ini menampilkannya dalam kotak lewat drawBox(). Dipanggil run() di
    // BAGIAN 6.
    void printInstructions() {
        // Argumen kedua { ... } langsung menjadi std::vector<std::string> untuk drawBox.
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

    // Membuat satu baris hasil "Label : nilai satuan" dengan 2 desimal, siap dimasukkan ke kotak.
    // Dipanggil run() di BAGIAN 6 untuk tiap baris angka hasil.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit) {
        // std::ostringstream adalah "aliran keluaran ke teks": menerima << seperti std::cout, tetapi hasilnya
        // ditampung sebagai teks. Jadi format std::cout sendiri tidak ikut berubah.
        std::ostringstream line;
        // std::left = rata kiri; std::setw(25) = lebar kolom label 25 karakter (sisanya diisi spasi);
        // std::fixed = desimal biasa, bukan notasi ilmiah; std::setprecision(2) = tepat 2 angka di belakang koma.
        line << std::left << std::setw(25) << label << ": " << std::fixed << std::setprecision(2) << value << ' ' << unit;
        return line.str();  // .str() mengambil teks yang sudah tersusun
    }

}  // namespace (akhir namespace tanpa nama yang dibuka setelah BAGIAN 1)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 3 =====
// Kali ini namespace-nya diberi nama: task3. Fungsi di dalamnya dipanggil dengan awalan task3::, jadi
// program bermenu (main.cpp di folder src) bisa memanggil task3::run() tanpa tertukar dengan run() milik
// tugas lain. Blok ini berakhir pada "}  // namespace task3".
namespace task3 {

    // Fungsi utama tugas ini. Alurnya: judul, sketsa, petunjuk, dua pertanyaan pilihan, input sisi, hitung,
    // lalu kotak hasil. Pekerjaan kecilnya diserahkan ke fungsi-fungsi bantu di BAGIAN 3-5.
    // Dipanggil oleh main() di BAGIAN 7 atau oleh program bermenu.
    void run() {
        enableUtf8Console();  // supaya program mandiri (tanpa menu) juga benar; aman dipanggil berulang
        printTitle("PROGRAM 3 : LUAS BANGUN YANG DIARSIR (PERSEGI - LINGKARAN)");
        // Dua baris teks dalam satu std::cout; << boleh dirangkai. \n = ganti baris, \u03c0 = simbol π.
        std::cout << "Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2))\n"
                  << "        Pi = \u03c0 (modul menulisnya phi)\n\n";
        printSketch();    // sketsa bangun, supaya pengguna tahu mana yang dimaksud S dan R
        printInstructions();  // kotak petunjuk pengisian

        // --- Tahap pengaturan: satuan dan nilai π ---
        const char* unit = chooseUnit();          // simbol satuan yang dipilih, mis. "cm"
        std::string piLabel;                    // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double pi = choosePi(piLabel);     // nilai π untuk perhitungan; piLabel diisi lewat referensi

        // --- Tahap input ---
        // Sisi persegi disimpan sebagai double karena boleh desimal. readPositiveDecimal (BAGIAN 4) menjamin
        // nilainya lebih dari 0, karena panjang sisi tidak mungkin nol atau negatif.
        // std::string("...") + unit + ") : " menyambung teks pertanyaan dengan simbol satuan yang dipilih.
        const double side = readPositiveDecimal(std::string("Masukkan Panjang Sisi Persegi (") + unit + ") : ");

        // --- Tahap perhitungan --- (const: nilainya tidak diubah lagi setelah dihitung)
        const double radius      = side / 2.0;  // lingkaran menyinggung sisi, jadi r = s / 2
        const double squareArea   = side * side;                  // luas persegi = s x s
        const double circleArea = pi * radius * radius;     // luas lingkaran = π x r x r
        const double shadedArea     = squareArea - circleArea;  // arsir = persegi - lingkaran

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit) + "\u00b2";  // satuan luas, mis. cm² (\u00b2 = pangkat dua)
        std::vector<std::string> resultLines;  // daftar baris hasil
        // Tiap push_back menambah satu baris; formatResultLine() (BAGIAN 5) membuat "Label : angka satuan"
        // dengan 2 desimal.
        resultLines.push_back("Pi (\u03c0) yang Digunakan    : " + piLabel);
        resultLines.push_back(formatResultLine("Sisi Persegi (S)", side, unit));
        resultLines.push_back(formatResultLine("Jari-Jari Lingkaran (R)", radius, unit) + "  (R = S / 2, lingkaran menyinggung sisi)");
        resultLines.push_back(formatResultLine("Luas Persegi", squareArea, areaUnit));
        resultLines.push_back(formatResultLine("Luas Lingkaran", circleArea, areaUnit));
        // Hasil akhir ditonjolkan hijau tebal.
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Luas Bangun yang Diarsir", shadedArea, areaUnit)));
        // Baris kosong, lalu semua baris hasil digambar di dalam kotak "Hasil Perhitungan".
        std::cout << '\n';
        drawBox("Hasil Perhitungan", resultLines);
    }

}  // namespace task3

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), dan program bermenu sudah punya. Jadi main() di bawah hanya ikut bila
// berkas ini dikompilasi sendiri. #ifndef BUILD_WITH_MENU bertanya: apakah makro BUILD_WITH_MENU BELUM
// didefinisikan? Makro itu diberikan lewat opsi -DBUILD_WITH_MENU saat menggabung ke program bermenu.
#ifndef BUILD_WITH_MENU
// int main() adalah titik awal program: fungsi pertama yang dijalankan sistem. Nilai int yang dikembalikan
// adalah kode status ke sistem.
int main() {
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 3 - Fadhlillah (2531070011)") << '\n';  // identitas, tebal
    task3::run();  // seluruh alur tugas 3 (BAGIAN 6)
    return 0;  // 0 = selesai tanpa error
}
#endif  // akhir bagian main() untuk kompilasi mandiri
