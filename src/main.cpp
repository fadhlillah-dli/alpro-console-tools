// ===== main.cpp — Program dengan Menu yang menggabungkan Tugas 1, 2, dan 3 =====
// Tiga tugas menjadi satu program utuh: tugas1_nilai_mahasiswa.cpp, tugas2_volume_tabung.cpp, dan tugas3_luas_arsir.cpp.
// Semuanya ada di folder src/ bersama berkas ini, dan masing-masing bisa dikompilasi sendiri.
// Isi main.cpp merupakan kode pendukung yang tidak bisa ditaruh di berkas tugas, seperti banner identitas, menu, pembacaan pilihan,
// jeda, dan perulangan menu.
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Cara mengompilasi program gabungan (jalankan dari folder proyek):
// 1. Buka Terminal VS Code lalu copy paste command di bawah ini :
//   g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
//   (Windows PowerShell: .\tugas_alpro.exe).
// Arti tiap bagian perintah itu:
//   -std=c++17         memakai aturan bahasa C++17
//   -Wall -Wextra      menampilkan peringatan tambahan
//   -DBUILD_WITH_MENU  memasang penanda bernama BUILD_WITH_MENU (alasannya ada di bawah)
//   -o tugas_alpro     nama program hasil kompilasi
//   src/*.cpp          semua berkas .cpp di folder src
//   && ./tugas_alpro   menjalankan program hasilnya, tetapi hanya bila kompilasi berhasil
// Kenapa perlu -DBUILD_WITH_MENU? Setiap berkas tugas punya main() sendiri. Opsi -D berfungsi untuk memasang penanda saat kompilasi.
// Bila penanda itu ada, main() di setiap berkas tugas tidak ikut dikompilasi, jadi yang dipakai hanya main() di bagian paling bawah berkas ini.
//
// 2. Cara lain di VS Code : buka menu Terminal > Run Task..., lalu pilih task "Build & Run".
// Task adalah perintah siap pakai; yang satu ini mengompilasi lalu menjalankan program.
// Daftar task ada di berkas .vscode/tasks.json. Pintasan Cmd/Ctrl+Shift+B (Cmd di macOS, Ctrl di
// Windows/Linux) hanya menjalankan task "Build (g++)": mengompilasi saja, tanpa menjalankan program.


// #include berfungsi untuk menyalin isi sebuah header ke berkas ini sebelum dikompilasi.
#include <algorithm> // <algorithm> digunakan untuk mengimpor pustaka (library) algoritma standar.
#include <cctype> // <cctype> digunakan untuk memeriksa apakah sebuah karakter berupa huruf, angka, tanda baca, atau spasi,
// serta untuk mengubah huruf kecil menjadi huruf besar (dan sebaliknya).
#include <cstddef> // <cstddef> digunakan untuk mengimpor definisi tipe data dan makro dasar sistem.
#include <cstdlib> // <cstdlib> digunakan untuk mengimpor pustaka utilitas umum dan berisi berbagai fungsi dasar untuk manajemen memori, kontrol alur program,
// konversi tipe data, pembuatan angka acak, hingga interaksi dengan sistem operasi.
#include <iostream> // <iostream> untuk mengimpor pustaka input dan output standar seperti menampilkan teks ke layar (output) dan membaca ketikan dari keyboard (input).
#include <string> // <string> digunakan untuk tipe std::string, yaitu teks yang panjangnya bisa berubah, dan fungsi std::getline untuk membaca satu baris ketikan utuh.
#include <string_view> // <string_view> digunakan untuk tipe std::string_view, yaitu "jendela" untuk membaca sebuah teks tanpa menyalinnya;
// dipakai karena menyalin teks yang hanya dibaca (tidak diubah) cuma membuang waktu dan memori.
#include <utility> // <utility> digunakan untuk tipe std::pair, yaitu pasangan dua nilai; di berkas ini dipakai untuk nama menu beserta keterangannya.
#include <vector> // <vector> digunakan untuk tipe std::vector, yaitu daftar berurutan yang bisa bertambah panjang; di berkas ini dipakai untuk baris-baris isi kotak.


