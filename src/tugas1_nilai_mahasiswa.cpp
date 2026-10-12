/*
 * Tugas Praktikum 1 — Modul 1: Input dan Output
 * Program 1 : Tools Nilai Mahasiswa
 * Alur : pengguna mengisi Nama Mata Kuliah dan jumlah mahasiswa (bebas, tidak dibatasi 3), lalu nama
 *        lengkap dan nilai tiap mahasiswa; nilai boleh desimal, ditulis 87.5 maupun 87,5. Di akhir,
 *        program menampilkan rekap nilai, nilai total, dan nilai rata-rata. Setiap input diperiksa dulu;
 *        kalau salah, pertanyaannya diulang. Aturannya: nama mahasiswa hanya boleh huruf/spasi/titik/
 *        tanda hubung/tanda petik, nama mata kuliah harus mengandung huruf, jumlah hanya angka 0-9,
 *        nilai hanya angka dengan satu pemisah desimal.
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas1 tugas1_nilai_mahasiswa.cpp && ./tugas1
 *   Artinya: g++ adalah compiler, program yang mengubah kode C++ menjadi program yang bisa dijalankan.
 *   -std=c++17 memakai aturan bahasa C++ versi 2017. -Wall -Wextra menampilkan semua peringatan (warning)
 *   supaya kesalahan kecil kelihatan. -o tugas1 memberi nama program hasilnya. && ./tugas1 menjalankan
 *   program itu bila kompilasinya berhasil.
 * Bila digabung ke program bermenu (main.cpp), tambahkan -DBUILD_WITH_MENU saat kompilasi supaya main()
 * di bagian paling bawah berkas ini tidak ikut (lihat BAGIAN 7).
 *
 * Susunan berkas: BAGIAN 1 pustaka, BAGIAN 2 konstanta dan tipe data, BAGIAN 3 warna dan kotak tampilan,
 * BAGIAN 4 fungsi bantu input, BAGIAN 5 judul/petunjuk/tabel, BAGIAN 6 alur utama task1::run()
 * (cara penulisan itu dijelaskan di BAGIAN 6), BAGIAN 7 main().
 * Komentar di berkas ini ditulis untuk pembaca yang baru belajar: tiap blok dibuka dengan gambaran besarnya
 * (apa yang kita lakukan dan kenapa), istilah baru dijelaskan sekali saat pertama muncul, dan alasan sebuah
 * pilihan kode ditulis di tempat pilihan itu dibuat.
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Program ini tidak menulis semuanya dari nol. Untuk mencetak ke layar, menyimpan teks, atau mengatur lebar
// kolom, kita meminjam dari pustaka standar C++. Caranya lewat #include: perintah preprocessor (tahap yang
// berjalan sebelum kompilasi) yang menyalin isi sebuah header ke berkas ini. Header adalah berkas berisi
// daftar fungsi dan tipe yang disediakan sebuah pustaka. Nama di antara < > berarti pustaka standar C++.
// Nama dari pustaka standar diawali std::, misalnya std::max; dibaca "max milik std". std adalah namespace
// (wadah nama) dan tanda :: dibaca "milik". Namespace dijelaskan lebih lengkap tepat sebelum BAGIAN 2.
// Tiap #include di bawah diberi catatan: apa yang kita pakai darinya, dan di mana.

// std::max dan std::min mengambil yang terbesar/terkecil dari dua nilai. Dipakai saat menghitung lebar
// kolom nama dan lebar kotak (BAGIAN 3 dan 6).
#include <algorithm>
// std::isalpha dan std::isdigit memeriksa apakah satu karakter itu huruf atau angka. Dipakai oleh
// fungsi-fungsi pemeriksa input di BAGIAN 4.
#include <cctype>
// std::size_t adalah bilangan bulat tak bertanda (tidak bisa negatif, mulai dari 0) untuk ukuran dan indeks.
// Panjang std::string dan jumlah elemen std::vector bertipe ini, jadi kita ikut memakainya.
#include <cstddef>
// std::exit menghentikan program saat itu juga. Setiap program yang selesai melapor satu angka ke sistem
// operasi: 0 berarti berhasil, EXIT_FAILURE berarti gagal. std::getenv membaca variabel lingkungan, yaitu
// pengaturan berbentuk NAMA=nilai yang diwariskan sistem operasi ke program (misalnya NO_COLOR atau COLUMNS).
// std::atoi mengubah teks menjadi int. Semuanya dipakai di BAGIAN 3 dan 4.
#include <cstdlib>
// Pengatur format keluaran: std::setw (lebar kolom), std::left dan std::right (rata kiri/kanan), std::fixed
// dan std::setprecision (jumlah angka desimal). Tabel rekap di BAGIAN 5 memerlukannya.
#include <iomanip>
// std::cin (masukan keyboard), std::cout (keluaran layar), dan std::cerr (keluaran pesan kesalahan).
// Program ini berbicara dengan pengguna lewat terminal, jadi ketiganya kita butuhkan.
#include <iostream>
// std::istringstream membaca angka dari teks, std::ostringstream menyusun teks dari angka. Input dibaca
// sebagai teks dulu baru diubah ke angka (BAGIAN 4), dan angka disusun menjadi teks tabel (BAGIAN 5).
#include <sstream>
// std::string adalah tipe teks yang panjangnya bisa berubah. Nama Mata Kuliah dan nama mahasiswa adalah teks.
#include <string>
// std::string_view adalah "pandangan" ke teks yang sudah ada: hanya dibaca, tanpa disalin. Parameter teks
// seperti prompt (teks pertanyaan ke pengguna), judul kotak, dan isi kolom tabel di BAGIAN 3 sampai 5 cukup
// dibaca, jadi tidak perlu disalin.
#include <string_view>
// std::vector adalah daftar (array) yang ukurannya bisa ditentukan saat program berjalan. Jumlah mahasiswa
// baru diketahui setelah pengguna mengetiknya, jadi array berukuran tetap tidak cocok.
#include <vector>
// Program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya "berbicara" dengan terminal
// berbeda: mendeteksi terminal, membaca lebar jendela, mengaktifkan warna, dan mengatur UTF-8 (dijelaskan di
// BAGIAN 3). Jadi di sini kita bertanya ke compiler: "ini sedang dikompilasi untuk Windows atau bukan?"
// Itulah arti #ifdef _WIN32 (if defined: apakah nama _WIN32 ada? di Windows nama itu otomatis ada).
// _WIN32 adalah makro: nama pengganti teks untuk compiler, seperti catatan tempel. Baris sampai #else hanya
// ikut dikompilasi di Windows, baris setelah #else hanya di macOS/Linux.
#ifdef _WIN32
    // Sebelum memanggil windows.h, kita pastikan dulu makro NOMINMAX sudah ada. NOMINMAX artinya "jangan
    // buat min dan max", karena windows.h punya kebiasaan membuat makro min/max sendiri yang merusak
    // std::min dan std::max kita. #ifndef = "if not defined": blok ini hanya jalan bila NOMINMAX belum ada,
    // dikarenakan MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan mendefinisikan ulang
    // nama yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    #define NOMINMAX 1  // bentuknya: #define NAMA NILAI; mulai baris ini NOMINMAX berarti 1
    #endif  // selesai mengecek NOMINMAX
    // windows.h adalah pustaka resmi Windows. Dari sini kita memakai fungsi untuk warna, UTF-8, dan lebar
    // jendela; semuanya dipanggil di fungsi-fungsi BAGIAN 3.
    #include <windows.h>
#else  // bukan Windows, berarti macOS atau Linux
    #include <cstdio>  // untuk stdout (saluran keluaran ke layar) dan fileno (nomor saluran itu)
    #include <sys/ioctl.h>  // untuk ioctl dan TIOCGWINSZ: bertanya ke terminal seberapa lebar jendelanya
    #include <unistd.h>  // untuk isatty (keluaran ini terminal?) dan STDOUT_FILENO (nomor saluran layar)
#endif  // selesai memilih pustaka sesuai sistem operasi

// Sebelum masuk ke isi, satu hal tentang nama. namespace adalah wadah yang mengelompokkan nama (fungsi,
// konstanta) supaya tidak bentrok dengan nama yang sama di tempat lain. Di sini kita memakai namespace tanpa
// nama (anonymous namespace: kata kunci namespace langsung diikuti { tanpa nama). Semua konstanta dan fungsi
// bantu di dalamnya menjadi milik berkas ini saja, tidak terlihat dari berkas lain. Kenapa perlu? Dikarenakan
// tugas2_volume_tabung.cpp dan tugas3_luas_arsir.cpp di folder src yang sama juga punya fungsi bantu bernama
// sama, misalnya colorize() dan drawBox(). Tanpa ini, penggabungan ke program bermenu main.cpp gagal dengan
// error "multiple definition" (satu nama didefinisikan lebih dari sekali). Blok ini berisi BAGIAN 2 sampai
// BAGIAN 5 dan ditutup tepat sebelum BAGIAN 6.
namespace {

    // ===== BAGIAN 2: KONSTANTA DAN TIPE DATA =====
    // Angka-angka yang menjadi aturan program (batas jumlah, batas nilai, lebar kolom) kita kumpulkan di sini
    // supaya mudah dicari dan diubah di satu tempat. Semuanya constexpr: kata kunci untuk nilai tetap
    // (konstanta) yang sudah pasti sejak kompilasi dan tidak bisa diubah saat program berjalan. Nama
    // konstanta ditulis HURUF_BESAR, itu kebiasaan umum.

    // Batas jumlah mahasiswa. Tipenya int (bilangan bulat, 4 byte), dikarenakan jumlah orang selalu bulat.
    // Keduanya dipakai saat memanggil readInteger() di BAGIAN 6.
    constexpr int STUDENT_COUNT_MIN = 1;     // rekap tanpa mahasiswa tidak bermakna, jadi minimal 1
    constexpr int STUDENT_COUNT_MAX = 1000;  // batas atas 1000 menolak salah ketik seperti 30000

    // Batas nilai. Tipenya double (bilangan desimal, 8 byte, presisi sekitar 15 digit), dikarenakan nilai
    // bisa desimal seperti 87.5; pada int, 87.5 terpotong menjadi 87. Dipakai saat memanggil readDecimal()
    // di BAGIAN 6.
    constexpr double SCORE_MIN = 0.0;    // nilai tidak mungkin negatif
    constexpr double SCORE_MAX = 100.0;  // skala nilai mata kuliah 0-100

    // Lebar kolom tabel, lebar garis judul, dan batas panjang teks, semuanya dalam satuan karakter.
    // Kenapa tipenya ada dua? Tiap konstanta mengikuti tipe pasangan yang akan dihitung bersamanya.
    // NO_COLUMN_WIDTH dan SCORE_COLUMN_WIDTH diberikan ke std::setw, yang meminta int. TITLE_WIDTH ikut int;
    // std::string(TITLE_WIDTH, ch) di printRule() (BAGIAN 3) mengubahnya otomatis tanpa peringatan.
    // Tiga sisanya dibandingkan dengan hasil displayWidth() (BAGIAN 3), yang bertipe std::size_t.
    // Membandingkan int dengan size_t (bertanda dengan tak bertanda) memunculkan peringatan compiler,
    // jadi ketiganya memakai std::size_t.
    constexpr int         NO_COLUMN_WIDTH    = 5;   // lebar kolom nomor urut ("No")
    constexpr std::size_t NAME_COLUMN_MIN_WIDTH    = 20;  // lebar minimal kolom nama
    constexpr int         SCORE_COLUMN_WIDTH = 8;   // lebar kolom nilai ("Nilai")
    constexpr int         TITLE_WIDTH       = 60;  // jumlah karakter '=' pada garis judul
    constexpr std::size_t NAME_MAX_LENGTH = 60;  // lebih panjang dari ini hampir pasti tempelan teks, bukan nama
    constexpr std::size_t COURSE_NAME_MAX_LENGTH = 60;  // batas nama mata kuliah, alasan yang sama

    // Satu mahasiswa punya dua data: nama dan nilai. Daripada disimpan di dua daftar terpisah yang bisa
    // tertukar, kita bungkus jadi satu dengan struct: tipe data buatan sendiri yang menggabungkan beberapa
    // variabel (anggota) menjadi satu kesatuan. Anggotanya diakses dengan tanda titik, misalnya student.name
    // (lihat fungsi run() di BAGIAN 6).
    struct Student {
        std::string name;     // huruf dan spasi, panjangnya tidak pasti, jadi std::string
        double      score{};  // boleh desimal, jadi double; {} memberi nilai awal 0
    };  // akhir struct Student

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, hasil akhir, dan pesan kesalahan gampang dibedakan, program memakai warna lewat kode
    // ANSI. Terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya. Kode ANSI adalah urutan
    // karakter khusus yang tidak dicetak; terminal memakainya untuk mengubah tampilan teks berikutnya.
    // Bentuknya: ESC, lalu "[", angka, dan "m". ESC adalah karakter nomor 27. Di kode ia ditulis "\033",
    // karena 033 dalam basis 8 (oktal) sama dengan 27.
    // Tipe konstantanya constexpr const char*. char* adalah pointer (penunjuk): variabel yang menyimpan
    // alamat memori, di sini alamat huruf pertama teks. constexpr membuat alamatnya tetap, const membuat
    // teksnya tetap.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kode 0: kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // kode 1: tebal, untuk prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // kode 1;36: cyan tebal, untuk judul program dan kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // kode 1;32: hijau tebal, untuk total dan rata-rata
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // kode 1;31: merah tebal, untuk pesan kesalahan
    constexpr const char* ANSI_DIM    = "\033[90m";    // kode 90: abu-abu, untuk garis kotak dan pembatas

    // Huruf seperti é, ö, dan garis kotak dikirim ke layar sebagai byte UTF-8. UTF-8 adalah aturan menyimpan
    // huruf sebagai byte. Huruf ASCII, yaitu 128 karakter dasar (huruf Inggris, angka, tanda baca), memakai
    // 1 byte. Huruf lain, yang kita sebut non-ASCII, memakai 2-4 byte.
    // Masalahnya, Command Prompt dan PowerShell bawaan Windows masih memakai code page lama (misalnya 437).
    // Code page adalah tabel yang dipakai konsol untuk mengubah byte menjadi huruf. Dengan tabel lama,
    // huruf non-ASCII tampil acak.
    // Jadi fungsi ini mengganti code page jendela konsol yang sedang dipakai ke UTF-8 (nomor 65001) sebelum
    // keluaran pertama. Hasilnya sama dengan perintah chcp 65001 di jalankan.bat (folder utama proyek).
    // Di macOS/Linux terminal sudah UTF-8, jadi fungsi ini tidak melakukan apa-apa.
    // void berarti fungsi tidak mengembalikan nilai; () kosong berarti tanpa parameter.
    // Dipanggil di awal fungsi run(), BAGIAN 6.
    void enableUtf8Console() {
    // Dua baris di bawah hanya ikut dikompilasi di Windows (arti #ifdef ada di BAGIAN 1).
    #ifdef _WIN32
        // CP_UTF8 adalah konstanta dari windows.h bernilai 65001.
        SetConsoleOutputCP(CP_UTF8);  // keluaran konsol diatur ke UTF-8
        SetConsoleCP(CP_UTF8);        // masukan (ketikan huruf non-ASCII) juga UTF-8
    #endif  // selesai bagian khusus Windows
    }

    // Boleh pakai warna atau tidak? Aturannya mengikuti kebiasaan program terminal: kalau pengguna menyetel
    // NO_COLOR, warna mati; kalau FORCE_COLOR, warna dipaksa hidup; selain itu warna hanya dipakai bila
    // keluaran benar-benar terminal, bukan berkas. Hasilnya bool: true berarti boleh berwarna.
    // Dipakai oleh colorize() di bawah.
    bool colorsEnabled() {
        // Pemeriksaannya cukup sekali seumur program, jadi hasilnya disimpan di variabel static (dibuat
        // sekali, diingat terus); const berarti tidak boleh diubah setelahnya. Nilai awalnya dihitung oleh
        // lambda, yaitu fungsi kecil tanpa nama berbentuk []() { ... }, yang langsung dijalankan oleh
        // tanda () di baris "}();" paling bawah.
        static const bool enabled = []() {
            // getenv membaca variabel lingkungan. Hasil nullptr (penunjuk kosong) berarti variabel itu tidak
            // ada; != berarti "tidak sama dengan". return di dalam lambda hanya keluar dari lambda: nilainya
            // menjadi isi enabled, bukan langsung keluar dari colorsEnabled().
            if (std::getenv("NO_COLOR") != nullptr) return false;  // pengguna minta tanpa warna
            if (std::getenv("FORCE_COLOR") != nullptr) return true;  // pengguna memaksa warna
        // Cara Windows sampai #else, cara macOS/Linux setelah #else.
        #ifdef _WIN32
            // Di Windows, Command Prompt baru mengerti kode ANSI setelah mode "virtual terminal" dinyalakan.
            // Mode konsol adalah angka yang tiap bitnya menyalakan satu fitur; bit seperti itu disebut bendera.
            // Caranya: ambil pegangan (HANDLE) ke layar, baca mode konsol yang sedang aktif, lalu tulis
            // kembali mode itu dengan tambahan satu bendera.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;  // DWORD = bilangan bulat 32-bit milik Windows, tempat mode konsol disimpan
            // Kalau pegangannya tidak sah, atau keluaran ternyata bukan konsol (misalnya dialihkan ke
            // berkas), menyerah saja: tanpa warna. || adalah OR (cukup salah satu benar), ! adalah NOT
            // (membalik true/false). &mode = alamat variabel mode, supaya GetConsoleMode bisa menulis ke sana.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // Tanda | (OR per bit) menyalakan bit ENABLE_VIRTUAL_TERMINAL_PROCESSING tanpa mematikan bit lain.
            // SetConsoleMode mengembalikan 0 bila gagal, jadi "!= 0" artinya berhasil.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // Di macOS/Linux cukup satu pertanyaan: apakah stdout sebuah terminal (tty)? isatty menjawab
            // bukan 0 bila ya, dan 0 bila keluaran dialihkan ke berkas atau diteruskan ke program lain (pipa).
            return isatty(fileno(stdout)) != 0;
        #endif  // selesai memilih cara deteksi sesuai sistem operasi
        }();  // tanda () ini menjalankan lambda sekali; hasilnya jadi isi variabel enabled
        return enabled;  // pemanggilan berikutnya tinggal memakai hasil yang tersimpan
    }  // akhir fungsi colorsEnabled()

    // Membungkus teks dengan kode warna, tetapi hanya bila warna sedang aktif. Kalau tidak, teks dikembalikan
    // apa adanya, dikarenakan kode warna yang ikut tersimpan ke berkas hanya jadi sampah karakter.
    // Parameter code adalah salah satu konstanta ANSI_... di atas; text adalah teks yang dibungkus.
    std::string colorize(const char* code, std::string_view text) {
        // Fungsi ini harus mengembalikan std::string, jadi std::string(text) membuat salinan dari string_view.
        if (!colorsEnabled()) return std::string(text);  // warna tidak aktif: teks polos
        // Tanda + pada std::string menyambung teks: kode warna, teks, lalu kode reset (kembali normal).
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Sebagian aksara memakai dua kolom di terminal: aksara CJK (Tionghoa, Jepang, Korea), Hangul, dan emoji.
    // Kalau kita menghitungnya satu kolom, tabel bergeser untuk nama berhuruf seperti itu.
    // Unicode adalah daftar besar yang memberi setiap huruf di dunia satu nomor; nomor itu disebut code point.
    // Fungsi ini menjawab: apakah sebuah code point termasuk aksara lebar?
    // Parameternya bertipe unsigned, yaitu bilangan bulat tak bertanda. Dipakai oleh displayWidth() di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Hasilnya true bila codePoint ada di salah satu rentang berikut; awalan 0x = heksadesimal (basis 16).
        // a >= X && a <= Y berarti a di antara X dan Y, keduanya termasuk; && = AND, || = OR.
        // 0x1100-0x115F Hangul Jamo, 0x2E80-0xA4CF aksara CJK dan sekitarnya, 0xAC00-0xD7A3 suku kata Hangul,
        // 0xF900-0xFAFF ideograf kompatibilitas, 0xFE30-0xFE4F bentuk CJK, 0xFF00-0xFF60 huruf lebar penuh,
        // 0xFFE0-0xFFE6 simbol lebar penuh, 0x1F300-0x1F64F dan 0x1F900-0x1F9FF emoji.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berapa kolom yang dipakai sebuah teks di layar? Jawabannya bukan jumlah byte-nya: huruf UTF-8 multibyte
    // (é, ö, garis kotak) tetap satu kolom, aksara lebar (CJK, emoji) dua kolom, dan kode warna ANSI nol
    // kolom. Fungsi ini menghitung lebar yang benar itu. Hasilnya std::size_t, dan dipakai di banyak tempat:
    // pembungkus teks, kotak, pemeriksa panjang nama, dan tabel.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah terhitung
        bool inEscape = false;  // true selama kita berada di dalam kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point itu
        // Kita jalan byte demi byte dengan range-based for: for (elemen : kumpulan) mengunjungi tiap elemen
        // satu per satu. const unsigned char c membuat nilai byte 0-255, dikarenakan char biasa bisa negatif
        // dan itu merusak perbandingan bit di bawah.
        for (const unsigned char c : text) {
            // Di dalam kode warna tidak ada yang dihitung; kita hanya menunggu 'm'. continue = lewati sisa
            // badan loop, langsung ke byte berikutnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = ESC, pembuka kode warna
            // Satu huruf non-ASCII terdiri dari satu byte awal dan 1-3 byte lanjutan. Pola bit ditulis seperti
            // 10xxxxxx: x boleh 0 atau 1. Byte lanjutan selalu diawali bit 10. c & 0xC0 (AND per bit dengan
            // 11000000) menyisakan 2 bit teratas saja; bila hasilnya 0x80 (10000000), c adalah byte lanjutan.
            // Byte seperti ini tidak menambah kolom; ia hanya menyumbang 6 bit ke code point.
            if ((c & 0xC0) == 0x80) {
                // Baris di bawah padat, jadi kita urai. Bila masih ada byte lanjutan yang ditunggu (pending > 0):
                // codePoint << 6 menggeser bitnya ke kiri untuk memberi ruang 6 bit, lalu | (OR per bit)
                // menempelkan 6 bit baru dari c & 0x3F. --pending mengurangi sisa tunggu satu (-- = kurang 1,
                // ++ = tambah 1). Kalau sisanya habis dan code point-nya aksara lebar, ++width menambah satu
                // kolom lagi.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;  // setiap byte awal = satu karakter = minimal satu kolom
            // Pola bit byte awal memberi tahu panjang karakter: 110xxxxx = 2 byte, 1110xxxx = 3 byte,
            // 11110xxx = 4 byte. Bit sisanya menjadi awal code point, dan pending = jumlah byte lanjutan yang
            // ditunggu. Selain pola itu berarti ASCII 1 byte, tanpa lanjutan.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }  // 2 byte: 5 bit awal
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }  // 3 byte: 4 bit awal
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }  // 4 byte: 3 bit awal
            else                         { pending = 0; }                        // ASCII
        }  // akhir for
        return width;
    }  // akhir fungsi displayWidth()

    // Seberapa lebar jendela terminal sekarang? Fungsi ini bertanya setiap kali dipanggil, jadi tampilan ikut
    // menyesuaikan bila jendela diubah ukurannya. Kalau tidak bisa dijawab (keluaran bukan terminal), kita
    // coba variabel lingkungan COLUMNS, lalu pakai 80 kolom, lebar bawaan terminal pada umumnya. Hasilnya
    // tidak pernah kurang dari 50, dikarenakan di bawah itu tabel tidak bisa rapi. Hasilnya int, dipakai oleh
    // drawBox() dan printError() di bawah.
    int terminalWidth() {
        int cols = 0;  // 0 berarti "belum diketahui"
    // Cara Windows sampai #else, cara macOS/Linux setelah #else.
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;  // struct Windows penampung informasi layar konsol
        // GetConsoleScreenBufferInfo mengisi info lewat alamatnya (&info). Bila berhasil, lebar jendela =
        // kolom kanan - kolom kiri + 1, dikarenakan kedua ujungnya termasuk.
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        struct winsize ws;  // struct dari <sys/ioctl.h> penampung ukuran jendela (baris dan kolom)
        // ioctl dengan TIOCGWINSZ bertanya ukuran jendela dan mengisinya ke ws; hasil 0 berarti berhasil.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // selesai memilih cara membaca lebar sesuai sistem operasi
        // Masih belum tahu? Coba COLUMNS. env adalah pointer ke teks hasil getenv, nullptr bila tidak ada;
        // std::atoi mengubah teks itu menjadi int (0 bila bukan angka).
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // masih belum tahu juga: pakai 80
        return std::max(50, cols);  // std::max memilih yang terbesar: tidak pernah kurang dari 50
    }  // akhir fungsi terminalWidth()

    // Jendela terminal bisa sempit, dan teks yang lebih lebar dari jendela tidak boleh terpotong sembarangan.
    // Fungsi ini memotong satu baris panjang menjadi beberapa baris, selalu pada spasi. Baris lanjutannya
    // diberi indentasi "gantung" (sejumlah spasi di depan) supaya sejajar dengan isi setelah label.
    // Hasilnya daftar baris, bertipe std::vector<std::string>: tipe isi vector ditulis di antara < >.
    // Tanda & pada const std::string& text membuat text menjadi referensi: nama lain untuk variabel milik
    // pemanggil, jadi teksnya tidak disalin. const (sudah dibahas di colorsEnabled()) menjaganya tetap utuh.
    // maxWidth = lebar maksimal dalam kolom, hangingIndent = jumlah spasi indentasi gantung.
    // Dipakai oleh drawBox() dan printError() di bawah.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil pemotongan
        // Jendela terlalu sempit untuk indentasi + 10 kolom isi? Kecilkan indentasinya jadi 2 spasi saja.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // salinan teks yang belum terpotong; dipendekkan tiap putaran
        // while = ulangi selama kondisinya benar. Selama sisa teks masih lebih lebar dari maxWidth (dihitung
        // per kolom), kita cari spasi terakhir sebelum batas, potong di sana, lalu ulangi pada sisanya.
        while (displayWidth(remaining) > maxWidth) {
            // col menghitung kolom; cutPos menyimpan posisi spasi tempat memotong; std::string::npos = "tidak ada".
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama di dalam kode warna ANSI
            // for dengan indeks i dari 0 sampai sebelum remaining.size(): menelusuri byte demi byte.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<T>(x) mengubah x ke tipe T secara terang-terangan. Di sini char (bisa negatif)
                // diubah ke unsigned char, nilai 0-255, supaya uji bit benar.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // di dalam kode warna: tunggu 'm'
                if (c == 0x1B) { inEscape = true; continue; }  // ESC: awal kode warna
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8: bukan kolom baru
                if (col >= maxWidth) break;  // break = keluar dari loop: batas lebar sudah lewat
                if (c == ' ' && col > hangingIndent) cutPos = i;  // spasi setelah indentasi: calon titik potong
                ++col;
            }  // akhir for
            if (cutPos == std::string::npos) break;  // tidak ada spasi untuk memotong: biarkan apa adanya
            // substr(0, cutPos) = potongan sebelum spasi; push_back menambahkannya di akhir vector.
            result.push_back(remaining.substr(0, cutPos));
            // Sisa setelah spasi (substr(cutPos + 1)) diberi awalan spasi sebanyak hangingIndent, dibuat
            // dengan std::string(jumlah, ' ').
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }  // akhir while
        result.push_back(remaining);  // sisa terakhir sudah muat: jadi baris penutup
        return result;
    }  // akhir fungsi wrapText()

    // Petunjuk dan rekap hasil ditampilkan di dalam kotak berjudul supaya terpisah jelas dari pertanyaan.
    // Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah melebihi lebar terminal: baris yang terlalu
    // panjang dibungkus dulu dengan wrapText(), dengan indentasi gantung sejajar label bila baris berpola
    // "Label : isi". Parameter title = judul kotak, lines = daftar baris isi (dibaca lewat referensi, tanpa
    // disalin). Dipakai oleh printInstructions() di BAGIAN 5 dan run() di BAGIAN 6.
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Lebar isi maksimal = lebar terminal dikurangi 5: 2 tepi kotak, 2 margin, dan 1 kolom cadangan,
        // dikarenakan sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela.
        // static_cast<std::size_t> mengubah hasil int menjadi size_t supaya sebanding dengan displayWidth().
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris isi setelah dibungkus; bisa lebih banyak dari lines
        // Bungkus tiap baris satu per satu. b dibaca lewat referensi (const std::string&) tanpa disalin.
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan: 2 spasi
            const std::size_t labelPos = b.find(" : ");  // posisi pemisah label dan isi; npos bila tidak ada
            // Ada label selebar paling banyak 28 kolom? Baris lanjutan disejajarkan dengan isi setelah " : ".
            // Label yang lebih lebar akan membuat lanjutan menjorok terlalu jauh, jadi tetap 2 spasi.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // lebar label + lebar " : "
            }
            // Setiap potongan hasil wrapText() masuk ke contentLines.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }  // akhir for
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Lebar dalam = baris terpanjang + 2 margin; std::max menyimpan yang terbesar.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // tetapi tidak melebihi lebar isi maksimal + 2 margin
        std::string filler;  // garis pengisi di sebelah kanan judul pada tepi atas
        // Karakter garis ditulis dengan notasi Unicode: backslash-u lalu 4 digit heksadesimal nomor
        // karakternya. U+2500 = garis mendatar (─). += menyambung satu garis ke filler tiap putaran.
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";
        // Sekarang kita mencetak. std::cout << x mencetak x ke layar; beberapa << berurutan mencetak
        // bagian-bagiannya berurutan (berbeda dengan << 6 di displayWidth(), yang menggeser bit), dan '\n'
        // adalah ganti baris. Tepi atas: sudut kiri atas U+250C dan garis, judul berwarna, garis pengisi,
        // sudut kanan atas U+2510.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Tiap baris isi: tepi kiri U+2502, spasi + teks, spasi pengisi sampai selebar kotak, tepi kanan U+2502.
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;  // margin kiri 1 spasi
            const std::size_t contentWidth = displayWidth(rowText);  // lebar tampil baris ini, tanpa kode warna
            // Jumlah spasi pengisi ditulis dengan operator ternary (kondisi ? nilaiJikaBenar : nilaiJikaSalah):
            // width - contentWidth bila width lebih besar, selain itu 0, dikarenakan size_t tidak boleh negatif.
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }  // akhir for
        std::string bottomLine;  // garis mendatar tepi bawah
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";  // ─ sebanyak lebar dalam kotak
        // Tepi bawah: sudut kiri bawah U+2514, garis, sudut kanan bawah U+2518, semuanya abu-abu.
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }  // akhir fungsi drawBox()

    // Pesan kesalahan dicetak merah dan dibungkus sesuai lebar terminal supaya tidak terpotong jendela sempit.
    // Baris lanjutan menjorok 6 spasi, sejajar dengan teks setelah tanda "[!]". Dipakai oleh fungsi-fungsi
    // pembaca input di BAGIAN 4 (kecuali readLine(), yang mengirim pesannya ke std::cerr).
    void printError(const std::string& text) {
        // Lebar maksimal = lebar terminal - 1, indentasi gantung 6 kolom; tiap potongan dicetak merah tebal.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Mencetak satu garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter ch, misalnya '='.
    // Dipakai oleh printTitle() di BAGIAN 5.
    void printRule(char ch) {
        // std::string(TITLE_WIDTH, ch) membuat teks berisi ch diulang TITLE_WIDTH kali.
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Input dibaca di banyak tempat: mata kuliah, jumlah, nama, nilai. Daripada menulis kode pembacaan dan
    // pemeriksaan yang sama berulang-ulang, kita pecah menjadi fungsi-fungsi kecil di bagian ini. Polanya
    // selalu sama: baca satu baris sebagai teks, rapikan, periksa; kalau salah, beri pesan dan tanya lagi.

    // Dasar dari semua pembaca input: tampilkan prompt (teks pertanyaan, dicetak tebal) lalu baca satu baris
    // ketikan pengguna. Hasilnya std::string.
    std::string readLine(std::string_view prompt) {
        std::string line;  // panjang ketikan tidak pasti, jadi std::string
        std::cout << colorize(ANSI_BOLD, prompt);  // tebal supaya pertanyaan mudah dibedakan dari keterangan
        // std::getline membaca satu baris penuh termasuk spasi (std::cin >> berhenti di spasi pertama).
        // Kalau input ditutup (Ctrl+D di macOS/Linux, Ctrl+Z lalu Enter di Windows), getline gagal.
        // Kita tangkap dengan ! (NOT) lalu menghentikan program, dikarenakan tanpa ini pertanyaan yang sama
        // akan diulang tanpa henti.
        if (!std::getline(std::cin, line)) {
            // std::cerr adalah saluran keluaran khusus pesan kesalahan, terpisah dari std::cout.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            std::exit(EXIT_FAILURE);  // hentikan program dengan kode gagal ke sistem operasi
        }
        return line;
    }  // akhir fungsi readLine()

    // Ketikan pengguna sering membawa spasi tak sengaja. Fungsi ini membuang spasi/tab di awal dan akhir
    // ("  Budi  " menjadi "Budi") dan merapatkan spasi beruntun di tengah menjadi satu spasi.
    // Dipakai oleh semua pembaca input di bawah.
    std::string normalizeSpaces(const std::string& text) {
        std::string result;  // teks yang sudah dirapikan
        bool pendingSpace = false;  // ada spasi yang belum ditulis?
        for (const char c : text) {  // periksa setiap karakter satu per satu
            // Spasi, tab ('\t'), atau CR ('\r', sisa akhir baris Windows) tidak langsung ditulis, hanya ditandai.
            if (c == ' ' || c == '\t' || c == '\r') {
                pendingSpace = !result.empty();  // true hanya bila result sudah berisi: spasi di awal diabaikan
            } else {
                if (pendingSpace) result += ' ';  // spasi beruntun ditulis sebagai satu spasi saja
                result += c;  // += menambahkan c ke akhir result
                pendingSpace = false;
            }
        }  // akhir for
        return result;  // spasi di akhir otomatis terbuang: tidak pernah ditulis
    }  // akhir fungsi normalizeSpaces()

    // Apakah teks hanya berisi angka 0-9, tanpa tanda, titik, huruf, atau spasi? Dipakai oleh
    // isDecimalFormat() dan readInteger() di bawah.
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;  // teks kosong bukan bilangan (empty() = true bila panjangnya 0)
        for (const char c : text) {
            // std::isdigit hanya aman untuk nilai 0-255, sedangkan char bisa negatif untuk byte non-ASCII;
            // static_cast<unsigned char> mengubahnya dulu.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;  // semua karakter lolos
    }

    // Apakah teks berbentuk bilangan desimal yang kita harapkan? Yaitu angka, boleh diikuti SATU pemisah
    // titik atau koma lalu angka lagi: 87, 87.5, atau 87,5. Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau
    // 87 5 ditolak, dikarenakan bukan cara menulis nilai yang wajar. Dipakai oleh parseDecimal() di bawah.
    bool isDecimalFormat(const std::string& text) {
        // auto = tipenya ditentukan compiler dari nilainya (di sini std::size_t). find_first_of(".,") mencari
        // posisi titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        const auto separator = text.find_first_of(".,");
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        // Bagian sebelum pemisah (substr(0, separator)) dan sesudahnya (substr(separator + 1)) harus digit
        // semua; pemisah kedua otomatis tertolak dikarenakan ikut berada di bagian sesudahnya.
        return isAllDigits(text.substr(0, separator))
            && isAllDigits(text.substr(separator + 1));
    }

    // Byte acak seperti 0xFF bukan huruf dan bukan UTF-8, jadi harus ditolak; huruf seperti é atau ö (UTF-8
    // sah) tetap diterima. Fungsi ini memeriksa aturannya: setiap karakter non-ASCII terdiri dari satu byte
    // awal (0xC2-0xF4) diikuti 1-3 byte lanjutan (0x80-0xBF). Dipakai oleh dua pemeriksa nama di bawah.
    bool isValidUtf8(const std::string& text) {
        std::size_t i = 0;  // indeks byte yang sedang diperiksa
        while (i < text.size()) {  // selama masih ada byte yang belum diperiksa (size() = jumlah byte)
            const unsigned char lead = static_cast<unsigned char>(text[i]);  // byte dibaca sebagai 0-255
            std::size_t extra = 0;  // jumlah byte lanjutan yang harus mengikuti
            // Rentang byte awal menentukan panjang karakter.
            if (lead < 0x80)                       extra = 0;    // ASCII biasa
            else if (lead >= 0xC2 && lead <= 0xDF) extra = 1;    // 2 byte, mis. é ö ü
            else if (lead >= 0xE0 && lead <= 0xEF) extra = 2;    // 3 byte, mis. aksara Asia
            else if (lead >= 0xF0 && lead <= 0xF4) extra = 3;    // 4 byte, mis. emoji
            else return false;                                   // 0x80-0xC1 dan 0xF5-0xFF bukan byte awal sah
            if (i + extra >= text.size()) return false;  // byte lanjutan terpotong di akhir teks
            for (std::size_t k = 1; k <= extra; ++k) {  // periksa setiap byte lanjutan
                const unsigned char cont = static_cast<unsigned char>(text[i + k]);
                if (cont < 0x80 || cont > 0xBF) return false;  // byte lanjutan harus 10xxxxxx
            }
            i += extra + 1;  // lompat ke byte awal karakter berikutnya
        }  // akhir while
        return true;
    }  // akhir fungsi isValidUtf8()

    // Apakah teks layak jadi nama mata kuliah? Syaratnya tiga. Harus mengandung huruf. Hanya boleh berisi
    // huruf, angka (Kalkulus 2), spasi, dan tanda titik, koma, &, /, kurung, tanda hubung, atau petik.
    // Tidak lebih panjang dari COURSE_NAME_MAX_LENGTH. Dipakai oleh readCourseName() di bawah.
    bool isValidCourseName(const std::string& text) {
        // Tiga alasan gugur langsung: kosong, lebih dari COURSE_NAME_MAX_LENGTH kolom, atau bukan UTF-8 sah.
        if (text.empty() || displayWidth(text) > COURSE_NAME_MAX_LENGTH || !isValidUtf8(text)) return false;
        bool hasLetter = false;  // sudah ada minimal satu huruf?
        for (const char c : text) {
            const unsigned char uc = static_cast<unsigned char>(c);  // 0-255 supaya aman untuk isalpha/isdigit
            // std::isalpha hanya mengenal ASCII, jadi byte >= 0x80 (bagian huruf UTF-8 seperti é, ö) kita
            // anggap huruf juga. Huruf: tandai hasLetter, lanjut ke karakter berikutnya.
            if (std::isalpha(uc) || uc >= 0x80) { hasLetter = true; continue; }
            // Angka atau salah satu tanda yang diizinkan: lewati. '\'' adalah karakter petik tunggal.
            if (std::isdigit(uc) || c == ' ' || c == '.' || c == ',' || c == '&' || c == '/'
                || c == '(' || c == ')' || c == '-' || c == '\'') continue;
            return false;  // karakter lain (mis. #, @, !) bukan bagian nama mata kuliah
        }  // akhir for
        return hasLetter;  // "123" ditolak: tidak mengandung huruf sama sekali
    }  // akhir fungsi isValidCourseName()

    // Meminta nama mata kuliah, dan mengulang pertanyaannya sampai lolos isValidCourseName() di atas.
    // Dipanggil oleh run() di BAGIAN 6.
    std::string readCourseName(std::string_view prompt) {
        while (true) {  // ulang terus sampai input sah; keluar lewat return
            const std::string text = normalizeSpaces(readLine(prompt));  // const: hasilnya tidak diubah lagi
            if (isValidCourseName(text)) return text;
            // Pesan kesalahan disusun dari potongan teks: std::to_string mengubah angka batas menjadi teks,
            // dan + menyambungnya. Dua teks literal (teks di antara tanda kutip ganda) yang berdampingan
            // otomatis digabung compiler menjadi satu.
            printError("  [!] Nama Mata Kuliah harus mengandung huruf dan hanya boleh berisi huruf, angka, spasi,"
                                        " atau tanda . , & / ( ) - ' (maksimal " + std::to_string(COURSE_NAME_MAX_LENGTH)
                                        + " karakter). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readCourseName()

    // Apakah teks layak jadi nama orang? Hanya huruf, spasi, titik (A. Rizky), tanda hubung (Anne-Marie),
    // atau tanda petik (Nur'aini); harus diawali huruf dan tidak terlalu panjang. Dengan ini ketikan seperti
    // "Luthfi 87.5" atau "123" ditolak, dikarenakan angka dan simbol bukan bagian nama.
    // Dipakai oleh readPersonName() di bawah.
    bool isValidPersonName(const std::string& name) {
        // Tiga alasan gugur langsung: kosong, lebih dari NAME_MAX_LENGTH kolom, atau bukan UTF-8 sah.
        if (name.empty() || displayWidth(name) > NAME_MAX_LENGTH || !isValidUtf8(name)) return false;
        // name.front() = karakter pertama. Ia harus huruf: ASCII (lolos isalpha) atau UTF-8 (>= 0x80).
        const unsigned char first = static_cast<unsigned char>(name.front());
        if (!std::isalpha(first) && first < 0x80) return false;
        for (const char c : name) {
            const unsigned char uc = static_cast<unsigned char>(c);  // 0-255 supaya aman untuk std::isalpha
            const bool isLetter = std::isalpha(uc) != 0 || uc >= 0x80;  // >= 0x80: huruf UTF-8 non-ASCII
            const bool isAllowedMark = (c == ' ' || c == '.' || c == '-' || c == '\'');  // tanda yang diizinkan
            if (!isLetter && !isAllowedMark) return false;  // angka atau simbol lain: bukan nama
        }  // akhir for
        return true;
    }  // akhir fungsi isValidPersonName()

    // Meminta nama mahasiswa, dan mengulang pertanyaannya sampai lolos isValidPersonName() di atas.
    // Dipanggil oleh run() di BAGIAN 6.
    std::string readPersonName(std::string_view prompt) {
        while (true) {  // ulang terus sampai nama sah; keluar lewat return
            const std::string name = normalizeSpaces(readLine(prompt));  // baca satu baris, rapikan spasinya
            if (isValidPersonName(name)) return name;
            // Pesan kesalahan: std::to_string mengubah batas menjadi teks, + menyambung potongannya.
            printError("  [!] Nama hanya boleh berisi huruf, spasi, titik, tanda hubung (-), atau tanda petik ('),"
                                        " diawali huruf, maksimal " + std::to_string(NAME_MAX_LENGTH)
                                        + " karakter. Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readPersonName()

    // Meminta bilangan bulat dari minValue sampai maxValue (keduanya termasuk); di program ini dipakai untuk
    // jumlah mahasiswa. Dipanggil oleh run() di BAGIAN 6.
    int readInteger(std::string_view prompt, int minValue, int maxValue) {
        while (true) {  // ulang terus sampai input sah
            // Kenapa dibaca sebagai teks dulu, bukan langsung std::cin >> angka? Dikarenakan bila std::cin >>
            // angka menerima huruf, std::cin masuk keadaan gagal dan menolak semua input berikutnya sampai
            // dipulihkan dengan std::cin.clear(). Lewat teks, ketikan salah (abc, 2.5) ditolak dengan aman.
            const std::string text = normalizeSpaces(readLine(prompt));
            // istringstream adalah aliran (stream) masukan dari teks: kita membaca darinya dengan >>,
            // persis seperti membaca dari std::cin.
            std::istringstream parser(text);
            int number = 0;  // 0 supaya tidak berisi nilai acak
            // Angkanya sah bila lolos lima pemeriksaan yang dirangkai dengan && (semuanya harus benar):
            // 1. isAllDigits(text): hanya digit 0-9, jadi +3, 3.0, 1e3, dan "3 orang" tertolak.
            // 2. parser >> number: >> mengambil angka dari parser ke number; gagal bila tidak bisa,
            //    misalnya angkanya terlalu besar untuk int.
            // 3. (parser >> std::ws).eof(): std::ws membuang spasi sisa, eof() true bila tidak ada sisa lagi.
            // 4-5. number >= minValue dan number <= maxValue: angkanya masih di dalam rentang.
            if (isAllDigits(text) && parser >> number && (parser >> std::ws).eof() && number >= minValue && number <= maxValue) {
                return number;
            }
            // Pesan kesalahan: std::to_string mengubah batas menjadi teks, + menyambung potongannya.
            printError("  [!] Harus bilangan bulat " + std::to_string(minValue) + "-" + std::to_string(maxValue)
                                        + " (hanya angka). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readInteger()

    // Mengubah teks menjadi bilangan desimal. Hasilnya bool: true bila teksnya sah. Angkanya sendiri dikirim
    // lewat parameter double& result, referensi ke variabel milik pemanggil, dikarenakan nilai kembalian
    // sudah dipakai untuk status sah/tidak. Parameter text adalah salinan, jadi boleh diubah di dalam fungsi.
    // Dipakai oleh readDecimal() di bawah.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu: hanya tulisan angka yang diterima
        // C++ hanya mengenal penulisan 87.5, jadi koma diganti titik. char& (referensi) mengubah karakter
        // langsung pada teksnya.
        for (char& c : text) {
            if (c == ',') c = '.';
        }  // akhir for
        std::istringstream parser(text);  // untuk membaca angka dari teks
        // parser >> result membaca angka desimal ke result; (parser >> std::ws).eof() memastikan tidak ada sisa.
        return (parser >> result) && (parser >> std::ws).eof();
    }

    // Meminta nilai desimal dari minValue sampai maxValue (keduanya termasuk); koma (87,5) diterima.
    // Dipanggil oleh run() di BAGIAN 6 untuk nilai mahasiswa.
    double readDecimal(std::string_view prompt, double minValue, double maxValue) {
        while (true) {  // ulang terus sampai nilai sah; keluar lewat return
            double score = 0.0;  // nilai awal yang jelas
            // parseDecimal() mengisi score lewat referensi dan mengembalikan true bila teks sah; && lalu
            // memastikan score berada dalam rentang.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), score) && score >= minValue && score <= maxValue) {
                return score;
            }
            // Batas ingin tampil "0-100", bukan "0.000000-100.000000"; std::to_string untuk double selalu
            // menulis 6 angka desimal. Jadi kita susun lewat std::ostringstream, aliran keluaran ke teks
            // (menyusun teks dari angka): << menulis angka dan "-" ke dalamnya, str() mengambil hasilnya.
            std::ostringstream range;
            range << minValue << "-" << maxValue;
            printError("  [!] Nilai harus angka " + range.str() + ", hanya digit dengan satu pemisah desimal"
                                        " (mis. 87.5 atau 87,5). Silakan ulangi.");
        }  // akhir while
    }  // akhir fungsi readDecimal()

    // ===== BAGIAN 5: JUDUL, PETUNJUK, DAN TABEL REKAP =====
    // Bagian ini mengurus tampilan: judul program, kotak petunjuk, dan bahan tabel rekap (satu baris
    // tabel dan angka 2 desimal). Isinya hanya menyusun dan mencetak; semua pembaca input ada di BAGIAN 4.

    // Mencetak judul program (cyan tebal) di antara dua garis '=' buatan printRule() (BAGIAN 3).
    // Dipanggil oleh run() di BAGIAN 6.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong sebelum judul
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // judul menjorok 2 spasi
        printRule('=');
    }

    // Sebelum pertanyaan pertama, pengguna perlu tahu bentuk input yang diterima: huruf/angka, rentang, cara
    // menulis desimal. Fungsi ini menampilkannya dalam kotak. Dipanggil oleh run() di BAGIAN 6.
    void printInstructions() {
        // Daftar baris ditulis langsung di antara { ... } (initializer list); compiler menjadikannya
        // std::vector<std::string> untuk parameter lines milik drawBox(). Angka 1-1000 dan 0-100 di sini
        // ditulis manual, jadi bila konstanta di BAGIAN 2 diubah, teks ini harus ikut diubah.
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

    // Membuat satu baris tabel (nomor, nama, nilai) dengan lebar kolom tetap, siap dimasukkan ke kotak.
    // Parameter no, name, score adalah teks tiap kolom; nameWidth lebar kolom nama. Hasilnya std::string.
    // Dipanggil oleh run() di BAGIAN 6.
    std::string formatTableRow(std::string_view no, std::string_view name, std::string_view score, int nameWidth) {
        // Kolom nama tidak memakai std::setw, dikarenakan setw menghitung byte: nama dengan huruf non-ASCII
        // (é, ö) memakai 2 byte per huruf dan kolomnya akan bergeser. Kita isi spasinya sendiri berdasarkan
        // lebar tampil dari displayWidth().
        const std::size_t shownWidth = displayWidth(name);  // lebar nama di layar
        const std::size_t targetWidth = static_cast<std::size_t>(nameWidth);  // lebar kolom, diubah ke size_t
        // Spasi tambahan = targetWidth - shownWidth bila positif, selain itu 0 (operator ternary, lihat drawBox()).
        // std::string(n, ' ') membuat n spasi; + menyambungnya di belakang nama.
        const std::string namePadded = std::string(name) + std::string(targetWidth > shownWidth ? targetWidth - shownWidth : 0, ' ');
        std::ostringstream line;  // menyusun teks baris tanpa mengubah format std::cout
        // std::left/std::right = rata kiri/kanan di dalam kolom; std::setw(n) = lebar kolom n karakter, hanya
        // berlaku untuk item berikutnya.
        line << std::left  << std::setw(NO_COLUMN_WIDTH)    << no
             << namePadded
             << std::right << std::setw(SCORE_COLUMN_WIDTH) << score;
        return line.str();  // str() mengambil teks hasil susunan
    }  // akhir fungsi formatTableRow()

    // Mengubah angka menjadi teks dengan tepat 2 desimal (87.5 -> "87.50"). Dipanggil oleh run() di BAGIAN 6.
    std::string formatTwoDecimals(double number) {
        std::ostringstream text;
        // std::fixed = tulis desimal biasa (bukan notasi ilmiah 8.75e+01); std::setprecision(2) = tepat 2 angka
        // di belakang titik desimal.
        text << std::fixed << std::setprecision(2) << number;
        return text.str();
    }

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 1 =====
// Fungsi run() dibungkus namespace bernama task1: semua nama di dalamnya "milik task1". Kenapa? Program
// bermenu main.cpp di folder yang sama memanggilnya sebagai task1::run(), dan tugas lain punya run() juga,
// jadi masing-masing perlu nama yang khas. Blok ini ditutup oleh "}  // akhir namespace task1".
namespace task1 {

    // Alur utama Tugas 1: judul, petunjuk, input, hitung, lalu rekap. Dipanggil oleh main() di BAGIAN 7,
    // atau oleh menu di main.cpp.
    void run() {
        // Konsol diatur ke UTF-8 di sini supaya program mandiri (tanpa menu) juga benar; aman dipanggil ulang.
        enableUtf8Console();
        printTitle("PROGRAM 1 : TOOLS NILAI MAHASISWA");
        std::cout << '\n';  // baris kosong
        printInstructions();

        // --- Pengaturan awal: mata kuliah dan jumlah mahasiswa ditentukan oleh pengguna ---
        // Keduanya const: setelah diisi tidak diubah lagi. readCourseName() mengulang pertanyaan sampai
        // jawabannya sah.
        const std::string courseName = readCourseName("Nama Mata Kuliah : ");
        // readInteger() membatasi jumlah dari STUDENT_COUNT_MIN sampai STUDENT_COUNT_MAX (1-1000, BAGIAN 2)
        // supaya salah ketik tertolak.
        const int studentCount = readInteger("Jumlah Mahasiswa : ", STUDENT_COUNT_MIN, STUDENT_COUNT_MAX);

        // Daftar mahasiswa dibuat sebesar jumlah yang diketik (alasan memakai vector ada di BAGIAN 1).
        // std::vector<Student> = vector yang tiap elemennya struct Student (BAGIAN 2); angka di dalam ( )
        // adalah jumlah elemen awal. Ukuran vector bertipe size_t sedangkan studentCount int, jadi diubah
        // dulu dengan static_cast.
        std::vector<Student> students(static_cast<std::size_t>(studentCount));

        // --- Tahap input: isi nama dan nilai setiap mahasiswa ---
        std::cout << '\n' << colorize(ANSI_TITLE, "Input Nilai " + courseName) << '\n';  // sub-judul cyan tebal
        int rowNumber = 1;  // nomor urut mulai 1: manusia menghitung dari 1
        // Kunjungi setiap mahasiswa dengan range-based for. Student& (referensi) penting di sini: yang terisi
        // harus data aslinya di dalam vector, bukan salinannya.
        for (Student& student : students) {
            // Label "Mahasiswa ke-N dari M" dicetak tebal; std::to_string mengubah angka menjadi teks.
            std::cout << colorize(ANSI_BOLD, "Mahasiswa ke-" + std::to_string(rowNumber) + " dari " + std::to_string(studentCount)) << '\n';
            // Anggota struct diakses dengan tanda titik: student.name dan student.score. Nama harus huruf,
            // jadi readPersonName(); nilai boleh desimal dan dibatasi SCORE_MIN sampai SCORE_MAX (0-100), jadi
            // readDecimal(). Keduanya ada di BAGIAN 4.
            student.name  = readPersonName   ("  Nama Lengkap  : ");
            student.score = readDecimal("  Nilai (0-100) : ", SCORE_MIN, SCORE_MAX);
            ++rowNumber;  // nomor urut berikutnya
        }  // akhir for

        // --- Tahap perhitungan: total, rata-rata, dan lebar kolom nama ---
        double      total     = 0.0;  // jumlah nilai desimal harus tetap desimal, jadi double
        std::size_t nameWidth = NAME_COLUMN_MIN_WIDTH;  // size_t: dibandingkan dengan hasil displayWidth()
        // Kali ini const Student& (referensi hanya-baca): data dibaca tanpa disalin dan tidak diubah.
        for (const Student& student : students) {
            total += student.score;  // += menjumlahkan ke total (singkatan total = total + student.score)
            // Kolom nama harus muat nama terpanjang + 2 (jarak ke kolom nilai); std::max menyimpan yang terbesar.
            nameWidth = std::max(nameWidth, displayWidth(student.name) + 2);
        }  // akhir for
        // Rata-rata = total dibagi jumlah mahasiswa. total sudah double, jadi hasil baginya desimal
        // (pada int, 11/2 = 5; pada double 5.5). students.size() bertipe size_t; static_cast<double>
        // mengubahnya ke double secara terang-terangan supaya kedua sisi pembagian sama-sama double.
        // Jumlah mahasiswa minimal 1 (BAGIAN 2), jadi tidak pernah terjadi pembagian dengan nol.
        const double average = total / static_cast<double>(students.size());

        // --- Tahap output: tabel rekap disusun ke vector, lalu digambar dalam satu kotak ---
        // formatTableRow() menerima lebar kolom nama sebagai int, dan garis tabel di bawah menjumlahkannya
        // dengan lebar kolom lain yang juga int. Jadi nameWidth (size_t) diubah dulu ke int.
        const int nameColumnWidth = static_cast<int>(nameWidth);
        // Garis '-' selebar tabel: panjangnya jumlah lebar ketiga kolom.
        const std::string tableRule(NO_COLUMN_WIDTH + nameColumnWidth + SCORE_COLUMN_WIDTH, '-');

        std::vector<std::string> summaryLines;  // jumlah baris mengikuti jumlah mahasiswa, jadi vector
        // push_back menambahkan satu baris di akhir vector; + menyambung label dengan isinya.
        summaryLines.push_back("Mata Kuliah      : " + courseName);
        summaryLines.push_back("Jumlah Mahasiswa : " + std::to_string(studentCount));
        summaryLines.push_back("");  // baris kosong pemisah
        // Judul kolom ("No", "Nama", "Nilai") disusun formatTableRow() (BAGIAN 5) lalu dicetak tebal.
        summaryLines.push_back(colorize(ANSI_BOLD, formatTableRow("No", "Nama", "Nilai", nameColumnWidth)));
        summaryLines.push_back(tableRule);  // garis atas tabel
        rowNumber = 1;  // penomoran tabel dimulai ulang dari 1
        // Satu baris tabel per mahasiswa: nomor, nama, dan nilai dengan 2 desimal (formatTwoDecimals()).
        for (const Student& student : students) {
            summaryLines.push_back(formatTableRow(std::to_string(rowNumber), student.name, formatTwoDecimals(student.score), nameColumnWidth));
            ++rowNumber;
        }  // akhir for
        summaryLines.push_back(tableRule);  // garis bawah tabel
        // Kolom No diisi "" supaya label total dan rata-rata sejajar dengan kolom nama. Keduanya hijau tebal,
        // dikarenakan inilah hasil akhir yang paling ditunggu pengguna.
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Total",     formatTwoDecimals(total),    nameColumnWidth)));
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Rata-Rata", formatTwoDecimals(average), nameColumnWidth)));
        std::cout << '\n';  // baris kosong sebelum kotak
        drawBox("Rekap Nilai Mahasiswa", summaryLines);  // kotak rekap (drawBox() di BAGIAN 3)
    }  // akhir fungsi run()

}  // akhir namespace task1

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), dan program bermenu main.cpp sudah punya. Jadi main() di bawah hanya
// ikut saat berkas ini dikompilasi sendiri. #ifndef BUILD_WITH_MENU (if not defined, lihat BAGIAN 1)
// bertanya: apakah makro BUILD_WITH_MENU belum ada? Saat digabung, kompilasi memakai -DBUILD_WITH_MENU
// (opsi -D mendefinisikan makro dari baris perintah), sehingga bagian ini sampai #endif dilewati preprocessor.
#ifndef BUILD_WITH_MENU
// main() adalah titik awal program: fungsi pertama yang dijalankan saat program dibuka. Standar C++
// mewajibkan main bertipe int, dan return 0 di akhirnya berarti program selesai tanpa error.
int main() {
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 1 - Fadhlillah (2531070011)") << '\n';  // identitas, tebal
    task1::run();  // :: memanggil fungsi run() di dalam namespace task1 (BAGIAN 6)
    return 0;
}
#endif  // akhir #ifndef BUILD_WITH_MENU
