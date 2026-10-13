// ===== tugas3_luas_arsir.cpp — Program 3 : Luas Bangun yang Diarsir =====
// Alur program: pengguna memilih satuan panjang (mm/cm/dm/m) dan nilai π, lalu mengetik sisi sebuah persegi (bujur sangkar).
// Di dalam persegi itu ada lingkaran yang menyinggung keempat sisinya, jadi jari-jarinya r = s / 2.
// Keluarannya: luas daerah yang diarsir, yaitu luas persegi dikurangi luas lingkaran.
// Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2)); Pi = π (modul menulisnya phi).
// Nilai π boleh dipilih: 3.14 sesuai modul, atau 22/7 untuk soal yang jari-jarinya kelipatan 7 (berarti sisi perseginya kelipatan 14).
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Cara mengompilasi berkas ini sendiri (jalankan dari folder proyek):
// 1. Buka Terminal VS Code lalu copy paste command di bawah ini :
//   g++ -std=c++17 -Wall -Wextra -o tugas3 src/tugas3_luas_arsir.cpp && ./tugas3
//   (Windows PowerShell: .\tugas3.exe).
// Arti tiap bagian perintah itu:
//   g++                        compiler C++, penerjemah kode C++ menjadi program yang bisa dijalankan
//   -std=c++17                 memakai aturan bahasa C++17
//   -Wall -Wextra              menampilkan peringatan tambahan, supaya kesalahan kecil cepat ketahuan
//   -o tugas3                  nama program hasil kompilasi
//   src/tugas3_luas_arsir.cpp  berkas ini, di folder src
//   && ./tugas3                menjalankan program hasilnya, tetapi hanya bila kompilasi berhasil
// Bila digabung ke program bermenu (src/main.cpp), semua berkas .cpp di folder src dikompilasi sekaligus dan perintahnya ditambah -DBUILD_WITH_MENU:
//   g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
// Opsi -D berfungsi untuk memasang penanda saat kompilasi.
// Bila penanda BUILD_WITH_MENU ada, main() di bagian paling bawah berkas ini (BAGIAN 7) tidak ikut dikompilasi, jadi yang dipakai hanya main() milik main.cpp.
//
// 2. Cara lain di VS Code : buka berkas ini di editor, lalu menu Terminal > Run Task... dan pilih task "Build & Run berkas yang sedang dibuka (tugas sendiri)".
// Task adalah perintah siap pakai; yang satu ini mengompilasi berkas yang sedang aktif di editor sebagai program sendiri (hasilnya src/tugas3_luas_arsir), lalu menjalankannya.
// Task "Build & Run" berbeda: ia mengompilasi lalu menjalankan program bermenu. Daftar task ada di berkas .vscode/tasks.json.


// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Program ini tidak menulis semuanya dari nol. Untuk mencetak ke layar, menyimpan teks, atau memilih nilai terbesar, kita meminjam dari pustaka standar C++.
// #include berfungsi untuk menyalin isi sebuah header ke berkas ini sebelum dikompilasi. Header adalah berkas pustaka berisi fungsi dan tipe siap pakai;
// nama di dalam < > adalah header yang sudah tersedia bersama compiler atau sistem operasi. Fungsi dan tipe milik pustaka standar C++ berawalan std:: (std = standard; tanda :: berarti "milik"),
// jadi std::cout dibaca "cout milik pustaka standar"; nama milik sistem operasi (misalnya isatty, dipakai di BAGIAN 3) dan makro (misalnya EXIT_FAILURE, dipakai di BAGIAN 4) tidak memakai awalan itu.
#include <algorithm> // <algorithm> digunakan untuk std::max dan std::min, yaitu memilih yang lebih besar atau lebih kecil dari dua angka; dipakai saat menghitung lebar jendela, lebar kotak, dan ukuran sketsa.
#include <cctype> // <cctype> digunakan untuk std::isdigit, yaitu memeriksa apakah satu karakter adalah angka 0-9; dipakai saat memeriksa ketikan di BAGIAN 4.
#include <cmath> // <cmath> digunakan untuk std::hypot (jarak dua titik, seperti sisi miring segitiga) dan std::fabs (nilai mutlak bilangan desimal); keduanya dipakai saat menghitung lingkaran sketsa di BAGIAN 5.
#include <cstddef> // <cstddef> digunakan untuk tipe std::size_t, yaitu bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan jumlah, misalnya panjang teks dan lebar kolom.
#include <cstdlib> // <cstdlib> digunakan untuk std::exit dan EXIT_FAILURE (menghentikan program dengan kode "gagal"), std::getenv (membaca variabel lingkungan, yaitu pengaturan
// yang diwariskan terminal ke program), dan std::atoi (mengubah teks menjadi bilangan bulat).
#include <iomanip> // <iomanip> digunakan untuk std::setw dan std::setprecision, yaitu pengatur lebar kolom dan jumlah angka desimal saat mencetak; dipakai formatResultLine() di BAGIAN 5.
#include <initializer_list> // <initializer_list> digunakan untuk tipe std::initializer_list, yaitu daftar nilai yang ditulis di dalam kurung kurawal { ... }; dipakai printOptions() di BAGIAN 5.
#include <iostream> // <iostream> digunakan untuk std::cin (membaca ketikan dari keyboard), std::cout (mencetak ke layar), dan std::cerr (mencetak pesan kesalahan).
#include <sstream> // <sstream> digunakan untuk std::istringstream (membaca angka dari teks) dan std::ostringstream (menyusun teks seperti mencetak).
#include <string> // <string> digunakan untuk tipe std::string (teks yang panjangnya bisa berubah), std::getline (membaca satu baris ketikan utuh), dan std::to_string (angka menjadi teks).
#include <string_view> // <string_view> digunakan untuk tipe std::string_view, yaitu "jendela" untuk membaca sebuah teks tanpa menyalinnya; lebih ringan untuk teks pertanyaan yang hanya perlu dicetak.
#include <vector> // <vector> digunakan untuk tipe std::vector, yaitu daftar berurutan yang bisa bertambah panjang; di berkas ini dipakai untuk baris-baris kotak dan titik-titik sketsa.


// #ifdef berfungsi untuk bertanya ke compiler apakah sebuah nama sudah didefinisikan; bila ya, baris sampai #else/#endif ikut dikompilasi, bila tidak, baris itu dilewati.
// Di sini kita perlu bertanya karena program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya berbicara dengan terminal berbeda:
// mendeteksi terminal, membaca lebar jendela, menyalakan warna, dan UTF-8 (semuanya di BAGIAN 3).
#ifdef _WIN32 // _WIN32 adalah nama yang otomatis ada saat program dikompilasi untuk Windows; artinya baris sampai #else hanya ikut dikompilasi di Windows.
    // #ifndef ("if not defined") berfungsi kebalikan dari #ifdef: baris sampai #endif hanya ikut dikompilasi bila nama itu BELUM didefinisikan.
    #ifndef NOMINMAX // NOMINMAX dicek dulu karena MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan mendefinisikan ulang nama yang sama menghasilkan peringatan.
    // #define berfungsi untuk membuat makro, yaitu nama yang oleh compiler diganti dengan teks lain; mulai baris di bawah, NOMINMAX berarti 1.
    #define NOMINMAX 1 // NOMINMAX 1 artinya "jangan buat min dan max" dinyalakan, karena windows.h punya kebiasaan membuat makro min/max sendiri yang merusak std::min dan std::max.
    #endif // selesai mengecek NOMINMAX
    #include <windows.h> // <windows.h> adalah pustaka resmi Windows; dari sini dipakai fungsi untuk warna (mode virtual terminal), UTF-8 (code page), dan lebar jendela konsol, semuanya di BAGIAN 3.
#else // #else artinya: bila bukan Windows (macOS atau Linux), pakai pustaka di bawah ini.
    #include <cstdio> // <cstdio> digunakan untuk stdout, yaitu "saluran" keluaran ke layar, dan fileno, yang memberi nomor saluran itu.
    #include <sys/ioctl.h> // <sys/ioctl.h> digunakan untuk ioctl dan TIOCGWINSZ, yaitu cara bertanya ke terminal seberapa lebar jendelanya.
    #include <unistd.h> // <unistd.h> digunakan untuk isatty (memeriksa apakah keluaran benar-benar terminal, bukan berkas) dan STDOUT_FILENO (nomor saluran keluaran layar).
#endif  // #endif berfungsi untuk menutup blok #ifdef/#ifndef; di sini artinya selesai memilih pustaka sesuai sistem operasi.

// namespace berfungsi untuk membuat wadah nama; nama di dalamnya terpisah dari nama yang sama di wadah lain.
// namespace { tanpa nama berfungsi untuk membuat isinya hanya terlihat dari berkas ini. Ini perlu karena tugas1_nilai_mahasiswa.cpp dan tugas2_volume_tabung.cpp (folder src) juga punya
// fungsi bantu bernama sama, misalnya colorsEnabled() dan drawBox() (di berkas ini ada di BAGIAN 3); bila ketiganya digabung ke program bermenu tanpa wadah ini, g++ gagal dengan error "multiple definition".
namespace { // namespace { adalah awal wadah tanpa nama; wadah ini ditutup pada baris "}  // akhir namespace tanpa nama" sebelum BAGIAN 6.

    // ===== BAGIAN 2: KONSTANTA =====
    // Angka yang tidak pernah berubah selama program jalan kita beri nama di sini, supaya tidak ada "angka ajaib" tersebar di dalam fungsi.
    // constexpr berfungsi untuk membuat konstanta: nilainya sudah pasti sejak kompilasi dan tidak bisa diubah.
    // double adalah tipe bilangan desimal (boleh punya angka di belakang koma); dipakai karena ukuran dan π tidak selalu bilangan bulat.
    constexpr double PI_MODULE   = 3.14;        // PI_MODULE   = 3.14; adalah nilai π bawaan, sesuai modul dan sama dengan program 2 (tugas2_volume_tabung.cpp); modul menulisnya "phi",
    // tetapi yang dimaksud π (pi), bukan φ (phi) rasio emas 1,618.
    constexpr double PI_FRACTION = 22.0 / 7.0;  // PI_FRACTION = 22.0 / 7.0; adalah nilai π soal sekolah bila jari-jari kelipatan 7 (sisi kelipatan 14); ditulis 22.0 / 7.0, bukan 22 / 7,
    // karena pembagian dua bilangan bulat hasilnya 3, bukan 3.142857.
    // int adalah tipe bilangan bulat biasa yang boleh bernilai negatif. Dua lebar di bawah dihitung dalam jumlah karakter.
    constexpr int    TITLE_WIDTH = 60;         // TITLE_WIDTH = 60; adalah panjang terbesar garis judul; di terminal yang lebih sempit garisnya selebar terminal dikurangi 1 kolom. Dipakai printRule() di BAGIAN 3.
    constexpr int    LABEL_WIDTH = 24;         // LABEL_WIDTH = 24; adalah lebar kolom label pertanyaan pilihan, agar semua opsi sejajar; dipakai printOptions() di BAGIAN 5.
    constexpr double DIMENSION_MAX = 1000000.0;  // DIMENSION_MAX = 1000000.0; adalah batas atas ukuran sisi: menolak salah ketik yang tidak masuk akal; dipakai readPositiveDecimal() di BAGIAN 4.
    // const char* adalah penunjuk (pointer) ke teks tetap: tanda * berarti penunjuk, yaitu alamat tempat teks itu berada di memori, dan const berarti hurufnya tidak boleh diubah.
    // Isinya kode warna ANSI, yaitu teks khusus yang tidak tercetak; terminal membacanya sebagai perintah ganti gaya. Bentuknya "\033[" + angka gaya + huruf m.
    // "\033" adalah karakter ESC (nomor 27, ditulis dalam oktal: 3 x 8 + 3 = 27). Kode ANSI dipakai karena terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_HATCH = "\033[31m"; // ANSI_HATCH = "\033[31m"; 31 artinya merah; dipakai untuk arsiran pada sketsa, meniru gambar di soal.