// #ifdef berfungsi untuk bertanya ke compiler apakah sebuah nama sudah didefinisikan; bila ya, baris sampai #else/#endif ikut dikompilasi, bila tidak, baris itu dilewati.
// Di sini kita perlu bertanya karena program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya berbicara dengan terminal berbeda.
#ifdef _WIN32 // _WIN32 adalah nama yang otomatis ada saat program dikompilasi untuk Windows; artinya baris sampai #else hanya ikut dikompilasi di Windows.
    // #ifndef ("if not defined") berfungsi kebalikan dari #ifdef: baris sampai #endif hanya ikut dikompilasi bila nama itu BELUM didefinisikan.
    #ifndef NOMINMAX // NOMINMAX dicek dulu karena MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan mendefinisikan ulang nama yang sama menghasilkan peringatan.
    // #define berfungsi untuk membuat makro, yaitu nama yang oleh compiler diganti dengan teks lain; mulai baris di bawah, NOMINMAX berarti 1.
    #define NOMINMAX 1 // NOMINMAX 1 artinya "jangan buat min dan max" dinyalakan, karena windows.h punya kebiasaan membuat makro min/max sendiri yang merusak std::min dan std::max.
    #endif // selesai mengecek NOMINMAX
    #include <windows.h> // <windows.h> adalah pustaka resmi Windows; dari sini dipakai fungsi untuk warna (mode virtual terminal), UTF-8 (code page), dan lebar jendela konsol.
#else // #else artinya: bila bukan Windows (macOS atau Linux), pakai pustaka di bawah ini.
    #include <cstdio> // <cstdio> digunakan untuk stdout, yaitu "saluran" keluaran ke layar, dan fileno, yang memberi nomor saluran itu.
    #include <sys/ioctl.h> // <sys/ioctl.h> digunakan untuk ioctl dan TIOCGWINSZ, yaitu cara bertanya ke terminal seberapa lebar jendelanya.
    #include <unistd.h> // <unistd.h> digunakan untuk isatty (memeriksa apakah keluaran benar-benar terminal, bukan berkas) dan STDOUT_FILENO (nomor saluran keluaran layar).
#endif  // #endif berfungsi untuk menutup blok #ifdef/#ifndef; di sini artinya selesai memilih pustaka sesuai sistem operasi.

// namespace berfungsi untuk membuat wadah nama. Ketiga program tugas sama-sama punya fungsi bernama run(), jadi masing-masing ditaruh di wadahnya sendiri agar tidak bentrok.
// Tiga baris di bawah hanya deklarasi, yaitu pemberitahuan ke compiler bahwa fungsi itu ada (void: tanpa hasil, () kosong: tanpa masukan), karena C++ mewajibkan fungsi dikenal sebelum dipanggil.
namespace task1 { void run(); } // task1 { void run(); } adalah deklarasi run() milik Program 1 (Tools Nilai Mahasiswa); isinya ada di src/tugas1_nilai_mahasiswa.cpp, dipanggil sebagai task1::run().
namespace task2 { void run(); } // task2 { void run(); } adalah deklarasi run() milik Program 2 (Volume Tabung); isinya ada di src/tugas2_volume_tabung.cpp, dipanggil sebagai task2::run().
namespace task3 { void run(); } // task3 { void run(); } adalah deklarasi run() milik Program 3 (Luas Bangun yang Diarsir); isinya ada di src/tugas3_luas_arsir.cpp, dipanggil sebagai task3::run().
namespace { // namespace { tanpa nama berfungsi untuk membuat isinya hanya terlihat dari berkas ini, karena nama seperti colorize() dan drawBox() juga ada di berkas tugas dan tidak boleh bentrok saat digabung; wadah ini ditutup tepat sebelum main().

