// ===== tugas1_nilai_mahasiswa.cpp — Program 1 : Tools Nilai Mahasiswa =====
// Alur program: pengguna mengisi Nama Mata Kuliah dan jumlah mahasiswa (1 sampai 1000, tidak harus 3), lalu nama lengkap dan nilai tiap mahasiswa;
// nilai boleh desimal, ditulis 87.5 maupun 87,5. Di akhir, program menampilkan rekap nilai, nilai total, dan nilai rata-rata dalam satu kotak.
// Setiap input diperiksa dulu; kalau salah, pertanyaannya diulang. Aturannya: nama mahasiswa hanya boleh huruf/spasi/titik/tanda hubung/tanda petik,
// nama mata kuliah harus mengandung huruf, jumlah hanya angka 0-9 (1-1000), nilai 0-100 dengan paling banyak satu pemisah desimal.
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Cara mengompilasi berkas ini sendiri (jalankan dari folder proyek):
// 1. Buka Terminal VS Code lalu copy paste command di bawah ini :
//   g++ -std=c++17 -Wall -Wextra -o tugas1 src/tugas1_nilai_mahasiswa.cpp && ./tugas1
//   (Windows PowerShell: .\tugas1.exe).
// Arti tiap bagian perintah itu:
//   g++                             compiler, yaitu program yang mengubah kode C++ menjadi program yang bisa dijalankan
//   -std=c++17                      memakai aturan bahasa C++17
//   -Wall -Wextra                   menampilkan peringatan tambahan supaya kesalahan kecil kelihatan
//   -o tugas1                       nama program hasil kompilasi
//   src/tugas1_nilai_mahasiswa.cpp  berkas yang dikompilasi, yaitu berkas ini
//   && ./tugas1                     menjalankan program hasilnya, tetapi hanya bila kompilasi berhasil
// Bila digabung ke program bermenu (main.cpp di folder src yang sama), kompilasinya memakai -DBUILD_WITH_MENU. Opsi -D berfungsi untuk memasang
// penanda bernama BUILD_WITH_MENU saat kompilasi. Bila penanda itu ada, main() di bagian paling bawah berkas ini (BAGIAN 7) tidak ikut dikompilasi,
// jadi yang dipakai hanya main() milik main.cpp.
//
// 2. Cara lain di VS Code : buka berkas ini di editor, lalu menu Terminal > Run Task..., pilih task "Build & Run berkas yang sedang dibuka (tugas sendiri)".
// Task adalah perintah siap pakai; yang satu ini menjalankan perintah g++ yang sama seperti di atas, hanya saja nama program hasilnya mengikuti nama berkas
// (src/tugas1_nilai_mahasiswa). Daftar task ada di berkas .vscode/tasks.json.
//
// Susunan berkas: BAGIAN 1 pustaka, BAGIAN 2 konstanta dan tipe data, BAGIAN 3 warna dan kotak tampilan, BAGIAN 4 fungsi bantu input,
// BAGIAN 5 judul/petunjuk/tabel, BAGIAN 6 alur utama task1::run(), BAGIAN 7 main().


// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Program ini tidak menulis semuanya dari nol: untuk mencetak ke layar, menyimpan teks, atau mengatur lebar kolom, kita meminjam dari pustaka standar C++.
// Nama dari pustaka standar diawali std::, misalnya std::max; dibaca "max milik std". std adalah namespace (wadah nama) dan tanda :: dibaca "milik".
// #include berfungsi untuk menyalin isi sebuah header ke berkas ini sebelum dikompilasi; header adalah berkas berisi daftar fungsi dan tipe yang disediakan sebuah pustaka.
#include <algorithm> // <algorithm> digunakan untuk std::max dan std::min, yang mengambil yang terbesar/terkecil dari dua nilai; dipakai saat menghitung lebar kolom nama dan lebar kotak (BAGIAN 3 dan 6).
#include <cctype> // <cctype> digunakan untuk std::isalpha dan std::isdigit, yang memeriksa apakah satu karakter itu huruf atau angka; dipakai oleh fungsi-fungsi pemeriksa input di BAGIAN 4.
#include <cstddef> // <cstddef> digunakan untuk std::size_t, yaitu bilangan bulat tak bertanda (tidak bisa negatif, mulai dari 0) untuk ukuran dan indeks; panjang std::string dan jumlah elemen std::vector bertipe ini.
#include <cstdlib> // <cstdlib> digunakan untuk std::exit (menghentikan program), EXIT_FAILURE (kode "gagal"), std::getenv (membaca variabel lingkungan seperti NO_COLOR atau COLUMNS),
// dan std::atoi (mengubah teks menjadi int); semuanya dipakai di BAGIAN 3 dan 4.
#include <iomanip> // <iomanip> digunakan untuk std::setw (lebar kolom) dan std::setprecision (jumlah angka desimal); pasangannya std::left, std::right, dan std::fixed sudah tersedia lewat <iostream>. Semuanya dipakai tabel rekap di BAGIAN 5.
#include <iostream> // <iostream> digunakan untuk std::cin (masukan keyboard), std::cout (keluaran layar), dan std::cerr (keluaran pesan kesalahan); program ini berbicara dengan pengguna lewat terminal.
#include <sstream> // <sstream> digunakan untuk std::istringstream (membaca angka dari teks) dan std::ostringstream (menyusun teks dari angka); input dibaca sebagai teks dulu baru diubah ke angka (BAGIAN 4).
#include <string> // <string> digunakan untuk tipe std::string, yaitu teks yang panjangnya bisa berubah, dan fungsi std::getline untuk membaca satu baris ketikan utuh; nama mata kuliah dan nama mahasiswa adalah teks.
#include <string_view> // <string_view> digunakan untuk tipe std::string_view, yaitu "jendela" untuk membaca sebuah teks tanpa menyalinnya;
// dipakai karena menyalin teks yang hanya dibaca (prompt, judul kotak, isi kolom tabel) cuma membuang waktu dan memori.
#include <vector> // <vector> digunakan untuk tipe std::vector, yaitu daftar berurutan yang bisa bertambah panjang; jumlah mahasiswa baru diketahui setelah pengguna mengetiknya, jadi array berukuran tetap tidak cocok.


// #ifdef berfungsi untuk bertanya ke compiler apakah sebuah nama sudah didefinisikan; bila ya, baris sampai #else/#endif ikut dikompilasi, bila tidak, baris itu dilewati.
// Di sini kita perlu bertanya karena program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya berbicara dengan terminal berbeda.
#ifdef _WIN32 // _WIN32 adalah nama yang otomatis ada saat program dikompilasi untuk Windows; artinya baris sampai #else hanya ikut dikompilasi di Windows.
    // #ifndef ("if not defined") berfungsi kebalikan dari #ifdef: baris sampai #endif hanya ikut dikompilasi bila nama itu BELUM didefinisikan.
    #ifndef NOMINMAX // NOMINMAX dicek dulu karena MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan mendefinisikan ulang nama yang sama menghasilkan peringatan.
    // #define berfungsi untuk membuat makro, yaitu nama yang oleh compiler diganti dengan teks lain; mulai baris di bawah, NOMINMAX berarti 1.
    #define NOMINMAX 1 // NOMINMAX 1 artinya "jangan buat min dan max" dinyalakan, karena windows.h punya kebiasaan membuat makro min/max sendiri yang merusak std::min dan std::max.
    #endif // selesai mengecek NOMINMAX
    #include <windows.h> // <windows.h> adalah pustaka resmi Windows; dari sini dipakai fungsi untuk warna (mode virtual terminal), UTF-8 (code page), dan lebar jendela konsol (BAGIAN 3).
#else // #else artinya: bila bukan Windows (macOS atau Linux), pakai pustaka di bawah ini.
    #include <cstdio> // <cstdio> digunakan untuk stdout, yaitu "saluran" keluaran ke layar, dan fileno, yang memberi nomor saluran itu.
    #include <sys/ioctl.h> // <sys/ioctl.h> digunakan untuk ioctl dan TIOCGWINSZ, yaitu cara bertanya ke terminal seberapa lebar jendelanya.
    #include <unistd.h> // <unistd.h> digunakan untuk isatty (memeriksa apakah keluaran benar-benar terminal, bukan berkas) dan STDOUT_FILENO (nomor saluran keluaran layar).
#endif // #endif berfungsi untuk menutup blok #ifdef/#ifndef; di sini artinya selesai memilih pustaka sesuai sistem operasi.

// namespace berfungsi untuk membuat wadah nama, supaya nama fungsi dan konstanta tidak bentrok dengan nama yang sama di tempat lain.
namespace { // namespace { tanpa nama berfungsi untuk membuat isinya hanya terlihat dari berkas ini, karena tugas2_volume_tabung.cpp dan tugas3_luas_arsir.cpp di folder src yang sama
// juga punya fungsi bantu bernama sama, misalnya colorize() dan drawBox(); tanpa ini, penggabungan ke program bermenu main.cpp gagal dengan error "multiple definition"
// (satu nama didefinisikan lebih dari sekali). Wadah ini berisi BAGIAN 2 sampai BAGIAN 5 dan ditutup tepat sebelum BAGIAN 6.

    // ===== BAGIAN 2: KONSTANTA DAN TIPE DATA =====
    // Angka-angka yang menjadi aturan program (batas jumlah, batas nilai, lebar kolom) dikumpulkan di sini supaya mudah dicari dan diubah di satu tempat.
    // constexpr berfungsi untuk membuat konstanta, yaitu nilai tetap yang sudah pasti sejak kompilasi dan tidak bisa diubah saat program berjalan; namanya ditulis HURUF_BESAR, itu kebiasaan umum.
    // int adalah tipe bilangan bulat biasa (4 byte) yang boleh bernilai negatif; dipakai untuk jumlah mahasiswa karena jumlah orang selalu bulat.
    constexpr int STUDENT_COUNT_MIN = 1;     // STUDENT_COUNT_MIN = 1 adalah batas bawah jumlah mahasiswa; rekap tanpa mahasiswa tidak bermakna, jadi minimal 1. Dipakai saat memanggil readInteger() di BAGIAN 6.
    constexpr int STUDENT_COUNT_MAX = 1000;  // STUDENT_COUNT_MAX = 1000 adalah batas atas jumlah mahasiswa; batas 1000 berfungsi untuk menolak salah ketik seperti 30000.

    // double adalah tipe bilangan desimal (8 byte, presisi sekitar 15 digit); dipakai untuk nilai karena nilai bisa desimal seperti 87.5, sedangkan pada int 87.5 terpotong menjadi 87.
    constexpr double SCORE_MIN = 0.0;    // SCORE_MIN = 0.0 adalah batas bawah nilai, karena nilai tidak mungkin negatif. Dipakai saat memanggil readDecimal() di BAGIAN 6.
    constexpr double SCORE_MAX = 100.0;  // SCORE_MAX = 100.0 adalah batas atas nilai, karena skala nilai mata kuliah 0-100.

