// ===== tugas2_volume_tabung.cpp — Program 2 : Volume Tabung =====
// Program ini menghitung volume tabung dari jari-jari (atau diameter) alas dan tingginya, lengkap dengan sketsa tabungnya.
// Ini Tugas Praktikum 2 dari Modul 1: Input dan Output.
// Alurnya: tampilkan judul, rumus, sketsa, dan petunjuk pengisian. Lalu pengguna memilih satuan panjang (mm/cm/dm/m),
// nilai π (3.14 sesuai modul, atau 22/7 untuk soal dengan kelipatan 7), dan ukuran yang diketahui (jari-jari atau diameter).
// Setelah itu pengguna mengisi ukuran dan tinggi. Program menampilkan luas alas, volume V = π x R x R x T,
// dan padanannya dalam liter. Modul menulis konstanta ini "phi", padahal yang dimaksud adalah π (pi).
// Berkas ini lengkap: bisa dikompilasi sendiri, dan bisa juga digabung ke program bermenu (main.cpp di folder yang sama).
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Cara mengompilasi berkas ini sendiri (jalankan dari folder proyek):
// 1. Buka Terminal VS Code lalu copy paste command di bawah ini :
//   g++ -std=c++17 -Wall -Wextra -o tugas2 src/tugas2_volume_tabung.cpp && ./tugas2
//   (Windows PowerShell: .\tugas2.exe).
// Arti tiap bagian perintah itu:
//   g++                           compiler C++, yaitu pengubah kode menjadi program yang bisa dijalankan
//   -std=c++17                    memakai aturan bahasa C++17
//   -Wall -Wextra                 menampilkan peringatan tambahan, supaya kesalahan kecil cepat ketahuan
//   -o tugas2                     nama program hasil kompilasi
//   src/tugas2_volume_tabung.cpp  berkas yang dikompilasi, yaitu berkas ini saja
//   && ./tugas2                   menjalankan program hasilnya, tetapi hanya bila kompilasi berhasil
// Bila digabung ke program bermenu, perintahnya mengompilasi semua berkas src/*.cpp sekaligus dan memakai -DBUILD_WITH_MENU.
// Kenapa perlu -DBUILD_WITH_MENU? Setiap berkas tugas punya main() sendiri. Opsi -D berfungsi untuk memasang penanda saat kompilasi.
// Bila penanda itu ada, main() di bagian paling bawah berkas ini (BAGIAN 7) tidak ikut dikompilasi, jadi yang dipakai hanya main() milik main.cpp.
//
// 2. Cara lain di VS Code : buka berkas ini di editor, lalu buka menu Terminal > Run Task... dan pilih task
// "Build & Run berkas yang sedang dibuka (tugas sendiri)". Task adalah perintah siap pakai; yang satu ini mengompilasi
// berkas yang sedang dibuka sebagai program sendiri lalu menjalankannya. Daftar task ada di berkas .vscode/tasks.json.

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Sebelum menulis program, kita "meminjam" dulu alat-alat yang sudah disediakan C++.
// Tulisan setelah // sampai akhir baris adalah komentar. Compiler mengabaikannya; komentar hanya untuk manusia.

// #include berfungsi untuk menyalin isi sebuah header ke berkas ini sebelum dikompilasi. Header adalah berkas berisi daftar fungsi dan tipe data milik sebuah pustaka; tanda <...> berarti header bawaan compiler atau sistem operasi.
#include <algorithm> // <algorithm> digunakan untuk std::max dan std::min, yaitu alat memilih nilai terbesar atau terkecil dari dua nilai; dipakai untuk membatasi lebar kotak dan ukuran sketsa.
#include <cctype> // <cctype> digunakan untuk std::isdigit, yang memeriksa apakah satu karakter adalah angka 0-9 (dipakai di BAGIAN 4).
#include <cmath> // <cmath> digunakan untuk std::cos dan std::sin, yang menghitung titik-titik elips saat menggambar sketsa tabung (BAGIAN 5 lanjutan).
#include <cstddef> // <cstddef> digunakan untuk std::size_t, yaitu tipe bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan posisi, misalnya lebar teks.
#include <cstdlib> // <cstdlib> digunakan untuk alat serbaguna: std::exit dan EXIT_FAILURE (menghentikan program dengan kode gagal), std::getenv (membaca variabel lingkungan),
// dan std::atoi (mengubah teks seperti "100" menjadi bilangan bulat 100).
#include <iomanip> // <iomanip> digunakan untuk std::setw dan std::setprecision, yaitu pengatur rupa angka saat dicetak (lebar kolom dan jumlah angka di belakang titik);
// keduanya dipakai bersama std::left dan std::fixed di formatResultLine() (BAGIAN 5 lanjutan).
#include <initializer_list> // <initializer_list> digunakan untuk std::initializer_list, yaitu daftar nilai yang ditulis langsung di dalam { ... }; dipakai printOptions() (BAGIAN 5).
#include <iostream> // <iostream> digunakan untuk mengimpor pustaka input dan output standar: std::cin (membaca ketikan dari keyboard), std::cout (mencetak ke layar),
// dan std::cerr (saluran khusus untuk pesan kesalahan).
#include <sstream> // <sstream> digunakan untuk std::istringstream (membaca angka dari teks, BAGIAN 4) dan std::ostringstream (menyusun teks berformat, BAGIAN 5 lanjutan).
#include <string> // <string> digunakan untuk tipe std::string, yaitu teks yang panjangnya bisa berubah, serta std::getline (membaca satu baris ketikan utuh) dan std::to_string.
#include <string_view> // <string_view> digunakan untuk tipe std::string_view, yaitu "jendela" untuk membaca sebuah teks tanpa menyalinnya;
// dipakai untuk prompt dan judul yang hanya dibaca, karena menyalin teks yang tidak diubah cuma membuang waktu dan memori.
#include <vector> // <vector> digunakan untuk tipe std::vector, yaitu daftar berurutan yang bisa bertambah panjang; dipakai untuk baris-baris isi kotak dan titik-titik sketsa.

// #ifdef berfungsi untuk bertanya ke compiler apakah sebuah nama sudah didefinisikan; bila ya, baris sampai #else/#endif ikut dikompilasi, bila tidak, baris itu dilewati.
// Di sini kita perlu bertanya karena program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya berbicara dengan terminal berbeda.
#ifdef _WIN32 // _WIN32 adalah nama yang otomatis ada saat program dikompilasi untuk Windows; artinya baris sampai #else hanya ikut dikompilasi di Windows.
    // #ifndef ("if not defined") berfungsi kebalikan dari #ifdef: baris sampai #endif hanya ikut dikompilasi bila nama itu BELUM didefinisikan.
    #ifndef NOMINMAX // NOMINMAX dicek dulu karena MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan mendefinisikan ulang nama yang sama menghasilkan peringatan.
    // #define berfungsi untuk membuat makro, yaitu nama yang oleh compiler diganti dengan teks lain; mulai baris di bawah, NOMINMAX berarti 1.
    #define NOMINMAX 1 // NOMINMAX 1 artinya "jangan buat min dan max" dinyalakan, karena windows.h punya kebiasaan membuat makro min/max sendiri yang merusak std::min dan std::max.
    // #endif berfungsi untuk menutup blok #ifdef/#ifndef/#else yang terakhir dibuka.
    #endif  // selesai mengecek NOMINMAX
    #include <windows.h> // <windows.h> adalah pustaka resmi Windows; dari sini dipakai fungsi untuk warna (mode virtual terminal), UTF-8 (code page), dan lebar jendela konsol, semuanya di BAGIAN 3.
// #else artinya "selain itu": baris sesudahnya sampai #endif dipakai bila syarat #ifdef tadi tidak terpenuhi.
#else  // #else di sini artinya bila bukan Windows (macOS atau Linux), pakai pustaka di bawah ini.
    #include <cstdio> // <cstdio> digunakan untuk stdout, yaitu "saluran" keluaran ke layar, dan fileno, yang memberi nomor saluran itu.
    #include <sys/ioctl.h> // <sys/ioctl.h> digunakan untuk ioctl dan TIOCGWINSZ, yaitu cara bertanya ke terminal seberapa lebar jendelanya; jawabannya ditampung di struct winsize.
    #include <unistd.h> // <unistd.h> digunakan untuk isatty (memeriksa apakah keluaran benar-benar terminal, bukan berkas) dan STDOUT_FILENO (nomor saluran keluaran layar, yaitu 1).
#endif  // selesai memilih pustaka sesuai sistem operasi

// namespace berfungsi untuk membuat wadah nama, supaya nama yang sama di berkas lain tidak bentrok.
namespace { // namespace { tanpa nama berfungsi untuk membuat isinya hanya terlihat dari berkas ini. Berkas tugas lain dan main.cpp juga punya fungsi bantu bernama sama
// (misalnya colorize() dan drawBox()); tanpa wadah ini, penggabungan ke program bermenu gagal dengan error "multiple definition" (satu nama dibuat dua kali).
// Wadah ini dibuka di sini dan ditutup tepat sebelum BAGIAN 6.

    // ===== BAGIAN 2: KONSTANTA =====
    // Konstanta adalah nilai tetap yang tidak bisa diubah selama program berjalan.
    // constexpr berfungsi untuk membuat konstanta yang nilainya sudah pasti sejak kompilasi; ini pengganti modern #define yang lebih aman karena punya tipe data.
    // double adalah tipe data untuk bilangan desimal (bilangan berkoma), misalnya 3.14; ukuran tabung boleh desimal, jadi perhitungannya memakai double.
    constexpr double PI_MODULE   = 3.14; // PI_MODULE   = 3.14 adalah nilai π bawaan sesuai modul (pilihan [1] di choosePi(), BAGIAN 5). Modul menulisnya "phi", padahal yang dimaksud
    // adalah π (pi): keliling lingkaran dibagi diameternya. φ (phi) sendiri adalah rasio emas 1,618, bukan konstanta ini.
    // / adalah pembagian. Bila kedua bilangannya bulat (misalnya 22 / 7), C++ membuang sisanya, jadi hasilnya ikut bulat.
    constexpr double PI_FRACTION = 22.0 / 7.0; // PI_FRACTION = 22.0 / 7.0 adalah π versi 22/7 (sekitar 3.142857), dipakai soal bila jari-jari atau diameter kelipatan 7 (pilihan [2]);
    // ditulis 22.0 / 7.0, bukan 22 / 7, karena 22 / 7 di C++ hasilnya 3.
    // int adalah tipe bilangan bulat biasa (tanpa koma) yang boleh bernilai negatif.
    constexpr int    TITLE_WIDTH = 60; // TITLE_WIDTH = 60 adalah lebar garis judul dalam karakter; dipakai printRule() di BAGIAN 3.
    constexpr int    LABEL_WIDTH = 24; // LABEL_WIDTH = 24 adalah lebar kolom label pada pertanyaan pilihan, supaya semua opsi sejajar; dipakai printOptions() di BAGIAN 5.
    constexpr double DIMENSION_MAX = 1000000.0; // DIMENSION_MAX = 1000000.0 adalah batas atas ukuran (satu juta); angka yang lebih besar hampir pasti salah ketik, jadi ditolak readPositiveDecimal() di BAGIAN 4.
    // const artinya "tetap": sesuatu yang ditandai const tidak boleh diubah setelah dibuat.
    // char adalah tipe untuk satu karakter (satu byte), misalnya 'A'.
    // const char* adalah penunjuk (alamat di memori) ke teks tetap yang hurufnya tidak boleh diubah; ini cara menyimpan teks gaya bahasa C.
    constexpr const char* ANSI_SURFACE = "\033[36m"; // ANSI_SURFACE = "\033[36m" adalah kode warna ANSI cyan biasa (36) untuk arsiran permukaan tutup tabung pada sketsa.
    // Kode ANSI tidak tercetak; terminal membacanya sebagai perintah ganti warna. Penjelasan lengkapnya ada di BAGIAN 3.