    // constexpr const char* berfungsi untuk membuat konstanta teks: constexpr artinya nilainya sudah pasti sejak kompilasi dan tidak bisa diubah,
    // sedangkan const char* adalah penunjuk (alamat di memori) ke teks tetap yang hurufnya tidak boleh diubah.
    // Isinya kode warna ANSI, yaitu teks khusus yang tidak tercetak; terminal membacanya sebagai perintah ganti gaya. Bentuknya "\033[" + angka gaya + huruf m.
    // "\033" adalah karakter ESC (nomor 27, ditulis dalam oktal: 3 x 8 + 3 = 27). Kode ANSI dipakai karena terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // ANSI_RESET  = "\033[0m"; kode untuk mengembalikan tampilan ke normal (tanpa warna/tebal); angka 0 artinya reset.
    constexpr const char* ANSI_BOLD  = "\033[1m";     // ANSI_BOLD  = "\033[1m"; angka 1 artinya tebal; dipakai untuk nama menu dan prompt (teks pertanyaan yang menunggu ketikan pengguna).
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // ANSI_TITLE  = "\033[1;36m"; 1;36 artinya tebal (1) sekaligus cyan (36); dipakai untuk judul kotak, termasuk judul banner.
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // ANSI_SUCCESS  = "\033[1;32m"; 1;32 artinya tebal dan hijau (32); dipakai untuk pesan penutup program.
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // ANSI_ERROR  = "\033[1;31m"; 1;31 artinya tebal dan merah (31); dipakai untuk pesan kesalahan.
    constexpr const char* ANSI_HINT = "\033[33m";    // ANSI_HINT = "\033[33m"; 33 artinya kuning; dipakai untuk ajakan "Tekan Enter untuk kembali ke menu...".
    constexpr const char* ANSI_DIM    = "\033[90m";    // ANSI_DIM    = "\033[90m"; 90 artinya abu-abu (hitam terang); dipakai untuk garis kotak agar tidak mencolok.