    // Enam konstanta di bawah adalah lebar kolom tabel, lebar garis judul, dan batas panjang teks, semuanya dalam satuan karakter.
    // Lebar yang diberikan ke std::setw (BAGIAN 5) bertipe int; TITLE_WIDTH juga int, karena di printRule() (BAGIAN 3) ia dibandingkan dengan terminalWidth() yang juga int.
    // Lebar yang dibandingkan dengan hasil displayWidth() (BAGIAN 3) bertipe std::size_t,
    // dikarenakan membandingkan bilangan bertanda (int) dengan tak bertanda (std::size_t) memunculkan peringatan compiler.
    constexpr int         NO_COLUMN_WIDTH    = 5;   // NO_COLUMN_WIDTH    = 5 adalah lebar kolom nomor urut ("No"); int karena diberikan ke std::setw di formatTableRow() (BAGIAN 5).
    // std::size_t adalah tipe bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan panjang (berasal dari <cstddef>).
    constexpr std::size_t NAME_COLUMN_MIN_WIDTH    = 20;  // NAME_COLUMN_MIN_WIDTH    = 20 adalah lebar minimal kolom nama; std::size_t karena dibandingkan dengan hasil displayWidth() di run() (BAGIAN 6).
    constexpr int         SCORE_COLUMN_WIDTH = 8;   // SCORE_COLUMN_WIDTH = 8 adalah lebar kolom nilai ("Nilai"); int karena diberikan ke std::setw.
    constexpr int         TITLE_WIDTH       = 60;  // TITLE_WIDTH       = 60 adalah jumlah karakter '=' pada garis judul; dipakai printRule() (BAGIAN 3).
    constexpr std::size_t NAME_MAX_LENGTH = 60;  // NAME_MAX_LENGTH = 60 adalah batas panjang nama mahasiswa; lebih panjang dari ini hampir pasti tempelan teks, bukan nama. Dipakai isValidPersonName() di BAGIAN 4.
    constexpr std::size_t COURSE_NAME_MAX_LENGTH = 60;  // COURSE_NAME_MAX_LENGTH = 60 adalah batas panjang nama mata kuliah, alasan yang sama. Dipakai isValidCourseName() di BAGIAN 4.

    // struct berfungsi untuk membuat tipe data sendiri yang menggabungkan beberapa variabel (anggota) menjadi satu kesatuan; anggotanya diakses dengan tanda titik, misalnya student.name.
    struct Student { // struct Student { adalah wadah data satu mahasiswa: nama dan nilai dibungkus jadi satu supaya tidak tertukar seperti bila disimpan di dua daftar terpisah. Dipakai run() di BAGIAN 6.
        // std::string adalah tipe teks milik pustaka standar yang panjangnya bisa berubah.
        std::string name;     // std::string name; adalah nama mahasiswa: huruf dan spasi yang panjangnya tidak pasti, jadi std::string.
        double      score{};  // double      score{}; adalah nilai mahasiswa: boleh desimal, jadi double; tanda {} berfungsi memberi nilai awal 0 supaya tidak berisi angka acak.
    };  // }; akhir struct Student; tanda ; wajib setelah kurung penutup struct.

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, hasil akhir, dan pesan kesalahan gampang dibedakan, program memakai warna lewat kode ANSI, yaitu teks khusus yang tidak tercetak;
    // terminal membacanya sebagai perintah ganti gaya. Bentuknya "\033[" + angka gaya + huruf m. "\033" adalah karakter ESC (nomor 27, ditulis dalam oktal: 3 x 8 + 3 = 27).
    // Kode ANSI dipakai karena terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    // const char* adalah penunjuk (alamat di memori) ke teks tetap yang hurufnya tidak boleh diubah; const berarti "tidak boleh diubah", dan * menandai penunjuk.
    constexpr const char* ANSI_RESET  = "\033[0m";     // ANSI_RESET  = "\033[0m"; kode untuk mengembalikan tampilan ke normal (tanpa warna/tebal); angka 0 artinya reset.
    constexpr const char* ANSI_BOLD  = "\033[1m";     // ANSI_BOLD  = "\033[1m"; angka 1 artinya tebal; dipakai untuk prompt (teks pertanyaan yang menunggu ketikan pengguna), label "Mahasiswa ke-N dari M", judul kolom tabel, dan baris identitas di main() (BAGIAN 7).
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // ANSI_TITLE  = "\033[1;36m"; 1;36 artinya tebal (1) sekaligus cyan (36); dipakai untuk judul program, judul kotak, dan sub-judul "Input Nilai ..." di run() (BAGIAN 6).
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // ANSI_SUCCESS  = "\033[1;32m"; 1;32 artinya tebal dan hijau (32); dipakai untuk baris nilai total dan rata-rata.
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // ANSI_ERROR  = "\033[1;31m"; 1;31 artinya tebal dan merah (31); dipakai untuk pesan kesalahan.
    constexpr const char* ANSI_DIM    = "\033[90m";    // ANSI_DIM    = "\033[90m"; 90 artinya abu-abu (hitam terang); dipakai untuk garis kotak dan garis pembatas agar tidak mencolok.

    // void berfungsi menandai fungsi yang tidak mengembalikan nilai apa pun; fungsi seperti ini dipanggil untuk efeknya saja. Isi fungsi ditulis di antara { dan }.
    void enableUtf8Console() { // enableUtf8Console() berfungsi untuk mengganti konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama; dipanggil di awal run() (BAGIAN 6).
    // UTF-8 adalah aturan menyimpan setiap huruf dan simbol sebagai 1 sampai 4 byte. Konsol Windows bawaan (Command Prompt, PowerShell) masih memakai code page lama,
    // misalnya 437 (code page adalah tabel aturan byte mana menjadi huruf apa), sehingga huruf seperti é, ö, dan garis kotak dari drawBox() di bawah tampil sebagai huruf acak.
    // Pengaturan ini sama dengan perintah chcp 65001 di berkas peluncur jalankan.bat. Di macOS/Linux terminal sudah UTF-8, jadi di sana isi fungsi ini kosong.
    #ifdef _WIN32 // _WIN32: dua baris di bawah hanya ikut dikompilasi di Windows, karena SetConsoleOutputCP dan SetConsoleCP hanya ada di windows.h.
        SetConsoleOutputCP(CP_UTF8);  // SetConsoleOutputCP(CP_UTF8) berfungsi untuk mengatur keluaran konsol ke UTF-8, agar byte UTF-8 tampil sebagai karakter yang benar; CP_UTF8 adalah konstanta Windows bernilai 65001.
        SetConsoleCP(CP_UTF8);        // SetConsoleCP(CP_UTF8) berfungsi untuk mengatur masukan (ketikan) konsol ke UTF-8 juga, agar huruf non-ASCII seperti é atau ö terbaca sama dengan keluarannya.
        // ASCII adalah kumpulan 128 karakter dasar: A-Z dan a-z tanpa aksen, angka 0-9, tanda baca, spasi, dan karakter kendali seperti pindah baris.
    #endif // akhir bagian khusus Windows
    } // akhir fungsi enableUtf8Console()