    // Tanda [] setelah nama berarti array: daftar bernomor yang jumlahnya tetap; nomornya (indeks) mulai dari 0.
    constexpr const char* UNITS[] = { "mm", "cm", "dm", "m" }; // UNITS[] = { "mm", "cm", "dm", "m" }; adalah daftar satuan panjang yang lazim di soal; urutannya = nomor pilihan 1-4 di chooseUnit() (BAGIAN 5).
    constexpr int         UNIT_COUNT   = 4;  // UNIT_COUNT   = 4; adalah jumlah isi UNITS; menjadi batas nomor pilihan di chooseUnit() (BAGIAN 5).
    constexpr int         DEFAULT_UNIT_INDEX   = 2; // DEFAULT_UNIT_INDEX   = 2; adalah NOMOR PILIHAN untuk cm (pilihan [2]), bukan indeks array: UNITS[2] adalah "dm", cm ada di UNITS[1];
    // chooseUnit() mengurangi 1 lebih dulu sebelum mengambil dari UNITS. Mengapa cm? Hampir semua soal luas daerah arsiran memakai cm.

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, pilihan yang ditegaskan, hasil akhir, dan pesan kesalahan mudah dibedakan, program memakai warna. Angka di dalam kode ANSI: 0 = normal, 1 = tebal,
    // 31 = merah, 32 = hijau, 33 = kuning, 36 = cyan, 90 = abu-abu. Dua angka dipisah ; berarti digabung.
    constexpr const char* ANSI_RESET  = "\033[0m";     // ANSI_RESET  = "\033[0m"; kode untuk mengembalikan tampilan ke normal (tanpa warna/tebal); angka 0 artinya reset.
    constexpr const char* ANSI_BOLD  = "\033[1m";     // ANSI_BOLD  = "\033[1m"; angka 1 artinya tebal; dipakai untuk prompt (teks pertanyaan yang menunggu ketikan pengguna) dan label penting.
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // ANSI_TITLE  = "\033[1;36m"; 1;36 artinya tebal (1) sekaligus cyan (36); dipakai untuk judul program, judul kotak, dan garis lingkaran sketsa.
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // ANSI_SUCCESS  = "\033[1;32m"; 1;32 artinya tebal dan hijau (32); dipakai untuk pilihan yang ditegaskan dan hasil akhir.
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // ANSI_ERROR  = "\033[1;31m"; 1;31 artinya tebal dan merah (31); dipakai untuk pesan kesalahan.
    constexpr const char* ANSI_HINT = "\033[33m"; // ANSI_HINT = "\033[33m"; 33 artinya kuning; dipakai untuk keterangan nilai bawaan [Enter = ...] dan garis jari-jari R pada sketsa.
    constexpr const char* ANSI_DIM    = "\033[90m";    // ANSI_DIM    = "\033[90m"; 90 artinya abu-abu (hitam terang); dipakai untuk garis kotak dan garis pembatas agar tidak mencolok.

    // void berfungsi menandai fungsi yang tidak mengembalikan nilai apa pun; fungsi seperti ini dipanggil untuk efeknya saja. Fungsi adalah sekumpulan perintah bernama yang bisa
    // dipanggil berulang; isinya ditulis di antara { dan }, dan () kosong berarti fungsi itu tidak menerima parameter (nilai masukan).
    void enableUtf8Console() { // enableUtf8Console() berfungsi untuk mengganti konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama; dipanggil run() di BAGIAN 6.
    // UTF-8 adalah aturan menyimpan setiap huruf dan simbol sebagai 1 sampai 4 byte: huruf Latin biasa 1 byte, π dan ² 2 byte, garis kotak dan Braille 3 byte. Konsol Windows bawaan
    // (Command Prompt, PowerShell) masih memakai code page lama, misalnya 437 (code page adalah tabel aturan byte mana menjadi huruf apa), sehingga π, ², garis kotak, dan sketsa Braille
    // tampil sebagai huruf acak. Garis kotak berasal dari drawBox() di bawah; sketsa Braille dari printSketch() di BAGIAN 5.
    // Pengaturan ini sama dengan perintah chcp 65001 di berkas peluncur jalankan.bat. Di macOS/Linux terminal sudah UTF-8, jadi di sana isi fungsi ini kosong.
    #ifdef _WIN32 // _WIN32: dua baris di bawah hanya ikut dikompilasi di Windows, karena SetConsoleOutputCP dan SetConsoleCP hanya ada di windows.h.
        SetConsoleOutputCP(CP_UTF8);  // SetConsoleOutputCP(CP_UTF8) berfungsi untuk mengatur keluaran konsol ke UTF-8, agar byte UTF-8 tampil sebagai karakter yang benar; CP_UTF8 adalah konstanta Windows bernilai 65001.
        SetConsoleCP(CP_UTF8);        // SetConsoleCP(CP_UTF8) berfungsi untuk mengatur masukan (ketikan) konsol ke UTF-8 juga, agar huruf di luar Latin biasa terbaca sama dengan keluarannya.
    #endif  // akhir bagian khusus Windows
    } // akhir fungsi enableUtf8Console()

    // bool adalah tipe data yang hanya punya dua nilai: true (benar) atau false (salah).
    bool colorsEnabled() { // colorsEnabled() berfungsi untuk menentukan boleh memakai warna atau tidak; hasil true berarti boleh berwarna. Dipakai colorize() di bawah.
    // Aturannya mengikuti kebiasaan program terminal: bila pengguna menyetel NO_COLOR, warna mati; bila FORCE_COLOR, warna dipaksa hidup; selain itu warna hanya dipakai
    // bila keluaran benar-benar terminal, bukan berkas. Di Windows, fungsi ini sekaligus menyalakan mode "virtual terminal" agar Command Prompt memahami kode ANSI.
        // static berfungsi untuk membuat variabel yang dibuat sekali saja (saat fungsi pertama kali dipanggil) lalu nilainya diingat sampai program selesai; jadi pemeriksaan di bawah cukup sekali.
        // lambda []() { ... } adalah fungsi kecil tanpa nama yang ditulis langsung di tempat; di sini ia menghitung nilai awal enabled, karena pemeriksaannya butuh beberapa langkah if.
        static const bool enabled = []() { // enabled = []() { artinya nilai awal enabled diambil dari hasil lambda ini; const membuat nilainya tidak bisa diubah lagi.
            // if berfungsi untuk menjalankan perintah hanya bila syarat di dalam kurung bernilai benar.
            // != artinya "tidak sama dengan".
            // nullptr adalah penunjuk kosong, yaitu penunjuk yang tidak menunjuk ke mana pun.
            // return berfungsi untuk mengakhiri fungsi (di sini lambda) saat itu juga dan memberikan hasilnya.
            if (std::getenv("NO_COLOR") != nullptr) return false; // std::getenv("NO_COLOR") berfungsi untuk membaca variabel lingkungan NO_COLOR, yaitu pengaturan yang diwariskan terminal ke program;
            // hasil nullptr berarti variabel itu tidak ada. Bila NO_COLOR ada, pengguna minta tanpa warna, jadi hasilnya false.
            if (std::getenv("FORCE_COLOR") != nullptr) return true; // std::getenv("FORCE_COLOR") != nullptr artinya pengguna memaksa warna, jadi hasilnya true tanpa memeriksa terminal.
        #ifdef _WIN32 // _WIN32: cara memeriksa terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
            // HANDLE adalah tipe Windows untuk "pegangan", yaitu tanda pengenal sebuah saluran (di sini layar konsol) yang dipakai saat memanggil fungsi Windows lain.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); // GetStdHandle(STD_OUTPUT_HANDLE) berfungsi untuk mengambil pegangan ke keluaran layar (konsol), lalu disimpan di output.
            // DWORD adalah tipe bilangan bulat tanpa tanda negatif milik Windows (32 bit).
            DWORD  mode     = 0;  // mode = 0 adalah tempat menyimpan mode konsol yang sedang aktif; nilainya diisi GetConsoleMode di baris berikutnya.
            // == artinya "sama dengan" (membandingkan), berbeda dengan = yang mengisi nilai.
            // || artinya "atau": hasilnya benar bila salah satu syarat benar.
            // ! artinya "tidak": membalik benar menjadi salah dan sebaliknya.
            // & di depan nama variabel berfungsi untuk mengambil alamatnya; &mode adalah alamat variabel mode, supaya GetConsoleMode bisa mengisi variabel itu.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false; // output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode) artinya pegangannya tidak sah,
            // atau keluaran bukan konsol (misalnya dialihkan ke berkas) sehingga mode tidak bisa dibaca; maka hasilnya false: tanpa warna.
            // | (OR per bit) berfungsi untuk menambahkan sebuah bendera ke nilai lama tanpa menghapus bendera yang sudah ada.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0; // SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) berfungsi untuk menyalakan mode
            // virtual terminal agar konsol memahami kode ANSI; SetConsoleMode mengembalikan 0 bila gagal, jadi != 0 berarti berhasil dan warna aktif.
        #else  // #else: bukan Windows (macOS atau Linux); di sini cukup satu pertanyaan: apakah keluaran layar sebuah terminal?
            return isatty(fileno(stdout)) != 0; // isatty(fileno(stdout)) != 0 artinya fileno(stdout) memberi nomor saluran keluaran layar, lalu isatty memeriksa apakah saluran itu terminal (bukan berkas);
            // hasil selain 0 berarti terminal, jadi warna dipakai.
        #endif  // akhir pemeriksaan terminal sesuai sistem operasi
        }();  // }(); adalah akhir lambda; tanda () langsung menjalankan lambda satu kali, dan hasilnya menjadi nilai awal enabled.
        return enabled;  // return enabled; mengembalikan hasil yang tersimpan, jadi pemanggilan berikutnya tidak memeriksa ulang.
    } // akhir fungsi colorsEnabled()

    // std::string adalah tipe teks milik pustaka standar yang panjangnya bisa berubah; di sini menjadi tipe hasil fungsi colorize().
    std::string colorize(const char* code, std::string_view text) { // colorize(const char* code, std::string_view text) berfungsi untuk membungkus text dengan kode warna, tetapi hanya bila warna boleh dipakai.
    // Parameter code diisi salah satu konstanta ANSI_... di atas, sedangkan text dibaca lewat std::string_view tanpa disalin. Hasilnya std::string baru, siap dicetak.
        if (!colorsEnabled()) return std::string(text);  // !colorsEnabled() artinya warna tidak boleh dipakai, jadi text dikembalikan apa adanya; std::string(text) membuat salinannya sebagai std::string.
        // + pada teks berfungsi untuk menyambung teks; code dan text diubah dulu ke std::string supaya bisa disambung.
        return std::string(code) + std::string(text) + ANSI_RESET; // std::string(code) + std::string(text) + ANSI_RESET artinya kode warna, lalu teks, lalu ANSI_RESET agar warna tidak terbawa ke teks berikutnya.
    } // akhir fungsi colorize()