    // void berfungsi menandai fungsi yang tidak mengembalikan nilai apa pun; fungsi seperti ini dipanggil untuk efeknya saja. Isi fungsi ditulis di antara { dan }.
    void enableUtf8Console() { // enableUtf8Console() berfungsi untuk mengganti konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama; dipanggil sekali di awal main().
    // UTF-8 adalah aturan menyimpan setiap huruf dan simbol sebagai 1 sampai 4 byte. Konsol Windows bawaan (Command Prompt, PowerShell) masih memakai code page lama,
    // misalnya 437 (code page adalah tabel aturan byte mana menjadi huruf apa), sehingga π, ², ³, garis kotak, dan sketsa Braille tampil sebagai huruf acak.
    // Garis kotak berasal dari drawBox() di bawah; simbol dan sketsa Braille berasal dari berkas tugas 2 dan 3 (³ hanya di tugas 2).
    // Pengaturan ini sama dengan perintah chcp 65001 di berkas peluncur jalankan.bat. Di macOS/Linux terminal sudah UTF-8, jadi di sana isi fungsi ini kosong.
    #ifdef _WIN32 // _WIN32: dua baris di bawah hanya ikut dikompilasi di Windows, karena SetConsoleOutputCP dan SetConsoleCP hanya ada di windows.h.
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
        #else   // #else: bukan Windows (macOS atau Linux); di sini cukup satu pertanyaan: apakah keluaran layar sebuah terminal?
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
    // UTF-8 menyimpan satu karakter dalam 1 sampai 4 byte, padahal garis kotak tetap selebar satu kolom; aksara lebar (CJK, emoji) memakai dua kolom, dan kode warna ANSI
    // memakai nol kolom. Karena itu teks dihitung byte demi byte.
        std::size_t width = 0;  // width = 0 adalah jumlah kolom yang sudah terhitung, mula-mula 0.
        bool inEscape = false;  // inEscape = false artinya belum berada di dalam kode warna "\033[...m"; nilainya true selama berada di dalamnya.
        unsigned codePoint = 0; // codePoint = 0 adalah code point yang sedang dirangkai dari byte-byte UTF-8.
        // int adalah tipe bilangan bulat biasa yang boleh bernilai negatif.
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
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; } // (codePoint << 6) | (c & 0x3F) artinya geser code point 6 bit ke kiri,
                // lalu tempelkan 6 bit data byte ini. Bila --pending menjadi 0, code point sudah lengkap; karakter lebar lalu mendapat satu kolom tambahan (++width),
                // karena kolom pertamanya sudah dihitung di byte awal.
                continue; // continue; artinya byte lanjutan bukan karakter baru, jadi tidak menambah kolom.
            } // akhir penanganan byte lanjutan
            ++width;                                       // ++width; artinya setiap byte awal = satu karakter = minimal satu kolom.
            // Pola bit byte awal menentukan berapa byte lanjutan yang menyusul; bit x milik byte awal disimpan sebagai awal code point.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; } // (c & 0xE0) == 0xC0 artinya byte awal berpola 110xxxxx: menyusul 1 byte lanjutan, dan 5 bit data (c & 0x1F) disimpan.
            // else if berfungsi untuk memeriksa syarat berikutnya hanya bila syarat sebelumnya salah; else terakhir dijalankan bila semua syarat salah.
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; } // (c & 0xF0) == 0xE0 artinya pola 1110xxxx: menyusul 2 byte lanjutan, dan 4 bit data (c & 0x0F) disimpan.
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; } // (c & 0xF8) == 0xF0 artinya pola 11110xxx: menyusul 3 byte lanjutan, dan 3 bit data (c & 0x07) disimpan.
            else                         { pending = 0; } // else { pending = 0; } artinya byte ASCII biasa (pola 0xxxxxxx) yang tidak punya byte lanjutan.
        } // akhir perulangan setiap byte
        return width; // return width; mengembalikan jumlah kolom yang terhitung.
    } // akhir fungsi displayWidth()

    int terminalWidth() { // terminalWidth() berfungsi untuk menanyakan lebar jendela terminal (dalam kolom) setiap kali dipanggil, jadi tampilan ikut menyesuaikan bila jendela diubah ukurannya.
    // Bila terminal tidak bisa ditanya (keluaran bukan terminal), dicoba variabel lingkungan COLUMNS; bila itu juga tidak ada, dipakai 80 kolom, ukuran bawaan terminal pada umumnya.
    // Hasilnya int dan tidak pernah di bawah 40: tata letak program (kotak, menu, sketsa, tabel) dirancang dan diuji mulai 40 kolom, dan 40 sudah cukup untuk Termux di ponsel tegak (sekitar 46 kolom); di terminal yang lebih sempit, terminal sendiri yang melipat baris.
        int cols = 0;  // cols = 0 artinya lebar belum diketahui.
    #ifdef _WIN32 // _WIN32: cara bertanya ke terminal berbeda di Windows dan macOS/Linux, jadi dipilih lagi; bagian sampai #else hanya untuk Windows.
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows. Struct adalah wadah berisi beberapa data bernama, dan tanda titik (.) berfungsi untuk mengambil salah satu datanya.
        CONSOLE_SCREEN_BUFFER_INFO info; // CONSOLE_SCREEN_BUFFER_INFO info; adalah tempat menampung keterangan jendela konsol; bagian srWindow menyimpan nomor kolom kiri (Left) dan kanan (Right).
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1; // GetConsoleScreenBufferInfo(...) mengisi info lewat alamatnya (&info);
        // bila berhasil, lebar jendela = Right - Left + 1.
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
    // const std::string& adalah referensi tetap (const&): fungsi memakai teks aslinya tanpa menyalin, dan const membuat teks itu tidak boleh diubah.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) { // wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent)
    // berfungsi untuk memotong text di spasi menjadi beberapa baris yang masing-masing muat dalam maxWidth kolom, supaya terminal tidak memotongnya sembarangan.
    // Baris lanjutan diberi indentasi gantung sebanyak hangingIndent spasi, agar sejajar dengan isi setelah label. Dipakai drawBox() dan printError() di bawah.
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
                if (c == ' ' && col > hangingIndent) cutPos = i; // c == ' ' && col > hangingIndent artinya spasi ini menjadi calon titik potong, asalkan letaknya setelah indentasi gantung;
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
    // berisi baris-baris lines. Kotak membuat informasi (petunjuk, menu, hasil) terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah
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
    // tidak terpotong jendela sempit. Pesannya selalu diawali "  [!] ", jadi baris lanjutan menjorok 6 spasi agar sejajar dengan teks setelah tanda "[!]". Dipakai readMenuChoice() di bawah.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) { // wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6) artinya lebar bungkus = lebar terminal
        // dikurangi 1 kolom cadangan, dan 6 = indentasi gantung.
            std::cout << colorize(ANSI_ERROR, b) << '\n'; // colorize(ANSI_ERROR, b) artinya setiap baris hasil bungkus dicetak merah tebal.
        } // akhir perulangan baris pesan
    } // akhir fungsi printError()

    void printBanner() { // printBanner() berfungsi untuk menampilkan banner identitas di dalam kotak; dipanggil sekali di awal main().
        drawBox("Tugas Praktikum Algoritma dan Struktur Data", { // drawBox("Tugas Praktikum Algoritma dan Struktur Data", { artinya gambar kotak berjudul itu; daftar teks di dalam { ... } langsung menjadi parameter lines.
            "Modul 1 : Input dan Output", // "Modul 1 : Input dan Output" adalah baris pertama isi banner; polanya "Label : isi", jadi drawBox() menyejajarkan lanjutannya.
            "Nama    : Fadhlillah", // "Nama    : Fadhlillah" adalah nama mahasiswa; spasi setelah Nama membuat tanda : sejajar dengan baris lain.
            "NIM     : 2531070011" // "NIM     : 2531070011" adalah nomor induk mahasiswa.
        }); // }); menutup daftar isi kotak dan pemanggilan drawBox().
    } // akhir fungsi printBanner()

    void printMenu() { // printMenu() berfungsi untuk menampilkan daftar pilihan program dalam kotak, lengkap dengan keterangan singkat tiap program.
    // [0] Keluar sengaja diletakkan terakhir, karena menu konsol biasanya menaruh "keluar" di akhir.
        std::cout << '\n';  // std::cout << '\n'; mencetak satu baris kosong sebagai pemisah dari tampilan sebelumnya.
        const bool isNarrow = terminalWidth() < 100; // terminalWidth() < 100 artinya jendela dianggap sempit bila lebarnya di bawah 100 kolom; isNarrow memilih tata letak menu:
        // sempit = dua baris per menu (keterangan diturunkan ke baris berikutnya), lebar = satu baris (keterangan sejajar di kanan nama program). Dua tata letak ini menjaga menu tetap rapi.
        // std::vector<std::pair<std::string, std::string>> adalah daftar berisi pasangan dua teks; setiap pasangan = { nama menu, keterangannya }.
        const std::vector<std::pair<std::string, std::string>> menuItems = { // menuItems adalah daftar menu; nama menu diberi spasi di akhir agar sama panjang, sehingga keterangan di kanannya sejajar lurus.
            { "[1] Tools Nilai Mahasiswa", "Rekap Nama, Nilai, Total, dan Rata-Rata satu mata kuliah" }, // { "[1] Tools Nilai Mahasiswa", ... } adalah menu Program 1 beserta keterangannya.
            { "[2] Volume Tabung        ", "V = Pi x R x R x T; satuan, nilai Pi, jari-jari/diameter dipilih" }, // { "[2] Volume Tabung        ", ... } adalah menu Program 2 beserta keterangannya.
            { "[3] Luas Bangun Diarsir  ", "Persegi dikurangi lingkaran di dalamnya; satuan dan Pi dipilih" }, // { "[3] Luas Bangun Diarsir  ", ... } adalah menu Program 3 beserta keterangannya.
            { "[0] Keluar               ", "Menutup program" }, // { "[0] Keluar               ", "Menutup program" } adalah pilihan untuk keluar dari program.
        }; // akhir daftar menuItems
        std::vector<std::string> lines;  // lines adalah baris-baris yang akan digambar di dalam kotak.
        // auto berfungsi untuk membiarkan compiler menentukan tipe variabel sendiri dari nilainya; di sini tipenya std::pair.
        for (const auto& item : menuItems) { // for (const auto& item : menuItems) artinya ambil setiap item tanpa menyalin; item.first adalah nama menu, dan item.second keterangannya.
            if (isNarrow) { lines.push_back(colorize(ANSI_BOLD, item.first)); lines.push_back("    " + item.second); } // if (isNarrow) artinya jendela sempit: nama menu tebal di satu baris,
            // lalu keterangan menjorok 4 spasi di baris berikutnya.
            else        { lines.push_back(colorize(ANSI_BOLD, item.first) + "   " + item.second); } // else artinya jendela lebar: satu baris, nama menu tebal dan keterangan dipisah 3 spasi.
        } // akhir perulangan item menu
        drawBox("Menu Program", lines); // drawBox("Menu Program", lines) menggambar kotak berjudul "Menu Program" berisi lines.
        std::cout << "Ketik nomor menu lalu tekan Enter.\n";  // "Ketik nomor menu lalu tekan Enter.\n" adalah petunjuk; \n di dalam string juga berarti pindah baris.
    } // akhir fungsi printMenu()

    int readMenuChoice() { // readMenuChoice() berfungsi untuk membaca nomor menu; hanya tepat satu digit 0-3 yang diterima, ketikan lain (misalnya 9, abc, atau 2.5) ditolak dan pertanyaannya diulang.
    // Hasilnya int: nomor pilihan yang sah.
        // while (true) berfungsi untuk mengulang tanpa henti, karena syaratnya selalu benar; jalan keluarnya hanya lewat return (atau std::exit).
        while (true) {  // while (true) { artinya tanya terus sampai pilihan sah.
            std::string text;  // text menampung satu baris ketikan pengguna.
            std::cout << colorize(ANSI_BOLD, "Pilihan Anda [0-3] : "); // "Pilihan Anda [0-3] : " adalah prompt, yaitu teks pertanyaan yang menunggu ketikan pengguna, dicetak tebal.
            // std::getline berfungsi untuk membaca satu baris ketikan utuh sampai Enter. Dipakai, bukan std::cin >>, karena seluruh baris perlu diperiksa: cin >> berhenti di spasi atau
            // di karakter yang bukan angka (misalnya 2.5 terbaca 2), dan sisa ketikannya tertinggal lalu ikut terbaca oleh pertanyaan berikutnya.
            if (!std::getline(std::cin, text)) { // !std::getline(std::cin, text) artinya pembacaan dari std::cin (masukan keyboard) gagal karena input sudah ditutup, jadi tidak ada ketikan lagi:
            // pengguna menekan Ctrl+D (macOS/Linux) atau Ctrl+Z lalu Enter (Windows), atau input dialihkan dari berkas (misalnya ./tugas_alpro < masukan.txt) dan isinya sudah habis.
                // std::cerr berfungsi untuk mencetak pesan galat lewat saluran khusus yang terpisah dari std::cout.
                std::cerr << "\nInput berakhir. Program dihentikan.\n";  // "\nInput berakhir. Program dihentikan.\n" adalah pesan bahwa input sudah habis.
                // std::exit berfungsi untuk menghentikan seluruh program saat itu juga, dari fungsi mana pun.
                std::exit(EXIT_FAILURE);  // std::exit(EXIT_FAILURE) artinya hentikan program dengan kode keluar EXIT_FAILURE ("gagal").
            } // akhir penanganan input yang berakhir
            const auto start  = text.find_first_not_of(" \t\r"); // text.find_first_not_of(" \t\r") mencari posisi karakter pertama yang bukan spasi, tab (\t), atau \r; \r adalah sisa akhir baris Windows
            // pada input dari berkas. auto di sini menjadi tipe posisi teks (std::size_t).
            const auto end = text.find_last_not_of(" \t\r"); // text.find_last_not_of(" \t\r") mencari posisi karakter terakhir yang bukan spasi, tab, atau \r.
            text = (start == std::string::npos) ? "" : text.substr(start, end - start + 1); // (start == std::string::npos) ? "" : ... artinya bila isinya spasi semua, text menjadi kosong; bila tidak,
            // text diganti potongan mulai posisi start sepanjang end - start + 1 karakter. Jadi spasi di awal dan akhir dibuang, dan " 2 " tetap diterima.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0])) && text[0] <= '3') { // text.size() == 1 && ... && text[0] <= '3' artinya sah hanya bila tepat satu karakter,
            // karakter itu angka 0-9 (std::isdigit), dan paling besar '3'; bentuk lain (+1, 1.0, 1 2, abc, kosong) ditolak. static_cast dipakai karena std::isdigit hanya aman untuk nilai 0-255.
                return text[0] - '0'; // text[0] - '0' berfungsi untuk mengubah karakter digit menjadi angka: karakter disimpan sebagai nomor kode berurutan ('0' = 48, '2' = 50), jadi '2' - '0' = 2.
            } // akhir pemeriksaan pilihan yang sah
            printError("  [!] Pilihan tidak dikenal. Ketik 1, 2, 3, atau 0 lalu tekan Enter."); // printError("  [!] Pilihan tidak dikenal. ...") artinya ketikan tidak sah: tampilkan pesan merah, lalu while (true) mengulang pertanyaan.
        } // akhir perulangan while (true)
    } // akhir fungsi readMenuChoice()

    void waitForEnter() { // waitForEnter() berfungsi untuk menahan layar sebentar supaya hasil sempat dibaca sebelum menu muncul lagi; ia membaca satu baris ketikan lalu membuangnya.
        std::cout << '\n' << colorize(ANSI_HINT, "Tekan Enter untuk kembali ke menu..."); // colorize(ANSI_HINT, "Tekan Enter untuk kembali ke menu...") adalah ajakan berwarna kuning.
        std::string discard;             // discard adalah tempat ketikan yang isinya diabaikan, karena yang dibutuhkan hanya tombol Enter.
        std::getline(std::cin, discard);  // std::getline(std::cin, discard) menunggu sampai pengguna menekan Enter; satu baris dibaca lalu dibuang.
    } // akhir fungsi waitForEnter()

}  // akhir namespace tanpa nama: semua di atas hanya terlihat di berkas ini