    // bool adalah tipe data yang hanya punya dua nilai: true (benar) atau false (salah).
    bool colorsEnabled() { // colorsEnabled() berfungsi untuk menentukan boleh memakai warna atau tidak; hasil true berarti boleh berwarna. Dipakai oleh colorize() di bawah.
    // Aturannya mengikuti kebiasaan program terminal: bila pengguna menyetel NO_COLOR, warna mati; bila FORCE_COLOR, warna dipaksa hidup; selain itu warna hanya dipakai
    // bila keluaran benar-benar terminal, bukan berkas. Di Windows, fungsi ini sekaligus menyalakan mode "virtual terminal" agar Command Prompt memahami kode ANSI.
        // static berfungsi untuk membuat variabel yang dibuat sekali saja (saat fungsi pertama kali dipanggil) lalu nilainya diingat sampai program selesai; jadi pemeriksaan di bawah cukup sekali.
        // lambda []() { ... } adalah fungsi kecil tanpa nama yang ditulis langsung di tempat; di sini ia menghitung nilai awal enabled, karena pemeriksaannya butuh beberapa langkah if.
        static const bool enabled = []() { // enabled = []() { artinya nilai awal enabled diambil dari hasil lambda ini; const membuat nilainya tidak bisa diubah lagi.
            // if berfungsi untuk menjalankan perintah hanya bila syarat di dalam kurung bernilai benar.
            // != artinya "tidak sama dengan".
            // nullptr adalah penunjuk kosong, yaitu penunjuk yang tidak menunjuk ke mana pun.
            // return berfungsi untuk mengakhiri fungsi (di sini lambda) saat itu juga dan memberikan hasilnya.
            if (std::getenv("NO_COLOR") != nullptr) return false;  // std::getenv("NO_COLOR") berfungsi untuk membaca variabel lingkungan NO_COLOR, yaitu pengaturan berbentuk NAMA=nilai yang diwariskan
            // sistem operasi ke program; hasil nullptr berarti variabel itu tidak ada. Bila NO_COLOR ada, pengguna minta tanpa warna, jadi hasilnya false.
            if (std::getenv("FORCE_COLOR") != nullptr) return true;  // std::getenv("FORCE_COLOR") != nullptr artinya pengguna memaksa warna, jadi hasilnya true tanpa memeriksa terminal.
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
    }  // akhir fungsi colorsEnabled()

    // Tipe hasil sebuah fungsi ditulis di depan namanya; colorize() di bawah menghasilkan std::string, bukan bool seperti colorsEnabled() di atas.
    std::string colorize(const char* code, std::string_view text) { // colorize(const char* code, std::string_view text) berfungsi untuk membungkus text dengan kode warna, tetapi hanya bila warna boleh dipakai.
    // Parameter code diisi salah satu konstanta ANSI_... di atas, sedangkan text dibaca lewat std::string_view tanpa disalin. Hasilnya std::string baru, siap dicetak.
        if (!colorsEnabled()) return std::string(text);  // !colorsEnabled() artinya warna tidak boleh dipakai, jadi text dikembalikan apa adanya; std::string(text) membuat salinannya sebagai std::string.
        // + pada teks berfungsi untuk menyambung teks; code dan text diubah dulu ke std::string supaya bisa disambung.
        return std::string(code) + std::string(text) + ANSI_RESET; // std::string(code) + std::string(text) + ANSI_RESET artinya kode warna, lalu teks, lalu ANSI_RESET agar warna tidak terbawa ke teks berikutnya.
    } // akhir fungsi colorize()

    // unsigned adalah bilangan bulat tanpa tanda negatif (0 ke atas).
    bool isWideCodePoint(unsigned codePoint) { // isWideCodePoint(unsigned codePoint) berfungsi untuk memeriksa apakah sebuah code point termasuk karakter lebar, yaitu karakter yang digambar terminal selebar dua kolom.
    // Code point adalah nomor unik sebuah karakter di Unicode, daftar baku semua karakter di dunia. Aksara CJK (Cina, Jepang, Korea), Hangul (huruf Korea), dan emoji termasuk lebar;
    // bila ini diabaikan, kolom tabel bergeser untuk nama berhuruf seperti itu. Dipakai di displayWidth() di bawah.
        // && artinya "dan": hasilnya benar hanya bila kedua syarat benar. >= artinya "lebih dari atau sama dengan", dan <= artinya "kurang dari atau sama dengan".
        // Angka berawalan 0x ditulis dalam heksadesimal (basis 16). Setiap pasangan (>= awal && <= akhir) adalah satu rentang karakter lebar.
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF) // (codePoint >= 0x1100 && codePoint <= 0x115F) adalah Hangul Jamo (huruf penyusun Hangul); 0x2E80-0xA4CF adalah CJK dan aksara Asia Timur lain.
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF) // || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) adalah suku kata Hangul; 0xF900-0xFAFF adalah aksara CJK kompatibilitas.
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60) // || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) adalah bentuk kompatibilitas CJK; 0xFF00-0xFF60 adalah huruf lebar penuh.
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F) // || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) adalah tanda lebar penuh (misalnya ¥); 0x1F300-0x1F64F adalah emoji.
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); // || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF); adalah emoji tambahan. Hasilnya true bila code point masuk salah satu rentang di atas.
    } // akhir fungsi isWideCodePoint()

    std::size_t displayWidth(std::string_view text) { // displayWidth(std::string_view text) berfungsi untuk menghitung berapa kolom layar yang dipakai text; hasilnya tidak selalu sama dengan jumlah byte-nya.
    // UTF-8 menyimpan satu karakter dalam 1 sampai 4 byte, padahal huruf é atau garis kotak tetap selebar satu kolom; aksara lebar (CJK, emoji) memakai dua kolom, dan kode warna ANSI
    // memakai nol kolom. Karena itu teks dihitung byte demi byte. Dipakai di banyak tempat: pembungkus teks, kotak, pemeriksa panjang nama, dan tabel.
        std::size_t width = 0;  // width = 0 adalah jumlah kolom yang sudah terhitung, mula-mula 0.
        bool inEscape = false;  // inEscape = false artinya belum berada di dalam kode warna "\033[...m"; nilainya true selama berada di dalamnya.
        unsigned codePoint = 0; // codePoint = 0 adalah code point yang sedang dirangkai dari byte-byte UTF-8.
        int pending = 0;        // pending = 0 adalah jumlah byte lanjutan UTF-8 yang masih ditunggu untuk melengkapi code point.
        // for (elemen : kumpulan) adalah perulangan "untuk setiap" (range-based for): mengambil setiap elemen satu per satu dari awal sampai akhir, tanpa perlu indeks.
        // unsigned char adalah satu byte bernilai 0-255; char biasa bisa negatif untuk byte non-ASCII, dan itu merusak perbandingan bit di bawah.
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
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }  // (c & 0xE0) == 0xC0 artinya byte awal berpola 110xxxxx: menyusul 1 byte lanjutan, dan 5 bit data (c & 0x1F) disimpan.
            // else if berfungsi untuk memeriksa syarat berikutnya hanya bila syarat sebelumnya salah; else terakhir dijalankan bila semua syarat salah.
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }  // (c & 0xF0) == 0xE0 artinya pola 1110xxxx: menyusul 2 byte lanjutan, dan 4 bit data (c & 0x0F) disimpan.
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }  // (c & 0xF8) == 0xF0 artinya pola 11110xxx: menyusul 3 byte lanjutan, dan 3 bit data (c & 0x07) disimpan.
            else                         { pending = 0; }                        // else { pending = 0; } artinya byte ASCII biasa (pola 0xxxxxxx) yang tidak punya byte lanjutan.
        }  // akhir perulangan setiap byte
        return width; // return width; mengembalikan jumlah kolom yang terhitung.
    }  // akhir fungsi displayWidth()

    int terminalWidth() { // terminalWidth() berfungsi untuk menanyakan lebar jendela terminal (dalam kolom) setiap kali dipanggil, jadi tampilan ikut menyesuaikan bila jendela diubah ukurannya.
    // Bila terminal tidak bisa ditanya (keluaran bukan terminal), dicoba variabel lingkungan COLUMNS; bila itu juga tidak ada, dipakai 80 kolom, ukuran bawaan terminal pada umumnya.
    // Hasilnya int dan tidak pernah di bawah 40, karena di bawah 40 kolom tabel rekap tidak muat lagi; 40 cukup untuk Termux tegak di ponsel (sekitar 46 kolom). Dipakai oleh drawBox(), printError(), dan printRule() di bawah.
        int cols = 0;  // cols = 0 artinya lebar belum diketahui.
    #ifdef _WIN32 // _WIN32: cara bertanya ke terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows; tanda titik (.) berfungsi untuk mengambil salah satu anggotanya.
        CONSOLE_SCREEN_BUFFER_INFO info;  // CONSOLE_SCREEN_BUFFER_INFO info; adalah tempat menampung keterangan jendela konsol; bagian srWindow menyimpan nomor kolom kiri (Left) dan kanan (Right).
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1; // GetConsoleScreenBufferInfo(...) mengisi info lewat alamatnya (&info);
        // bila berhasil, lebar jendela = Right - Left + 1, karena kedua ujungnya termasuk.
    #else  // #else: bukan Windows (macOS atau Linux)
        // struct di depan winsize menandakan bahwa winsize adalah sebuah struct; winsize berasal dari <sys/ioctl.h>.
        struct winsize ws;  // struct winsize ws; adalah tempat menampung ukuran jendela; bagian ws_col berisi jumlah kolom.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col; // ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) berfungsi untuk meminta ukuran jendela dari saluran keluaran layar (STDOUT_FILENO)
        // dan mengisinya ke ws lewat alamatnya (&ws); hasil 0 berarti berhasil, lalu cols diisi jumlah kolom.
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // std::atoi berfungsi untuk mengubah teks angka, misalnya "100", menjadi bilangan int 100 (0 bila bukan angka).
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); } // cols <= 0 artinya lebar masih belum diketahui, jadi dicoba variabel lingkungan COLUMNS;
        // env menunjuk ke teksnya, atau bernilai nullptr bila COLUMNS tidak ada.
        if (cols <= 0) cols = 80;  // cols = 80 adalah lebar bawaan bila lebar tetap belum diketahui.
        // std::max berfungsi untuk memilih yang lebih besar dari dua nilai (berasal dari <algorithm>).
        return std::max(40, cols);  // std::max(40, cols) artinya hasilnya tidak pernah di bawah 40 kolom.
    }  // akhir fungsi terminalWidth()

    // std::vector<std::string> adalah daftar berurutan berisi teks; di sini menjadi tipe hasil fungsi, satu elemen untuk satu baris. Tipe isi vector ditulis di antara < >.
    // const std::string& adalah referensi tetap (const&): tanda & membuat text menjadi nama lain untuk variabel milik pemanggil, jadi teksnya tidak disalin, dan const membuat teks itu tidak boleh diubah.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) { // wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent)
    // berfungsi untuk memotong text di spasi menjadi beberapa baris yang masing-masing muat dalam maxWidth kolom, supaya terminal tidak memotongnya sembarangan.
    // Baris lanjutan diberi indentasi gantung sebanyak hangingIndent spasi, agar sejajar dengan isi setelah label. Dipakai drawBox() dan printError() di bawah.
    // Bila tidak ada spasi yang pas, dipakai cadangan: spasi setelah teks dimulai (lanjutannya menjorok 2 spasi), atau kata yang terlalu panjang dipotong tepat di batas lebar.
        std::vector<std::string> result;  // result adalah daftar baris hasil, mula-mula kosong.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2; // hangingIndent + 10 > maxWidth artinya jendela terlalu sempit untuk indentasi gantung + 10 kolom isi, jadi indentasi dikecilkan menjadi 2 spasi.
        std::string remaining = text;  // remaining adalah sisa teks yang belum dipotong; disalin dari text karena isinya akan diganti di setiap putaran.
        // while berfungsi untuk mengulang isi blok selama syarat di dalam kurung masih benar.
        while (displayWidth(remaining) > maxWidth) { // displayWidth(remaining) > maxWidth artinya sisa teks masih lebih lebar dari maxWidth, jadi potong satu baris lagi dari depannya.
            std::size_t col = 0, cutPos = std::string::npos; // col menghitung kolom layar, dan cutPos menyimpan posisi spasi tempat memotong; std::string::npos adalah nilai khusus yang berarti "tidak ditemukan" (belum ada spasi).
            std::size_t looseCutPos = std::string::npos, hardCutPos = std::string::npos; // looseCutPos dan hardCutPos adalah dua titik potong cadangan, mula-mula std::string::npos (belum ada):
            // looseCutPos = spasi terakhir setelah teks dimulai, hardCutPos = posisi byte tempat batas lebar tercapai. Keduanya dipakai setelah perulangan for di bawah.
            bool seenText = false; // seenText = false: nilainya menjadi true begitu bertemu karakter pertama yang bukan spasi, supaya spasi indentasi di depan baris tidak dipakai sebagai titik potong.
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
                // break berfungsi untuk keluar dari perulangan saat itu juga.
                if (col >= maxWidth) { hardCutPos = i; break; } // col >= maxWidth artinya batas lebar tercapai: hardCutPos = i mencatat posisi byte ini untuk potongan paksa, lalu break berhenti mencari.
                if (c == ' ' && col > hangingIndent) cutPos = i;  // c == ' ' && col > hangingIndent artinya spasi ini menjadi calon titik potong, asalkan letaknya setelah indentasi gantung;
                // yang akhirnya tersimpan adalah spasi terakhir sebelum batas lebar.
                if (c == ' ' && seenText) looseCutPos = i; // c == ' ' && seenText artinya spasi ini ada setelah teks dimulai, jadi dicatat sebagai titik potong cadangan looseCutPos.
                if (c != ' ') seenText = true; // c != ' ' artinya karakter ini bukan spasi, jadi seenText = true: teks di baris ini sudah dimulai.
                ++col;  // ++col; artinya karakter ini memakai satu kolom.
            }  // akhir perulangan mencari titik potong
            // Bila tidak ada spasi setelah indentasi gantung (misalnya angka hasil yang sangat panjang setelah label), dipakai spasi cadangan.
            if (cutPos == std::string::npos && looseCutPos != std::string::npos) { cutPos = looseCutPos; hangingIndent = 2; } // cutPos == std::string::npos && looseCutPos != std::string::npos
            // artinya hanya ada spasi cadangan: cutPos = looseCutPos memotong di sana, dan hangingIndent = 2 membuat lanjutannya cukup menjorok 2 spasi supaya isinya muat.
            if (cutPos != std::string::npos) { // cutPos != std::string::npos artinya ada spasi untuk memotong: spasinya dibuang dan sisa teks pindah ke baris berikutnya.
                result.push_back(remaining.substr(0, cutPos)); // remaining.substr(0, cutPos) mengambil teks dari awal sampai sebelum spasi, lalu push_back menambahkannya ke ujung result.
                remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1); // std::string(hangingIndent, ' ') adalah hangingIndent buah spasi, disambung dengan sisa teks mulai setelah spasi (cutPos + 1).
            } else if (hardCutPos != std::string::npos && maxWidth > hangingIndent) { // hardCutPos != std::string::npos && maxWidth > hangingIndent artinya tidak ada spasi sama sekali, jadi satu kata
            // lebih panjang dari satu baris; kata itu dipotong tepat di batas lebar. Syarat maxWidth > hangingIndent menjamin setiap putaran memotong sedikitnya satu huruf, jadi perulangan pasti selesai.
                result.push_back(remaining.substr(0, hardCutPos)); // remaining.substr(0, hardCutPos) mengambil teks dari awal sampai batas lebar, lalu push_back menambahkannya ke ujung result.
                remaining = std::string(hangingIndent, ' ') + remaining.substr(hardCutPos); // remaining.substr(hardCutPos) adalah sisa kata mulai tepat di batas lebar; tidak ada huruf yang dibuang.
            } else { // else artinya tidak ada titik potong yang aman sama sekali.
                break; // break; keluar dari while dan membiarkan sisa teks apa adanya.
            } // akhir pemilihan titik potong
        }  // akhir perulangan while
        result.push_back(remaining);  // result.push_back(remaining); artinya sisa terakhir sudah muat, jadi ia menjadi baris terakhir.
        return result; // return result; mengembalikan daftar baris hasil.
    }  // akhir fungsi wrapText()

    void drawBox(std::string_view title, const std::vector<std::string>& lines) { // drawBox(std::string_view title, const std::vector<std::string>& lines) berfungsi untuk menggambar kotak berjudul title
    // berisi baris-baris lines. Kotak membuat petunjuk dan rekap hasil terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah
    // melebihi lebar terminal; baris yang terlalu panjang dibungkus dengan wrapText(), dan baris berpola "Label : isi" dibuat sejajar dengan isi setelah label.
    // Garis kotak ditulis sebagai kode Unicode di dalam string: "\u250c" = ┌, "\u2510" = ┐, "\u2514" = └, "\u2518" = ┘, "\u2500" = ─ (garis datar), dan "\u2502" = │ (garis tegak).
    // Dipakai oleh printInstructions() di BAGIAN 5 dan run() di BAGIAN 6.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5); // terminalWidth() - 5 adalah lebar isi terbesar: lebar terminal dikurangi 2 tepi kotak, 2 margin, dan 1 kolom cadangan;
        // kolom cadangan perlu karena sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela. static_cast<std::size_t> mengubah hasil int menjadi std::size_t.
        // Judul yang lebih lebar dari garis atas (bisa terjadi di terminal sempit) dilipat per kata: potongan pertama di garis atas, sisanya di baris isi pertama.
        const std::vector<std::string> titleLines = wrapText(std::string(title), maxWidth - 2, 0); // wrapText(std::string(title), maxWidth - 2, 0) melipat judul tanpa indentasi gantung;
        // maxWidth - 2 karena garis atas "┌─ judul ─┐" butuh 4 kolom di luar judul, sedangkan lebar dalam kotak paling besar maxWidth + 2. Judul yang muat tetap satu potongan.
        const std::string& titleTop = titleLines[0]; // titleLines[0] adalah potongan judul pertama (titleTop), yang ditulis di garis atas kotak.
        std::vector<std::string> contentLines;  // contentLines adalah baris isi setelah dibungkus, siap digambar; bisa lebih banyak dari lines.
        for (std::size_t i = 1; i < titleLines.size(); ++i) contentLines.push_back(" " + colorize(ANSI_TITLE, titleLines[i])); // titleLines.size() adalah jumlah potongan judul; mulai dari i = 1 artinya potongan ke-2 dan seterusnya (bila ada)
        // menjadi baris isi pertama, cyan tebal seperti judul, dan didahului 1 spasi supaya lurus dengan judul di garis atas.
        for (const std::string& b : lines) { // for (const std::string& b : lines) artinya ambil setiap baris b di lines tanpa menyalinnya, lalu tentukan indentasi gantungnya dan bungkus dengan wrapText().
            std::size_t hangingIndent = 2;  // hangingIndent = 2 adalah bawaan: baris lanjutan menjorok 2 spasi.
            const std::size_t labelPos = b.find(" : ");  // b.find(" : ") mencari posisi " : " pertama, atau std::string::npos bila tidak ada.
            const std::size_t leadingSpaces = b.find_first_not_of(' '); // b.find_first_not_of(' ') berfungsi untuk mencari posisi karakter pertama yang bukan spasi, yaitu banyaknya spasi di depan baris;
            // hasilnya std::string::npos bila barisnya kosong atau hanya berisi spasi.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) { // labelPos != std::string::npos && ... <= 28 artinya ada " : " dan label di depannya paling lebar 28 kolom,
            // jadi baris ini berpola "Label : isi"; label yang lebih lebar akan membuat lanjutan menjorok terlalu jauh, jadi tetap 2 spasi.
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // displayWidth(b.substr(0, labelPos)) + 3 artinya lebar label ditambah 3 untuk " : ", jadi lanjutannya sejajar dengan isi setelah " : ".
            } else if (leadingSpaces != std::string::npos && leadingSpaces > 2) { // leadingSpaces != std::string::npos && leadingSpaces > 2 artinya bukan pola "Label : isi", tetapi barisnya
            // sudah menjorok lebih dari 2 spasi (misalnya keterangan menu atau contoh di kotak petunjuk).
                hangingIndent = leadingSpaces; // hangingIndent = leadingSpaces artinya lanjutannya menjorok sejauh baris itu sendiri, supaya tetap lurus di bawahnya.
            } // akhir pemeriksaan pola "Label : isi" dan indentasi baris
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece); // wrapText(b, maxWidth, hangingIndent) memotong b, lalu setiap potongan (piece) dimasukkan ke contentLines.
        }  // akhir perulangan membungkus baris isi
        std::size_t width = displayWidth(titleTop) + 4;  // displayWidth(titleTop) + 4 adalah lebar dalam minimal: "─ " sebelum judul (2), spasi setelah judul (1), dan paling sedikit satu ─ pengisi (1).
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2); // std::max(width, displayWidth(b) + 2) artinya kotak diperlebar sampai baris terpanjang + 2, yaitu margin kiri dan kanan.
        // std::min berfungsi untuk memilih yang lebih kecil dari dua nilai (berasal dari <algorithm>).
        width = std::min(width, maxWidth + 2);  // std::min(width, maxWidth + 2) artinya lebar kotak tidak boleh lebih dari maxWidth + 2 margin.
        std::string filler;  // filler adalah garis pengisi di kanan judul, mula-mula kosong.
        // += berfungsi untuk menambahkan teks ke ujung teks yang sudah ada.
        for (std::size_t i = displayWidth(titleTop) + 3; i < width; ++i) filler += "\u2500"; // filler += "\u2500" artinya tambahkan ─ sebanyak sisa lebar setelah "─ judul ".
        // std::cout << berfungsi untuk mencetak ke layar; beberapa << berurutan mencetak bagian-bagiannya satu per satu dari kiri ke kanan (berbeda dengan << 6 di displayWidth(), yang menggeser bit).
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, titleTop) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n'; // colorize(ANSI_DIM, "\u250c\u2500 ") ... adalah garis atas ┌─ judul ───┐:
        // garisnya abu-abu dan judulnya cyan tebal; '\n' adalah karakter pindah baris.
        for (const std::string& b : contentLines) { // for (const std::string& b : contentLines) artinya cetak setiap baris isi: │, teks, spasi pengisi sampai lebar kotak, lalu │ lagi.
            const std::string rowText = " " + b;  // " " + b artinya satu spasi margin kiri di depan baris.
            const std::size_t contentWidth = displayWidth(rowText);  // contentWidth adalah lebar rowText di layar, tanpa kode warna.
            // Operator ternary (a ? b : c) artinya "bila a benar pakai b, bila tidak pakai c". Di baris ini ia menjaga jumlah spasi pengisi paling sedikit 0, karena std::size_t
            // tidak bisa negatif: pengurangan yang hasilnya di bawah 0 akan berubah menjadi angka raksasa.
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ') // width > contentWidth ? width - contentWidth : 0 adalah jumlah spasi pengisi.
                      << colorize(ANSI_DIM, "\u2502") << '\n'; // colorize(ANSI_DIM, "\u2502") adalah │ garis tegak kanan, lalu '\n' pindah baris.
        }  // akhir perulangan baris isi
        std::string bottomLine;  // bottomLine adalah garis bawah, mula-mula kosong.
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";  // bottomLine += "\u2500" artinya tambahkan ─ sebanyak lebar kotak.
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n'; // "\u2514" + bottomLine + "\u2518" adalah garis bawah └───┘ berwarna abu-abu.
    }  // akhir fungsi drawBox()

    void printError(const std::string& text) { // printError(const std::string& text) berfungsi untuk mencetak pesan kesalahan berwarna merah yang dibungkus sesuai lebar terminal, agar pesan panjang
    // tidak terpotong jendela sempit. Pesannya selalu diawali "  [!] ", jadi baris lanjutan menjorok 6 spasi agar sejajar dengan teks setelah tanda "[!]".
    // Dipakai oleh fungsi-fungsi pembaca input di BAGIAN 4 (kecuali readLine(), yang mengirim pesannya ke std::cerr).
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) { // wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6) artinya lebar bungkus = lebar terminal
        // dikurangi 1 kolom cadangan, dan 6 = indentasi gantung.
            std::cout << colorize(ANSI_ERROR, b) << '\n'; // colorize(ANSI_ERROR, b) artinya setiap baris hasil bungkus dicetak merah tebal.
        } // akhir perulangan baris pesan
    } // akhir fungsi printError()

    // char adalah tipe satu karakter, ditulis di antara petik tunggal, misalnya '='.
    void printRule(char ch) { // printRule(char ch) berfungsi untuk mencetak satu garis pembatas abu-abu sepanjang TITLE_WIDTH (60) karakter ch, atau selebar terminal dikurangi 1 kolom cadangan bila terminalnya lebih sempit. Dipakai oleh printTitle() di BAGIAN 5.
        const std::size_t ruleWidth = static_cast<std::size_t>(std::min(TITLE_WIDTH, terminalWidth() - 1)); // ruleWidth = std::min(TITLE_WIDTH, terminalWidth() - 1) adalah panjang garis: yang lebih kecil antara 60 dan lebar terminal dikurangi 1 kolom cadangan, supaya garis tidak melipat di terminal sempit (misalnya 46 kolom di Termux); static_cast<std::size_t> mengubah hasil int itu menjadi std::size_t, tipe panjang yang diminta std::string.
        std::cout << colorize(ANSI_DIM, std::string(ruleWidth, ch)) << '\n'; // std::string(ruleWidth, ch) membuat teks berisi ch diulang ruleWidth kali, lalu dicetak abu-abu dan diakhiri pindah baris.
    } // akhir fungsi printRule()

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Input dibaca di banyak tempat: mata kuliah, jumlah, nama, nilai. Daripada menulis kode pembacaan dan pemeriksaan yang sama berulang-ulang, kita pecah menjadi
    // fungsi-fungsi kecil di bagian ini. Polanya selalu sama: baca satu baris sebagai teks, rapikan, periksa; kalau salah, beri pesan dan tanya lagi.

    std::string readLine(std::string_view prompt) { // readLine(std::string_view prompt) berfungsi untuk menampilkan prompt (teks pertanyaan, dicetak tebal) lalu membaca satu baris ketikan pengguna;
    // ini dasar dari semua pembaca input di bawah. Hasilnya std::string.
        std::string line;  // line menampung satu baris ketikan; panjangnya tidak pasti, jadi std::string.
        std::cout << colorize(ANSI_BOLD, prompt);  // colorize(ANSI_BOLD, prompt) mencetak prompt tebal supaya pertanyaan mudah dibedakan dari keterangan.
        // std::getline berfungsi untuk membaca satu baris ketikan utuh sampai Enter, termasuk spasi. Dipakai, bukan std::cin >>, karena seluruh baris perlu diperiksa: cin >> berhenti di spasi atau
        // di karakter yang bukan angka (misalnya 2.5 terbaca 2), dan sisa ketikannya tertinggal lalu ikut terbaca oleh pertanyaan berikutnya.
        if (!std::getline(std::cin, line)) { // !std::getline(std::cin, line) artinya pembacaan dari std::cin (masukan keyboard) gagal karena input sudah ditutup: pengguna menekan Ctrl+D (macOS/Linux)
        // atau Ctrl+Z lalu Enter (Windows), atau input dialihkan dari berkas dan isinya sudah habis. Tanpa pemeriksaan ini, pertanyaan yang sama akan diulang tanpa henti.
            // std::cerr berfungsi untuk mencetak pesan galat lewat saluran khusus yang terpisah dari std::cout.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n"; // "\nInput berakhir sebelum data lengkap. Program dihentikan.\n" adalah pesan bahwa input sudah habis; \n di dalam string berarti pindah baris.
            // std::exit berfungsi untuk menghentikan seluruh program saat itu juga, dari fungsi mana pun.
            std::exit(EXIT_FAILURE);  // std::exit(EXIT_FAILURE) artinya hentikan program dengan kode keluar EXIT_FAILURE ("gagal") ke sistem operasi.
        } // akhir penanganan input yang berakhir
        return line; // return line; mengembalikan baris ketikan apa adanya; perapian spasi dilakukan normalizeSpaces() di bawah.
    }  // akhir fungsi readLine()

    std::string normalizeSpaces(const std::string& text) { // normalizeSpaces(const std::string& text) berfungsi untuk membuang spasi/tab di awal dan akhir ("  Budi  " menjadi "Budi") dan merapatkan
    // spasi beruntun di tengah menjadi satu spasi, karena ketikan pengguna sering membawa spasi tak sengaja. Dipakai oleh semua pembaca input di bawah.
        std::string result;  // result adalah teks yang sudah dirapikan, mula-mula kosong.
        bool pendingSpace = false;  // pendingSpace = false artinya belum ada spasi yang menunggu untuk ditulis.
        for (const char c : text) {  // for (const char c : text) artinya periksa setiap karakter c di text satu per satu.
            if (c == ' ' || c == '\t' || c == '\r') { // c == ' ' || c == '\t' || c == '\r' artinya karakter ini spasi, tab ('\t'), atau CR ('\r', sisa akhir baris Windows): tidak langsung ditulis, hanya ditandai.
                pendingSpace = !result.empty();  // !result.empty() artinya true hanya bila result sudah berisi; jadi spasi di awal teks diabaikan. empty() bernilai true bila panjangnya 0.
            } else { // else artinya karakter ini bukan spasi.
                if (pendingSpace) result += ' ';  // if (pendingSpace) result += ' '; artinya spasi beruntun yang tertunda ditulis sebagai satu spasi saja.
                result += c;  // result += c; menambahkan karakter c ke akhir result.
                pendingSpace = false; // pendingSpace = false; artinya spasi yang tertunda sudah selesai ditangani.
            } // akhir cabang bukan spasi
        }  // akhir perulangan setiap karakter
        return result;  // return result; mengembalikan teks rapi; spasi di akhir otomatis terbuang karena tidak pernah ditulis.
    }  // akhir fungsi normalizeSpaces()

    bool isAllDigits(const std::string& text) { // isAllDigits(const std::string& text) berfungsi untuk memeriksa apakah teks hanya berisi angka 0-9, tanpa tanda, titik, huruf, atau spasi.
    // Dipakai oleh isDecimalFormat() dan readInteger() di bawah.
        if (text.empty()) return false;  // text.empty() artinya teks kosong; teks kosong bukan bilangan, jadi hasilnya false.
        for (const char c : text) { // for (const char c : text) artinya periksa setiap karakter c di text.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false; // !std::isdigit(static_cast<unsigned char>(c)) artinya c bukan digit, jadi langsung false; static_cast dipakai karena std::isdigit
            // hanya aman untuk nilai 0-255, sedangkan char bisa negatif untuk byte non-ASCII.
        } // akhir perulangan setiap karakter
        return true;  // return true; artinya semua karakter lolos sebagai digit.
    } // akhir fungsi isAllDigits()

    bool isDecimalFormat(const std::string& text) { // isDecimalFormat(const std::string& text) berfungsi untuk memeriksa apakah teks berbentuk bilangan desimal yang wajar: angka, boleh diikuti SATU pemisah
    // titik atau koma lalu angka lagi (87, 87.5, atau 87,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 87 5 ditolak. Dipakai oleh parseDecimal() di bawah.
        // auto berfungsi untuk membiarkan compiler menentukan tipe variabel sendiri dari nilainya; di sini tipenya std::size_t.
        const auto separator = text.find_first_of(".,"); // text.find_first_of(".,") mencari posisi titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        if (separator == std::string::npos) return isAllDigits(text);  // separator == std::string::npos artinya tanpa pemisah, jadi teks harus seluruhnya digit.
        return isAllDigits(text.substr(0, separator)) // isAllDigits(text.substr(0, separator)) artinya bagian sebelum pemisah harus digit semua,
            && isAllDigits(text.substr(separator + 1)); // && isAllDigits(text.substr(separator + 1)); artinya bagian sesudah pemisah juga; pemisah kedua otomatis tertolak karena ikut berada di bagian sesudahnya.
    } // akhir fungsi isDecimalFormat()

    bool isValidUtf8(const std::string& text) { // isValidUtf8(const std::string& text) berfungsi untuk memeriksa apakah teks berisi UTF-8 yang sah: setiap karakter non-ASCII terdiri dari satu byte awal (0xC2-0xF4)
    // diikuti 1-3 byte lanjutan (0x80-0xBF). Byte acak seperti 0xFF bukan huruf dan bukan UTF-8, jadi ditolak; huruf seperti é atau ö tetap diterima. Dipakai oleh dua pemeriksa nama di bawah.
        std::size_t i = 0;  // i = 0 adalah indeks byte yang sedang diperiksa, mulai dari 0.
        while (i < text.size()) {  // while (i < text.size()) artinya ulangi selama masih ada byte yang belum diperiksa; size() = jumlah byte.
            const unsigned char lead = static_cast<unsigned char>(text[i]);  // lead adalah byte ke-i dibaca sebagai 0-255; ia calon byte awal sebuah karakter.
            std::size_t extra = 0;  // extra = 0 adalah jumlah byte lanjutan yang harus mengikuti; nilainya ditentukan dari rentang lead di bawah.
            if (lead < 0x80)                       extra = 0;    // lead < 0x80 artinya ASCII biasa: tanpa byte lanjutan.
            else if (lead >= 0xC2 && lead <= 0xDF) extra = 1;    // lead >= 0xC2 && lead <= 0xDF artinya karakter 2 byte, misalnya é ö ü.
            else if (lead >= 0xE0 && lead <= 0xEF) extra = 2;    // lead >= 0xE0 && lead <= 0xEF artinya karakter 3 byte, misalnya aksara Asia.
            else if (lead >= 0xF0 && lead <= 0xF4) extra = 3;    // lead >= 0xF0 && lead <= 0xF4 artinya karakter 4 byte, misalnya emoji.
            else return false;                                   // else return false; artinya 0x80-0xC1 dan 0xF5-0xFF bukan byte awal yang sah.
            if (i + extra >= text.size()) return false;  // i + extra >= text.size() artinya byte lanjutan terpotong di akhir teks, jadi tidak sah.
            for (std::size_t k = 1; k <= extra; ++k) {  // for (std::size_t k = 1; k <= extra; ++k) artinya periksa setiap byte lanjutan ke-1 sampai ke-extra.
                const unsigned char cont = static_cast<unsigned char>(text[i + k]); // cont adalah byte lanjutan ke-k setelah lead, dibaca sebagai 0-255.
                if (cont < 0x80 || cont > 0xBF) return false;  // cont < 0x80 || cont > 0xBF artinya byte lanjutan di luar pola 10xxxxxx, jadi tidak sah.
            } // akhir perulangan byte lanjutan
            i += extra + 1;  // i += extra + 1; artinya i = i + (extra + 1): pada angka, += menambah nilai variabel; di sini i lompat ke byte awal karakter berikutnya.
        }  // akhir perulangan while
        return true; // return true; artinya seluruh teks lolos sebagai UTF-8 sah.
    }  // akhir fungsi isValidUtf8()

    bool isValidCourseName(const std::string& text) { // isValidCourseName(const std::string& text) berfungsi untuk memeriksa apakah teks layak jadi nama mata kuliah: harus mengandung huruf; hanya boleh berisi huruf,
    // angka (Kalkulus 2), spasi, dan tanda titik, koma, &, /, kurung, tanda hubung, atau petik; tidak lebih panjang dari COURSE_NAME_MAX_LENGTH. Dipakai oleh readCourseName() di bawah.
        if (text.empty() || displayWidth(text) > COURSE_NAME_MAX_LENGTH || !isValidUtf8(text)) return false; // text.empty() || displayWidth(text) > COURSE_NAME_MAX_LENGTH || !isValidUtf8(text) artinya tiga alasan gugur langsung:
        // kosong, lebih dari COURSE_NAME_MAX_LENGTH (60) kolom, atau bukan UTF-8 sah.
        bool hasLetter = false;  // hasLetter = false artinya belum ditemukan satu huruf pun.
        for (const char c : text) { // for (const char c : text) artinya periksa setiap karakter c di text.
            const unsigned char uc = static_cast<unsigned char>(c);  // uc adalah c dalam bentuk 0-255 supaya aman untuk std::isalpha dan std::isdigit.
            if (std::isalpha(uc) || uc >= 0x80) { hasLetter = true; continue; } // std::isalpha(uc) || uc >= 0x80 artinya huruf: std::isalpha hanya mengenal ASCII, jadi byte >= 0x80 (bagian huruf UTF-8 seperti é, ö)
            // dianggap huruf juga; tandai hasLetter, lalu lanjut ke karakter berikutnya.
            if (std::isdigit(uc) || c == ' ' || c == '.' || c == ',' || c == '&' || c == '/' // std::isdigit(uc) || c == ' ' || ... artinya angka atau salah satu tanda yang diizinkan,
                || c == '(' || c == ')' || c == '-' || c == '\'') continue; // || c == '(' || c == ')' || c == '-' || c == '\'' melengkapi daftar tanda yang diizinkan ('\'' adalah karakter petik tunggal), lalu lewati.
            return false;  // return false; artinya karakter lain (misalnya #, @, !) bukan bagian nama mata kuliah.
        }  // akhir perulangan setiap karakter
        return hasLetter;  // return hasLetter; artinya sah hanya bila ada huruf; "123" ditolak karena tidak mengandung huruf sama sekali.
    }  // akhir fungsi isValidCourseName()

    std::string readCourseName(std::string_view prompt) { // readCourseName(std::string_view prompt) berfungsi untuk meminta nama mata kuliah dan mengulang pertanyaannya sampai lolos isValidCourseName() di atas.
    // Dipanggil oleh run() di BAGIAN 6.
        // while (true) berfungsi untuk mengulang tanpa henti, karena syaratnya selalu benar; jalan keluarnya hanya lewat return.
        while (true) {  // while (true) { artinya tanya terus sampai input sah.
            const std::string text = normalizeSpaces(readLine(prompt));  // normalizeSpaces(readLine(prompt)) membaca satu baris lalu merapikan spasinya; const artinya text tidak diubah lagi.
            if (isValidCourseName(text)) return text; // isValidCourseName(text) artinya teks sah, jadi langsung dikembalikan sebagai hasil.
            // std::to_string berfungsi untuk mengubah angka menjadi teks, supaya bisa disambung dengan + ke teks lain.
            printError("  [!] Nama Mata Kuliah harus mengandung huruf dan hanya boleh berisi huruf, angka, spasi," // printError("  [!] Nama Mata Kuliah harus ... adalah pesan kesalahan; teksnya dipecah ke tiga baris. Teks literal di baris ini dan di awal baris berikutnya berdampingan tanpa +, jadi digabung compiler menjadi satu.
                                        " atau tanda . , & / ( ) - ' (maksimal " + std::to_string(COURSE_NAME_MAX_LENGTH) // " atau tanda . , & / ( ) - ' (maksimal " + std::to_string(COURSE_NAME_MAX_LENGTH) menyambung batas 60 sebagai teks;
                                        + " karakter). Silakan ulangi."); // + " karakter). Silakan ulangi."); menyambung ujung pesan dengan +, lalu ) menutup pemanggilan printError().
        }  // akhir perulangan while (true)
    }  // akhir fungsi readCourseName()

    bool isValidPersonName(const std::string& name) { // isValidPersonName(const std::string& name) berfungsi untuk memeriksa apakah teks layak jadi nama orang: hanya huruf, spasi, titik (A. Rizky), tanda hubung
    // (Anne-Marie), atau tanda petik (Nur'aini); harus diawali huruf dan tidak terlalu panjang. Ketikan seperti "Luthfi 87.5" atau "123" ditolak, karena angka dan simbol bukan bagian nama.
    // Dipakai oleh readPersonName() di bawah.
        if (name.empty() || displayWidth(name) > NAME_MAX_LENGTH || !isValidUtf8(name)) return false; // name.empty() || displayWidth(name) > NAME_MAX_LENGTH || !isValidUtf8(name) artinya tiga alasan gugur langsung:
        // kosong, lebih dari NAME_MAX_LENGTH (60) kolom, atau bukan UTF-8 sah.
        const unsigned char first = static_cast<unsigned char>(name.front());  // name.front() adalah karakter pertama; first menyimpannya dalam bentuk 0-255.
        if (!std::isalpha(first) && first < 0x80) return false; // !std::isalpha(first) && first < 0x80 artinya karakter pertama bukan huruf ASCII dan bukan byte UTF-8, jadi nama tidak diawali huruf: tolak.
        for (const char c : name) { // for (const char c : name) artinya periksa setiap karakter c di name.
            const unsigned char uc = static_cast<unsigned char>(c);  // uc adalah c dalam bentuk 0-255 supaya aman untuk std::isalpha.
            const bool isLetter = std::isalpha(uc) != 0 || uc >= 0x80;  // isLetter artinya c huruf ASCII (std::isalpha bukan 0) atau byte UTF-8 (>= 0x80) dari huruf non-ASCII.
            const bool isAllowedMark = (c == ' ' || c == '.' || c == '-' || c == '\'');  // isAllowedMark artinya c salah satu tanda yang diizinkan: spasi, titik, tanda hubung, atau petik tunggal.
            if (!isLetter && !isAllowedMark) return false;  // !isLetter && !isAllowedMark artinya angka atau simbol lain: bukan nama, jadi tolak.
        }  // akhir perulangan setiap karakter
        return true; // return true; artinya semua karakter lolos.
    }  // akhir fungsi isValidPersonName()

    std::string readPersonName(std::string_view prompt) { // readPersonName(std::string_view prompt) berfungsi untuk meminta nama mahasiswa dan mengulang pertanyaannya sampai lolos isValidPersonName() di atas.
    // Dipanggil oleh run() di BAGIAN 6.
        while (true) {  // while (true) { artinya tanya terus sampai nama sah; keluar lewat return.
            const std::string name = normalizeSpaces(readLine(prompt));  // normalizeSpaces(readLine(prompt)) membaca satu baris lalu merapikan spasinya.
            if (isValidPersonName(name)) return name; // isValidPersonName(name) artinya nama sah, jadi langsung dikembalikan.
            printError("  [!] Nama hanya boleh berisi huruf, spasi, titik, tanda hubung (-), atau tanda petik ('),"  // printError("  [!] Nama hanya boleh berisi huruf, ... adalah pesan kesalahan yang dipecah ke tiga baris;
                                        " diawali huruf, maksimal " + std::to_string(NAME_MAX_LENGTH) // " diawali huruf, maksimal " + std::to_string(NAME_MAX_LENGTH) menyambung batas 60 sebagai teks;
                                        + " karakter. Silakan ulangi."); // + " karakter. Silakan ulangi."); menutup pesan.
        }  // akhir perulangan while (true)
    }  // akhir fungsi readPersonName()

    int readInteger(std::string_view prompt, int minValue, int maxValue) { // readInteger(std::string_view prompt, int minValue, int maxValue) berfungsi untuk meminta bilangan bulat dari minValue sampai maxValue
    // (keduanya termasuk); di program ini dipakai untuk jumlah mahasiswa. Dipanggil oleh run() di BAGIAN 6.
        while (true) {  // while (true) { artinya tanya terus sampai input sah.
            const std::string text = normalizeSpaces(readLine(prompt)); // normalizeSpaces(readLine(prompt)) membaca ketikan sebagai teks dulu, bukan langsung std::cin >> angka, karena bila std::cin >> angka
            // menerima huruf, std::cin masuk keadaan gagal dan menolak semua input berikutnya sampai dipulihkan dengan std::cin.clear(). Lewat teks, ketikan salah (abc, 2.5) ditolak dengan aman.
            // std::istringstream adalah aliran (stream) masukan dari teks: kita membaca darinya dengan >>, persis seperti membaca dari std::cin.
            std::istringstream parser(text); // parser(text) adalah aliran yang isinya teks ketikan, siap dibaca sebagai angka.
            int number = 0;  // number = 0 adalah tempat angka hasil baca; diberi 0 supaya tidak berisi nilai acak.
            // >> pada aliran berfungsi untuk mengambil nilai dari aliran ke variabel; hasilnya gagal bila teksnya tidak bisa dibaca sebagai tipe itu.
            if (isAllDigits(text) && parser >> number && (parser >> std::ws).eof() && number >= minValue && number <= maxValue) { // isAllDigits(text) && parser >> number && (parser >> std::ws).eof() && ... artinya
            // sah bila lolos lima pemeriksaan: hanya digit 0-9 (jadi +3, 3.0, 1e3, dan "3 orang" tertolak); angkanya terbaca (gagal bila terlalu besar untuk int);
            // std::ws membuang spasi sisa dan eof() true bila tidak ada sisa lagi; number >= minValue; dan number <= maxValue.
                return number; // return number; mengembalikan angka yang sah.
            } // akhir pemeriksaan angka yang sah
            printError("  [!] Harus bilangan bulat " + std::to_string(minValue) + "-" + std::to_string(maxValue) // printError("  [!] Harus bilangan bulat " + std::to_string(minValue) + "-" + std::to_string(maxValue) menyusun pesan dengan batas minValue-maxValue sebagai teks (di program ini 1-1000, dari pemanggilan di run() BAGIAN 6),
                                        + " (hanya angka). Silakan ulangi."); // + " (hanya angka). Silakan ulangi."); menutup pesan.
        }  // akhir perulangan while (true)
    }  // akhir fungsi readInteger()

    // double& adalah referensi ke variabel double milik pemanggil: fungsi menulis langsung ke variabel itu, bukan ke salinannya.
    bool parseDecimal(std::string text, double& result) { // parseDecimal(std::string text, double& result) berfungsi untuk mengubah teks menjadi bilangan desimal; hasilnya true bila teksnya sah, dan angkanya
    // dikirim lewat result, karena nilai kembalian sudah dipakai untuk status sah/tidak. Parameter text adalah salinan, jadi boleh diubah di dalam fungsi. Dipakai oleh readDecimal() di bawah.
        if (!isDecimalFormat(text)) return false;  // !isDecimalFormat(text) artinya format diperiksa dulu: bukan tulisan angka yang wajar, jadi false.
        // char& adalah referensi ke satu karakter di dalam teks, jadi mengubah c berarti mengubah karakter itu langsung pada teksnya.
        for (char& c : text) { // for (char& c : text) artinya kunjungi setiap karakter text dan boleh mengubahnya.
            if (c == ',') c = '.'; // c == ',' artinya koma; diganti titik karena C++ hanya mengenal penulisan 87.5.
        }  // akhir perulangan mengganti koma
        std::istringstream parser(text);  // parser(text) adalah aliran untuk membaca angka dari teks.
        return (parser >> result) && (parser >> std::ws).eof(); // (parser >> result) membaca angka desimal ke result, dan (parser >> std::ws).eof() memastikan tidak ada sisa; keduanya harus benar.
    } // akhir fungsi parseDecimal()

    double readDecimal(std::string_view prompt, double minValue, double maxValue) { // readDecimal(std::string_view prompt, double minValue, double maxValue) berfungsi untuk meminta nilai desimal dari minValue
    // sampai maxValue (keduanya termasuk); koma (87,5) diterima. Dipanggil oleh run() di BAGIAN 6 untuk nilai mahasiswa.
        while (true) {  // while (true) { artinya tanya terus sampai nilai sah; keluar lewat return.
            double score = 0.0;  // score = 0.0 adalah tempat nilai hasil baca, dengan nilai awal yang jelas.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), score) && score >= minValue && score <= maxValue) { // parseDecimal(normalizeSpaces(readLine(prompt)), score) mengisi score lewat referensi dan
            // mengembalikan true bila teks sah; && score >= minValue && score <= maxValue lalu memastikan score berada di antara minValue dan maxValue (di program ini 0-100).
                return score; // return score; mengembalikan nilai yang sah.
            } // akhir pemeriksaan nilai yang sah
            // std::ostringstream adalah aliran keluaran ke teks: << menulis angka dan teks ke dalamnya, dan str() mengambil hasilnya sebagai std::string.
            std::ostringstream range; // range adalah aliran untuk menyusun teks batas, karena std::to_string untuk double selalu menulis 6 angka desimal ("0.000000-100.000000"), padahal kita ingin "0-100".
            range << minValue << "-" << maxValue; // range << minValue << "-" << maxValue; menulis batas bawah, tanda "-", lalu batas atas ke dalam range.
            printError("  [!] Nilai harus angka " + range.str() + ", hanya digit dengan satu pemisah desimal" // printError("  [!] Nilai harus angka " + range.str() + ... menyambung pesan dengan teks batas dari range.str();
                                        " (mis. 87.5 atau 87,5). Silakan ulangi."); // " (mis. 87.5 atau 87,5). Silakan ulangi."); menutup pesan; dua teks literal berdampingan digabung compiler.
        }  // akhir perulangan while (true)
    }  // akhir fungsi readDecimal()

    // ===== BAGIAN 5: JUDUL, PETUNJUK, DAN TABEL REKAP =====
    // Bagian ini mengurus tampilan: judul program, kotak petunjuk, dan bahan tabel rekap (satu baris tabel dan angka 2 desimal).
    // Isinya hanya menyusun dan mencetak; semua pembaca input ada di BAGIAN 4.

    void printTitle(std::string_view text) { // printTitle(std::string_view text) berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis '=' buatan printRule() (BAGIAN 3). Dipanggil oleh run() di BAGIAN 6.
        std::cout << '\n';  // std::cout << '\n'; mencetak satu baris kosong sebelum judul.
        printRule('='); // printRule('='); mencetak garis '=' sepanjang 60 karakter (atau selebar terminal dikurangi 1 kolom) di atas judul.
        // Judul dilipat per kata dengan wrapText() (BAGIAN 3) supaya di terminal sempit (misalnya 46 kolom di Termux) tidak terpotong di tengah kata oleh terminal.
        for (const std::string& piece : wrapText("  " + std::string(text), static_cast<std::size_t>(terminalWidth() - 1), 2)) { // wrapText("  " + std::string(text), terminalWidth() - 1, 2) artinya judul yang menjorok 2 spasi dilipat selebar terminal dikurangi 1, dan baris lanjutannya ikut menjorok 2 spasi; std::string(text) mengubah string_view agar bisa disambung.
            std::cout << colorize(ANSI_TITLE, piece) << '\n';  // colorize(ANSI_TITLE, piece) mencetak tiap potongan judul cyan tebal, satu potongan per baris.
        } // akhir perulangan potongan judul
        printRule('='); // printRule('='); mencetak garis '=' lagi di bawah judul.
    } // akhir fungsi printTitle()

    void printInstructions() { // printInstructions() berfungsi untuk menampilkan bentuk input yang diterima (huruf/angka, rentang, cara menulis desimal) dalam kotak sebelum pertanyaan pertama.
    // Dipanggil oleh run() di BAGIAN 6.
        // Daftar di antara { ... } adalah initializer list: compiler menjadikannya std::vector<std::string> untuk parameter lines milik drawBox().
        drawBox("Petunjuk Pengisian", { // drawBox("Petunjuk Pengisian", { artinya gambar kotak berjudul itu; angka 1-1000 dan 0-100 di bawah ditulis manual, jadi bila konstanta di BAGIAN 2 diubah, teks ini harus ikut diubah.
            "- Nama Mata Kuliah : Huruf, boleh Angka dan Spasi", // "- Nama Mata Kuliah : Huruf, boleh Angka dan Spasi" adalah aturan nama mata kuliah; polanya "Label : isi", jadi drawBox() menyejajarkan lanjutannya.
            "                     (contoh: Algoritma dan Pemrograman, Kalkulus 2)", // "                     (contoh: ...)" adalah contoh nama mata kuliah; spasi di depannya membuat teks sejajar dengan isi di baris atasnya.
            "- Jumlah Mahasiswa : Angka bulat 1-1000", // "- Jumlah Mahasiswa : Angka bulat 1-1000" adalah aturan jumlah mahasiswa (STUDENT_COUNT_MIN sampai STUDENT_COUNT_MAX di BAGIAN 2).
            "                     (contoh: 3)", // "                     (contoh: 3)" adalah contoh jumlah mahasiswa.
            "- Nama Lengkap     : Huruf dan Spasi; tanda titik (.), tanda hubung (-), dan tanda petik (') diperbolehkan", // "- Nama Lengkap     : Huruf dan Spasi; ..." adalah aturan nama mahasiswa.
            "                     (contoh: Muhammad A. Rizky, Siti Nur'aini)", // "                     (contoh: Muhammad A. Rizky, Siti Nur'aini)" adalah contoh nama mahasiswa.
            "- Nilai            : Angka 0-100; nilai desimal ditulis dengan tanda titik atau koma", // "- Nilai            : Angka 0-100; ..." adalah aturan nilai (SCORE_MIN sampai SCORE_MAX di BAGIAN 2).
            "                     (contoh: 87, 87,5 atau 87.5)", // "                     (contoh: 87, 87,5 atau 87.5)" adalah contoh cara menulis nilai.
            "Ketik jawaban lalu tekan Enter. Input yang tidak sesuai akan diminta ulang." // "Ketik jawaban lalu tekan Enter. ..." adalah baris penutup petunjuk, tanpa koma di akhir karena ini elemen terakhir.
        }); // }); menutup daftar isi kotak dan pemanggilan drawBox().
        std::cout << '\n';  // std::cout << '\n'; mencetak satu baris kosong setelah kotak.
    }  // akhir fungsi printInstructions()

    std::string formatTableRow(std::string_view no, std::string_view name, std::string_view score, int nameWidth) { // formatTableRow(std::string_view no, std::string_view name, std::string_view score, int nameWidth)
    // berfungsi untuk membuat satu baris tabel (nomor, nama, nilai) dengan lebar kolom tetap, siap dimasukkan ke kotak; no, name, score adalah teks tiap kolom, nameWidth lebar kolom nama.
    // Hasilnya std::string. Dipanggil oleh run() di BAGIAN 6.
        const std::size_t shownWidth = displayWidth(name);  // shownWidth adalah lebar nama di layar menurut displayWidth(), bukan jumlah byte-nya.
        const std::size_t targetWidth = static_cast<std::size_t>(nameWidth);  // targetWidth adalah lebar kolom nama, diubah dari int ke std::size_t supaya bisa dibandingkan dengan shownWidth.
        const std::string namePadded = std::string(name) + std::string(targetWidth > shownWidth ? targetWidth - shownWidth : 0, ' '); // std::string(name) + std::string(targetWidth > shownWidth ? targetWidth - shownWidth : 0, ' ')
        // artinya nama disambung spasi sebanyak targetWidth - shownWidth bila positif, selain itu 0 (operator ternary, sudah dijelaskan di drawBox() di BAGIAN 3). Kolom nama tidak memakai std::setw, karena setw menghitung byte:
        // nama dengan huruf non-ASCII (é, ö) memakai 2 byte per huruf dan kolomnya akan bergeser.
        std::ostringstream line;  // line adalah aliran untuk menyusun teks baris tanpa mengubah format std::cout.
        // std::left dan std::right berfungsi untuk merata-kirikan/merata-kanankan isi di dalam kolom; std::setw(n) berfungsi untuk mengatur lebar kolom n karakter, hanya berlaku untuk item berikutnya.
        line << std::left  << std::setw(NO_COLUMN_WIDTH)    << no // line << std::left  << std::setw(NO_COLUMN_WIDTH)    << no artinya nomor urut rata kiri selebar 5 kolom,
             << namePadded // << namePadded artinya nama yang sudah diberi spasi pengisi,
             << std::right << std::setw(SCORE_COLUMN_WIDTH) << score; // << std::right << std::setw(SCORE_COLUMN_WIDTH) << score; artinya nilai rata kanan selebar 8 kolom.
        return line.str();  // line.str() mengambil teks hasil susunan dari aliran, lalu dikembalikan.
    }  // akhir fungsi formatTableRow()

    std::string formatTwoDecimals(double number) { // formatTwoDecimals(double number) berfungsi untuk mengubah angka menjadi teks dengan tepat 2 desimal (87.5 menjadi "87.50"). Dipanggil oleh run() di BAGIAN 6.
        std::ostringstream text; // text adalah aliran untuk menyusun teks angka.
        // std::fixed berfungsi untuk menulis desimal biasa (bukan notasi ilmiah 8.75e+01); std::setprecision(2) berfungsi untuk menulis tepat 2 angka di belakang titik desimal.
        text << std::fixed << std::setprecision(2) << number; // text << std::fixed << std::setprecision(2) << number; menulis number ke aliran dengan format 2 desimal.
        return text.str(); // return text.str(); mengambil hasilnya sebagai std::string.
    } // akhir fungsi formatTwoDecimals()

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2): semua di atas hanya terlihat di berkas ini

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 1 =====
// Fungsi run() dibungkus namespace bernama task1: semua nama di dalamnya "milik task1". Program bermenu main.cpp di folder yang sama memanggilnya sebagai task1::run(),
// dan tugas lain punya run() juga, jadi masing-masing perlu nama yang khas.
namespace task1 { // namespace task1 { membuka wadah bernama task1; ditutup oleh "}  // akhir namespace task1" di bawah.