    // unsigned adalah bilangan bulat tanpa tanda negatif (0 ke atas).
    bool isWideCodePoint(unsigned codePoint) { // isWideCodePoint(unsigned codePoint) berfungsi untuk memeriksa apakah sebuah code point termasuk karakter lebar, yaitu karakter yang digambar terminal selebar dua kolom.
    // Code point adalah nomor unik sebuah karakter di Unicode, daftar baku semua karakter di dunia. Aksara CJK (Cina, Jepang, Korea), Hangul (huruf Korea), dan emoji termasuk lebar;
    // bila ini diabaikan, lebar kotak dan pembungkusan baris jadi salah. Dipakai di displayWidth() di bawah.
        // && artinya "dan": hasilnya benar hanya bila kedua syarat benar. >= artinya "lebih dari atau sama dengan", dan <= artinya "kurang dari atau sama dengan".
        // Angka berawalan 0x ditulis dalam heksadesimal (basis 16). Setiap pasangan (>= awal && <= akhir) adalah satu rentang karakter lebar.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF) // (codePoint >= 0x1100 && codePoint <= 0x115F) adalah Hangul Jamo (huruf penyusun Hangul); 0x2E80-0xA4CF adalah CJK dan aksara Asia Timur lain.
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF) // || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) adalah suku kata Hangul; 0xF900-0xFAFF adalah aksara CJK kompatibilitas.
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60) // || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) adalah bentuk kompatibilitas CJK; 0xFF00-0xFF60 adalah huruf lebar penuh.
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F) // || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) adalah tanda lebar penuh (misalnya ¥); 0x1F300-0x1F64F adalah emoji.
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); // || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); adalah emoji tambahan. Hasilnya true bila code point masuk salah satu rentang di atas.
    } // akhir fungsi isWideCodePoint()

    // std::size_t adalah tipe bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan panjang (berasal dari <cstddef>).
    std::size_t displayWidth(std::string_view text) { // displayWidth(std::string_view text) berfungsi untuk menghitung berapa kolom layar yang dipakai text; hasilnya tidak selalu sama dengan jumlah byte-nya.
    // UTF-8 menyimpan satu karakter dalam 1 sampai 4 byte, padahal π, ², garis kotak, dan Braille tetap selebar satu kolom; aksara lebar (CJK, emoji) memakai dua kolom, dan kode warna ANSI
    // memakai nol kolom. Karena itu teks dihitung byte demi byte. Dipakai oleh pembungkus baris wrapText() dan penggambar kotak drawBox() di bawah, serta printSketch() di BAGIAN 5 untuk mengukur keterangan terpanjang.
        std::size_t width = 0;  // width = 0 adalah jumlah kolom yang sudah terhitung, mula-mula 0.
        bool inEscape = false;  // inEscape = false artinya belum berada di dalam kode warna "\033[...m"; nilainya true selama berada di dalamnya.
        unsigned codePoint = 0; // codePoint = 0 adalah code point yang sedang dirangkai dari byte-byte UTF-8.
        int pending = 0;        // pending = 0 adalah jumlah byte lanjutan UTF-8 yang masih ditunggu untuk melengkapi code point.
        // for (elemen : kumpulan) adalah perulangan "untuk setiap" (range-based for): mengambil setiap elemen satu per satu dari awal sampai akhir, tanpa perlu indeks.
        // unsigned char adalah satu byte bernilai 0-255; char biasa bisa negatif untuk byte >= 128, dan itu merusak perbandingan di bawah.
        for (const unsigned char c : text) { // for (const unsigned char c : text) artinya ambil setiap byte di text satu per satu ke variabel c; const berarti c tidak diubah di dalam perulangan.
            // continue berfungsi untuk langsung lanjut ke putaran berikutnya tanpa menjalankan baris di bawahnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; } // if (inEscape) artinya sedang di dalam kode warna: byte dilewati (nol kolom) sampai huruf 'm' penutupnya; 'm' bertanda petik tunggal adalah satu karakter.
            if (c == 0x1B) { inEscape = true; continue; }  // c == 0x1B artinya byte ini karakter ESC (nomor 27, sama dengan "\033"), pembuka kode warna.
            // & di antara dua nilai adalah AND per bit: berfungsi untuk mengambil bit tertentu saja dari sebuah byte. Satu byte = 8 bit, masing-masing 0 atau 1.
            if ((c & 0xC0) == 0x80) { // (c & 0xC0) == 0x80 artinya byte ini byte lanjutan UTF-8, yang selalu berpola 10xxxxxx: & 0xC0 mengambil 2 bit teratas, dan hasil 0x80 berarti "10".
                // << berfungsi untuk menggeser bit ke kiri. -- berfungsi untuk mengurangi nilai variabel sebanyak satu, dan ++ untuk menambahnya sebanyak satu.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; } // (codePoint << 6) | (c & 0x3F) artinya geser code point 6 bit ke kiri,
                // lalu tempelkan 6 bit data byte ini. Bila --pending menjadi 0, code point sudah lengkap; karakter lebar lalu mendapat satu kolom tambahan (++width),
                // karena kolom pertamanya sudah dihitung di byte awal.
                continue; // continue; artinya byte lanjutan bukan karakter baru, jadi tidak menambah kolom.
            } // akhir penanganan byte lanjutan
            ++width;  // ++width; artinya setiap byte awal = satu karakter = minimal satu kolom.
            // Pola bit byte awal menentukan berapa byte lanjutan yang menyusul; bit x milik byte awal disimpan sebagai awal code point.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; } // (c & 0xE0) == 0xC0 artinya byte awal berpola 110xxxxx: menyusul 1 byte lanjutan, dan 5 bit data (c & 0x1F) disimpan.
            // else if berfungsi untuk memeriksa syarat berikutnya hanya bila syarat sebelumnya salah; else terakhir dijalankan bila semua syarat salah.
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; } // (c & 0xF0) == 0xE0 artinya pola 1110xxxx: menyusul 2 byte lanjutan, dan 4 bit data (c & 0x0F) disimpan.
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; } // (c & 0xF8) == 0xF0 artinya pola 11110xxx: menyusul 3 byte lanjutan, dan 3 bit data (c & 0x07) disimpan.
            else                         { pending = 0; } // else { pending = 0; } artinya byte ASCII biasa (pola 0xxxxxxx, huruf Latin biasa) yang tidak punya byte lanjutan.
        } // akhir perulangan setiap byte
        return width; // return width; mengembalikan jumlah kolom yang terhitung.
    } // akhir fungsi displayWidth()

    int terminalWidth() { // terminalWidth() berfungsi untuk menanyakan lebar jendela terminal (dalam kolom) setiap kali dipanggil, jadi tampilan ikut menyesuaikan bila jendela diubah ukurannya.
    // Bila terminal tidak bisa ditanya (keluaran bukan terminal), dicoba variabel lingkungan COLUMNS; bila itu juga tidak ada, dipakai 80 kolom, ukuran bawaan terminal pada umumnya.
    // Hasilnya int dan tidak pernah di bawah 40: tata letak program (kotak, menu, sketsa, tabel) dirancang dan diuji mulai 40 kolom, dan 40 sudah cukup untuk Termux di ponsel tegak (sekitar 46 kolom); di terminal yang lebih sempit, terminal sendiri yang melipat baris.
        int cols = 0;  // cols = 0 artinya lebar belum diketahui; setiap cara di bawah hanya mengisinya bila berhasil.
    #ifdef _WIN32 // _WIN32: cara bertanya ke terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows. Struct adalah wadah berisi beberapa data bernama, dan tanda titik (.) berfungsi untuk mengambil salah satu datanya.
        CONSOLE_SCREEN_BUFFER_INFO info; // CONSOLE_SCREEN_BUFFER_INFO info; adalah tempat menampung keterangan jendela konsol; bagian srWindow menyimpan nomor kolom kiri (Left) dan kanan (Right).
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1; // GetConsoleScreenBufferInfo(...) mengisi info lewat alamatnya (&info);
        // bila berhasil, lebar jendela = Right - Left + 1, karena nomor kolom kanan dan kiri keduanya termasuk.
    #else  // #else: bukan Windows (macOS atau Linux)
        // struct di depan winsize menandakan bahwa winsize adalah sebuah struct; winsize berasal dari <sys/ioctl.h>.
        struct winsize ws; // struct winsize ws; adalah tempat menampung ukuran jendela; bagian ws_col berisi jumlah kolom.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col; // ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) berfungsi untuk meminta ukuran jendela dari saluran keluaran layar (STDOUT_FILENO)
        // dan mengisinya ke ws lewat alamatnya (&ws); hasil 0 berarti berhasil, lalu cols diisi jumlah kolom.
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // std::atoi berfungsi untuk mengubah teks angka, misalnya "100", menjadi bilangan int 100.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); } // cols <= 0 artinya lebar masih belum diketahui, jadi dicoba variabel lingkungan COLUMNS;
        // env menunjuk ke teksnya, atau bernilai nullptr bila COLUMNS tidak ada.
        if (cols <= 0) cols = 80;  // cols = 80 adalah lebar bawaan bila lebar tetap belum diketahui.
        // std::max berfungsi untuk memilih yang lebih besar dari dua nilai (berasal dari <algorithm>).
        return std::max(40, cols);  // std::max(40, cols) artinya hasilnya tidak pernah di bawah 40 kolom.
    } // akhir fungsi terminalWidth()

    // std::vector<std::string> adalah daftar berurutan berisi teks; di sini menjadi tipe hasil fungsi, satu elemen untuk satu baris.
    // const std::string& adalah referensi tetap (const&): tanda & yang ditulis setelah nama tipe berarti fungsi memakai teks milik pemanggil langsung tanpa menyalin,
    // dan const membuat teks itu tidak boleh diubah. Ini beda dengan &mode di colorsEnabled() di atas, yang ditulis di depan variabel dan memberi alamatnya.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) { // wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent)
    // berfungsi untuk memotong text di spasi menjadi beberapa baris yang masing-masing muat dalam maxWidth kolom, supaya terminal tidak memotongnya sembarangan.
    // Baris lanjutan diberi indentasi gantung sebanyak hangingIndent spasi, agar sejajar dengan isi setelah label. maxWidth dan hangingIndent diterima sebagai salinan, jadi boleh diubah di dalam fungsi.
    // Dipakai drawBox() dan printError() di bawah, readLine() di BAGIAN 4, printOptions() dan printTitle() di BAGIAN 5, run() di BAGIAN 6, dan main() di BAGIAN 7.
    // Lebar dihitung per karakter dengan displayWidth() (aksara lebar seperti CJK memakai 2 kolom). Bila tidak ada spasi yang pas, dipakai cadangan: spasi setelah teks dimulai (lanjutannya menjorok 2 spasi), atau kata yang terlalu panjang dipotong di batas lebar.
    // Kode warna yang masih menyala di ujung sebuah baris dimatikan di sana dan dinyalakan lagi di awal baris berikutnya; tanda aksen gabungan tetap menempel pada hurufnya, dan spasi biasa di kedua sisi titik potong dibuang.
        std::vector<std::string> result;  // result adalah daftar baris hasil, mula-mula kosong.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2; // hangingIndent + 10 > maxWidth artinya jendela terlalu sempit untuk indentasi gantung + 10 kolom isi, jadi indentasi dikecilkan menjadi 2 spasi.
        std::string remaining = text;  // remaining adalah sisa teks yang belum dipotong; disalin dari text karena isinya akan diganti di setiap putaran.
        // while berfungsi untuk mengulang isi blok selama syarat di dalam kurung masih benar.
        while (displayWidth(remaining) > maxWidth) { // displayWidth(remaining) > maxWidth artinya sisa teks masih lebih lebar dari maxWidth, jadi potong satu baris lagi dari depannya.
            std::size_t col = 0, cutPos = std::string::npos; // col menghitung kolom layar, dan cutPos menyimpan posisi spasi tempat memotong; std::string::npos adalah nilai khusus yang berarti "tidak ditemukan" (belum ada spasi).
            std::size_t looseCutPos = std::string::npos, hardCutPos = std::string::npos; // looseCutPos dan hardCutPos adalah dua titik potong cadangan, mula-mula std::string::npos (belum ada):
            // looseCutPos = spasi terakhir setelah teks dimulai, hardCutPos = posisi byte tempat batas lebar tercapai. Keduanya dipakai setelah perulangan for di bawah.
            bool seenText = false; // seenText = false artinya belum bertemu teks; nilainya menjadi true begitu bertemu karakter pertama yang bukan spasi, supaya spasi indentasi di depan baris tidak dipakai sebagai titik potong.
            bool inEscape = false;  // inEscape = false: nilainya true selama berada di dalam kode warna, yang tidak memakai kolom.
            // for (awal; syarat; langkah) adalah perulangan for biasa: mulai dari awal, berulang selama syarat benar, dan menjalankan langkah setiap selesai satu putaran.
            for (std::size_t i = 0; i < remaining.size(); ++i) { // for (std::size_t i = 0; i < remaining.size(); ++i) artinya periksa setiap byte remaining dari posisi 0 sampai terakhir untuk mencari
            // spasi terakhir sebelum batas lebar; remaining.size() adalah jumlah byte.
                // static_cast<...> berfungsi untuk mengubah tipe sebuah nilai secara tertulis, sehingga perubahan itu jelas terlihat di kode.
                const unsigned char c = static_cast<unsigned char>(remaining[i]); // static_cast<unsigned char>(remaining[i]) mengubah byte ke-i (dihitung dari 0) menjadi unsigned char 0-255,
                // karena byte UTF-8 di atas 127 bisa terbaca sebagai angka negatif pada char biasa.
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // if (inEscape) artinya lewati isi kode warna sampai huruf 'm'.
                if (c == 0x1B) { inEscape = true; continue; }  // c == 0x1B artinya ESC, awal kode warna.
                if ((c & 0xC0) == 0x80) continue;  // (c & 0xC0) == 0x80 artinya byte lanjutan UTF-8; ia bukan karakter baru, jadi tidak menambah kolom.
                std::size_t charEnd = i + 1; // charEnd adalah posisi byte sesudah karakter ini; mula-mula i + 1, karena satu karakter paling sedikit satu byte.
                while (charEnd < remaining.size() && (static_cast<unsigned char>(remaining[charEnd]) & 0xC0) == 0x80) ++charEnd; // (... & 0xC0) == 0x80 artinya byte berikutnya
                // adalah byte lanjutan UTF-8 milik karakter yang sama, jadi ++charEnd melewatinya.
                // += berfungsi untuk menambahkan nilai di kanan ke variabel di kiri: angka dijumlahkan, sedangkan teks disambung di ujungnya.
                // Tanda aksen gabungan (U+0300 sampai U+036F; dalam UTF-8 dua byte 0xCC 0x80 sampai 0xCD 0xAF) digambar menempel pada huruf sebelumnya,
                // jadi ia ikut menjadi bagian karakter ini supaya potongan paksa tidak memisahkan huruf dari aksennya.
                while (charEnd + 1 < remaining.size() && (static_cast<unsigned char>(remaining[charEnd]) == 0xCC // charEnd + 1 < remaining.size() artinya masih ada dua byte untuk diperiksa;
                       || (static_cast<unsigned char>(remaining[charEnd]) == 0xCD && static_cast<unsigned char>(remaining[charEnd + 1]) <= 0xAF))) { // == 0xCC || (== 0xCD && <= 0xAF) artinya
                // dua byte itu adalah satu tanda aksen gabungan.
                    charEnd += 2; // charEnd += 2 artinya lewati tanda aksen itu (dua byte), karena ia milik huruf yang sama.
                } // akhir perulangan tanda aksen gabungan
                const std::size_t charWidth = displayWidth(remaining.substr(i, charEnd - i)); // charWidth adalah lebar karakter ini di layar menurut displayWidth():
                // 2 kolom untuk aksara lebar (CJK, Hangul, emoji), 1 kolom untuk huruf lain, ditambah 1 kolom untuk setiap tanda aksen gabungan yang menempel, karena displayWidth() menghitung aksen itu tersendiri.
                // break berfungsi untuk keluar dari perulangan saat itu juga.
                if (col + charWidth > maxWidth) { hardCutPos = i; break; } // col + charWidth > maxWidth artinya karakter ini tidak muat lagi di baris: hardCutPos = i mencatat posisinya untuk potongan paksa, lalu break berhenti mencari.
                if (c == ' ' && col > hangingIndent) cutPos = i;  // c == ' ' && col > hangingIndent artinya spasi ini menjadi calon titik potong, asalkan letaknya setelah indentasi gantung;
                // yang akhirnya tersimpan adalah spasi terakhir sebelum batas lebar.
                if (c == ' ' && seenText) looseCutPos = i; // c == ' ' && seenText artinya spasi ini ada setelah teks dimulai, jadi dicatat sebagai titik potong cadangan looseCutPos.
                if (c != ' ') seenText = true; // c != ' ' artinya karakter ini bukan spasi, jadi seenText = true: teks di baris ini sudah dimulai.
                col += charWidth; // col += charWidth artinya kolom bertambah selebar karakter ini, sama dengan hitungan displayWidth() untuk seluruh teks.
                i = charEnd - 1; // i = charEnd - 1 artinya lompat ke byte terakhir karakter ini (termasuk tanda aksennya), supaya ++i di perulangan for langsung menuju karakter berikutnya.
            } // akhir perulangan mencari titik potong
            // Bila tidak ada spasi setelah indentasi gantung (misalnya angka hasil yang sangat panjang setelah label), dipakai spasi cadangan.
            if (cutPos == std::string::npos && looseCutPos != std::string::npos) { cutPos = looseCutPos; hangingIndent = 2; } // cutPos == std::string::npos && looseCutPos != std::string::npos artinya
            // hanya ada spasi cadangan: cutPos = looseCutPos memotong di sana, dan hangingIndent = 2 membuat lanjutannya cukup menjorok 2 spasi supaya isinya muat.
            std::size_t pieceEnd = 0, restStart = 0; // pieceEnd adalah posisi byte akhir potongan baris ini, dan restStart adalah posisi byte awal sisa teks untuk baris berikutnya.
            if (cutPos != std::string::npos) { // cutPos != std::string::npos artinya ada spasi untuk memotong.
                pieceEnd = cutPos; // pieceEnd = cutPos artinya potongan berakhir tepat sebelum spasi itu.
                restStart = remaining.find_first_not_of(' ', cutPos); // remaining.find_first_not_of(' ', cutPos) adalah posisi huruf pertama setelah spasi itu,
                // jadi spasi di titik potong (juga spasi ganda) dibuang dan baris berikutnya tidak menjorok terlalu jauh.
                if (restStart == std::string::npos) restStart = remaining.size(); // restStart == std::string::npos artinya setelah spasi itu tidak ada huruf lagi, jadi sisanya kosong.
            } else if (hardCutPos != std::string::npos && col > hangingIndent) { // hardCutPos != std::string::npos && col > hangingIndent artinya tidak ada spasi yang bisa dipakai
            // sebelum batas lebar (satu kata yang lebih panjang dari satu baris, atau kata yang berakhir tepat di batas lebar sehingga spasi sesudahnya tidak sempat diperiksa), jadi teks dipotong tepat di batas lebar. Syarat col > hangingIndent artinya potongan
            // memuat huruf selain spasi indentasi, sehingga sisa teks selalu memendek dan perulangan pasti selesai.
                pieceEnd = hardCutPos; // pieceEnd = hardCutPos artinya potongan berakhir tepat sebelum karakter yang tidak muat.
                restStart = remaining.find_first_not_of(' ', hardCutPos); // remaining.find_first_not_of(' ', hardCutPos) adalah awal sisa teks: karakter yang tidak muat itu,
                // atau huruf sesudahnya bila karakter itu spasi (spasi itu tidak pindah ke awal baris berikutnya); tidak ada huruf yang dibuang.
                if (restStart == std::string::npos) restStart = remaining.size(); // restStart == std::string::npos artinya sesudahnya hanya ada spasi, jadi sisanya kosong.
            } else { // else artinya tidak ada titik potong yang aman sama sekali.
                break; // break artinya keluar dari while dan membiarkan sisa teks apa adanya.
            } // akhir pemilihan titik potong
            while (pieceEnd > 0 && remaining[pieceEnd - 1] == ' ') --pieceEnd; // remaining[pieceEnd - 1] == ' ' artinya potongan berakhir dengan spasi (misalnya spasi pertama
            // dari spasi ganda), jadi --pieceEnd membuangnya dari ujung baris.
            std::string piece = remaining.substr(0, pieceEnd); // piece adalah teks baris ini: remaining.substr(0, pieceEnd) mengambil byte dari awal sampai sebelum pieceEnd.
            // Kode warna yang masih menyala di ujung piece dimatikan dulu dengan ANSI_RESET, lalu dinyalakan lagi di awal baris berikutnya,
            // supaya warnanya tidak bocor ke garis tepi kotak dan baris lanjutan tetap berwarna sama. Hanya kode warna terakhir yang dibawa, karena program ini tidak pernah menumpuk dua kode warna pada satu teks.
            std::string activeColor; // activeColor adalah kode warna yang masih menyala di ujung piece; mula-mula kosong.
            const std::size_t lastEscape = piece.rfind('\033'); // piece.rfind('\033') adalah posisi kode warna terakhir di piece (dicari dari belakang), atau std::string::npos bila tidak ada.
            if (lastEscape != std::string::npos) activeColor = piece.substr(lastEscape, piece.find('m', lastEscape) - lastEscape + 1); // piece.substr(lastEscape, ...) adalah kode warna itu,
            // mulai dari ESC ('\033') sampai huruf 'm' penutupnya.
            if (activeColor == ANSI_RESET) activeColor.clear(); // activeColor == ANSI_RESET artinya kode terakhir justru mematikan warna; activeColor.clear() berfungsi untuk mengosongkannya.
            if (!activeColor.empty()) piece += ANSI_RESET; // !activeColor.empty() artinya masih ada warna yang menyala, jadi piece += ANSI_RESET mematikannya di ujung baris.
            result.push_back(piece); // result.push_back(piece) artinya potongan ini menjadi satu baris hasil.
            remaining = std::string(hangingIndent, ' ') + activeColor + remaining.substr(restStart); // std::string(hangingIndent, ' ') + activeColor + remaining.substr(restStart) adalah
            // baris berikutnya: spasi indentasi gantung, kode warna yang dinyalakan lagi (bila ada), lalu sisa teks.
        } // akhir perulangan while
        result.push_back(remaining);  // result.push_back(remaining); artinya sisa terakhir sudah muat, jadi ia menjadi baris terakhir.
        return result; // return result; mengembalikan daftar baris hasil.
    } // akhir fungsi wrapText()

    void drawBox(std::string_view title, const std::vector<std::string>& lines) { // drawBox(std::string_view title, const std::vector<std::string>& lines) berfungsi untuk menggambar kotak berjudul title
    // berisi baris-baris lines. Kotak membuat informasi (petunjuk, sketsa, hasil) terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah
    // melebihi lebar terminal; baris yang terlalu panjang dibungkus dengan wrapText(), dan baris berpola "Label : isi" dibuat sejajar dengan isi setelah label.
    // Garis kotak ditulis sebagai kode Unicode di dalam string: "\u250c" = ┌, "\u2510" = ┐, "\u2514" = └, "\u2518" = ┘, "\u2500" = ─ (garis datar), dan "\u2502" = │ (garis tegak).
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5); // terminalWidth() - 5 adalah lebar isi terbesar: lebar terminal dikurangi 2 tepi kotak, 2 margin, dan 1 kolom cadangan;
        // kolom cadangan perlu karena sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela. static_cast<std::size_t> mengubah hasil int menjadi std::size_t.
        // Judul yang lebih lebar dari garis atas (bisa terjadi di terminal sempit) dilipat per kata: potongan pertama di garis atas, sisanya di baris isi pertama.
        const std::vector<std::string> titleLines = wrapText(std::string(title), maxWidth - 2, 0); // wrapText(std::string(title), maxWidth - 2, 0) berfungsi untuk melipat judul tanpa indentasi gantung;
        // maxWidth - 2 karena garis atas "┌─ judul ─┐" butuh 4 kolom di luar judul, sedangkan lebar dalam kotak paling besar maxWidth + 2. Judul yang muat tetap satu potongan.
        const std::string& titleTop = titleLines[0]; // titleLines[0] adalah potongan judul pertama (titleTop), yang ditulis di garis atas kotak.
        std::vector<std::string> contentLines;  // contentLines adalah baris isi setelah dibungkus, siap digambar.
        for (std::size_t i = 1; i < titleLines.size(); ++i) contentLines.push_back(" " + colorize(ANSI_TITLE, titleLines[i])); // titleLines.size() adalah jumlah potongan judul; mulai dari i = 1 artinya potongan ke-2 dan seterusnya (bila ada)
        // menjadi baris isi pertama, cyan tebal seperti judul, dan didahului 1 spasi supaya lurus dengan judul di garis atas.
        for (const std::string& b : lines) { // for (const std::string& b : lines) artinya ambil setiap baris b di lines tanpa menyalinnya, lalu tentukan indentasi gantungnya dan bungkus dengan wrapText().
            std::size_t hangingIndent = 2;  // hangingIndent = 2 adalah bawaan: baris lanjutan menjorok 2 spasi.
            const std::size_t labelPos = b.find(" : ");  // b.find(" : ") mencari posisi " : " pertama, atau std::string::npos bila tidak ada.
            const std::size_t leadingSpaces = b.find_first_not_of(' '); // b.find_first_not_of(' ') berfungsi untuk mencari posisi karakter pertama yang bukan spasi, yaitu banyaknya spasi di depan baris;
            // hasilnya std::string::npos bila barisnya kosong atau hanya berisi spasi.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) { // labelPos != std::string::npos && ... <= 28 artinya ada " : " dan label di depannya paling lebar 28 kolom,
            // jadi baris ini berpola "Label : isi".
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // displayWidth(b.substr(0, labelPos)) + 3 artinya lebar label ditambah 3 untuk " : ", jadi lanjutannya sejajar dengan isi setelah " : ".
            } else if (leadingSpaces != std::string::npos && leadingSpaces > 2) { // leadingSpaces != std::string::npos && leadingSpaces > 2 artinya bukan pola "Label : isi", tetapi barisnya
            // sudah menjorok lebih dari 2 spasi (misalnya keterangan menu atau contoh di kotak petunjuk).
                hangingIndent = leadingSpaces; // hangingIndent = leadingSpaces artinya lanjutannya menjorok sejauh baris itu sendiri, supaya tetap lurus di bawahnya.
            } // akhir pemeriksaan pola "Label : isi" dan indentasi baris
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece); // wrapText(b, maxWidth, hangingIndent) memotong b, lalu setiap potongan (piece) dimasukkan ke contentLines.
        } // akhir perulangan membungkus baris isi
        std::size_t width = displayWidth(titleTop) + 4;  // displayWidth(titleTop) + 4 adalah lebar dalam minimal: "─ " sebelum judul (2), spasi setelah judul (1), dan paling sedikit satu ─ pengisi (1).
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2); // std::max(width, displayWidth(b) + 2) artinya kotak diperlebar sampai baris terpanjang + 2, yaitu margin kiri dan kanan.
        // std::min berfungsi untuk memilih yang lebih kecil dari dua nilai (berasal dari <algorithm>).
        width = std::min(width, maxWidth + 2);  // std::min(width, maxWidth + 2) artinya lebar kotak tidak boleh lebih dari maxWidth + 2 margin.
        std::string filler;  // filler adalah garis pengisi di kanan judul, mula-mula kosong.
        for (std::size_t i = displayWidth(titleTop) + 3; i < width; ++i) filler += "\u2500";  // filler += "\u2500" artinya tambahkan ─ sebanyak sisa lebar setelah "─ judul ".
        // std::cout << berfungsi untuk mencetak ke layar; beberapa << berurutan mencetak bagian-bagiannya satu per satu dari kiri ke kanan.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, titleTop) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n'; // colorize(ANSI_DIM, "\u250c\u2500 ") ... adalah garis atas ┌─ judul ───┐:
        // garisnya abu-abu dan judulnya cyan tebal; '\n' adalah karakter pindah baris.
        for (const std::string& b : contentLines) { // for (const std::string& b : contentLines) artinya cetak setiap baris isi: │, teks, spasi pengisi sampai lebar kotak, lalu │ lagi.
            const std::string rowText = " " + b;  // " " + b artinya satu spasi margin kiri di depan baris.
            const std::size_t contentWidth = displayWidth(rowText); // contentWidth adalah lebar rowText di layar.
            // Operator ternary (a ? b : c) artinya "bila a benar pakai b, bila tidak pakai c". Di baris ini ia menjaga jumlah spasi pengisi paling sedikit 0, karena std::size_t
            // tidak bisa negatif: pengurangan yang hasilnya di bawah 0 akan berubah menjadi angka raksasa.
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ') // width > contentWidth ? width - contentWidth : 0 adalah jumlah spasi pengisi.
                      << colorize(ANSI_DIM, "\u2502") << '\n'; // colorize(ANSI_DIM, "\u2502") adalah │ garis tegak kanan, lalu '\n' pindah baris.
        } // akhir perulangan baris isi
        std::string bottomLine;  // bottomLine adalah garis bawah, mula-mula kosong.
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500"; // bottomLine += "\u2500" artinya tambahkan ─ sebanyak lebar kotak.
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';  // "\u2514" + bottomLine + "\u2518" adalah garis bawah └───┘ berwarna abu-abu.
    } // akhir fungsi drawBox()

    void printError(const std::string& text) { // printError(const std::string& text) berfungsi untuk mencetak pesan kesalahan berwarna merah yang dibungkus sesuai lebar terminal, agar pesan panjang
    // tidak terpotong jendela sempit. Pesannya selalu diawali "  [!] ", jadi baris lanjutan menjorok 6 spasi agar sejajar dengan teks setelah tanda "[!]". Dipakai fungsi-fungsi pembaca input di BAGIAN 4.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) { // wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6) artinya lebar bungkus = lebar terminal
        // dikurangi 1 kolom cadangan, dan 6 = indentasi gantung.
            std::cout << colorize(ANSI_ERROR, b) << '\n'; // colorize(ANSI_ERROR, b) artinya setiap baris hasil bungkus dicetak merah tebal.
        } // akhir perulangan baris pesan
    } // akhir fungsi printError()

    // char adalah tipe satu karakter, misalnya '=' (ditulis di antara petik tunggal).
    void printRule(char ch) { // printRule(char ch) berfungsi untuk mencetak satu baris garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter ch, atau selebar terminal dikurangi 1 kolom cadangan bila terminalnya lebih sempit; dipakai printTitle() di BAGIAN 5.
        const std::size_t ruleWidth = static_cast<std::size_t>(std::min(TITLE_WIDTH, terminalWidth() - 1)); // ruleWidth = std::min(TITLE_WIDTH, terminalWidth() - 1) adalah panjang garis: yang lebih kecil antara 60 dan lebar terminal dikurangi 1 kolom cadangan, supaya garis tidak melipat di terminal sempit (misalnya 46 kolom di Termux); static_cast<std::size_t> mengubah hasil int itu menjadi std::size_t, tipe panjang yang diminta std::string.
        std::cout << colorize(ANSI_DIM, std::string(ruleWidth, ch)) << '\n'; // std::string(ruleWidth, ch) berfungsi untuk membuat teks berisi ruleWidth kali karakter ch, lalu dicetak abu-abu dan diakhiri pindah baris.
    } // akhir fungsi printRule()

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====

    std::string readLine(std::string_view prompt) { // readLine(std::string_view prompt) berfungsi untuk mencetak prompt (teks pertanyaan) dengan huruf tebal, lalu membaca satu baris ketikan dan
    // mengembalikannya sebagai std::string. Semua pembacaan ketikan lewat satu pintu ini, jadi cara menangani input yang tiba-tiba berakhir cukup ditulis sekali.
        std::string line;  // line adalah tempat menampung ketikan; std::string karena panjang ketikan tidak pasti.
        std::cout << colorize(ANSI_BOLD, prompt);  // colorize(ANSI_BOLD, prompt) mencetak pertanyaan tebal supaya mudah dibedakan dari keterangan.
        // std::getline berfungsi untuk membaca satu baris ketikan utuh sampai Enter. Dipakai, bukan std::cin >>, karena seluruh baris perlu diperiksa: cin >> berhenti di spasi atau
        // di karakter yang bukan angka (misalnya 7,5 terbaca 7), dan sisa ketikannya tertinggal lalu ikut terbaca oleh pertanyaan berikutnya.
        if (!std::getline(std::cin, line)) { // !std::getline(std::cin, line) artinya pembacaan dari std::cin (masukan keyboard) gagal karena input sudah ditutup, jadi tidak ada ketikan lagi:
        // pengguna menekan Ctrl+D (macOS/Linux) atau Ctrl+Z lalu Enter (Windows), atau input dialihkan dari berkas dan isinya sudah habis. Program tidak boleh lanjut tanpa data.
            // std::cerr berfungsi untuk mencetak pesan galat lewat saluran khusus yang terpisah dari std::cout.
            std::cerr << '\n'; // std::cerr << '\n' artinya pindah baris dulu, karena kursor masih di belakang pertanyaan terakhir.
            for (const std::string& piece : wrapText("Input berakhir sebelum data lengkap. Program dihentikan.", static_cast<std::size_t>(terminalWidth() - 1), 0)) { // wrapText(...) berfungsi untuk
            // melipat pesan bahwa input sudah habis supaya muat di terminal sempit; 0 artinya tanpa indentasi gantung.
                std::cerr << piece << '\n'; // std::cerr << piece << '\n' artinya cetak satu potongan pesan, lalu pindah baris.
            } // akhir perulangan potongan pesan
            // std::exit berfungsi untuk menghentikan seluruh program saat itu juga, dari fungsi mana pun.
            std::exit(EXIT_FAILURE);  // std::exit(EXIT_FAILURE) artinya hentikan program dengan kode keluar EXIT_FAILURE ("gagal").
        } // akhir penanganan input yang berakhir
        return line; // return line; mengembalikan ketikan apa adanya; spasinya dirapikan oleh normalizeSpaces() di bawah.
    } // akhir fungsi readLine()

    std::string normalizeSpaces(const std::string& text) { // normalizeSpaces(const std::string& text) berfungsi untuk membuang spasi dan tab di awal dan akhir teks, lalu merapatkan spasi beruntun
    // menjadi satu, supaya salah ketik spasi tidak membuat input ditolak. Parameternya referensi const karena teks hanya dibaca, tidak perlu disalin.
        std::string result;                  // result adalah teks yang sudah dirapikan, mula-mula kosong.
        bool pendingSpace = false;         // pendingSpace = false artinya belum ada spasi yang tertunda (sudah dibaca tetapi belum ditulis ke result).
        for (const char c : text) {         // for (const char c : text) artinya ambil setiap karakter text satu per satu ke variabel c.
            if (c == ' ' || c == '\t' || c == '\r') { // c == ' ' || c == '\t' || c == '\r' artinya karakter ini spasi, tab ('\t'), atau CR ('\r', sisa akhir baris Windows): tidak langsung ditulis, hanya ditandai.
                pendingSpace = !result.empty(); // !result.empty() artinya "hasil sudah berisi": spasi ditandai tertunda hanya bila result tidak kosong, jadi spasi di awal diabaikan.
            } else { // } else { artinya karakter ini bukan spasi.
                if (pendingSpace) result += ' ';  // if (pendingSpace) result += ' '; artinya spasi beruntun yang tertunda ditulis sebagai satu spasi saja.
                result += c;                      // result += c; menambahkan karakter ini ke akhir result.
                pendingSpace = false; // pendingSpace = false; artinya spasi tertunda sudah ditangani.
            } // akhir cabang bukan spasi
        } // akhir perulangan setiap karakter
        return result;                       // return result; mengembalikan teks rapi; spasi di akhir otomatis terbuang karena tidak pernah ditulis.
    } // akhir fungsi normalizeSpaces()

    bool isAllDigits(const std::string& text) { // isAllDigits(const std::string& text) berfungsi untuk memeriksa apakah text hanya berisi angka 0-9, tanpa tanda, titik, huruf, atau spasi; dipakai isDecimalFormat() di bawah.
        if (text.empty()) return false;  // text.empty() artinya teks kosong: bukan bilangan, hasilnya false.
        for (const char c : text) { // for (const char c : text) artinya periksa setiap karakter text satu per satu.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false; // !std::isdigit(static_cast<unsigned char>(c)) artinya karakter ini bukan digit, jadi langsung false; std::isdigit hanya aman untuk nilai 0-255,
            // dan static_cast<unsigned char> membawa char (yang bisa negatif untuk byte UTF-8) ke rentang aman itu.
        } // akhir perulangan setiap karakter
        return true; // return true; artinya semua karakter digit.
    } // akhir fungsi isAllDigits()

    bool isDecimalFormat(const std::string& text) { // isDecimalFormat(const std::string& text) berfungsi untuk memeriksa apakah text berbentuk bilangan desimal yang kita harapkan: angka, boleh diikuti SATU
    // pemisah titik atau koma lalu angka lagi (14, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, karena bukan cara menulis ukuran yang wajar. Dipakai parseDecimal() di bawah.
        // auto berfungsi untuk membiarkan compiler menentukan tipe variabel sendiri dari nilainya; di sini tipenya posisi teks (std::size_t).
        const auto separator = text.find_first_of(".,"); // text.find_first_of(".,") mencari posisi titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        if (separator == std::string::npos) return isAllDigits(text);  // separator == std::string::npos artinya tanpa pemisah: seluruh teks harus digit.
        return isAllDigits(text.substr(0, separator))             // isAllDigits(text.substr(0, separator)) artinya bagian sebelum pemisah harus digit; substr(0, separator) = teks dari awal sampai sebelum pemisah.
            && isAllDigits(text.substr(separator + 1));           // && isAllDigits(text.substr(separator + 1)); artinya bagian sesudah pemisah juga harus digit; pemisah kedua (8.7.5) otomatis tertolak,
            // karena bagian sesudahnya masih memuat titik.
    } // akhir fungsi isDecimalFormat()

    // double& adalah referensi ke variabel double milik pemanggil: fungsi mengisi variabel itu langsung, jadi hasil angka "dikirim balik" lewat parameter.
    bool parseDecimal(std::string text, double& result) { // parseDecimal(std::string text, double& result) berfungsi untuk mengubah text menjadi bilangan desimal yang ditaruh di result; hasil true bila teksnya sah.
    // Parameter text diterima sebagai salinan (tanpa &), jadi boleh diubah di dalam fungsi. Dipakai readPositiveDecimal() di bawah.
        if (!isDecimalFormat(text)) return false;  // !isDecimalFormat(text) artinya formatnya salah, jadi langsung false; format diperiksa dulu agar hanya tulisan angka yang lolos.
        for (char& c : text) {        // for (char& c : text) artinya ambil setiap karakter text lewat referensi (char&), jadi mengubah c berarti mengubah karakter di dalam text.
            if (c == ',') c = '.';    // c == ',' artinya koma diganti titik, karena C++ hanya mengenal penulisan 7.5.
        } // akhir perulangan mengganti koma
        // std::istringstream adalah "aliran masukan dari teks": membaca angka dengan operator >> persis seperti std::cin membaca ketikan.
        std::istringstream parser(text); // parser(text) adalah aliran yang isinya text, siap dibaca sebagai angka.
        // >> pada aliran berfungsi untuk membaca nilai berikutnya dari aliran ke sebuah variabel.
        return (parser >> result) && (parser >> std::ws).eof(); // (parser >> result) artinya berhasil membaca angka ke result; (parser >> std::ws).eof() artinya setelah spasi dilewati (std::ws)
        // tidak ada sisa karakter lagi (eof = end of file, akhir aliran). Keduanya harus benar (&&) agar teks dinyatakan sah.
    } // akhir fungsi parseDecimal()

    double readPositiveDecimal(std::string_view prompt) { // readPositiveDecimal(std::string_view prompt) berfungsi untuk meminta bilangan desimal yang lebih besar dari 0 dan paling besar DIMENSION_MAX (1000000),
    // diulang sampai sah; hasilnya double. Dipanggil run() di BAGIAN 6 untuk sisi persegi.
        // while (true) berfungsi untuk mengulang tanpa henti, karena syaratnya selalu benar; jalan keluarnya hanya lewat return (atau std::exit di readLine() di atas bila input berakhir).
        while (true) {  // while (true) { artinya tanya terus sampai angkanya sah.
            double value = 0.0;  // value = 0.0 adalah tempat hasil angka; double karena ukuran boleh desimal, 0.0 sebagai nilai awal yang jelas.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) { // parseDecimal(normalizeSpaces(readLine(prompt)), value) adalah tiga langkah bersarang:
            // readLine membaca ketikan, normalizeSpaces merapikan spasi, parseDecimal mengubahnya ke angka di value; value > 0.0 menolak nol dan negatif, karena panjang sisi tidak mungkin nol
            // atau negatif; value <= DIMENSION_MAX menolak angka raksasa.
                return value; // return value; mengembalikan angka yang sah dan menghentikan perulangan.
            } // akhir pemeriksaan angka yang sah
            // Teks di dalam tanda kutip disebut string literal. Dua string literal yang berdampingan otomatis disambung compiler menjadi satu, jadi pesan panjang bisa dipecah ke dua baris kode.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan" // printError("  [!] Harus angka lebih besar dari 0 ...") artinya ketikan tidak sah: tampilkan pesan merah,
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi."); // " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi."); adalah sambungan pesan yang sama; setelah itu while (true) mengulang pertanyaan.
        } // akhir perulangan while (true)
    } // akhir fungsi readPositiveDecimal()

    int readChoice(std::string_view prompt, int maxValue, int defaultValue) { // readChoice(std::string_view prompt, int maxValue, int defaultValue) berfungsi untuk meminta pilihan berupa satu digit 1..maxValue;
    // Enter tanpa isi berarti pilihan bawaan (defaultValue), supaya pilihan paling umum (cm, π = 3.14) cukup ditekan Enter. Hasilnya int, nomor pilihan. Dipakai chooseUnit() dan choosePi() di BAGIAN 5.
        while (true) {  // while (true) { artinya diulang terus, karena pilihan salah harus diminta lagi.
            const std::string text = normalizeSpaces(readLine(prompt));  // normalizeSpaces(readLine(prompt)) adalah ketikan yang sudah dirapikan spasinya; const karena tidak diubah lagi.
            if (text.empty()) return defaultValue;  // text.empty() artinya pengguna hanya menekan Enter, jadi hasilnya pilihan bawaan.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) { // text.size() == 1 && std::isdigit(...) artinya sah bila tepat satu karakter dan karakter itu digit; text[0] adalah karakter pertama (dihitung dari 0).
                const int choice = text[0] - '0'; // text[0] - '0' berfungsi untuk mengubah karakter digit menjadi angka: karakter disimpan sebagai nomor kode berurutan ('0' = 48, '2' = 50), jadi '2' - '0' = 2.
                if (choice >= 1 && choice <= maxValue) return choice;  // choice >= 1 && choice <= maxValue artinya nomor harus berada di rentang 1..maxValue; bila ya, itulah hasilnya.
            } // akhir pemeriksaan satu digit
            // std::to_string berfungsi untuk mengubah angka menjadi teks supaya bisa disambung dengan + ke pesan.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue) // printError("  [!] Pilihan tidak dikenal. ..." + std::to_string(maxValue) artinya ketikan tidak sah: pesan merah,
                                        + ", atau tekan Enter untuk memakai bawaan."); // + ", atau tekan Enter untuk memakai bawaan."); adalah sambungan pesan yang sama; setelah itu while (true) mengulang pertanyaan.
        } // akhir perulangan while (true)
    } // akhir fungsi readChoice()

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // std::initializer_list<std::string_view> adalah daftar teks yang ditulis pemanggil dalam kurung kurawal { "...", "..." }; tipe isinya ditulis di dalam < >.
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) { // printOptions(std::string_view label, std::initializer_list<std::string_view> line) berfungsi untuk mencetak satu
    // pertanyaan pilihan dengan bentuk yang selalu sama: baris pertama berisi label (tebal) dan opsi pertama, baris berikutnya opsi lain yang disejajarkan di bawahnya, baris terakhir nilai bawaan (kuning).
    // Dengan begitu pengguna langsung mengenali pola "[nomor] pilihan" dan "[Enter = bawaan]". Dipakai chooseUnit() dan choosePi() di bawah.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // terminalWidth() - 1 adalah lebar maksimal satu baris: lebar terminal dikurangi 1 kolom cadangan.
        bool isFirst = true;  // isFirst = true adalah penanda baris pertama, yang memuat label.
        for (const std::string_view b : line) {  // for (const std::string_view b : line) artinya ambil setiap baris opsi b berurutan.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter"); // b.substr(0, 6) == "[Enter" artinya 6 karakter pertama baris ini "[Enter": inilah baris nilai bawaan.
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b); // isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b) artinya baris bawaan diwarnai kuning, baris lain dijadikan std::string apa adanya.
            const std::string start = isFirst // start adalah awalan baris; isFirst ? ... : ... memilih bentuknya: baris pertama memuat label, baris berikutnya hanya spasi penyejajar.
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": " // ? colorize(ANSI_BOLD, ...) + ": " artinya label tebal ditambah spasi sampai selebar LABEL_WIDTH (24), lalu ": ";
                // ternary di dalam std::string(n, ' ') menghitung spasi tambahan: LABEL_WIDTH - label.size(), atau 0 bila label sudah lebih panjang.
                : std::string(LABEL_WIDTH + 2, ' '); // : std::string(LABEL_WIDTH + 2, ' '); artinya baris berikutnya diberi spasi selebar LABEL_WIDTH + 2 (26) agar sejajar dengan isi setelah ": ".
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n'; // wrapText(start + content, maxWidth, LABEL_WIDTH + 2) membungkus baris agar opsi panjang
            // tidak terpotong di jendela sempit; setiap potongan (piece) dicetak lalu pindah baris.
            isFirst = false;  // isFirst = false; artinya setelah baris pertama tercetak, baris berikutnya bukan baris label lagi.
        } // akhir perulangan baris opsi
    } // akhir fungsi printOptions()

    const char* chooseUnit() { // chooseUnit() berfungsi untuk menampilkan daftar satuan, membaca pilihan, lalu menegaskan simbol satuan yang terpilih; hasilnya const char*, teks tetap simbol satuan. Dipanggil run() di BAGIAN 6.
        // Dua kolom opsi satuan butuh 41 kolom layar (label 26 + opsi 15) ditambah 1 kolom cadangan; di terminal yang lebih sempit opsinya ditulis
        // satu per baris, supaya nomor pilihan tidak terpisah dari satuannya dan urutannya tetap 1, 2, 3, 4.
        if (terminalWidth() < 42) { // terminalWidth() < 42 artinya terminal terlalu sempit untuk dua kolom opsi.
            printOptions("Pilih Satuan Panjang", { "[1] mm", "[2] cm", "[3] dm", "[4] m", "[Enter = cm]" }); // printOptions("Pilih Satuan Panjang", { "[1] mm", ... }) artinya
            // satu opsi per baris, lalu baris bawaan [Enter = cm].
        } else { // else artinya terminal cukup lebar untuk dua kolom opsi.
            printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" }); // printOptions("Pilih Satuan Panjang", { ... }) mencetak pertanyaan satuan; argumen kedua { ... } adalah daftar baris opsi
        } // akhir pemilihan tata letak opsi satuan
        // untuk parameter initializer_list. Argumen adalah nilai yang dikirim ke parameter saat fungsi dipanggil.
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX); // readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX) membaca nomor 1..UNIT_COUNT (4);
        // DEFAULT_UNIT_INDEX (2 = cm) dipakai bila pengguna hanya menekan Enter.
        const char* unit = UNITS[selection - 1];  // UNITS[selection - 1] mengambil simbol satuan dari array; - 1 karena nomor pilihan mulai dari 1, sedangkan indeks array dari 0.
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit) << "\n\n"; // std::string("  -> Satuan panjang : ") + unit menyambung teks tetap dengan simbol satuan, dicetak hijau sebagai penegasan;
        // "\n\n" membuat satu baris kosong setelahnya.
        return unit; // return unit; mengembalikan simbol satuan yang dipilih.
    } // akhir fungsi chooseUnit()

    // std::string& bekerja seperti double& pada parseDecimal() di BAGIAN 4, hanya saja yang diisi fungsi adalah teks milik pemanggil.
    double choosePi(std::string& legend) { // choosePi(std::string& legend) berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih; nilai π untuk perhitungan dikembalikan
    // lewat return (double), sedangkan teks nilai π untuk ditampilkan di hasil dikirim lewat parameter legend. Dipanggil run() di BAGIAN 6.
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)", // printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)", adalah awal pertanyaan nilai π; baris pertama memuat opsi 1.
                                      "[2] 22/7  (soal dengan sisi persegi kelipatan 14)", // "[2] 22/7  (soal dengan sisi persegi kelipatan 14)", adalah opsi 2.
                                      "[Enter = 3.14]" }); // "[Enter = 3.14]" }); adalah baris nilai bawaan, lalu }); menutup daftar dan pemanggilan printOptions().
        const int selection = readChoice("Pilihan Anda            : ", 2, 1); // readChoice("Pilihan Anda            : ", 2, 1) membaca nomor 1..2; 1 = nomor bawaan (3.14) bila pengguna hanya menekan Enter.
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // (selection == 2) ? "22/7 (3.142857)" : "3.14" artinya ternary memilih teks sesuai nomor; disimpan ke legend milik pemanggil.
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // "  -> Nilai Pi (\u03c0)   : " + legend adalah penegasan hijau; \u03c0 adalah cara menulis simbol π (code point U+03C0) di dalam string.
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // (selection == 2) ? PI_FRACTION : PI_MODULE artinya ternary lagi: nilai π sesuai nomor pilihan.
    } // akhir fungsi choosePi()

    void printTitle(std::string_view text) { // printTitle(std::string_view text) berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas '=' sepanjang TITLE_WIDTH (atau selebar terminal dikurangi 1 kolom); dipanggil run() di BAGIAN 6.
        std::cout << '\n';  // std::cout << '\n'; mencetak baris kosong supaya judul tidak menempel pada teks sebelumnya.
        printRule('='); // printRule('=') mencetak garis '=' di atas judul.
        // Judul dilipat per kata dengan wrapText() (BAGIAN 3) supaya di terminal sempit (misalnya 46 kolom di Termux) tidak terpotong di tengah kata oleh terminal.
        for (const std::string& piece : wrapText("  " + std::string(text), static_cast<std::size_t>(terminalWidth() - 1), 2)) { // wrapText("  " + std::string(text), terminalWidth() - 1, 2) artinya judul yang menjorok 2 spasi dilipat selebar terminal dikurangi 1, dan baris lanjutannya ikut menjorok 2 spasi.
            std::cout << colorize(ANSI_TITLE, piece) << '\n';  // colorize(ANSI_TITLE, piece) berfungsi untuk mencetak tiap potongan judul cyan tebal, satu potongan per baris.
        } // akhir perulangan potongan judul
        printRule('='); // printRule('=') mencetak garis '=' di bawah judul.
    } // akhir fungsi printTitle()

    std::string repeatText(std::string_view text, int n) { // repeatText(std::string_view text, int n) berfungsi untuk mengulang text sebanyak n kali. Mengapa tidak std::string(n, c) saja? Karena itu hanya bisa
    // mengulang satu byte, sedangkan karakter UTF-8 seperti ─ terdiri dari beberapa byte. Dipakai printSketch() di bawah.
        std::string result; // result adalah teks hasil, mula-mula kosong.
        for (int i = 0; i < n; ++i) result += text;  // for (int i = 0; i < n; ++i) result += text; artinya tambahkan text ke akhir result, diulang n kali.
        return result; // return result; mengembalikan teks hasil.
    } // akhir fungsi repeatText()

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // Sketsa bangun digambar di terminal, yang hanya bisa menampilkan karakter. Triknya: karakter Braille Unicode (U+2800 s.d. U+28FF). Setiap karakter Braille mewakili 8 titik
    // yang bisa dinyalakan satu per satu, tersusun 2 kolom x 4 baris. Jadi satu sel terminal memberi resolusi 2 x 4 titik, delapan kali lebih halus daripada satu karakter blok.
    // struct yang kita buat sendiri boleh berisi variabel (anggota data) dan juga fungsi (anggota fungsi) yang bekerja pada isinya; keduanya dipakai lewat tanda titik, misalnya canvas.toLines() di printSketch() di bawah.
    struct BrailleCanvas { // BrailleCanvas { adalah awal "kanvas gambar" kita; blok ini berakhir pada "};  // akhir struct BrailleCanvas" sebelum printSketch().
        int cols, rows;                 // cols, rows adalah ukuran kanvas dalam sel terminal (kolom, baris); dua variabel int dalam satu baris, dipisah koma.
        std::vector<unsigned char> layers; // layers adalah daftar satu byte untuk tiap titik: 0 = kosong, lebih dari 0 = nomor lapisan gambar (menentukan warna); titik disimpan baris demi baris, dari kiri atas.

        // Constructor adalah fungsi bernama sama dengan struct, dijalankan saat kanvas dibuat. Bagian setelah : (daftar inisialisasi) mengisi anggota data sebelum isi { } dijalankan.
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {} // BrailleCanvas(int k, int row) : cols(k), rows(row), layers(...) {} artinya cols = k, rows = row,
        // dan layers diisi (2k) x (4 row) titik bernilai 0; static_cast<std::size_t>(2 * k) mengubah int ke std::size_t, tipe ukuran yang diminta vector. Isi { } kosong karena semuanya sudah diisi.
        // const setelah () berfungsi menandai fungsi anggota yang tidak mengubah isi struct.
        int pixelWidth()  const { return 2 * cols; }    // pixelWidth()  const { return 2 * cols; } adalah lebar kanvas dalam titik: 2 titik per sel ke samping.
        int pixelHeight() const { return 4 * rows; } // pixelHeight() const { return 4 * rows; } adalah tinggi kanvas dalam titik: 4 titik per sel ke bawah.

        void setPixel(int x, int y, unsigned char layer) { // setPixel(int x, int y, unsigned char layer) berfungsi untuk menyalakan satu titik di (x, y) dengan nomor lapisan layer; lapisan bernomor lebih besar
        // menimpa yang kecil, supaya garis lingkaran tetap terlihat di atas arsiran.
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight() artinya titik di luar kanvas: abaikan (return tanpa nilai, karena fungsinya void).
            // unsigned char& adalah referensi ke satu byte di dalam layers; mengubah ref berarti mengubah layers langsung.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x]; // layers[static_cast<std::size_t>(y) * pixelWidth() + x] adalah byte titik (x, y); indeksnya y * lebar + x, karena titik disimpan baris demi baris.
            if (layer > ref) ref = layer; // layer > ref artinya lapisan baru lebih tinggi dari yang tersimpan, jadi ditimpa.
        } // akhir fungsi setPixel()

        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const { // cellString(int cx, int cy, const std::vector<const char*>& layerColors) const berfungsi untuk mengubah
        // 8 titik di satu sel (kolom cx, baris cy) menjadi satu karakter Braille berwarna sesuai lapisan tertingginya; layerColors adalah daftar kode warna, indeksnya nomor lapisan; const di akhir:
        // fungsi ini hanya membaca kanvas. Dipakai toLines() di bawah dan printSketch() untuk baris tengah.
            // static const pada array berfungsi untuk membuatnya sekali saja dan tidak pernah berubah. Array 2 dimensi [baris][kolom] adalah daftar berisi daftar.
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} }; // bit[4][2] = { {0x01, 0x08}, ... } adalah peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40;
            // kolom kanan 0x08, 0x10, 0x20, 0x80. Urutan ganjil ini mengikuti standar Braille Unicode (titik 7-8 ditambahkan belakangan di baris ke-4).
            unsigned char value = 0, maxLayer = 0;  // value = 0, maxLayer = 0 artinya value = gabungan bit titik menyala, dan maxLayer = lapisan tertinggi; keduanya mula-mula 0.
            for (int dy = 0; dy < 4; ++dy) { // for (int dy = 0; dy < 4; ++dy) artinya periksa 4 baris titik (dy) di dalam sel ini.
                for (int dx = 0; dx < 2; ++dx) { // for (int dx = 0; dx < 2; ++dx) artinya periksa 2 kolom titik (dx); dua for bersarang = 8 titik.
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx]; // layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx] adalah nomor lapisan titik ini:
                    // posisi titik di kanvas adalah baris cy * 4 + dy, kolom cx * 2 + dx; indeks = baris * lebar + kolom.
                    // |= berfungsi untuk menambahkan bit (OR per bit) ke nilai yang sudah ada.
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); } // if (layer) artinya titik menyala (bukan 0): bit titik itu ditambahkan ke value, dan std::max menyimpan lapisan tertinggi.
                } // akhir perulangan kolom titik
            } // akhir perulangan baris titik
            if (!value) return " ";  // !value artinya tidak ada titik menyala: sel kosong = spasi biasa.
            const unsigned codePoint = 0x2800u + value; // 0x2800u + value adalah kode karakter Braille = U+2800 + pola bit; akhiran u pada 0x2800u berarti bilangan unsigned.
            std::string s; // s adalah tempat menyusun 3 byte UTF-8 karakter Braille, mula-mula kosong.
            // >> di antara dua bilangan berfungsi untuk menggeser bit ke kanan (kebalikan <<); di sini dipakai untuk memindahkan bit-bit atas code point ke posisi paling kanan agar bisa diambil.
            // Code point 0x2800-0x28FF ditulis dalam UTF-8 sebagai tiga byte berpola 1110xxxx 10xxxxxx 10xxxxxx; & 0x3F menyisakan 6 bit terbawah, dan | menempelkan awalan byte 0xE0 atau 0x80. static_cast<char> menjadikannya satu byte char.
            s += static_cast<char>(0xE0 | (codePoint >> 12)); // static_cast<char>(0xE0 | (codePoint >> 12)) adalah byte pertama: awalan 1110 ditambah 4 bit tertinggi code point.
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)); // static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)) adalah byte kedua: awalan 10 ditambah 6 bit tengah.
            s += static_cast<char>(0x80 | (codePoint & 0x3F)); // static_cast<char>(0x80 | (codePoint & 0x3F)) adalah byte ketiga: awalan 10 ditambah 6 bit terbawah.
            return colorize(layerColors[maxLayer], s);  // colorize(layerColors[maxLayer], s) artinya karakter Braille diwarnai sesuai lapisan tertinggi di sel ini.
        } // akhir fungsi cellString()

        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const { // toLines(const std::vector<const char*>& layerColors) const berfungsi untuk mengubah seluruh kanvas menjadi
        // baris-baris teks siap cetak, satu std::string per baris sel. Dipakai printSketch() di bawah.
            std::vector<std::string> lines; // lines adalah daftar baris hasil, mula-mula kosong.
            for (int cy = 0; cy < rows; ++cy) {  // for (int cy = 0; cy < rows; ++cy) artinya untuk tiap baris sel dari atas ke bawah.
                std::string row; // row adalah teks satu baris sel, mula-mula kosong.
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);  // for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors); artinya tiap sel dari kiri ke kanan disambung ke row.
                lines.push_back(row); // lines.push_back(row); menambahkan baris ini ke akhir lines.
            } // akhir perulangan baris sel
            return lines; // return lines; mengembalikan semua baris.
        } // akhir fungsi toLines()
    };  // akhir struct BrailleCanvas (tanda ; wajib setelah } penutup struct)

    void printSketch() { // printSketch() berfungsi untuk menggambar sketsa yang tampil sebelum pertanyaan pertama: persegi dengan lingkaran di dalamnya. Lingkaran dihitung dari persamaannya (titik yang
    // berjarak R dari pusat), jadi hasilnya benar-benar bulat; daerah di luar lingkaran diarsir garis diagonal merah seperti di buku cetak, dan garis jari-jari R dibubuhkan di baris tengah. Dipanggil run() di BAGIAN 6.
        const int available = terminalWidth() - 8; // terminalWidth() - 8 adalah lebar yang tersedia untuk gambar: lebar terminal dikurangi 8 kolom untuk tepi kotak, margin, dan indentasi.
        int canvasCols = std::max(20, std::min(48, available - 3)); // std::max(20, std::min(48, available - 3)) artinya lebar kanvas = available - 3 (2 = garis tepi │ kiri dan kanan persegi, 1 = cadangan supaya baris panah ◄──►  S tidak melipat di terminal 46 kolom), dijepit antara 20 dan 48 sel.
        // % berfungsi untuk menghitung sisa bagi; canvasCols % 2 adalah sisa pembagian dengan 2 (0 bila genap, 1 bila ganjil).
        if (canvasCols % 2 != 0) --canvasCols;  // canvasCols % 2 != 0 artinya lebar ganjil, jadi dikurangi 1 (--canvasCols) agar pusatnya tepat di tengah.
        const int canvasRows = canvasCols / 2; // canvasCols / 2 adalah jumlah baris = setengah kolom, supaya bidangnya persegi (ingat: 2 x 4 titik per sel).
        const std::vector<std::string> legend = { // legend = { adalah daftar baris keterangan simbol; std::vector<std::string> bisa diisi langsung lewat { ... }.
            "R = jari-jari lingkaran = S / 2", // "R = jari-jari lingkaran = S / 2", adalah keterangan 1.
            "S = sisi persegi = 2 x R (lingkaran menyinggung sisi)", // "S = sisi persegi = 2 x R (lingkaran menyinggung sisi)", adalah keterangan 2.
            colorize(ANSI_HATCH, "merah") + " : daerah yang diarsir (persegi - lingkaran)", // colorize(ANSI_HATCH, "merah") + " : daerah yang diarsir (persegi - lingkaran)", adalah keterangan 3; kata "merah" dicetak merah.
            colorize(ANSI_TITLE, "cyan") + "  : garis lingkaran", // colorize(ANSI_TITLE, "cyan") + "  : garis lingkaran", adalah keterangan 4; kata "cyan" dicetak cyan tebal.
            "Luas Arsir = (S x S) - (Pi x R x R)", // "Luas Arsir = (S x S) - (Pi x R x R)", adalah keterangan 5, rumusnya.
        }; // akhir daftar legend
        std::size_t longestLegend = 0;  // longestLegend = 0 adalah lebar keterangan terpanjang (dalam kolom), mula-mula 0.
        for (const std::string& k : legend) longestLegend = std::max(longestLegend, displayWidth(k)); // for (const std::string& k : legend) ... artinya periksa tiap keterangan k; std::max menyimpan lebar terbesar.
        const bool legendOnRight = (3 + canvasCols + 2 + 3 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5); // (3 + canvasCols + 2 + 3 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5)
        // artinya keterangan ditaruh di samping gambar hanya bila baris terlebar (3 = indentasi, 2 = tepi persegi, 3 = jarak ke keterangan) masih muat di dalam kotak (lebar terminal - 5);
        // kalau tidak, drawBox() (BAGIAN 3, dipanggil di akhir fungsi ini) akan membungkus baris itu dan gambarnya rusak. static_cast<std::size_t> menyamakan tipe kedua sisi perbandingan, supaya compiler tidak memberi peringatan.

        BrailleCanvas canvas(canvasCols, canvasRows);  // BrailleCanvas canvas(canvasCols, canvasRows); membuat kanvas kosong; ini memanggil constructor di struct di atas.
        const double cx = canvas.pixelWidth() / 2.0, cy = canvas.pixelHeight() / 2.0;  // cx = canvas.pixelWidth() / 2.0, cy = canvas.pixelHeight() / 2.0 adalah pusat kanvas dalam satuan titik; tanda titik (.) memanggil anggota canvas.
        const double radiusPx  = std::min(cx, cy) - 0.5;  // std::min(cx, cy) - 0.5 adalah jari-jari lingkaran dalam titik; - 0.5 agar lingkaran menyinggung sisi persegi.
        for (int y = 0; y < canvas.pixelHeight(); ++y) { // for (int y = 0; y < canvas.pixelHeight(); ++y) artinya kunjungi setiap baris titik y dari atas ke bawah.
            for (int x = 0; x < canvas.pixelWidth(); ++x) { // for (int x = 0; x < canvas.pixelWidth(); ++x) artinya kunjungi setiap kolom titik x; dua for bersarang = semua titik (x, y) kanvas.
                const double dist = std::hypot(x + 0.5 - cx, y + 0.5 - cy);  // std::hypot(x + 0.5 - cx, y + 0.5 - cy) adalah jarak titik ke pusat: akar(a² + b²); + 0.5 = tengah titik.
                if (std::fabs(dist - radiusPx) <= 0.9)          canvas.setPixel(x, y, 2);  // std::fabs(dist - radiusPx) <= 0.9 artinya jarak hampir sama dengan R (selisih <= 0.9, std::fabs = nilai mutlak): garis lingkaran, lapisan 2.
                else if (dist > radiusPx && (x + y) % 6 == 0)   canvas.setPixel(x, y, 1);  // else if (dist > radiusPx && (x + y) % 6 == 0) artinya titik di luar lingkaran dengan (x + y) kelipatan 6: arsiran diagonal, lapisan 1,
                // karena x + y tetap sepanjang satu garis miring.
            } // akhir perulangan kolom titik
        } // akhir perulangan baris titik
        const std::vector<const char*> layerColors = { "", ANSI_HATCH, ANSI_TITLE }; // layerColors = { "", ANSI_HATCH, ANSI_TITLE } adalah warna per lapisan: indeks 0 (kosong) = "", 1 (arsiran) = merah, 2 (garis lingkaran) = cyan tebal.
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // canvas.toLines(layerColors) mengubah kanvas menjadi baris-baris teks, disimpan di figureLines.

        // Kurung kurawal { } di bawah berdiri sendiri, tanpa if atau for di depannya: variabel yang dibuat di dalamnya hilang setelah } penutup, jadi tidak bercampur dengan sisa fungsi.
        { // { adalah awal blok garis jari-jari "●──R──►" yang ditimpakan pada baris tengah, dari pusat ke tepi kanan lingkaran.
            const int r = canvasRows / 2, centerCol = canvasCols / 2;  // r = canvasRows / 2, centerCol = canvasCols / 2 artinya r = baris tengah, centerCol = kolom pusat.
            std::string radiusLine = "\u25cf";  // radiusLine = "\u25cf" adalah awal garis jari-jari: ● (U+25CF) pusat lingkaran.
            for (int c = centerCol + 1; c < canvasCols - 1; ++c) radiusLine += (c == (centerCol + canvasCols - 1) / 2) ? "R" : "\u2500"; // for (int c = centerCol + 1; c < canvasCols - 1; ++c) artinya dari kolom setelah pusat
            // sampai sebelum tepi kanan: huruf "R" tepat di tengah ruas (dipilih ternary), selain itu garis ─ ("\u2500").
            radiusLine += "\u25ba";  // radiusLine += "\u25ba" menambahkan ► (U+25BA), ujung jari-jari.
            std::string row; // row adalah teks baris tengah yang baru, mula-mula kosong.
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, r, layerColors); // for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, r, layerColors); artinya separuh kiri baris tengah tetap gambar Braille.
            figureLines[r] = row + colorize(ANSI_HINT, radiusLine); // figureLines[r] = row + colorize(ANSI_HINT, radiusLine); artinya separuh kanan baris tengah diganti garis jari-jari kuning.
        }  // akhir blok garis jari-jari

        std::vector<std::string> sideNotes(canvasRows, ""); // sideNotes(canvasRows, "") adalah daftar berisi canvasRows teks kosong, satu per baris gambar; tempat keterangan di samping kanan.
        if (legendOnRight) { // if (legendOnRight) artinya jendela cukup lebar: keterangan ditaruh di samping kanan gambar.
            const int positions[5] = { canvasRows / 8, canvasRows / 8 + 2, canvasRows / 8 + 5, canvasRows / 8 + 6, canvasRows / 2 }; // positions[5] = { ... } adalah lima nomor baris tempat tiap keterangan diletakkan,
            // dihitung dari tinggi kanvas: 1/8, 1/8 + 2, 1/8 + 5, 1/8 + 6, dan baris tengah; urutannya sejajar dengan isi legend di atas.
            for (int i = 0; i < 5; ++i) if (positions[i] < canvasRows) sideNotes[positions[i]] = "   " + legend[i]; // for (int i = 0; i < 5; ++i) if (positions[i] < canvasRows) ... artinya tiap keterangan ditaruh di barisnya,
            // hanya bila nomor baris masih di dalam kanvas; "   " = 3 spasi jarak ke keterangan.
        } // akhir pemeriksaan keterangan di samping

        std::vector<std::string> lines; // lines adalah semua baris kotak yang akan disusun: panah sisi S, tepi atas persegi, baris-baris gambar, tepi bawah, lalu keterangan di bawah bila perlu.
        lines.push_back("   \u25c4" + repeatText("\u2500", canvasCols - 2) + "\u25ba  S"); // "   \u25c4" + repeatText("\u2500", canvasCols - 2) + "\u25ba  S" adalah panah ◄────► bertanda S (◄ = U+25C4, ► = U+25BA): panjang sisi persegi.
        lines.push_back("   " + colorize(ANSI_DIM, "\u250c" + repeatText("\u2500", canvasCols) + "\u2510")); // "\u250c" + repeatText("\u2500", canvasCols) + "\u2510" adalah sisi atas persegi: ┌ + garis ─ selebar kanvas + ┐, abu-abu.
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + colorize(ANSI_DIM, "\u2502") + figureLines[r] + colorize(ANSI_DIM, "\u2502") + sideNotes[r]); // for (int r = 0; r < canvasRows; ++r) ... artinya tiap baris gambar
        // diapit │ ("\u2502") kiri dan kanan, lalu keterangan samping (kosong bila tidak ada).
        lines.push_back("   " + colorize(ANSI_DIM, "\u2514" + repeatText("\u2500", canvasCols) + "\u2518")); // "\u2514" + repeatText("\u2500", canvasCols) + "\u2518" adalah sisi bawah persegi: └ + garis ─ + ┘.
        if (!legendOnRight) for (const std::string& k : legend) lines.push_back("   " + k);  // if (!legendOnRight) artinya jendela sempit: tiap keterangan k ditaruh di bawah gambar, menjorok 3 spasi.
        drawBox("Sketsa Bangun yang Diarsir", lines); // drawBox("Sketsa Bangun yang Diarsir", lines) menggambar semuanya di dalam kotak berjudul itu.
        std::cout << '\n';  // std::cout << '\n'; mencetak baris kosong setelah kotak.
    } // akhir fungsi printSketch()

    void printInstructions() { // printInstructions() berfungsi untuk menampilkan kotak petunjuk sebelum pertanyaan pertama, supaya pengguna tahu bentuk input yang diterima: pilihan, angka lebih dari 0,
    // dan cara menulis desimal. Dipanggil run() di BAGIAN 6.
        drawBox("Petunjuk Pengisian", { // drawBox("Petunjuk Pengisian", { artinya gambar kotak berjudul itu; argumen kedua { ... } langsung menjadi std::vector<std::string> untuk parameter lines.
            "- Satuan Panjang   : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)", // "- Satuan Panjang   : ..." adalah petunjuk satuan; polanya "Label : isi", jadi drawBox() menyejajarkan lanjutannya.
            "                     (contoh: Pilih [2] untuk satuan \"cm\")", // "                     (contoh: Pilih [2] untuk satuan \"cm\")" adalah contoh satuan; \" di dalam string = tanda kutip yang ikut dicetak.
            "- Nilai Pi (\u03c0)     : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila sisi persegi kelipatan 14", // "- Nilai Pi (\u03c0)     : ..." adalah petunjuk nilai π; \u03c0 = simbol π.
            "                     (contoh: Pilih [2] untuk nilai \"22/7\")", // "                     (contoh: Pilih [2] untuk nilai \"22/7\")" adalah contoh nilai π.
            "- Sisi Persegi     : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma", // "- Sisi Persegi     : ..." adalah petunjuk sisi persegi.
            "                     (contoh: 14, 14,5 atau 14.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)", // "                     (contoh: 14, 14,5 atau 14.5; ...)" adalah contoh cara menulis angka.
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang." // "Ketik jawaban lalu tekan Enter ..." adalah baris penutup petunjuk.
        }); // }); menutup daftar isi kotak dan pemanggilan drawBox().
        std::cout << '\n';  // std::cout << '\n'; mencetak baris kosong setelah kotak.
    } // akhir fungsi printInstructions()

    std::string formatResultLine(std::string_view label, double value, std::string_view unit) { // formatResultLine(std::string_view label, double value, std::string_view unit) berfungsi untuk membuat satu baris hasil
    // "Label : nilai satuan" dengan 2 desimal, siap dimasukkan ke kotak. Dipanggil run() di BAGIAN 6 untuk tiap baris angka hasil.
        // std::ostringstream adalah "aliran keluaran ke teks": menerima << seperti std::cout, tetapi hasilnya ditampung sebagai teks, jadi pengaturan format std::cout sendiri tidak ikut berubah.
        std::ostringstream line; // line adalah aliran teks tempat baris hasil disusun.
        // std::left berfungsi untuk rata kiri; std::setw(n) mengatur lebar kolom berikutnya n karakter (sisanya diisi spasi); std::fixed memaksa desimal biasa, bukan notasi ilmiah;
        // std::setprecision(n) bersama std::fixed mengatur tepat n angka di belakang koma.
        line << std::left << std::setw(25) << label << ": " << std::fixed << std::setprecision(2) << value << ' ' << unit; // std::left << std::setw(25) << label << ": " << std::fixed << std::setprecision(2) << value << ' ' << unit
        // artinya label rata kiri selebar 25 karakter, lalu ": ", lalu value dengan tepat 2 desimal, spasi, dan satuan.
        return line.str();  // line.str() mengambil teks yang sudah tersusun di dalam aliran, lalu dikembalikan.
    } // akhir fungsi formatResultLine()

}  // akhir namespace tanpa nama: semua di atas hanya terlihat di berkas ini

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 3 =====
namespace task3 { // namespace task3 { adalah wadah bernama task3: fungsi di dalamnya dipanggil dengan awalan task3::, jadi program bermenu (src/main.cpp) bisa memanggil task3::run()
// tanpa tertukar dengan run() milik tugas lain. Blok ini berakhir pada "}  // akhir namespace task3".