// int main() adalah titik awal program: fungsi yang pertama kali dijalankan saat program dibuka. Standar C++ mewajibkan main bertipe int, yaitu bilangan bulat
// yang dikirim ke sistem operasi saat program selesai; nilai 0 (lihat return 0 di bawah) menandakan program selesai tanpa error.
int main() { // main() adalah fungsi utama program gabungan; berkat -DBUILD_WITH_MENU, hanya main() ini yang ikut dikompilasi.
    enableUtf8Console();  // enableUtf8Console(); menyiapkan konsol Windows untuk UTF-8 sebelum ada keluaran apa pun.
    printBanner();  // printBanner(); menampilkan identitas sekali di awal.

    while (true) {  // while (true) { artinya tampilkan menu berulang-ulang sampai pengguna memilih keluar.
        printMenu();  // printMenu(); menampilkan kotak menu.
        const int choice = readMenuChoice();  // readMenuChoice() menunggu nomor menu yang sah (0-3); const int choice artinya nilainya tidak diubah lagi.

        // switch berfungsi untuk mengarahkan satu nilai ke salah satu case; bentuk ini lebih rapi dibaca daripada rangkaian if-else, karena pilihannya sedikit dan tetap.
        switch (choice) { // switch (choice) artinya pilih jalur sesuai nilai choice.
            // case N: berfungsi untuk menandai jalur yang dijalankan bila nilainya N; break di ujung tiap case menghentikan switch agar case berikutnya tidak ikut jalan.
            case 1: task1::run(); break;  // case 1: task1::run() artinya jalankan Program 1: Tools Nilai Mahasiswa, yaitu fungsi run() di dalam namespace task1.
            case 2: task2::run(); break;  // case 2: task2::run() artinya jalankan Program 2: Volume Tabung.
            case 3: task3::run(); break;  // case 3: task3::run() artinya jalankan Program 3: Luas Bangun yang Diarsir.
            case 0:  // case 0: artinya Keluar: cetak salam penutup berwarna hijau, lalu akhiri program.
                std::cout << '\n' << colorize(ANSI_SUCCESS, "Terima kasih, program selesai.") << '\n'; // colorize(ANSI_SUCCESS, "Terima kasih, program selesai.") adalah salam penutup hijau tebal.
                return 0;  // return 0; mengakhiri main(), jadi program selesai sekaligus keluar dari perulangan; 0 berarti selesai tanpa error.
            // default berfungsi untuk menandai jalur yang dijalankan bila tidak ada case yang cocok.
            default:  // default: tidak akan terjadi, karena pilihan sudah dibatasi 0-3 oleh readMenuChoice().
                break;  // break; ditulis agar switch lengkap.
        } // akhir switch
        waitForEnter();  // waitForEnter(); menahan hasil di layar sebelum menu muncul lagi.
    } // akhir perulangan menu while (true)
}  // akhir main()