    void run() { // run() adalah alur utama Tugas 1: judul, petunjuk, input, hitung, lalu rekap. Dipanggil oleh main() di BAGIAN 7, atau oleh menu di main.cpp sebagai task1::run().
        enableUtf8Console(); // enableUtf8Console(); mengatur konsol Windows ke UTF-8 di sini supaya program mandiri (tanpa menu) juga benar; aman dipanggil ulang.
        printTitle("PROGRAM 1 : TOOLS NILAI MAHASISWA"); // printTitle("PROGRAM 1 : TOOLS NILAI MAHASISWA"); mencetak judul program di antara dua garis '='.
        std::cout << '\n';  // std::cout << '\n'; mencetak satu baris kosong.
        printInstructions(); // printInstructions(); menampilkan kotak petunjuk pengisian.

        // --- Pengaturan awal: mata kuliah dan jumlah mahasiswa ditentukan oleh pengguna ---
        const std::string courseName = readCourseName("Nama Mata Kuliah : "); // readCourseName("Nama Mata Kuliah : ") mengulang pertanyaan sampai jawabannya sah; const artinya courseName tidak diubah lagi.
        const int studentCount = readInteger("Jumlah Mahasiswa : ", STUDENT_COUNT_MIN, STUDENT_COUNT_MAX); // readInteger("Jumlah Mahasiswa : ", STUDENT_COUNT_MIN, STUDENT_COUNT_MAX) membatasi jumlah 1-1000 (BAGIAN 2)
        // supaya salah ketik tertolak.

        // std::vector<Student> adalah vector yang tiap elemennya struct Student (BAGIAN 2); angka di dalam ( ) adalah jumlah elemen awal.
        std::vector<Student> students(static_cast<std::size_t>(studentCount)); // students(static_cast<std::size_t>(studentCount)) membuat daftar mahasiswa sebesar jumlah yang diketik; ukuran vector bertipe std::size_t
        // sedangkan studentCount int, jadi diubah dulu dengan static_cast.

        // --- Tahap input: isi nama dan nilai setiap mahasiswa ---
        std::cout << '\n'; // std::cout << '\n'; mencetak satu baris kosong sebelum sub-judul.
        // Sub-judul memuat nama mata kuliah yang bisa sampai 60 kolom, jadi dilipat per kata dengan wrapText() (BAGIAN 3); lanjutannya menjorok 12 spasi, lurus dengan nama setelah "Input Nilai ".
        for (const std::string& piece : wrapText("Input Nilai " + courseName, static_cast<std::size_t>(terminalWidth() - 1), 12)) { // wrapText("Input Nilai " + courseName, ..., 12) memotong sub-judul
        // sesuai lebar terminal dikurangi 1 kolom cadangan; 12 adalah indentasi gantung.
            std::cout << colorize(ANSI_TITLE, piece) << '\n'; // colorize(ANSI_TITLE, piece) mencetak potongan sub-judul dengan cyan tebal, lalu pindah baris.
        } // akhir perulangan sub-judul
        int rowNumber = 1;  // rowNumber = 1 adalah nomor urut mahasiswa, mulai 1 karena manusia menghitung dari 1.
        // Student& (referensi tanpa const) berfungsi agar yang terisi adalah data aslinya di dalam vector, bukan salinannya.
        for (Student& student : students) { // for (Student& student : students) artinya kunjungi setiap mahasiswa di students dan isi datanya langsung.
            std::cout << colorize(ANSI_BOLD, "Mahasiswa ke-" + std::to_string(rowNumber) + " dari " + std::to_string(studentCount)) << '\n'; // colorize(ANSI_BOLD, "Mahasiswa ke-" + ... ) adalah label
            // "Mahasiswa ke-N dari M" dicetak tebal; std::to_string mengubah angka menjadi teks.
            student.name  = readPersonName   ("  Nama Lengkap  : "); // student.name  = readPersonName   ("  Nama Lengkap  : "); mengisi anggota name lewat tanda titik; nama harus huruf, jadi readPersonName() (BAGIAN 4).
            student.score = readDecimal("  Nilai (0-100) : ", SCORE_MIN, SCORE_MAX); // student.score = readDecimal("  Nilai (0-100) : ", SCORE_MIN, SCORE_MAX); mengisi nilai; boleh desimal dan dibatasi 0-100, jadi readDecimal().
            ++rowNumber;  // ++rowNumber; artinya nomor urut berikutnya.
        }  // akhir perulangan input mahasiswa

        // --- Tahap perhitungan: total, rata-rata, dan lebar kolom nama ---
        double      total     = 0.0;  // total = 0.0 adalah jumlah semua nilai; jumlah nilai desimal harus tetap desimal, jadi double.
        std::size_t nameWidth = NAME_COLUMN_MIN_WIDTH;  // nameWidth = NAME_COLUMN_MIN_WIDTH (20) adalah lebar kolom nama, mula-mula lebar minimal; std::size_t karena di perulangan tepat di bawah ia dibandingkan dengan hasil displayWidth() (BAGIAN 3).
        for (const Student& student : students) { // for (const Student& student : students) artinya kali ini const Student& (referensi hanya-baca): data dibaca tanpa disalin dan tidak diubah.
            total += student.score;  // total += student.score; menjumlahkan nilai ke total (singkatan dari total = total + student.score).
            nameWidth = std::max(nameWidth, displayWidth(student.name) + 2); // std::max(nameWidth, displayWidth(student.name) + 2) artinya kolom nama harus muat nama terpanjang + 2 (jarak ke kolom nilai).
        }  // akhir perulangan menghitung
        const double average = total / static_cast<double>(students.size()); // total / static_cast<double>(students.size()) adalah rata-rata: total sudah double, jadi hasil baginya desimal (pada int, 11/2 = 5;
        // pada double 5.5); students.size() bertipe std::size_t, jadi diubah ke double dengan static_cast supaya kedua sisi sama-sama double. Jumlah minimal 1 (BAGIAN 2), jadi tidak pernah dibagi nol.
        // Kolom nama dibatasi supaya tabel selalu muat di kotak rekap: lebar isi kotak (terminalWidth() - 5, sama seperti di drawBox() BAGIAN 3) dikurangi kolom No dan kolom Nilai.
        // Di terminal 80 kolom batasnya 62, tepat cukup untuk nama terpanjang yang diizinkan (60 kolom + 2 kolom jarak), jadi batas ini hanya terasa di terminal yang lebih sempit; nama yang lebih panjang dari kolomnya dilipat di bawah.
        const std::size_t nameWidthLimit = static_cast<std::size_t>(terminalWidth() - 5 - NO_COLUMN_WIDTH - SCORE_COLUMN_WIDTH); // nameWidthLimit adalah lebar kolom nama terbesar yang masih muat;
        // di terminal tersempit (40 kolom) nilainya 22, tetap di atas NAME_COLUMN_MIN_WIDTH (20).
        nameWidth = std::min(nameWidth, nameWidthLimit); // std::min(nameWidth, nameWidthLimit) artinya kolom nama tidak lebih lebar dari batas itu.

        // --- Tahap output: tabel rekap disusun ke vector, lalu digambar dalam satu kotak ---
        const int nameColumnWidth = static_cast<int>(nameWidth); // static_cast<int>(nameWidth) mengubah nameWidth ke int, karena formatTableRow() menerima lebar kolom nama sebagai int dan garis tabel di bawah
        // menjumlahkannya dengan lebar kolom lain yang juga int.
        const std::string tableRule(NO_COLUMN_WIDTH + nameColumnWidth + SCORE_COLUMN_WIDTH, '-'); // tableRule(NO_COLUMN_WIDTH + nameColumnWidth + SCORE_COLUMN_WIDTH, '-') adalah garis '-' selebar tabel: jumlah lebar ketiga kolom.

        std::vector<std::string> summaryLines;  // summaryLines adalah baris-baris isi kotak rekap; jumlahnya mengikuti jumlah mahasiswa, jadi vector.
        summaryLines.push_back("Mata Kuliah      : " + courseName); // summaryLines.push_back("Mata Kuliah      : " + courseName); menambahkan baris nama mata kuliah; push_back menaruhnya di akhir vector.
        summaryLines.push_back("Jumlah Mahasiswa : " + std::to_string(studentCount)); // summaryLines.push_back("Jumlah Mahasiswa : " + std::to_string(studentCount)); menambahkan baris jumlah mahasiswa.
        summaryLines.push_back("");  // summaryLines.push_back(""); menambahkan baris kosong sebagai pemisah.
        summaryLines.push_back(colorize(ANSI_BOLD, formatTableRow("No", "Nama", "Nilai", nameColumnWidth))); // formatTableRow("No", "Nama", "Nilai", nameColumnWidth) menyusun judul kolom (BAGIAN 5), lalu dicetak tebal.
        summaryLines.push_back(tableRule);  // summaryLines.push_back(tableRule); menambahkan garis atas tabel.
        rowNumber = 1;  // rowNumber = 1; artinya penomoran tabel dimulai ulang dari 1.
        for (const Student& student : students) { // for (const Student& student : students) artinya satu baris tabel per mahasiswa: nomor, nama, dan nilai dengan 2 desimal.
            const std::vector<std::string> nameLines = wrapText(student.name, nameWidth - 2, 0); // wrapText(student.name, nameWidth - 2, 0) melipat nama per kata supaya muat di kolom nama
            // (dikurangi 2 kolom jarak ke kolom nilai); nama yang muat tetap satu potongan.
            summaryLines.push_back(formatTableRow(std::to_string(rowNumber), nameLines[0], formatTwoDecimals(student.score), nameColumnWidth)); // formatTableRow(std::to_string(rowNumber), nameLines[0], ...)
            // menyusun baris pertama mahasiswa ini: nomor, potongan nama pertama, dan nilai; formatTwoDecimals() membuat nilai tampil 2 desimal.
            for (std::size_t i = 1; i < nameLines.size(); ++i) summaryLines.push_back(formatTableRow("", nameLines[i], "", nameColumnWidth)); // nameLines.size() adalah jumlah potongan nama; mulai dari i = 1 artinya potongan ke-2
            // dan seterusnya (bila ada) masing-masing menjadi baris sendiri, dengan kolom No dan kolom Nilai kosong.
            ++rowNumber; // ++rowNumber; artinya nomor berikutnya.
        }  // akhir perulangan baris tabel
        summaryLines.push_back(tableRule);  // summaryLines.push_back(tableRule); menambahkan garis bawah tabel.
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Total",     formatTwoDecimals(total),    nameColumnWidth))); // formatTableRow("", "Nilai Total",     formatTwoDecimals(total),    nameColumnWidth)
        // adalah baris total: kolom No diisi "" supaya label sejajar dengan kolom nama; hijau tebal karena inilah hasil akhir yang paling ditunggu pengguna.
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Rata-Rata", formatTwoDecimals(average), nameColumnWidth))); // formatTableRow("", "Nilai Rata-Rata", formatTwoDecimals(average), nameColumnWidth)
        // adalah baris rata-rata, juga hijau tebal.
        std::cout << '\n';  // std::cout << '\n'; mencetak satu baris kosong sebelum kotak.
        drawBox("Rekap Nilai Mahasiswa", summaryLines);  // drawBox("Rekap Nilai Mahasiswa", summaryLines); menggambar kotak rekap (drawBox() di BAGIAN 3).
    }  // akhir fungsi run()

}  // akhir namespace task1

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), dan program bermenu main.cpp sudah punya. Jadi main() di bawah hanya ikut saat berkas ini dikompilasi sendiri.
#ifndef BUILD_WITH_MENU // BUILD_WITH_MENU dicek: bila penanda ini belum ada (kompilasi sendiri), bagian sampai #endif ikut; bila ada (kompilasi dengan -DBUILD_WITH_MENU), bagian ini dilewati.
// int main() adalah titik awal program: fungsi yang pertama kali dijalankan saat program dibuka. Standar C++ mewajibkan main bertipe int, yaitu bilangan bulat
// yang dikirim ke sistem operasi saat program selesai; nilai 0 (lihat return 0 di bawah) menandakan program selesai tanpa error.
int main() { // main() adalah fungsi utama saat berkas ini dijalankan sendiri sebagai program tugas1.
    // Baris identitas dilipat per kata dengan wrapText() (BAGIAN 3), supaya di terminal sempit tidak terpotong di tengah kata.
    for (const std::string& piece : wrapText("Tugas Praktikum 1 - Fadhlillah (2531070011)", static_cast<std::size_t>(terminalWidth() - 1), 0)) { // wrapText("Tugas Praktikum 1 - Fadhlillah (2531070011)", ..., 0) memotong identitas sesuai lebar terminal dikurangi 1 kolom cadangan, tanpa indentasi gantung (0).
        std::cout << colorize(ANSI_BOLD, piece) << '\n'; // colorize(ANSI_BOLD, piece) mencetak potongan identitas dengan huruf tebal, lalu pindah baris.
    } // akhir perulangan baris identitas
    task1::run();  // task1::run(); memanggil fungsi run() di dalam namespace task1 (BAGIAN 6); tanda :: berarti "milik".
    return 0; // return 0; mengakhiri main(); 0 berarti selesai tanpa error.
} // akhir main()
#endif  // selesai #ifndef BUILD_WITH_MENU