    void run() { // run() berfungsi sebagai fungsi utama tugas ini. Alurnya: judul, baris rumus, sketsa, petunjuk, dua pertanyaan pilihan, input sisi, hitung, lalu kotak hasil; pekerjaan kecilnya diserahkan ke
    // fungsi-fungsi bantu di BAGIAN 3-5. Dipanggil oleh main() di BAGIAN 7 atau oleh program bermenu.
        enableUtf8Console();  // enableUtf8Console(); menyiapkan konsol Windows untuk UTF-8, supaya program mandiri (tanpa menu) juga benar; aman dipanggil berulang.
        printTitle("PROGRAM 3 : LUAS BANGUN YANG DIARSIR (PERSEGI - LINGKARAN)"); // printTitle("PROGRAM 3 : LUAS BANGUN YANG DIARSIR (PERSEGI - LINGKARAN)") mencetak judul program di antara dua garis.
        // Baris rumus dilipat per kata dengan wrapText() (BAGIAN 3) bila terminalnya sempit; lanjutannya menjorok 8 spasi, lurus dengan isi setelah "Rumus : ".
        for (const std::string& piece : wrapText("Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2))", static_cast<std::size_t>(terminalWidth() - 1), 8)) { // wrapText("Rumus : ...", ..., 8) berfungsi untuk memotong
        // baris rumus sesuai lebar terminal dikurangi 1 kolom cadangan; 8 adalah indentasi gantung.
            std::cout << piece << '\n'; // std::cout << piece << '\n' berfungsi untuk mencetak satu potongan rumus, lalu pindah baris.
        } // akhir perulangan baris rumus
        std::cout << "        Pi = \u03c0 (modul menulisnya phi)\n\n"; // "        Pi = \u03c0 (modul menulisnya phi)\n\n" adalah baris kedua rumus; \u03c0 = simbol π, \n\n = pindah baris lalu satu baris kosong.
        printSketch();    // printSketch(); menampilkan sketsa bangun, supaya pengguna tahu mana yang dimaksud S dan R.
        printInstructions();  // printInstructions(); menampilkan kotak petunjuk pengisian.

        // --- Tahap pengaturan: satuan dan nilai π ---
        const char* unit = chooseUnit();          // chooseUnit() menanyakan satuan; unit adalah simbol satuan yang dipilih, misalnya "cm".
        std::string piLabel;                    // piLabel adalah teks "3.14" atau "22/7 (3.142857)" untuk ditampilkan di hasil; diisi oleh choosePi() di baris berikutnya.
        const double pi = choosePi(piLabel);     // choosePi(piLabel) menanyakan nilai π; hasilnya nilai π untuk perhitungan, dan piLabel diisi lewat referensi.

        // --- Tahap input ---
        const double side = readPositiveDecimal(std::string("Masukkan Panjang Sisi Persegi (") + unit + ") : "); // readPositiveDecimal(std::string("Masukkan Panjang Sisi Persegi (") + unit + ") : ") menanyakan sisi persegi;
        // teks pertanyaan disambung dengan simbol satuan yang dipilih. Disimpan sebagai double karena boleh desimal; readPositiveDecimal() (BAGIAN 4) menjamin nilainya lebih dari 0.

        // --- Tahap perhitungan --- (const: nilainya tidak diubah lagi setelah dihitung)
        const double radius      = side / 2.0;  // radius      = side / 2.0 adalah jari-jari: lingkaran menyinggung sisi, jadi r = s / 2.
        const double squareArea   = side * side;                  // squareArea   = side * side adalah luas persegi = s x s.
        const double circleArea = pi * radius * radius;     // circleArea = pi * radius * radius adalah luas lingkaran = π x r x r.
        const double shadedArea     = squareArea - circleArea;  // shadedArea     = squareArea - circleArea adalah luas arsir = persegi - lingkaran.

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit) + "\u00b2";  // std::string(unit) + "\u00b2" adalah satuan luas, misalnya cm²; \u00b2 = simbol pangkat dua (²).
        std::vector<std::string> resultLines;  // resultLines adalah daftar baris hasil, mula-mula kosong.
        resultLines.push_back("Pi (\u03c0) yang Digunakan    : " + piLabel); // "Pi (\u03c0) yang Digunakan    : " + piLabel adalah baris pertama hasil: nilai π yang dipakai.
        resultLines.push_back(formatResultLine("Sisi Persegi (S)", side, unit)); // formatResultLine("Sisi Persegi (S)", side, unit) adalah baris sisi persegi dengan 2 desimal.
        resultLines.push_back(formatResultLine("Jari-Jari Lingkaran (R)", radius, unit) + "  (R = S / 2, lingkaran menyinggung sisi)"); // formatResultLine("Jari-Jari Lingkaran (R)", radius, unit) + "  (R = S / 2, ...)" adalah baris jari-jari beserta keterangannya.
        resultLines.push_back(formatResultLine("Luas Persegi", squareArea, areaUnit)); // formatResultLine("Luas Persegi", squareArea, areaUnit) adalah baris luas persegi; satuannya luas, misalnya cm².
        resultLines.push_back(formatResultLine("Luas Lingkaran", circleArea, areaUnit)); // formatResultLine("Luas Lingkaran", circleArea, areaUnit) adalah baris luas lingkaran.
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Luas Bangun yang Diarsir", shadedArea, areaUnit))); // colorize(ANSI_SUCCESS, formatResultLine("Luas Bangun yang Diarsir", shadedArea, areaUnit)) adalah hasil akhir, ditonjolkan hijau tebal.
        std::cout << '\n'; // std::cout << '\n'; mencetak baris kosong sebelum kotak hasil.
        drawBox("Hasil Perhitungan", resultLines); // drawBox("Hasil Perhitungan", resultLines) menggambar semua baris hasil di dalam kotak "Hasil Perhitungan".
    } // akhir fungsi run()

}  // akhir namespace task3

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), dan program bermenu (src/main.cpp) sudah punya. Jadi main() di bawah hanya ikut bila berkas ini dikompilasi sendiri.
#ifndef BUILD_WITH_MENU // BUILD_WITH_MENU dicek: baris sampai #endif hanya ikut dikompilasi bila penanda itu BELUM didefinisikan, yaitu saat berkas ini dikompilasi sendiri tanpa opsi -DBUILD_WITH_MENU.
// int main() adalah titik awal program: fungsi yang pertama kali dijalankan saat program dibuka. Standar C++ mewajibkan main bertipe int, yaitu bilangan bulat
// yang dikirim ke sistem operasi saat program selesai; nilai 0 (lihat return 0 di bawah) menandakan program selesai tanpa error.
int main() { // main() adalah fungsi utama saat berkas ini menjadi program sendiri (tugas3).
    // Baris identitas dilipat per kata dengan wrapText() (BAGIAN 3), supaya di terminal sempit tidak terpotong di tengah kata.
    for (const std::string& piece : wrapText("Tugas Praktikum 3 - Fadhlillah (2531070011)", static_cast<std::size_t>(terminalWidth() - 1), 0)) { // wrapText("Tugas Praktikum 3 - Fadhlillah (2531070011)", ..., 0) berfungsi untuk memotong identitas sesuai lebar terminal dikurangi 1 kolom cadangan, tanpa indentasi gantung (0).
        std::cout << colorize(ANSI_BOLD, piece) << '\n'; // colorize(ANSI_BOLD, piece) berfungsi untuk mencetak potongan identitas dengan huruf tebal, lalu pindah baris.
    } // akhir perulangan baris identitas
    task3::run();  // task3::run(); menjalankan seluruh alur tugas 3 (BAGIAN 6).
    return 0;  // return 0; mengakhiri main(); 0 berarti selesai tanpa error.
} // akhir main()
#endif  // akhir bagian main() untuk kompilasi mandiri