    // struct berfungsi untuk membuat tipe data buatan sendiri yang mengelompokkan beberapa data di bawah satu nama; datanya disebut anggota dan dibaca dengan titik, misalnya unit.symbol di chooseUnit() (BAGIAN 5).
    struct LengthUnit { // LengthUnit { adalah tipe satuan panjang. Nama satuan dan faktor konversinya ke liter selalu dipakai berpasangan, jadi kita kemas menjadi satu.
        const char* symbol;   // symbol adalah simbol satuan panjang, misalnya "cm"; ditampilkan di prompt dan di baris hasil.
        double      litersPerCubicUnit; // litersPerCubicUnit adalah isi 1 satuan kubik dalam liter; dasarnya 1 L = 1 dm³ (satuan SI turunan).
    };  // }; adalah akhir struct LengthUnit; titik koma setelah } penutup struct wajib ditulis.
    // [] di belakang nama membuat array, yaitu deretan data bertipe sama yang diakses dengan nomor urut (indeks) mulai dari 0; kurung siku kosong artinya compiler sendiri yang menghitung jumlah isinya.
    constexpr LengthUnit UNITS[] = { {"mm", 0.000001}, {"cm", 0.001}, {"dm", 1.0}, {"m", 1000.0} }; // UNITS[] = { ... } adalah daftar satuan yang lazim di soal, urutannya sama dengan nomor pilihan 1-4, jadi UNITS[0] = mm dan UNITS[3] = m.
    // Setiap {"cm", 0.001} mengisi symbol lalu litersPerCubicUnit, sesuai urutan. Karena 1 dm³ = 1 L, maka 1 cm³ = 0,001 L (= 1 mL),
    // 1 mm³ = 0,000001 L, dan 1 m³ = 1000 L.
    constexpr int    UNIT_COUNT   = 4; // UNIT_COUNT   = 4 adalah banyaknya satuan di UNITS; dipakai sebagai nomor pilihan terbesar di chooseUnit() (BAGIAN 5).
    constexpr int    DEFAULT_UNIT_INDEX   = 2; // DEFAULT_UNIT_INDEX   = 2 adalah pilihan bawaan bila pengguna hanya menekan Enter: 2 = cm, karena hampir semua soal tabung memakai cm.
    // Hati-hati, ini nomor pilihan (mulai 1), bukan indeks array, jadi chooseUnit() mengambil UNITS[2 - 1].

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, pilihan yang ditegaskan, hasil akhir, dan pesan kesalahan mudah dibedakan, kita beri warna dengan kode warna ANSI.
    // Kode ANSI adalah teks khusus yang tidak tercetak; terminal membacanya sebagai perintah ganti gaya. Bentuknya "\033[" + angka gaya + huruf m.
    // "\033" adalah karakter ESC (nomor 27, ditulis dalam oktal: 3 x 8 + 3 = 27). Kode ANSI dipakai karena terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // ANSI_RESET  = "\033[0m" adalah kode untuk mengembalikan tampilan ke normal (tanpa warna/tebal); angka 0 artinya reset.
    constexpr const char* ANSI_BOLD  = "\033[1m";     // ANSI_BOLD  = "\033[1m" artinya tebal (angka 1); dipakai untuk prompt (teks pertanyaan yang menunggu ketikan) dan label pertanyaan pilihan.
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // ANSI_TITLE  = "\033[1;36m" artinya tebal (1) sekaligus cyan (36); dipakai untuk judul program, judul kotak, garis sketsa, dan tulisan TABUNG.
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // ANSI_SUCCESS  = "\033[1;32m" artinya tebal dan hijau (32); dipakai untuk pilihan yang ditegaskan dan hasil akhir (Volume Tabung).
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // ANSI_ERROR  = "\033[1;31m" artinya tebal dan merah (31); dipakai untuk pesan kesalahan.
    constexpr const char* ANSI_HINT = "\033[33m"; // ANSI_HINT = "\033[33m" artinya kuning (33); dipakai untuk keterangan [Enter = ...], garis jari-jari R, panah T, dan keterangan di samping sketsa.
    constexpr const char* ANSI_DIM    = "\033[90m"; // ANSI_DIM    = "\033[90m" artinya abu-abu (90, hitam terang); dipakai untuk garis kotak, garis pembatas, tepi alas putus-putus, dan keterangannya.

    // void berfungsi menandai fungsi yang tidak mengembalikan nilai apa pun; fungsi seperti ini dipanggil untuk efeknya saja. Isi fungsi ditulis di antara { dan }.
    void enableUtf8Console() { // enableUtf8Console() berfungsi untuk mengganti konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama; dipanggil di awal run() (BAGIAN 6).
    // UTF-8 adalah aturan menyimpan setiap huruf dan simbol sebagai 1 sampai 4 byte (byte = satuan data 8 bit, bernilai 0 sampai 255). Konsol Windows bawaan
    // (Command Prompt, PowerShell) masih memakai code page lama, misalnya 437 (code page adalah tabel aturan byte mana menjadi huruf apa), sehingga π, ², ³,
    // garis kotak, dan sketsa Braille tampil sebagai huruf acak. Pengaturan ini sama dengan perintah chcp 65001 di berkas peluncur jalankan.bat (folder proyek).
    // Di macOS/Linux terminal sudah UTF-8, jadi di sana isi fungsi ini kosong.
    #ifdef _WIN32  // #ifdef _WIN32 di sini artinya dua baris di bawah hanya ikut dikompilasi di Windows, karena SetConsoleOutputCP dan SetConsoleCP hanya ada di windows.h.
        SetConsoleOutputCP(CP_UTF8); // SetConsoleOutputCP(CP_UTF8) berfungsi untuk mengatur keluaran konsol ke UTF-8, agar byte UTF-8 tampil sebagai karakter yang benar; CP_UTF8 adalah konstanta Windows bernilai 65001.
        SetConsoleCP(CP_UTF8); // SetConsoleCP(CP_UTF8) berfungsi untuk mengatur masukan (ketikan) konsol ke UTF-8 juga, agar huruf non-ASCII seperti é atau π terbaca sama dengan keluarannya.
        // ASCII adalah kumpulan 128 karakter dasar: A-Z dan a-z tanpa aksen, angka 0-9, tanda baca, spasi, dan karakter kendali seperti pindah baris.
    #endif  // akhir bagian khusus Windows
    } // akhir fungsi enableUtf8Console()

    // bool adalah tipe data yang hanya punya dua nilai: true (benar) atau false (salah).
    bool colorsEnabled() { // colorsEnabled() berfungsi untuk menentukan boleh memakai warna atau tidak; hasil true berarti boleh berwarna.
    // Aturannya mengikuti kebiasaan program terminal: bila pengguna menyetel NO_COLOR, warna mati; bila FORCE_COLOR, warna dipaksa hidup; selain itu warna hanya dipakai
    // bila keluaran benar-benar terminal, bukan berkas. Di Windows, fungsi ini sekaligus menyalakan mode "virtual terminal" agar Command Prompt memahami kode ANSI.
        // static berfungsi untuk membuat variabel yang dibuat sekali saja (saat fungsi pertama kali dipanggil) lalu nilainya diingat sampai program selesai; jadi pemeriksaan di bawah cukup sekali.
        // lambda []() { ... } adalah fungsi kecil tanpa nama yang ditulis langsung di tempat; di sini ia menghitung nilai awal enabled, karena pemeriksaannya butuh beberapa langkah if.
        static const bool enabled = []() { // enabled = []() { artinya nilai awal enabled diambil dari hasil lambda ini; const membuat nilainya tidak bisa diubah lagi.
            // if berfungsi untuk menjalankan perintah hanya bila syarat di dalam kurung bernilai benar.
            // std:: adalah awalan untuk nama yang berasal dari pustaka standar C++; tanda :: berarti "milik", jadi std::getenv adalah getenv milik pustaka standar.
            // != artinya "tidak sama dengan".
            // nullptr adalah penunjuk kosong, yaitu penunjuk yang tidak menunjuk ke mana pun.
            // return berfungsi untuk mengakhiri fungsi (di sini lambda) saat itu juga dan memberikan hasilnya.
            if (std::getenv("NO_COLOR") != nullptr) return false; // std::getenv("NO_COLOR") berfungsi untuk membaca variabel lingkungan NO_COLOR, yaitu pengaturan bernama yang diwariskan terminal ke program;
            // hasil nullptr berarti variabel itu tidak ada. Bila NO_COLOR ada, pengguna minta tanpa warna, jadi hasilnya false.
            if (std::getenv("FORCE_COLOR") != nullptr) return true; // std::getenv("FORCE_COLOR") != nullptr artinya pengguna memaksa warna, jadi hasilnya true tanpa memeriksa terminal.
        #ifdef _WIN32  // #ifdef _WIN32 di sini artinya cara memeriksa terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
            // HANDLE adalah tipe Windows untuk "pegangan", yaitu tanda pengenal sebuah saluran (di sini layar konsol) yang dipakai saat memanggil fungsi Windows lain.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); // GetStdHandle(STD_OUTPUT_HANDLE) berfungsi untuk mengambil pegangan ke keluaran layar (konsol), lalu disimpan di output.
            // DWORD adalah tipe bilangan bulat tanpa tanda negatif milik Windows (32 bit).
            DWORD  mode     = 0; // mode     = 0 adalah tempat menyimpan mode konsol yang sedang aktif; nilainya diisi GetConsoleMode di baris berikutnya.
            // == artinya "sama dengan" (membandingkan), berbeda dengan = yang mengisi nilai.
            // || artinya "atau": hasilnya benar bila salah satu syarat benar.
            // ! artinya "tidak": membalik benar menjadi salah dan sebaliknya.
            // & di depan nama variabel berfungsi untuk mengambil alamatnya; &mode adalah alamat variabel mode, supaya GetConsoleMode bisa mengisi variabel itu.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false; // output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode) artinya pegangannya tidak sah, atau keluaran bukan konsol (misalnya dialihkan
            // ke berkas) sehingga mode tidak bisa dibaca; maka hasilnya false: tanpa warna.
            // | (OR per bit) berfungsi untuk menambahkan sebuah bendera ke nilai lama tanpa menghapus bendera yang sudah ada.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0; // SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) berfungsi untuk menyalakan mode virtual terminal agar konsol memahami kode ANSI;
            // SetConsoleMode mengembalikan 0 bila gagal, jadi != 0 berarti berhasil dan warna aktif.
        #else  // #else di sini artinya bukan Windows (macOS atau Linux); cukup satu pertanyaan: apakah keluaran layar sebuah terminal?
            return isatty(fileno(stdout)) != 0; // isatty(fileno(stdout)) != 0 artinya fileno(stdout) memberi nomor saluran keluaran layar, lalu isatty memeriksa apakah saluran itu terminal (bukan berkas);
            // hasil selain 0 berarti terminal, jadi warna dipakai.
        #endif  // akhir pemeriksaan terminal sesuai sistem operasi
        }();  // }(); adalah akhir lambda; tanda () langsung menjalankan lambda satu kali, dan hasilnya menjadi nilai awal enabled.
        return enabled;  // return enabled; artinya kembalikan hasil yang tersimpan, jadi pemanggilan berikutnya tidak memeriksa ulang.
    } // akhir fungsi colorsEnabled()

    // std::string adalah tipe teks milik pustaka standar yang panjangnya bisa berubah; di sini menjadi tipe hasil fungsi colorize().
    // std::string_view adalah "jendela baca" ke sebuah teks tanpa menyalinnya; cocok untuk teks yang hanya dibaca.
    std::string colorize(const char* code, std::string_view text) { // colorize(const char* code, std::string_view text) berfungsi untuk membungkus text dengan kode warna, tetapi hanya bila warna boleh dipakai.
    // Parameter (masukan fungsi, ditulis di dalam kurung) code diisi salah satu konstanta ANSI_... di atas, sedangkan text dibaca lewat std::string_view
    // tanpa disalin. Hasilnya std::string baru, siap dicetak.
        if (!colorsEnabled()) return std::string(text);  // !colorsEnabled() artinya warna tidak boleh dipakai, jadi text dikembalikan apa adanya; std::string(text) membuat salinannya sebagai std::string.
        // + pada teks berfungsi untuk menyambung teks; code dan text diubah dulu ke std::string supaya bisa disambung.
        return std::string(code) + std::string(text) + ANSI_RESET; // std::string(code) + std::string(text) + ANSI_RESET artinya kode warna, lalu teks, lalu ANSI_RESET agar warna tidak terbawa ke teks berikutnya.
    } // akhir fungsi colorize()

    // unsigned adalah bilangan bulat tanpa tanda negatif (0 ke atas).
    bool isWideCodePoint(unsigned codePoint) { // isWideCodePoint(unsigned codePoint) berfungsi untuk memeriksa apakah sebuah code point termasuk karakter lebar, yaitu karakter yang digambar terminal selebar dua kolom.
    // Code point adalah nomor unik sebuah karakter di Unicode, daftar baku semua karakter di dunia (misalnya π bernomor 0x3C0). Aksara CJK (Cina, Jepang, Korea),
    // Hangul (huruf Korea), dan emoji termasuk lebar; bila ini diabaikan, lebar kotak dan pemotongan baris jadi salah. Dipakai di displayWidth() di bawah.
        // && artinya "dan": hasilnya benar hanya bila kedua syarat benar.
        // >= artinya "lebih dari atau sama dengan", dan <= artinya "kurang dari atau sama dengan"; tanpa tanda = (> dan <) artinya "lebih dari" dan "kurang dari".
        // 0x di depan angka artinya angka itu ditulis dalam heksadesimal (basis 16), cara lazim menulis nomor karakter. Setiap pasangan (>= awal && <= akhir) adalah satu rentang karakter lebar.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF) // (codePoint >= 0x1100 && codePoint <= 0x115F) adalah Hangul Jamo (huruf penyusun Hangul); 0x2E80-0xA4CF adalah CJK dan aksara Asia Timur lain.
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF) // || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) adalah suku kata Hangul; 0xF900-0xFAFF adalah aksara CJK kompatibilitas.
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60) // || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) adalah bentuk kompatibilitas CJK; 0xFF00-0xFF60 adalah huruf lebar penuh.
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F) // || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) adalah tanda lebar penuh (misalnya ¥ versi lebar penuh); 0x1F300-0x1F64F adalah emoji.
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); // || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); adalah emoji tambahan. Hasilnya true bila code point masuk salah satu rentang di atas.
    } // akhir fungsi isWideCodePoint()

    // std::size_t adalah tipe bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan panjang (berasal dari <cstddef>).
    std::size_t displayWidth(std::string_view text) { // displayWidth(std::string_view text) berfungsi untuk menghitung berapa kolom layar yang dipakai text; hasilnya tidak selalu sama dengan jumlah byte-nya.
    // UTF-8 menyimpan satu karakter dalam 1 sampai 4 byte, padahal π, ², ³, garis kotak, dan karakter Braille tetap selebar satu kolom; aksara lebar (CJK, emoji)
    // memakai dua kolom, dan kode warna ANSI memakai nol kolom. Karena itu teks dihitung byte demi byte.
        std::size_t width = 0;  // width = 0 adalah jumlah kolom yang sudah terhitung, mula-mula 0.
        bool inEscape = false;  // inEscape = false artinya belum berada di dalam kode warna "\033[...m"; nilainya true selama berada di dalamnya.
        unsigned codePoint = 0; // codePoint = 0 adalah code point yang sedang dirangkai dari byte-byte UTF-8.
        int pending = 0;        // pending = 0 adalah jumlah byte lanjutan UTF-8 yang masih ditunggu untuk melengkapi code point.
        // for (elemen : kumpulan) adalah perulangan "untuk setiap" (range-based for): mengambil setiap elemen satu per satu dari awal sampai akhir, tanpa perlu indeks.
        // unsigned char adalah satu byte bernilai 0-255.
        for (const unsigned char c : text) { // for (const unsigned char c : text) artinya ambil setiap byte di text satu per satu ke variabel c; const berarti c tidak diubah di dalam perulangan.
            // continue berfungsi untuk langsung lanjut ke putaran berikutnya tanpa menjalankan baris di bawahnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; } // if (inEscape) artinya sedang di dalam kode warna: byte dilewati (nol kolom) sampai huruf 'm' penutupnya; 'm' bertanda petik tunggal adalah satu karakter.
            if (c == 0x1B) { inEscape = true; continue; }  // c == 0x1B artinya byte ini karakter ESC (nomor 27, sama dengan "\033"), pembuka kode warna.
            // & di antara dua nilai adalah AND per bit: berfungsi untuk mengambil bit tertentu saja dari sebuah byte. Satu byte = 8 bit, masing-masing 0 atau 1.
            if ((c & 0xC0) == 0x80) { // (c & 0xC0) == 0x80 artinya byte ini byte lanjutan UTF-8, yang selalu berpola 10xxxxxx: & 0xC0 mengambil 2 bit teratas, dan hasil 0x80 berarti "10".
                // << berfungsi untuk menggeser bit ke kiri. -- berfungsi untuk mengurangi nilai variabel sebanyak satu, dan ++ untuk menambahnya sebanyak satu.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; } // (codePoint << 6) | (c & 0x3F) artinya geser code point 6 bit ke kiri, lalu tempelkan 6 bit data byte ini. Bila --pending menjadi 0, code point sudah lengkap;
                // karakter lebar lalu mendapat satu kolom tambahan (++width), karena kolom pertamanya sudah dihitung di byte awal.
                continue; // continue; artinya byte lanjutan bukan karakter baru, jadi tidak menambah kolom.
            } // akhir penanganan byte lanjutan
            ++width;                                       // ++width; artinya setiap byte awal = satu karakter = minimal satu kolom.
            // Pola bit byte awal menentukan berapa byte lanjutan yang menyusul; bit x milik byte awal disimpan sebagai awal code point.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; } // (c & 0xE0) == 0xC0 artinya byte awal berpola 110xxxxx: menyusul 1 byte lanjutan, dan 5 bit data (c & 0x1F) disimpan.
            // else if berfungsi untuk memeriksa syarat berikutnya hanya bila syarat sebelumnya salah; else terakhir dijalankan bila semua syarat salah.
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; } // (c & 0xF0) == 0xE0 artinya pola 1110xxxx: menyusul 2 byte lanjutan, dan 4 bit data (c & 0x0F) disimpan.
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; } // (c & 0xF8) == 0xF0 artinya pola 11110xxx: menyusul 3 byte lanjutan, dan 3 bit data (c & 0x07) disimpan.
            else                         { pending = 0; } // else { pending = 0; } artinya byte ASCII biasa (pola 0xxxxxxx) yang tidak punya byte lanjutan.
        }  // akhir perulangan setiap byte
        return width; // return width; artinya kembalikan jumlah kolom yang terhitung.
    } // akhir fungsi displayWidth()

    int terminalWidth() { // terminalWidth() berfungsi untuk menanyakan lebar jendela terminal (dalam kolom) setiap kali dipanggil, jadi tampilan ikut menyesuaikan bila jendela diubah ukurannya.
    // Bila terminal tidak bisa ditanya (keluaran bukan terminal), dicoba variabel lingkungan COLUMNS; bila itu juga tidak ada, dipakai 80 kolom, ukuran bawaan
    // terminal pada umumnya. Hasilnya int dan tidak pernah di bawah 50, karena di bawah 50 kolom kotak dan sketsa tidak bisa rapi.
        int cols = 0;  // cols = 0 artinya lebar belum diketahui.
    #ifdef _WIN32  // #ifdef _WIN32 di sini artinya cara bertanya ke terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows untuk menampung keadaan konsol; tanda titik (.) berfungsi untuk mengambil salah satu datanya.
        CONSOLE_SCREEN_BUFFER_INFO info; // CONSOLE_SCREEN_BUFFER_INFO info; adalah tempat menampung keterangan jendela konsol; bagian srWindow menyimpan nomor kolom kiri (Left) dan kanan (Right).
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1; // GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info) berfungsi untuk mengisi info lewat alamatnya (&info) dan bernilai bukan-0 bila berhasil;
        // GetStdHandle memberi pegangan ke layar, sama seperti di colorsEnabled() di atas. Lebar jendela = Right - Left + 1.
    #else  // #else di sini artinya bukan Windows (macOS atau Linux).
        // struct di depan winsize menandakan bahwa winsize adalah sebuah struct; winsize berasal dari <sys/ioctl.h>.
        struct winsize ws; // struct winsize ws; adalah tempat menampung ukuran jendela; bagian ws_col berisi jumlah kolom.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col; // ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) berfungsi untuk meminta ukuran jendela dari saluran keluaran layar (STDOUT_FILENO) dan mengisinya ke ws lewat
        // alamatnya (&ws); hasil 0 berarti berhasil, lalu cols diisi jumlah kolom.
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // std::atoi berfungsi untuk mengubah teks angka, misalnya "100", menjadi bilangan int 100.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); } // cols <= 0 artinya lebar masih belum diketahui, jadi dicoba variabel lingkungan COLUMNS; env menunjuk ke teksnya, atau bernilai nullptr bila COLUMNS tidak ada.
        if (cols <= 0) cols = 80;  // cols = 80 adalah lebar bawaan bila lebar tetap belum diketahui.
        // std::max berfungsi untuk memilih yang lebih besar dari dua nilai (berasal dari <algorithm>).
        return std::max(50, cols);  // std::max(50, cols) artinya hasilnya tidak pernah di bawah 50 kolom.
    } // akhir fungsi terminalWidth()

    // std::vector<std::string> adalah daftar berurutan berisi teks; di sini menjadi tipe hasil fungsi, satu elemen untuk satu baris.
    // const std::string& adalah referensi tetap (const&): & membuat fungsi memakai teks aslinya tanpa menyalin, dan const membuat teks itu tidak boleh diubah.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) { // wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) berfungsi untuk memotong text di spasi menjadi beberapa baris
    // yang masing-masing muat dalam maxWidth kolom, supaya terminal tidak memotongnya sembarangan. Baris lanjutan diberi indentasi gantung sebanyak
    // hangingIndent spasi, agar sejajar dengan isi setelah label. Dipakai drawBox(), printError(), dan printOptions() di bawah.
        std::vector<std::string> result;  // result adalah daftar baris hasil, mula-mula kosong.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2; // hangingIndent + 10 > maxWidth artinya jendela terlalu sempit untuk indentasi gantung + 10 kolom isi, jadi indentasi dikecilkan menjadi 2 spasi.
        std::string remaining = text;  // remaining adalah sisa teks yang belum dipotong; disalin dari text karena isinya akan diganti di setiap putaran.
        // while berfungsi untuk mengulang isi blok selama syarat di dalam kurung masih benar.
        while (displayWidth(remaining) > maxWidth) {  // displayWidth(remaining) > maxWidth artinya sisa teks masih lebih lebar dari maxWidth, jadi potong satu baris lagi dari depannya.
            std::size_t col = 0, cutPos = std::string::npos; // col menghitung kolom layar, dan cutPos menyimpan posisi spasi tempat memotong; std::string::npos adalah nilai khusus yang berarti "tidak ditemukan" (belum ada spasi).
            bool inEscape = false;  // inEscape = false: nilainya true selama berada di dalam kode warna, yang tidak memakai kolom.
            // for (awal; syarat; langkah) adalah perulangan for biasa: mulai dari awal, berulang selama syarat benar, dan menjalankan langkah setiap selesai satu putaran.
            for (std::size_t i = 0; i < remaining.size(); ++i) { // for (std::size_t i = 0; i < remaining.size(); ++i) artinya periksa setiap byte remaining dari posisi 0 sampai terakhir untuk mencari spasi terakhir
            // sebelum batas lebar; remaining.size() adalah jumlah byte.
                // static_cast<...> berfungsi untuk mengubah tipe sebuah nilai secara tertulis, sehingga perubahan itu jelas terlihat di kode.
                const unsigned char c = static_cast<unsigned char>(remaining[i]); // static_cast<unsigned char>(remaining[i]) berfungsi untuk mengubah byte ke-i (dihitung dari 0) menjadi unsigned char 0-255, karena byte UTF-8 di atas 127
                // bisa terbaca sebagai angka negatif pada char biasa.
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // if (inEscape) artinya lewati isi kode warna sampai huruf 'm'.
                if (c == 0x1B) { inEscape = true; continue; }  // c == 0x1B artinya ESC, awal kode warna.
                if ((c & 0xC0) == 0x80) continue; // (c & 0xC0) == 0x80 artinya byte lanjutan UTF-8; ia bukan karakter baru, jadi tidak menambah kolom (penjelasannya di displayWidth() di atas).
                // break berfungsi untuk keluar dari perulangan saat itu juga.
                if (col >= maxWidth) break;  // col >= maxWidth artinya batas lebar tercapai, jadi berhenti mencari.
                if (c == ' ' && col > hangingIndent) cutPos = i; // c == ' ' && col > hangingIndent artinya spasi ini menjadi calon titik potong, asalkan letaknya setelah indentasi gantung; yang akhirnya tersimpan
                // adalah spasi terakhir sebelum batas lebar.
                ++col; // ++col; artinya karakter ini memakai satu kolom.
            } // akhir perulangan mencari titik potong
            if (cutPos == std::string::npos) break;        // cutPos == std::string::npos artinya tidak ada spasi untuk memotong: keluar dari while dan biarkan sisa teks apa adanya.
            result.push_back(remaining.substr(0, cutPos)); // remaining.substr(0, cutPos) adalah teks dari awal sampai sebelum spasi; push_back berfungsi untuk menambahkannya ke ujung result.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1); // std::string(hangingIndent, ' ') adalah hangingIndent buah spasi, disambung dengan sisa teks mulai setelah spasi (cutPos + 1).
        }  // akhir perulangan while
        result.push_back(remaining);  // result.push_back(remaining); artinya sisa terakhir sudah muat, jadi ia menjadi baris terakhir.
        return result; // return result; artinya kembalikan daftar baris hasil.
    } // akhir fungsi wrapText()

    void drawBox(std::string_view title, const std::vector<std::string>& lines) { // drawBox(std::string_view title, const std::vector<std::string>& lines) berfungsi untuk menggambar kotak berjudul title berisi baris-baris lines.
    // Kotak membuat informasi (sketsa, petunjuk, hasil) terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah melebihi
    // lebar terminal; baris yang terlalu panjang dibungkus dengan wrapText(), dan baris berpola "Label : isi" dibuat sejajar dengan isi setelah label.
    // Garis kotak ditulis sebagai kode Unicode di dalam string: "\u250c" = ┌, "\u2510" = ┐, "\u2514" = └, "\u2518" = ┘, "\u2500" = ─ (garis datar),
    // dan "\u2502" = │ (garis tegak).
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5); // terminalWidth() - 5 adalah lebar isi terbesar: lebar terminal dikurangi 2 tepi kotak, 2 margin, dan 1 kolom cadangan; kolom cadangan perlu karena
        // sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela. static_cast<std::size_t> mengubah hasil int menjadi std::size_t.
        std::vector<std::string> contentLines;  // contentLines adalah baris isi setelah dibungkus, siap digambar.
        for (const std::string& b : lines) {  // for (const std::string& b : lines) artinya ambil setiap baris b di lines tanpa menyalinnya, lalu tentukan indentasi gantungnya dan bungkus dengan wrapText().
            std::size_t hangingIndent = 2;  // hangingIndent = 2 adalah bawaan: baris lanjutan menjorok 2 spasi.
            const std::size_t labelPos = b.find(" : ");  // b.find(" : ") berfungsi untuk mencari posisi " : " pertama; hasilnya std::string::npos bila tidak ada.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) { // labelPos != std::string::npos && ... <= 28 artinya ada " : " dan label di depannya paling lebar 28 kolom, jadi baris ini berpola "Label : isi".
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // displayWidth(b.substr(0, labelPos)) + 3 artinya lebar label ditambah 3 untuk " : ", jadi lanjutannya sejajar dengan isi setelah " : ".
            } // akhir pemeriksaan pola "Label : isi"
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece); // wrapText(b, maxWidth, hangingIndent) memotong b, lalu setiap potongan (piece) dimasukkan ke contentLines.
        } // akhir perulangan membungkus baris isi
        std::size_t width = displayWidth(title) + 4;  // displayWidth(title) + 4 adalah lebar dalam minimal: "─ " sebelum judul (2), spasi setelah judul (1), dan paling sedikit satu ─ pengisi (1).
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2); // std::max(width, displayWidth(b) + 2) artinya kotak diperlebar sampai baris terpanjang + 2, yaitu margin kiri dan kanan.
        // std::min berfungsi untuk memilih yang lebih kecil dari dua nilai (berasal dari <algorithm>).
        width = std::min(width, maxWidth + 2);  // std::min(width, maxWidth + 2) artinya lebar kotak tidak boleh lebih dari maxWidth + 2 margin.
        std::string filler; // filler adalah garis pengisi di kanan judul, mula-mula kosong.
        // += berfungsi untuk menambahkan teks ke ujung teks yang sudah ada.
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // filler += "\u2500" artinya tambahkan ─ sebanyak sisa lebar setelah "─ judul ".
        // std::cout << berfungsi untuk mencetak ke layar; beberapa << berurutan mencetak bagian-bagiannya satu per satu dari kiri ke kanan.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n'; // colorize(ANSI_DIM, "\u250c\u2500 ") ... adalah garis atas ┌─ judul ───┐: garisnya abu-abu dan judulnya cyan tebal; '\n' adalah karakter pindah baris.
        for (const std::string& b : contentLines) { // for (const std::string& b : contentLines) artinya cetak setiap baris isi: │, teks, spasi pengisi sampai lebar kotak, lalu │ lagi.
            const std::string rowText = " " + b; // " " + b artinya satu spasi margin kiri di depan baris.
            const std::size_t contentWidth = displayWidth(rowText); // contentWidth adalah lebar rowText di layar.
            // Operator ternary (a ? b : c) artinya "bila a benar pakai b, bila tidak pakai c". Di baris ini ia menjaga jumlah spasi pengisi paling sedikit 0,
            // karena std::size_t tidak bisa negatif: pengurangan yang hasilnya di bawah 0 akan berubah menjadi angka raksasa.
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ') // width > contentWidth ? width - contentWidth : 0 adalah jumlah spasi pengisi.
                      << colorize(ANSI_DIM, "\u2502") << '\n'; // colorize(ANSI_DIM, "\u2502") adalah │ garis tegak kanan, lalu '\n' pindah baris.
        } // akhir perulangan baris isi
        std::string bottomLine; // bottomLine adalah garis bawah, mula-mula kosong.
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500"; // bottomLine += "\u2500" artinya tambahkan ─ sebanyak lebar kotak.
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n'; // "\u2514" + bottomLine + "\u2518" adalah garis bawah └───┘ berwarna abu-abu.
    } // akhir fungsi drawBox()

    void printError(const std::string& text) { // printError(const std::string& text) berfungsi untuk mencetak pesan kesalahan berwarna merah yang dibungkus sesuai lebar terminal, agar pesan panjang
    // tidak terpotong jendela sempit. Pesannya selalu diawali "  [!] ", jadi baris lanjutan menjorok 6 spasi agar sejajar dengan teks setelah tanda "[!]".
    // Dipakai readPositiveDecimal() dan readChoice() di BAGIAN 4.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) { // wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6) artinya lebar bungkus = lebar terminal dikurangi 1 kolom cadangan, dan 6 = indentasi gantung.
            std::cout << colorize(ANSI_ERROR, b) << '\n';  // colorize(ANSI_ERROR, b) artinya setiap baris hasil bungkus dicetak merah tebal.
        } // akhir perulangan baris pesan
    } // akhir fungsi printError()

    void printRule(char ch) { // printRule(char ch) berfungsi untuk mencetak satu garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter, berisi karakter ch yang diulang;
    // dipakai printTitle() di BAGIAN 5.
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n'; // std::string(TITLE_WIDTH, ch) adalah teks berisi ch yang diulang TITLE_WIDTH kali; colorize(ANSI_DIM, ...) membuatnya abu-abu, lalu '\n' pindah baris.
    } // akhir fungsi printRule()

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Di bagian ini kita membaca ketikan pengguna, lalu memeriksa apakah ketikan itu sah. Validasinya sengaja ketat:
    // ketikan yang salah tidak boleh diam-diam diterima, karena hasil hitungnya akan ikut salah tanpa disadari.

    std::string readLine(std::string_view prompt) { // readLine(std::string_view prompt) berfungsi untuk menampilkan pertanyaan (prompt) dengan huruf tebal, lalu membaca satu baris ketikan.
    // Hasilnya std::string berisi ketikan pengguna, tanpa tombol Enter-nya. Semua masukan program ini dibaca lewat fungsi ini.
        std::string line;                   // line adalah tempat ketikan; std::string karena panjang ketikan tidak pasti.
        std::cout << colorize(ANSI_BOLD, prompt); // colorize(ANSI_BOLD, prompt) artinya pertanyaan dicetak tebal supaya beda dari keterangan; tanpa '\n', jadi ketikan muncul di baris yang sama.
        // std::getline berfungsi untuk membaca satu baris ketikan utuh sampai Enter, termasuk spasinya. Dipakai, bukan std::cin >>, karena seluruh baris perlu diperiksa:
        // cin >> berhenti di spasi atau di karakter yang tidak cocok (misalnya 7.5abc terbaca 7.5), dan sisa ketikannya tertinggal lalu ikut terbaca oleh pertanyaan berikutnya.
        if (!std::getline(std::cin, line)) { // !std::getline(std::cin, line) artinya pembacaan dari std::cin (masukan keyboard) gagal karena input sudah ditutup, jadi tidak ada ketikan lagi:
        // pengguna menekan Ctrl+D (macOS/Linux) atau Ctrl+Z lalu Enter (Windows), atau input dialihkan dari berkas dan isinya sudah habis.
            // std::cerr berfungsi untuk mencetak pesan galat lewat saluran khusus yang terpisah dari std::cout.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n"; // "\nInput berakhir sebelum data lengkap. Program dihentikan.\n" adalah pesan bahwa input habis sebelum semua data terisi; "\n" di awal artinya pindah baris dulu.
            // std::exit berfungsi untuk menghentikan seluruh program saat itu juga, dari fungsi mana pun.
            std::exit(EXIT_FAILURE); // std::exit(EXIT_FAILURE) artinya hentikan program dengan kode keluar EXIT_FAILURE ("gagal"), karena tanpa data lengkap perhitungan tidak bisa dilanjutkan.
        } // akhir penanganan input yang berakhir
        return line;  // return line; artinya serahkan baris yang dibaca ke pemanggil.
    } // akhir fungsi readLine()

    std::string normalizeSpaces(const std::string& text) { // normalizeSpaces(const std::string& text) berfungsi untuk merapikan spasi: membuang spasi/tab di awal dan akhir, lalu merapatkan spasi beruntun menjadi satu.
    // Contoh: "  7   5 " menjadi "7 5". Parameternya const& karena teks hanya dibaca, jadi tidak perlu disalin.
        std::string result; // result adalah tempat menampung teks yang sudah rapi, mula-mula kosong.
        bool pendingSpace = false;         // pendingSpace = false: nilainya true bila ada spasi yang sudah terlihat tetapi belum ditulis ke result.
        for (const char c : text) {         // for (const char c : text) artinya periksa setiap karakter text satu per satu; const char berarti c hanya dibaca.
            if (c == ' ' || c == '\t' || c == '\r') { // c == ' ' || c == '\t' || c == '\r' artinya c adalah spasi, tab ('\t'), atau CR ('\r', sisa akhir baris Windows); karakter ini tidak langsung ditulis, cukup dicatat.
                pendingSpace = !result.empty();  // !result.empty() artinya hasil sudah berisi sesuatu (empty() bernilai true bila teks kosong). Jadi spasi di awal diabaikan, dan spasi di tengah dicatat.
            } else {  // } else { artinya karakter selain spasi: tulis dulu spasi yang tertunda (bila ada), lalu karakternya.
                if (pendingSpace) result += ' ';  // if (pendingSpace) result += ' ' artinya spasi beruntun ditulis sebagai satu spasi saja.
                result += c;  // result += c; artinya tambahkan c di akhir hasil.
                pendingSpace = false; // pendingSpace = false; artinya tidak ada lagi spasi yang tertunda.
            } // akhir if-else
        } // akhir perulangan setiap karakter
        return result;                       // return result; artinya kembalikan teks yang rapi; spasi di akhir otomatis terbuang karena tidak pernah ditulis.
    } // akhir fungsi normalizeSpaces()

    bool isAllDigits(const std::string& text) { // isAllDigits(const std::string& text) berfungsi untuk memeriksa apakah text hanya berisi angka 0-9; tanda, titik, huruf, atau spasi membuat hasilnya false.
        if (text.empty()) return false;     // text.empty() artinya teks kosong; teks kosong bukan bilangan, jadi hasilnya false.
        for (const char c : text) {         // for (const char c : text) artinya setiap karakter diperiksa; semuanya harus digit.
            // std::isdigit berfungsi untuk memeriksa apakah satu karakter adalah angka 0-9 (berasal dari <cctype>).
            if (!std::isdigit(static_cast<unsigned char>(c))) return false; // !std::isdigit(static_cast<unsigned char>(c)) artinya c bukan digit, jadi langsung false. static_cast<unsigned char> dipakai karena std::isdigit hanya aman
            // untuk nilai 0-255, sedangkan char bisa negatif untuk huruf non-ASCII.
        } // akhir perulangan setiap karakter
        return true;  // return true; artinya semua karakter digit.
    } // akhir fungsi isAllDigits()

    bool isDecimalFormat(const std::string& text) { // isDecimalFormat(const std::string& text) berfungsi untuk memeriksa bentuk bilangan desimal: angka, boleh diikuti SATU titik atau koma, lalu angka lagi
    // (7, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, karena itu bukan cara menulis ukuran yang kita harapkan dari pengguna.
        // auto berfungsi untuk membiarkan compiler menentukan tipe variabel sendiri dari nilainya; di sini tipenya posisi teks (std::size_t).
        const auto separator = text.find_first_of(".,"); // text.find_first_of(".,") berfungsi untuk mencari titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        if (separator == std::string::npos) return isAllDigits(text);  // separator == std::string::npos artinya tanpa pemisah: teks harus digit semua.
        return isAllDigits(text.substr(0, separator))             // isAllDigits(text.substr(0, separator)) artinya teks sebelum pemisah harus digit.
            && isAllDigits(text.substr(separator + 1));           // && isAllDigits(text.substr(separator + 1)) artinya teks sesudah pemisah sampai akhir juga harus digit, jadi pemisah kedua (misalnya 8.7.5) tertolak.
    } // akhir fungsi isDecimalFormat()

    // & setelah nama tipe (misalnya double&) membuat reference, yaitu nama lain untuk variabel milik pemanggil; mengubahnya berarti mengubah variabel aslinya.
    bool parseDecimal(std::string text, double& result) { // parseDecimal(std::string text, double& result) berfungsi untuk mengubah teks menjadi bilangan desimal; hasilnya true bila teksnya sah.
    // text sengaja ditulis tanpa &, jadi yang diterima salinan yang boleh diubah tanpa mengganggu aslinya; angka hasilnya ditulis langsung ke result milik pemanggil.
        if (!isDecimalFormat(text)) return false;  // !isDecimalFormat(text) artinya bentuknya salah, jadi langsung false; hanya tulisan angka yang lolos ke langkah berikutnya.
        for (char& c : text) { // for (char& c : text) artinya c menjadi nama lain untuk setiap karakter di dalam text, jadi mengubah c berarti mengubah text.
            if (c == ',') c = '.';    // c == ',' artinya koma diganti titik, karena C++ hanya mengenal penulisan 7.5.
        } // akhir perulangan mengganti koma
        // std::istringstream berfungsi untuk membaca data dari sebuah teks, mirip std::cin membaca keyboard (berasal dari <sstream>).
        std::istringstream parser(text);  // parser(text) adalah pembaca angka yang sumbernya isi text.
        // >> pada aliran masukan (stream) seperti parser berfungsi untuk membaca nilai ke variabel di kanannya; hasilnya bisa dipakai sebagai benar/salah.
        return (parser >> result) && (parser >> std::ws).eof(); // (parser >> result) && (parser >> std::ws).eof() artinya angka berhasil dibaca ke result, lalu std::ws melewati spasi dan .eof() memastikan sudah sampai
        // akhir teks. Jadi sah hanya bila angkanya terbaca tanpa sisa.
    } // akhir fungsi parseDecimal()

    double readPositiveDecimal(std::string_view prompt) { // readPositiveDecimal(std::string_view prompt) berfungsi untuk meminta bilangan desimal yang lebih besar dari 0 dan paling besar DIMENSION_MAX (1000000, BAGIAN 2).
    // Pertanyaannya diulang sampai jawabannya sah; prompt adalah teks pertanyaan yang ditampilkan.
        // while (true) berfungsi untuk mengulang tanpa henti, karena syaratnya selalu benar; jalan keluarnya hanya lewat return saat input sah.
        while (true) { // while (true) { artinya tanya terus sampai jawabannya sah.
            double value = 0.0; // value = 0.0 adalah tempat angka hasil; double karena ukuran boleh desimal (misalnya 7.5), dan 0.0 hanya nilai awal yang jelas.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) { // parseDecimal(normalizeSpaces(readLine(prompt)), value) artinya baca baris, rapikan spasinya, lalu ubah ke angka. value > 0.0 menolak nol dan negatif,
            // karena ukuran bangun tidak mungkin begitu, dan value <= DIMENSION_MAX menolak angka yang terlalu besar (hampir pasti salah ketik).
                return value; // return value; artinya angka yang sah dikembalikan, dan perulangan selesai.
            } // akhir pemeriksaan angka yang sah
            // "..." "..." (dua teks yang ditulis berurutan) otomatis digabung compiler menjadi satu teks; jadi dua baris di bawah adalah satu pesan.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan" // printError("  [!] Harus angka lebih besar dari 0 ...") artinya ketikan tidak sah: tampilkan pesan merah, lalu while (true) mengulang pertanyaan.
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi."); // " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi." adalah lanjutan pesan yang sama di baris atas.
        }  // akhir perulangan while (true): kembali meminta input
    } // akhir fungsi readPositiveDecimal()

    int readChoice(std::string_view prompt, int maxValue, int defaultValue) { // readChoice(std::string_view prompt, int maxValue, int defaultValue) berfungsi untuk meminta pilihan berupa satu digit dari 1 sampai maxValue.
    // Enter tanpa isi berarti pilihan bawaan (defaultValue), jadi pilihan paling umum (cm, π = 3.14, jari-jari) cukup dijawab dengan Enter.
        while (true) {  // while (true) { artinya diulang, karena pilihan yang salah harus ditanyakan lagi.
            const std::string text = normalizeSpaces(readLine(prompt));  // text = normalizeSpaces(readLine(prompt)) artinya baca satu baris lalu rapikan spasinya; const karena tidak diubah lagi.
            if (text.empty()) return defaultValue;  // text.empty() artinya hanya Enter yang ditekan, jadi pilih bawaan (defaultValue).
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) { // text.size() == 1 && std::isdigit(...) artinya sah bila tepat satu karakter dan karakter itu angka 0-9; text[0] adalah karakter pertama (indeks 0).
                const int choice = text[0] - '0'; // text[0] - '0' berfungsi untuk mengubah karakter digit menjadi angka: kode karakter '0' sampai '9' berurutan ('0' = 48, '7' = 55), jadi '7' - '0' = 7.
                if (choice >= 1 && choice <= maxValue) return choice;  // choice >= 1 && choice <= maxValue artinya nomornya ada di rentang 1 sampai maxValue, jadi sah dan dikembalikan.
            } // akhir pemeriksaan satu digit
            // std::to_string berfungsi untuk mengubah angka menjadi teks, supaya bisa disambung ke teks lain dengan +.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue) // printError("  [!] Pilihan tidak dikenal. ..." + std::to_string(maxValue) artinya pesan merah yang menyebut nomor terbesar, misalnya "1-4".
                                        + ", atau tekan Enter untuk memakai bawaan."); // + ", atau tekan Enter untuk memakai bawaan." adalah lanjutan pesan yang sama; setelah itu while (true) mengulang pertanyaan.
        } // akhir perulangan while (true)
    } // akhir fungsi readChoice()

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====
    // Di bagian ini ada fungsi yang menampilkan pertanyaan pilihan (satuan, nilai π, ukuran yang diketahui) dan judul program.

    // std::initializer_list adalah daftar nilai yang ditulis langsung di dalam kurung kurawal { ... } saat fungsi dipanggil (berasal dari <initializer_list>).
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) { // printOptions(std::string_view label, std::initializer_list<std::string_view> line) berfungsi untuk menampilkan pertanyaan pilihan dengan bentuk yang sama,
    // supaya pengguna langsung mengenali pola "[nomor] pilihan" dan "[Enter = bawaan]". Baris pertama memuat label (tebal) dan opsi pertama, baris berikutnya
    // opsi lain yang disejajarkan di bawahnya, dan baris terakhir nilai bawaan (kuning).
    // Contoh: printOptions("Pilih Nilai Pi", { "[1] ...", "[2] ...", "[Enter = 3.14]" }).
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // terminalWidth() - 1 adalah lebar terminal dikurangi 1 kolom cadangan.
        bool isFirst = true;  // isFirst = true menandai baris pertama, yang memuat label.
        for (const std::string_view b : line) {  // for (const std::string_view b : line) artinya ambil setiap teks opsi b secara berurutan.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter"); // b.substr(0, 6) == "[Enter" artinya 6 karakter pertama b adalah "[Enter", jadi baris ini baris bawaan.
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b); // isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b) artinya baris bawaan dicetak kuning, baris lain apa adanya.
            const std::string start = isFirst // start = isFirst adalah awal baris; isFirst memilih salah satu dari dua bentuk di baris berikut (ternary).
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": " // ? colorize(ANSI_BOLD, ...) + ": " artinya baris pertama: label tebal ditambah spasi sampai selebar LABEL_WIDTH (24), lalu ": ".
                // label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0 adalah jumlah spasi tambahannya, paling sedikit 0.
                : std::string(LABEL_WIDTH + 2, ' '); // : std::string(LABEL_WIDTH + 2, ' ') artinya baris berikutnya diawali 26 spasi (selebar label + ": "), supaya opsinya sejajar.
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n'; // wrapText(start + content, maxWidth, LABEL_WIDTH + 2) berfungsi untuk memotong opsi yang panjang (BAGIAN 3) dengan baris lanjutan menjorok 26 spasi,
            // lalu setiap potongan dicetak.
            isFirst = false;  // isFirst = false; artinya baris berikutnya bukan baris pertama lagi.
        } // akhir perulangan setiap opsi
    } // akhir fungsi printOptions()

    const LengthUnit& chooseUnit() { // chooseUnit() berfungsi untuk menampilkan daftar satuan, membaca pilihan, lalu menegaskan satuan yang terpilih.
    // Hasilnya const LengthUnit&: reference ke salah satu isi UNITS (BAGIAN 2), jadi tidak perlu disalin.
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" }); // printOptions("Pilih Satuan Panjang", { ... }) artinya tampilkan empat satuan dalam dua baris, lalu baris bawaan [Enter = cm].
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX); // readChoice(..., UNIT_COUNT, DEFAULT_UNIT_INDEX) artinya terima nomor 1 sampai UNIT_COUNT (4); Enter saja = DEFAULT_UNIT_INDEX (2 = cm).
        const LengthUnit& unit = UNITS[selection - 1]; // UNITS[selection - 1] artinya ambil satuan terpilih; dikurangi 1 karena nomor pilihan mulai dari 1 sedangkan indeks array mulai dari 0.
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit.symbol) << "\n\n"; // colorize(ANSI_SUCCESS, ...) artinya penegasan hijau tebal, misalnya "  -> Satuan panjang : cm", supaya pilihan pasti terbaca. std::string("...") perlu
        // supaya teks dan unit.symbol bisa disambung dengan +, dan "\n\n" artinya pindah baris dua kali (satu baris kosong).
        return unit; // return unit; artinya kembalikan reference ke satuan terpilih.
    } // akhir fungsi chooseUnit()

    double choosePi(std::string& legend) { // choosePi(std::string& legend) berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    // Hasilnya (double) adalah π untuk perhitungan; teks π untuk tampilan dititipkan lewat legend, reference ke variabel milik pemanggil.
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)", // printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)", adalah awal daftar pilihan π; opsi [1] = 3.14 sesuai modul.
                                      "[2] 22/7  (soal dengan jari-jari atau diameter kelipatan 7)", // "[2] 22/7  (soal dengan jari-jari atau diameter kelipatan 7)", adalah opsi [2] = 22/7.
                                      "[Enter = 3.14]" }); // "[Enter = 3.14]" }); adalah baris bawaan; }); menutup daftar dan pemanggilan printOptions().
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);  // readChoice(..., 2, 1) artinya terima nomor 1-2; Enter saja = 1 (3.14).
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // legend = (selection == 2) ? "22/7 (3.142857)" : "3.14" artinya pilih teks π untuk tampilan sesuai nomor (ternary).
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) artinya penegasan hijau tebal; \u03c0 adalah kode Unicode untuk π.
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // (selection == 2) ? PI_FRACTION : PI_MODULE artinya pilihan 2 = 22/7, selain itu 3.14 (BAGIAN 2).
    } // akhir fungsi choosePi()

    bool chooseDiameterInput() { // chooseDiameterInput() berfungsi untuk menanyakan ukuran mana yang diketahui soal, jari-jari atau diameter, lalu menegaskannya.
    // Hasilnya true bila diameter yang diketahui, false bila jari-jari.
        printOptions("Ukuran yang Diketahui", { "[1] Jari-jari (R)", // printOptions("Ukuran yang Diketahui", { "[1] Jari-jari (R)", adalah awal daftar pilihan; opsi [1] = jari-jari.
                                             "[2] Diameter (D), program menghitung R = D / 2", // "[2] Diameter (D), program menghitung R = D / 2", adalah opsi [2] = diameter; jari-jarinya dihitung program.
                                             "[Enter = Jari-jari]" }); // "[Enter = Jari-jari]" }); adalah baris bawaan; }); menutup daftar dan pemanggilan printOptions().
        const bool diameter = (readChoice("Pilihan Anda            : ", 2, 1) == 2);  // diameter = (readChoice(..., 2, 1) == 2) artinya diameter bernilai true hanya bila pilihan 2; Enter saja = 1 (jari-jari).
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Ukuran         : ") + (diameter ? "Diameter" : "Jari-jari")) << "\n\n"; // colorize(ANSI_SUCCESS, ...) artinya penegasan hijau tebal; ternary (diameter ? "Diameter" : "Jari-jari") memilih teksnya.
        return diameter; // return diameter; artinya kembalikan jawabannya ke pemanggil.
    } // akhir fungsi chooseDiameterInput()

    void printTitle(std::string_view text) { // printTitle(std::string_view text) berfungsi untuk mencetak judul program (cyan tebal) yang diapit dua garis pembatas.
        std::cout << '\n';  // std::cout << '\n'; artinya cetak baris kosong sebelum judul.
        printRule('=');  // printRule('=') artinya garis "=" sepanjang TITLE_WIDTH di atas judul (BAGIAN 3).
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // colorize(ANSI_TITLE, "  " + std::string(text)) artinya judul cyan tebal yang menjorok 2 spasi.
        printRule('='); // printRule('=') artinya garis "=" lagi di bawah judul.
    } // akhir fungsi printTitle()

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // Bagaimana menggambar tabung yang halus hanya dengan teks? Kita pinjam karakter Braille Unicode, bernomor U+2800 s.d. U+28FF
    // ("U+" adalah penulisan nomor Unicode dalam heksadesimal, sama dengan 0x2800 s.d. 0x28FF). Setiap karakter Braille punya 8 titik (2 kolom x 4 baris)
    // yang bisa dinyalakan satu per satu, jadi satu sel terminal memberi 8 titik gambar. Teknik ini juga dipakai pustaka seperti drawille.
    struct BrailleCanvas { // BrailleCanvas { adalah "kanvas" gambar: ia menyimpan titik-titik yang menyala, lalu mengubahnya menjadi baris teks Braille.
    // Selain data, struct ini juga punya fungsi anggota, yang dipanggil dengan titik, misalnya canvas.setPixel(...) di printSketch() di bawah.
        int cols, rows;                 // cols, rows adalah ukuran kanvas dalam sel terminal (kolom dan baris karakter).
        std::vector<unsigned char> layers; // layers adalah nomor lapisan tiap titik: 0 = kosong, lebih dari 0 = nomor lapisan (penentu warna). Titik disimpan berderet baris demi baris,
        // jadi titik (x, y) ada di indeks y * pixelWidth() + x; pixelWidth() adalah fungsi anggota beberapa baris di bawah.

        // Constructor adalah fungsi bernama sama dengan struct-nya yang otomatis jalan saat kanvas dibuat; bagian setelah titik dua (:) mengisi anggota satu per satu.
        // * di antara dua bilangan adalah perkalian.
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {} // BrailleCanvas(int k, int row) : cols(k), rows(row), layers(...) artinya cols = k, rows = row, dan layers berisi (2 x k) x (4 x row) titik yang semuanya
        // mulai dari 0 (kosong); {} kosong karena tidak ada langkah lain. Contoh pemakaiannya: BrailleCanvas canvas(canvasCols, canvasRows) di printSketch().
        // const setelah () pada fungsi anggota artinya fungsi itu hanya membaca, tidak mengubah isi kanvas.
        int pixelWidth()  const { return 2 * cols; }    // pixelWidth()  const { return 2 * cols; } adalah lebar kanvas dalam titik: 2 titik per sel ke samping.
        int pixelHeight() const { return 4 * rows; } // pixelHeight() const { return 4 * rows; } adalah tinggi kanvas dalam titik: 4 titik per sel ke bawah.

        void setPixel(int x, int y, unsigned char layer) { // setPixel(int x, int y, unsigned char layer) berfungsi untuk menyalakan satu titik di (x, y) dengan nomor lapisan layer; x = posisi mendatar,
        // y = posisi tegak (0 = paling atas). Lapisan bernomor lebih besar menimpa yang lebih kecil, jadi garis tetap terlihat di atas arsiran.
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight() artinya titik di luar kanvas, jadi diabaikan; return tanpa nilai langsung keluar dari fungsi void.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x]; // ref adalah reference ke isi layers di posisi (x, y); mengubah ref berarti mengubah isi layers itu.
            if (layer > ref) ref = layer;  // layer > ref artinya simpan hanya bila lapisannya lebih tinggi dari yang sudah ada.
        } // akhir fungsi setPixel()

        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const { // cellString(int cx, int cy, const std::vector<const char*>& layerColors) const berfungsi untuk mengubah 8 titik dalam satu sel (cx, cy) menjadi satu
        // karakter Braille UTF-8 yang berwarna sesuai lapisan tertingginya; layerColors adalah daftar kode warna dengan indeks = nomor lapisan.
            // [4][2] di belakang nama membuat array dua dimensi: 4 baris x 2 kolom, dibaca bit[baris][kolom].
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} }; // bit[4][2] = { ... } adalah peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40, dan kolom kanan 0x08, 0x10, 0x20, 0x80.
            // static di sini membuat tabel ini dibuat sekali saja, bukan setiap kali fungsi dipanggil.
            unsigned char value = 0, maxLayer = 0; // value = 0 adalah gabungan bit titik yang menyala; maxLayer = 0 adalah lapisan tertinggi di sel ini (penentu warna).
            for (int dy = 0; dy < 4; ++dy) {  // for (int dy = 0; dy < 4; ++dy) artinya dy = baris titik di dalam sel (0-3).
                for (int dx = 0; dx < 2; ++dx) {  // for (int dx = 0; dx < 2; ++dx) artinya dx = kolom titik di dalam sel (0-1).
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx]; // layer adalah nomor lapisan titik (cx * 2 + dx, cy * 4 + dy) yang diambil dari layers.
                    // |= berfungsi untuk menggabungkan bit dengan OR lalu menyimpan hasilnya ke variabel yang sama (a |= b sama dengan a = a | b).
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); } // if (layer) artinya titik menyala (angka bukan 0 dianggap benar): bit-nya dinyalakan di value, dan maxLayer mencatat lapisan tertinggi lewat std::max.
                } // akhir perulangan dx
            } // akhir perulangan dy
            if (!value) return " ";                       // !value artinya tidak ada titik yang menyala, jadi sel kosong ditulis sebagai spasi biasa.
            const unsigned codePoint = 0x2800u + value; // codePoint = 0x2800u + value adalah nomor Unicode karakter Braille yang dimaksud; akhiran u pada 0x2800u menandai bilangan unsigned.
            // Karakter ini disandikan (diubah) ke UTF-8 tiga byte berpola 1110xxxx 10xxxxxx 10xxxxxx; bit x diisi bit-bit codePoint.
            std::string s; // s adalah tempat menyusun byte UTF-8, mula-mula kosong.
            // >> di antara dua bilangan berfungsi untuk menggeser bit ke kanan (kebalikan <<).
            s += static_cast<char>(0xE0 | (codePoint >> 12)); // 0xE0 | (codePoint >> 12) adalah byte pertama berpola 1110xxxx: 4 bit teratas codePoint ditempel ke pola 0xE0; static_cast<char> menjadikannya satu byte.
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)); // 0x80 | ((codePoint >> 6) & 0x3F) adalah byte kedua berpola 10xxxxxx: 6 bit tengah codePoint.
            s += static_cast<char>(0x80 | (codePoint & 0x3F)); // 0x80 | (codePoint & 0x3F) adalah byte ketiga berpola 10xxxxxx: 6 bit terbawah codePoint.
            return colorize(layerColors[maxLayer], s);  // colorize(layerColors[maxLayer], s) artinya karakter Braille diberi warna sesuai lapisan tertinggi di sel ini.
        } // akhir fungsi cellString()

        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const { // toLines(const std::vector<const char*>& layerColors) const berfungsi untuk mengubah seluruh kanvas menjadi baris-baris teks siap cetak,
        // satu std::string per baris sel.
            std::vector<std::string> lines; // lines adalah daftar baris hasil, mula-mula kosong.
            for (int cy = 0; cy < rows; ++cy) {  // for (int cy = 0; cy < rows; ++cy) artinya setiap baris sel, dari atas ke bawah.
                std::string row; // row adalah teks satu baris sel, mula-mula kosong.
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);  // row += cellString(cx, cy, layerColors) artinya sambung karakter setiap sel dari kiri ke kanan.
                lines.push_back(row); // lines.push_back(row) artinya baris yang sudah lengkap dimasukkan ke daftar.
            } // akhir perulangan baris sel
            return lines; // return lines; artinya kembalikan semua baris gambar.
        } // akhir fungsi toLines()
    };  // }; adalah akhir struct BrailleCanvas

    void printSketch() { // printSketch() berfungsi untuk menggambar sketsa tabung di kanvas Braille, mirip gambar di buku cetak, lalu menampilkannya di dalam kotak.
    // Tutup dan alas berupa elips yang dihitung dari persamaannya: x = a cos t, y = b sin t, dengan a dan b = setengah sumbu mendatar dan tegak elips
    // (semiMajor dan semiMinor di bawah). Dinding berupa dua garis tegak, permukaan tutup diarsir tipis, dan tepi alas yang tertutup badan tabung digambar
    // putus-putus. Garis jari-jari R dan panah tinggi T ditulis sebagai teks kuning supaya langsung terbaca.
        const int available = terminalWidth() - 8; // available adalah lebar terminal dikurangi 8: tepi dan margin kotak (5) + indentasi gambar (3).
        int canvasCols = std::max(24, std::min(46, available - 6)); // std::max(24, std::min(46, available - 6)) artinya sisakan 6 kolom di kanan untuk panah T, lalu batasi lebar kanvas: paling besar 46 sel, paling kecil 24 sel.
        // % berfungsi untuk mengambil sisa bagi, misalnya 7 % 2 = 1.
        if (canvasCols % 2 != 0) --canvasCols;  // canvasCols % 2 != 0 artinya lebarnya ganjil, jadi dikurangi 1 (--canvasCols) supaya genap; dengan lebar genap, kolom tengah canvasCols / 2
        // membagi kanvas menjadi dua bagian sama lebar.
        const int canvasRows = std::max(12, canvasCols * 21 / 46);  // canvasCols * 21 / 46 adalah tinggi yang sebanding dengan gambar asli (21 baris untuk 46 sel), supaya tabung tidak gepeng; std::max(12, ...) membuatnya
        // minimal 12 baris. Pembagian bilangan bulat di sini membuang sisanya.
        const std::vector<std::string> legend = { // legend adalah empat baris keterangan gambar, ditulis langsung dengan { ... } (indeks 0 sampai 3).
            "R = jari-jari (pusat ke tepi), D = diameter = 2 x R", // "R = jari-jari (pusat ke tepi), D = diameter = 2 x R", adalah legend[0]: arti R dan D.
            "T = tinggi tabung (jarak alas ke tutup)", // "T = tinggi tabung (jarak alas ke tutup)", adalah legend[1]: arti T.
            "V = Luas alas x T = Pi x R x R x T", // "V = Luas alas x T = Pi x R x R x T", adalah legend[2]: rumus volume.
            "garis putus-putus = tepi alas yang tersembunyi di belakang badan tabung", // "garis putus-putus = ...", adalah legend[3]: arti garis putus-putus; baris ini selalu ditaruh di bawah gambar.
        }; // }; adalah akhir daftar legend
        std::size_t longestLegend = 0; // longestLegend adalah lebar keterangan terlebar di antara legend[0] sampai legend[2]; hanya ketiganya yang bisa ditaruh di samping gambar.
        for (int i = 0; i < 3; ++i) longestLegend = std::max(longestLegend, displayWidth(legend[static_cast<std::size_t>(i)])); // std::max(longestLegend, displayWidth(legend[...])) artinya periksa legend[0] sampai legend[2] dan simpan lebar yang terbesar.
        const bool legendOnRight = (3 + canvasCols + 5 + 6 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5); // legendOnRight bernilai true bila baris terlebar (3 indentasi + gambar + 5 kolom panah "    │" + 6 jarak + keterangan terpanjang) masih muat di dalam
        // kotak (lebar terminal - 5). Bila tidak muat, drawBox() (BAGIAN 3) akan memotong barisnya dan gambarnya rusak, jadi keterangan ditaruh di bawah.
        constexpr double PI_DRAWING = 3.14159265358979; // PI_DRAWING = 3.14159265358979 adalah π yang lebih teliti, khusus untuk menggambar; volume tetap memakai π pilihan pengguna.

        BrailleCanvas canvas(canvasCols, canvasRows);  // canvas(canvasCols, canvasRows) artinya buat kanvas kosong lewat constructor BrailleCanvas di atas.
        const double semiMajor = canvas.pixelWidth() / 2.0 - 2.0;        // semiMajor adalah setengah sumbu mendatar elips (dalam titik): setengah lebar kanvas dikurangi 2 titik, supaya dinding kanan yang tebalnya 2 titik masih muat.
        const double semiMinor = canvas.pixelHeight() * 9.0 / 84.0;      // semiMinor adalah setengah sumbu tegak elips: 9/84 tinggi kanvas. Angka per 84 berasal dari gambar asli yang tingginya 84 titik (21 baris x 4 titik),
        // jadi bentuknya sama untuk kanvas sebesar apa pun.
        const double cx = canvas.pixelWidth() / 2.0;             // cx adalah pusat mendatar kanvas, sekaligus pusat elips tutup dan alas.
        const double yTop = canvas.pixelHeight() * 11.0 / 84.0, yBottom = yTop + canvas.pixelHeight() * 58.0 / 84.0; // yTop adalah posisi tegak pusat elips tutup (11/84 tinggi kanvas), dan yBottom pusat elips alas, 58/84 tinggi kanvas di bawahnya.

        // [&] di depan lambda artinya lambda boleh memakai variabel di sekitarnya (canvas, cx, semiMajor, semiMinor, PI_DRAWING) secara langsung, tanpa disalin.
        auto drawEllipse = [&](double yCenter, unsigned char frontLayer, bool dashedBack) { // drawEllipse = [&](double yCenter, unsigned char frontLayer, bool dashedBack) adalah lambda yang disimpan ke variabel (tipenya auto) untuk menggambar
        // satu elips berpusat di tinggi yCenter dengan lapisan frontLayer; bila dashedBack bernilai true, bagian belakangnya (atas) digambar putus-putus abu-abu.
            const int N = 720;  // N = 720 adalah jumlah langkah sudut, supaya garisnya tersambung rapat.
            for (int i = 0; i < N; ++i) { // for (int i = 0; i < N; ++i) artinya hitung satu titik elips untuk setiap langkah sudut.
                const double t = 2.0 * PI_DRAWING * i / N;  // t = 2.0 * PI_DRAWING * i / N adalah sudut t dari 0 sampai hampir 2π (satu putaran penuh), dalam radian.
                // std::cos dan std::sin berfungsi untuk menghitung kosinus dan sinus sebuah sudut dalam radian (berasal dari <cmath>).
                const int x = static_cast<int>(cx + semiMajor * std::cos(t)), y = static_cast<int>(yCenter + semiMinor * std::sin(t)); // x = static_cast<int>(cx + semiMajor * std::cos(t)) dan y = static_cast<int>(yCenter + semiMinor * std::sin(t)) adalah titik elips
                // (rumus elips: x = a cos t, y = b sin t, digeser ke pusatnya); static_cast<int> membuang angka desimal, karena posisi titik di kanvas harus bilangan bulat.
                const bool isBack = std::sin(t) < 0;  // isBack = std::sin(t) < 0 artinya y lebih kecil dari pusat (lebih ke atas di layar), yaitu tepi belakang yang tertutup badan tabung.
                if (isBack && dashedBack) { if ((i / 18) % 2 == 0) canvas.setPixel(x, y, 4); } // isBack && dashedBack artinya tepi belakang alas: hanya digambar pada kelompok 18 langkah yang berselang-seling ((i / 18) % 2 == 0), dengan lapisan 4
                // (abu-abu), sehingga tampak putus-putus.
                else canvas.setPixel(x, y, frontLayer); // else canvas.setPixel(x, y, frontLayer) artinya selain itu titik digambar utuh dengan lapisan frontLayer.
            } // akhir perulangan langkah sudut
        };  // }; adalah akhir lambda drawEllipse
        for (int y = 0; y < canvas.pixelHeight(); ++y) {           // for (int y = 0; y < canvas.pixelHeight(); ++y) artinya periksa setiap baris titik kanvas untuk membuat arsiran tipis di permukaan tutup (lapisan 1).
            for (int x = 0; x < canvas.pixelWidth(); ++x) { // for (int x = 0; x < canvas.pixelWidth(); ++x) artinya periksa setiap titik di baris itu.
                const double ex = (x + 0.5 - cx) / semiMajor, ey = (y + 0.5 - yTop) / semiMinor; // ex dan ey adalah jarak titik ke pusat tutup (diukur dari tengah titik, + 0.5), dibagi setengah sumbu elips.
                if (ex * ex + ey * ey < 0.9 && (x + y) % 8 == 0) canvas.setPixel(x, y, 1); // ex * ex + ey * ey < 0.9 artinya titik ada di dalam elips, sedikit ke dalam dari tepinya; (x + y) % 8 == 0 hanya memilih titik pada garis-garis miring
                // berjarak 8, supaya arsirannya tipis.
            } // akhir perulangan x
        } // akhir perulangan y
        drawEllipse(yTop, 2, false);                                      // drawEllipse(yTop, 2, false) artinya gambar tutup: elips penuh dengan lapisan 2.
        const int xLeft = static_cast<int>(cx - semiMajor), xRight = static_cast<int>(cx + semiMajor); // xLeft dan xRight adalah posisi mendatar dinding kiri dan kanan, yaitu tepi kiri dan kanan elips.
        for (int y = static_cast<int>(yTop); y <= static_cast<int>(yBottom); ++y) { // for (int y = static_cast<int>(yTop); y <= static_cast<int>(yBottom); ++y) artinya gambar dinding dari tutup sampai alas, tebal 2 titik, dengan lapisan 3.
            canvas.setPixel(xLeft, y, 3); canvas.setPixel(xLeft + 1, y, 3); // canvas.setPixel(xLeft, y, 3); canvas.setPixel(xLeft + 1, y, 3); adalah dua titik dinding kiri.
            canvas.setPixel(xRight, y, 3); canvas.setPixel(xRight + 1, y, 3); // canvas.setPixel(xRight, y, 3); canvas.setPixel(xRight + 1, y, 3); adalah dua titik dinding kanan.
        } // akhir perulangan dinding
        drawEllipse(yBottom, 2, true);                                        // drawEllipse(yBottom, 2, true) artinya gambar alas: bagian depan utuh, bagian belakang putus-putus.
        const std::vector<const char*> layerColors = { "", ANSI_SURFACE, ANSI_TITLE, ANSI_TITLE, ANSI_DIM }; // layerColors adalah warna tiap lapisan, indeksnya = nomor lapisan: 0 = kosong (tanpa warna), 1 = arsiran (ANSI_SURFACE), 2 = elips tutup dan alas,
        // 3 = dinding (keduanya ANSI_TITLE), 4 = tepi alas putus-putus (ANSI_DIM).
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // figureLines = canvas.toLines(layerColors) adalah gambar sebagai baris-baris teks.

        const int rowTop = static_cast<int>(yTop) / 4, rowBottom = static_cast<int>(yBottom) / 4; // rowTop dan rowBottom adalah baris sel untuk tutup dan alas: posisi titik dibagi 4, karena tinggi satu sel 4 titik.
        // { ... } tanpa if atau for membuat blok tersendiri; variabel di dalamnya (misalnya row) hanya berlaku di blok itu, jadi namanya boleh dipakai lagi di blok berikutnya.
        {   // { membuka blok garis jari-jari "●──R──►" di baris tengah tutup.
            const int centerCol = canvasCols / 2;  // centerCol = canvasCols / 2 adalah kolom tengah kanvas, yaitu pusat tutup.
            std::string radiusLine = "\u25cf";  // radiusLine = "\u25cf" artinya garis jari-jari dimulai dengan ● (titik pusat); \u25cf adalah kode Unicode untuk ●.
            for (int c = centerCol + 1; c < canvasCols - 2; ++c) radiusLine += (c == (centerCol + canvasCols - 2) / 2) ? "R" : "\u2500"; // radiusLine += (...) ? "R" : "\u2500" artinya tambahkan garis ─ dari kolom setelah pusat sampai sebelum ujung panah, dengan huruf R di kolom tengahnya
            // ((centerCol + canvasCols - 2) / 2).
            radiusLine += "\u25ba";  // radiusLine += "\u25ba" artinya tambahkan ► (ujung panah di tepi tutup).
            std::string row; // row adalah baris tutup yang baru, mula-mula kosong.
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, rowTop, layerColors); // row += canvas.cellString(c, rowTop, layerColors) artinya salin sel kanvas di kiri pusat.
            figureLines[rowTop] = row + colorize(ANSI_HINT, radiusLine) + canvas.cellString(canvasCols - 1, rowTop, layerColors); // figureLines[rowTop] = ... artinya baris tutup diganti: sel kiri + garis jari-jari kuning + sel paling kanan (tepi tutup).
        } // akhir blok garis jari-jari
        {   // { membuka blok tulisan TABUNG di tengah badan tabung.
            const int r = (rowTop + rowBottom) / 2;  // r = (rowTop + rowBottom) / 2 adalah baris tengah antara tutup dan alas.
            std::string row; // row adalah baris tengah yang baru, mula-mula kosong.
            for (int c = 0; c < canvasCols; ++c) { // for (int c = 0; c < canvasCols; ++c) artinya susun baris itu kolom demi kolom.
                if (c == canvasCols / 2 - 3) { row += colorize(ANSI_TITLE, "TABUNG"); c += 5; } // c == canvasCols / 2 - 3 artinya di kolom tengah - 3 sisipkan "TABUNG" (6 huruf, cyan tebal), lalu lompati 5 kolom (c += 5); ditambah ++c dari for,
                // totalnya 6 kolom, jadi lebar baris tetap sama.
                else row += canvas.cellString(c, r, layerColors); // else row += canvas.cellString(c, r, layerColors) artinya kolom lain diisi sel kanvas biasa.
            } // akhir perulangan kolom
            figureLines[r] = row;  // figureLines[r] = row artinya ganti baris tengah gambar dengan baris bertulisan.
        } // akhir blok tulisan TABUNG
        // Panah tinggi T selalu di kanan gambar; keterangan ditaruh di samping bila jendela lebar, di bawah bila sempit.
        std::vector<std::string> sideNotes(canvasRows, ""); // sideNotes(canvasRows, "") adalah teks di kanan setiap baris gambar; (canvasRows, "") membuat canvasRows buah teks kosong.
        for (int r = rowTop; r <= rowBottom; ++r) { // for (int r = rowTop; r <= rowBottom; ++r) artinya isi baris dari tutup sampai alas dengan panah tinggi T.
            sideNotes[r] = (r == rowTop) ? "  \u2500\u2500\u25b2" : (r == rowBottom) ? "  \u2500\u2500\u25bc" : "    \u2502"; // sideNotes[r] = ... artinya ternary bersusun: baris tutup diberi "  ──▲", baris alas "  ──▼", dan baris di antaranya garis tegak "    │".
        } // akhir perulangan panah T
        sideNotes[(rowTop + rowBottom) / 2] += "  T";  // += "  T" artinya huruf T ditaruh di tengah panah.
        if (legendOnRight) {  // if (legendOnRight) artinya jendela lebar: tiga keterangan pertama ditaruh di samping panah.
            sideNotes[rowTop + 1]              += "      " + legend[0]; // sideNotes[rowTop + 1] += "      " + legend[0] artinya keterangan R dan D di bawah ujung atas panah.
            sideNotes[(rowTop + rowBottom) / 2]    += "   " + legend[1]; // sideNotes[(rowTop + rowBottom) / 2] += "   " + legend[1] artinya keterangan T di baris tengah, setelah huruf T.
            sideNotes[rowBottom - 1]               += "      " + legend[2]; // sideNotes[rowBottom - 1] += "      " + legend[2] artinya rumus volume di atas ujung bawah panah.
        } // akhir penempatan keterangan di samping

        std::vector<std::string> lines; // lines adalah isi kotak sketsa, mula-mula kosong.
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + figureLines[r] + colorize(ANSI_HINT, sideNotes[r])); // lines.push_back("   " + figureLines[r] + colorize(ANSI_HINT, sideNotes[r])) artinya setiap baris = 3 spasi indentasi + baris gambar + catatan samping kuning.
        if (!legendOnRight) for (int i = 0; i < 3; ++i) lines.push_back("   " + legend[i]); // !legendOnRight artinya jendela sempit: tiga keterangan pertama ditaruh di bawah gambar.
        lines.push_back("   " + colorize(ANSI_DIM, legend[3]));  // colorize(ANSI_DIM, legend[3]) artinya keterangan garis putus-putus ditulis abu-abu dan selalu di bawah.
        drawBox("Sketsa Tabung", lines);  // drawBox("Sketsa Tabung", lines) artinya semuanya digambar di dalam kotak berjudul (BAGIAN 3).
        std::cout << '\n';  // std::cout << '\n'; artinya baris kosong setelah kotak.
    } // akhir fungsi printSketch()

    void printInstructions() { // printInstructions() berfungsi untuk menunjukkan bentuk jawaban yang diterima (nomor pilihan, angka > 0, cara menulis desimal) sebelum pertanyaan pertama,
    // supaya pengguna tidak salah ketik.
        drawBox("Petunjuk Pengisian", { // drawBox("Petunjuk Pengisian", { artinya gambar kotak berjudul itu; daftar teks di dalam { ... } otomatis menjadi std::vector<std::string> untuk parameter lines.
            // Di dalam teks, \" adalah tanda kutip yang ikut tercetak, dan \u03c0 tercetak sebagai π.
            "- Satuan Panjang     : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)", // "- Satuan Panjang     : ..." adalah petunjuk satuan: nomor pilihan 1-4, Enter saja = cm.
            "                       (contoh: Pilih [2] untuk satuan \"cm\")", // (contoh: Pilih [2] untuk satuan \"cm\") adalah contohnya; spasi di depannya membuat contoh sejajar dengan isi setelah " : ".
            "- Nilai Pi (\u03c0)       : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila jari-jari atau diameter kelipatan 7", // "- Nilai Pi (\u03c0)       : ..." adalah petunjuk nilai π: 1 = 3.14 sesuai modul, 2 = 22/7 bila jari-jari atau diameter kelipatan 7.
            "                       (contoh: Pilih [2] untuk nilai \"22/7\")", // (contoh: Pilih [2] untuk nilai \"22/7\") adalah contoh memilih 22/7.
            "- Ukuran Diketahui   : Nomor pilihan 1-2; 1 = jari-jari, 2 = diameter (program menghitung R = D / 2)", // "- Ukuran Diketahui   : ..." adalah petunjuk ukuran yang diketahui: 1 = jari-jari, 2 = diameter.
            "                       (contoh: Pilih [2] bila soal menyebut \"diameter\")", // (contoh: Pilih [2] bila soal menyebut \"diameter\") adalah contoh memilih diameter.
            "- Jari-Jari/Diameter : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma", // "- Jari-Jari/Diameter : ..." adalah petunjuk mengisi ukuran: angka lebih dari 0, desimalnya boleh memakai titik atau koma.
            "                       (contoh: 7, 7,5 atau 7.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)", // (contoh: 7, 7,5 atau 7.5; ...) adalah contohnya, termasuk peringatan tanpa pemisah ribuan, karena 1.000 akan terbaca sebagai 1.
            "- Tinggi Tabung      : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma", // "- Tinggi Tabung      : ..." adalah petunjuk mengisi tinggi; aturannya sama dengan ukuran alas.
            "                       (contoh: 10, 10,5 atau 10.5)", // (contoh: 10, 10,5 atau 10.5) adalah contoh tinggi.
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang." // "Ketik jawaban lalu tekan Enter ..." adalah aturan umum: Enter tanpa isi = bawaan, dan input yang tidak sesuai diminta ulang.
        }); // }); adalah akhir daftar isi kotak dan pemanggilan drawBox().
        std::cout << '\n';  // std::cout << '\n'; artinya baris kosong setelah kotak.
    } // akhir fungsi printInstructions()

    // = 2 pada parameter (int decimals = 2) adalah argumen bawaan: bila pemanggil tidak mengisinya, nilainya 2.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit, int decimals = 2) { // formatResultLine(std::string_view label, double value, std::string_view unit, int decimals = 2) berfungsi untuk menyusun satu baris hasil
    // "Label : nilai satuan" dengan decimals angka desimal, siap masuk kotak.
        // std::ostringstream berfungsi untuk menyusun teks berformat, mirip std::cout tetapi hasilnya disimpan sebagai teks (berasal dari <sstream>).
        std::ostringstream line;  // line adalah tempat teks disusun, tanpa mengubah format std::cout.
        // std::left berfungsi untuk membuat teks rata kiri di dalam lebar kolomnya.
        // std::setw berfungsi untuk mengatur lebar kolom bagi nilai berikutnya yang dicetak (berasal dari <iomanip>).
        // std::fixed berfungsi untuk menulis angka desimal dalam bentuk biasa, bukan bentuk ilmiah seperti 1e+06.
        // std::setprecision berfungsi untuk mengatur jumlah angka di belakang titik bila dipakai bersama std::fixed (berasal dari <iomanip>).
        line << std::left << std::setw(22) << label << ": " << std::fixed << std::setprecision(decimals) << value; // line << std::left << std::setw(22) << label artinya label rata kiri selebar 22 kolom, supaya titik dua semua baris sejajar; lalu ": " dan value
        // dengan tepat decimals angka di belakang titik.
        if (!unit.empty()) line << ' ' << unit;  // !unit.empty() artinya satuan ditambahkan (didahului satu spasi) hanya bila tidak kosong.
        return line.str();  // line.str() berfungsi untuk mengambil teks yang sudah tersusun.
    } // akhir fungsi formatResultLine()

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2): semua di atas hanya terlihat di berkas ini

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 2 =====
// Program bermenu (berkas main.cpp) memanggil fungsi di bawah ini sebagai task2::run(), dibaca "run milik task2".
namespace task2 { // namespace task2 { adalah wadah nama bernama task2; run() ditaruh di sini supaya namanya khas dan tidak tertukar dengan run() milik tugas lain.

    void run() { // run() berfungsi untuk menjalankan alur lengkap Program 2: tampilan awal, tahap pengaturan, input, perhitungan, lalu output.
    // Fungsi ini sengaja tidak berada di namespace tanpa nama, karena main.cpp harus bisa memanggilnya.
        enableUtf8Console(); // enableUtf8Console(); artinya pindahkan konsol Windows ke UTF-8 (BAGIAN 3); dipanggil di sini juga supaya program mandiri (tanpa menu) tetap benar,
        // dan memanggilnya berulang kali pun aman.
        printTitle("PROGRAM 2 : VOLUME TABUNG");  // printTitle("PROGRAM 2 : VOLUME TABUNG") artinya tampilkan judul program (BAGIAN 5).
        std::cout << "Rumus: V = Pi x R x R x T   (Pi = \u03c0; modul menulisnya phi)\n\n"; // "Rumus: V = Pi x R x R x T   (Pi = \u03c0; modul menulisnya phi)\n\n" adalah baris rumus; \u03c0 tercetak sebagai π, dan \n\n pindah baris dua kali.
        printSketch();    // printSketch(); artinya tampilkan sketsa, supaya pengguna tahu bagian mana yang dimaksud R, D, dan T.
        printInstructions();  // printInstructions(); artinya tampilkan kotak petunjuk pengisian (BAGIAN 5 lanjutan).

        // --- Tahap pengaturan: satuan, nilai π, dan ukuran yang diketahui ---
        const LengthUnit& unit = chooseUnit(); // unit = chooseUnit() adalah satuan terpilih; cukup reference (&) ke salah satu isi UNITS (BAGIAN 2), tidak perlu disalin.
        std::string   piLabel;                  // piLabel adalah teks "3.14" atau "22/7 (3.142857)" untuk ditampilkan di hasil; isinya diisi choosePi() di baris berikutnya.
        const double  pi = choosePi(piLabel);    // pi = choosePi(piLabel) adalah π untuk perhitungan; choosePi() juga mengisi piLabel lewat reference.
        const bool    diameterGiven = chooseDiameterInput();  // diameterGiven = chooseDiameterInput() artinya diameterGiven bernilai true bila soal menyebut diameter.

        // --- Tahap input ---
        const std::string label = diameterGiven ? "Masukkan Diameter Alas (" : "Masukkan Jari-Jari Alas ("; // label adalah awal prompt, dipilih dengan ternary sesuai ukuran yang diketahui (diameter atau jari-jari).
        const double inputSize = readPositiveDecimal(label + unit.symbol + ") : "); // inputSize = readPositiveDecimal(label + unit.symbol + ") : ") artinya minta ukuran dengan prompt seperti "Masukkan Jari-Jari Alas (cm) : ";
        // prompt menyertakan satuan supaya pengguna tahu satuan yang dipakai. double karena ukuran boleh desimal (misalnya 7.5).
        const double height = readPositiveDecimal(std::string("Masukkan Tinggi Tabung (") + unit.symbol + ")  : "); // height = readPositiveDecimal(...) artinya minta tinggi tabung. std::string("Masukkan Tinggi Tabung (") perlu karena dua teks mentah
        // (teks dalam tanda kutip dan unit.symbol yang bertipe const char*) tidak bisa disambung langsung dengan +.
        const double radius = diameterGiven ? inputSize / 2.0 : inputSize; // radius = diameterGiven ? inputSize / 2.0 : inputSize artinya bila yang diketahui diameter, jari-jari = diameter / 2; bila tidak, jari-jari = inputSize.

        // --- Tahap perhitungan ---
        const double baseArea = pi * radius * radius;  // baseArea = pi * radius * radius adalah luas alas lingkaran = π x R x R.
        const double volume   = baseArea * height;  // volume   = baseArea * height adalah volume = luas alas x tinggi; dihitung dua langkah supaya rumusnya mudah dibaca.
        const double liters    = volume * unit.litersPerCubicUnit; // liters    = volume * unit.litersPerCubicUnit adalah padanan liter: volume x isi 1 satuan kubik dalam liter (BAGIAN 2), karena soal cerita sering meminta liter.

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit.symbol) + "\u00b2";  // areaUnit adalah satuan luas, misalnya cm²; "\u00b2" adalah kode Unicode untuk ² (pangkat dua).
        const std::string volumeUnit = std::string(unit.symbol) + "\u00b3";  // volumeUnit adalah satuan volume, misalnya cm³; "\u00b3" adalah kode Unicode untuk ³ (pangkat tiga).
        std::vector<std::string> resultLines; // resultLines adalah baris-baris hasil; vector karena jumlah barisnya berbeda untuk jari-jari dan diameter.
        resultLines.push_back("Pi (\u03c0) yang Digunakan : " + piLabel); // "Pi (\u03c0) yang Digunakan : " + piLabel adalah baris pertama: nilai π yang dipakai.
        if (diameterGiven) {  // if (diameterGiven) artinya diameter diketahui: tampilkan langkah R = D / 2 seperti penyelesaian soal.
            resultLines.push_back(formatResultLine("Diameter (D)", inputSize, unit.symbol)); // formatResultLine("Diameter (D)", inputSize, unit.symbol) adalah baris diameter yang diketikkan.
            resultLines.push_back(formatResultLine("Jari-Jari (R) = D / 2", radius, unit.symbol)); // formatResultLine("Jari-Jari (R) = D / 2", radius, unit.symbol) adalah baris jari-jari hasil bagi dua.
        } else {  // } else { artinya jari-jari yang diketahui: cukup satu baris.
            resultLines.push_back(formatResultLine("Jari-Jari (R)", radius, unit.symbol)); // formatResultLine("Jari-Jari (R)", radius, unit.symbol) adalah baris jari-jari.
        } // akhir if-else ukuran yang diketahui
        resultLines.push_back(formatResultLine("Tinggi (T)", height, unit.symbol)); // formatResultLine("Tinggi (T)", height, unit.symbol) adalah baris tinggi.
        resultLines.push_back(formatResultLine("Luas Alas", baseArea, areaUnit)); // formatResultLine("Luas Alas", baseArea, areaUnit) adalah baris luas alas, satuannya pangkat dua.
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Volume Tabung", volume, volumeUnit))); // colorize(ANSI_SUCCESS, formatResultLine("Volume Tabung", volume, volumeUnit)) adalah hasil akhir, ditonjolkan hijau tebal.
        resultLines.push_back(formatResultLine("Setara", liters, "liter (1 liter = 1 dm\u00b3)", 3)); // formatResultLine("Setara", liters, "liter (1 liter = 1 dm\u00b3)", 3) adalah padanan liter dengan 3 angka desimal (argumen terakhir = decimals).
        std::cout << '\n';  // std::cout << '\n'; artinya baris kosong sebelum kotak hasil.
        drawBox("Hasil Perhitungan", resultLines);  // drawBox("Hasil Perhitungan", resultLines) artinya semua baris hasil dicetak di dalam kotak (BAGIAN 3).
    }  // akhir fungsi run()

}  // akhir namespace task2

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), padahal program bermenu sudah punya main() sendiri. Jadi main() di bawah hanya kita sertakan saat berkas ini
// dikompilasi sendiri.
#ifndef BUILD_WITH_MENU // #ifndef BUILD_WITH_MENU artinya "bila penanda BUILD_WITH_MENU belum ada"; penanda itu baru ada bila perintah kompilasi memakai -DBUILD_WITH_MENU.
// int main() adalah titik awal program: fungsi yang pertama kali dijalankan saat program dibuka. Standar C++ mewajibkan main bertipe int, yaitu bilangan bulat
// yang dikirim ke sistem operasi saat program selesai; nilai 0 (lihat return 0 di bawah) menandakan program selesai tanpa error.
int main() { // main() adalah fungsi utama saat berkas ini dikompilasi sendiri: cetak identitas, lalu jalankan Program 2.
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 2 - Fadhlillah (2531070011)") << '\n'; // colorize(ANSI_BOLD, "Tugas Praktikum 2 - Fadhlillah (2531070011)") adalah baris identitas yang dicetak tebal.
    task2::run(); // task2::run(); artinya jalankan Program 2 (BAGIAN 6).
    return 0;  // return 0; artinya program selesai tanpa error.
} // akhir fungsi main()
#endif  // selesai bagian yang hanya ada bila berkas ini dikompilasi sendiri (tanpa BUILD_WITH_MENU)
