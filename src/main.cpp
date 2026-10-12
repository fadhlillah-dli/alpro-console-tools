// ===== main.cpp — program bermenu yang menggabungkan Tugas 1, 2, dan 3 =====
// Ketiga tugas praktikum sebenarnya sudah jadi program utuh: tugas1_nilai_mahasiswa.cpp,
// tugas2_volume_tabung.cpp, dan tugas3_luas_arsir.cpp. Semuanya ada di folder src/ bersama berkas ini,
// dan masing-masing bisa dikompilasi sendiri. Berkas ini merangkai ketiganya menjadi satu program bermenu.
// Isinya kode pendukung yang tidak bisa ditaruh di berkas tugas: banner identitas, menu, pembacaan pilihan,
// jeda, dan perulangan menu.
// Baris yang diawali // adalah komentar: catatan untuk pembaca yang diabaikan oleh compiler.
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Cara mengompilasi program gabungan (jalankan dari folder proyek):
//   g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
//   (Windows PowerShell: .\tugas_alpro.exe).
// Arti tiap bagian perintah itu:
//   -std=c++17         memakai aturan bahasa C++17
//   -Wall -Wextra      menampilkan peringatan tambahan
//   -DBUILD_WITH_MENU  memasang penanda bernama BUILD_WITH_MENU (alasannya ada di bawah)
//   -o tugas_alpro     nama program hasil kompilasi
//   src/*.cpp          semua berkas .cpp di folder src
//   && ./tugas_alpro   menjalankan program hasilnya, tetapi hanya bila kompilasi berhasil
// Kenapa perlu -DBUILD_WITH_MENU? Setiap berkas tugas punya main() sendiri, padahal satu program hanya boleh
// punya satu main(). Opsi -D berfungsi untuk memasang penanda tadi saat kompilasi. Bila penanda itu ada, main()
// di setiap berkas tugas tidak ikut dikompilasi, jadi yang dipakai hanya main() di bagian paling bawah berkas ini.
//
// Cara lain di VS Code (editor kode): buka menu Terminal > Run Task..., lalu pilih task "Build & Run".
// Task adalah perintah siap pakai; yang satu ini mengompilasi lalu menjalankan program.
// Daftar task ada di berkas .vscode/tasks.json. Pintasan Cmd/Ctrl+Shift+B (Cmd di macOS, Ctrl di
// Windows/Linux) hanya menjalankan task "Build (g++)": mengompilasi saja, tanpa menjalankan program.

// Kita butuh banyak fungsi siap pakai dari pustaka, tetapi compiler baru mengenal sebuah fungsi setelah
// deklarasinya terlihat. Deklarasi adalah pemberitahuan "fungsi ini ada", lengkap dengan tipe dan parameternya.
// #include berfungsi untuk menyalin isi sebuah header ke berkas ini sebelum dikompilasi.
// Header adalah berkas berisi deklarasi fungsi dan tipe milik pustaka.
// Tanda <...> berarti header itu milik pustaka standar C++ atau milik sistem operasi.
// Awalan std:: (misalnya std::cout) menandai nama yang berasal dari pustaka standar C++.
#include <algorithm>    // std::max dan std::min: memilih yang lebih besar atau lebih kecil dari dua nilai
#include <cctype>       // std::isdigit: memeriksa apakah sebuah karakter adalah angka 0-9
// std::size_t adalah tipe bilangan bulat tanpa tanda negatif yang dipakai C++ untuk ukuran dan panjang.
#include <cstddef>      // di berkas ini std::size_t dipakai untuk lebar teks
// <cstdlib> berisi alat umum. Yang kita pakai di berkas ini: std::exit dan EXIT_FAILURE untuk menghentikan
// program; std::getenv untuk membaca pengaturan dari terminal; std::atoi untuk mengubah teks angka menjadi int.
// Masing-masing dijelaskan lagi saat dipakai di bawah.
#include <cstdlib>
#include <iostream>     // std::cin membaca ketikan, std::cout menulis ke layar, std::cerr untuk pesan galat
#include <string>       // std::string (teks yang panjangnya bisa berubah) dan std::getline (membaca satu baris)
// std::string_view adalah "jendela" untuk membaca sebuah teks tanpa menyalinnya.
// Kita memakainya dikarenakan menyalin teks yang tidak akan diubah hanya membuang waktu dan memori.
#include <string_view>
#include <utility>      // std::pair: pasangan dua nilai, di sini nama menu dan keterangannya
#include <vector>       // std::vector: daftar berurutan yang bisa bertambah panjang, di sini baris isi kotak

// Program ini harus jalan di Windows maupun macOS/Linux, padahal cara ketiganya
// "berbicara" dengan terminal berbeda. Jadi di sini kita bertanya ke compiler:
// "ini sedang dikompilasi untuk Windows atau bukan?" Itulah arti #ifdef _WIN32
// (if defined: apakah nama _WIN32 ada? di Windows nama itu otomatis ada).
// Baris sampai #else hanya ikut di Windows, baris setelah #else hanya di macOS/Linux.
#ifdef _WIN32
    // Sebelum memanggil windows.h, kita pastikan dulu makro NOMINMAX sudah ada.
    // Makro itu seperti catatan tempel untuk compiler: nama yang diganti teks lain.
    // NOMINMAX artinya "jangan buat min dan max", karena windows.h punya kebiasaan
    // membuat makro min/max sendiri yang merusak std::min dan std::max kita.
    // #ifndef = "if not defined": blok ini hanya jalan bila NOMINMAX belum ada,
    // dikarenakan MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu dan
    // mendefinisikan ulang nama yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    #define NOMINMAX 1  // bentuknya: #define NAMA NILAI; mulai baris ini NOMINMAX berarti 1
    #endif              // selesai mengecek NOMINMAX
    // windows.h adalah pustaka resmi Windows. Dari sini kita memakai fungsi untuk
    // warna, UTF-8, dan lebar jendela; semuanya dipanggil di fungsi-fungsi di bawah.
    // UTF-8 adalah aturan menyimpan setiap huruf dan simbol, termasuk π dan garis kotak, sebagai 1 sampai 4 byte.
    #include <windows.h>
#else   // bukan Windows, berarti macOS atau Linux
    #include <cstdio>       // untuk fileno dan stdout: "saluran" keluaran ke layar
    #include <sys/ioctl.h>  // untuk ioctl dan TIOCGWINSZ: bertanya ke terminal seberapa lebar jendelanya
    #include <unistd.h>     // untuk isatty: memeriksa apakah keluaran benar-benar terminal, bukan berkas
#endif  // selesai memilih pustaka sesuai sistem operasi

// Isi ketiga program tugas ada di berkas lain, padahal main() di bawah perlu memanggilnya.
// C++ mewajibkan fungsi dideklarasikan sebelum dipanggil; isinya (definisinya) ditulis di berkas tugas
// masing-masing di folder src/.
// Ketiga fungsi itu sama-sama bernama run(). Masing-masing ditaruh di namespace (wadah nama) sendiri,
// yaitu task1, task2, dan task3, dikarenakan satu wadah tidak boleh berisi dua fungsi bernama sama.
// Cara memanggilnya: task1::run(), task2::run(), dan task3::run().
// void berarti fungsi tidak mengembalikan nilai, dan kurung () kosong berarti fungsi tidak menerima masukan.
namespace task1 { void run(); }  // Program 1: Tools Nilai Mahasiswa, isinya di src/tugas1_nilai_mahasiswa.cpp
namespace task2 { void run(); }  // Program 2: Volume Tabung, isinya di src/tugas2_volume_tabung.cpp
namespace task3 { void run(); }  // Program 3: Luas Bangun yang Diarsir, isinya di src/tugas3_luas_arsir.cpp

// Nama-nama di bawah, misalnya colorize() dan drawBox(), juga ada di berkas tugas, dan nama yang sama tidak
// boleh bentrok saat semua berkas digabung. Jalan keluarnya: bungkus semuanya dengan namespace tanpa nama, yaitu
// kata namespace yang langsung diikuti kurung kurawal. Isinya hanya terlihat dari berkas ini.
// Bungkus ini ditutup oleh baris "}  // akhir namespace tanpa nama" tepat sebelum main().
namespace {

    // ===== WARNA DAN KOTAK TAMPILAN =====
    // Supaya mudah dibedakan, judul, nomor menu, ajakan menekan Enter, dan pesan kesalahan diberi warna sendiri.
    // Kita memakai kode warna ANSI, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    // Kode ANSI adalah teks khusus yang tidak tercetak; terminal membacanya sebagai perintah ganti gaya.
    // Bentuknya: "\033", lalu "[", angka gaya, dan huruf m sebagai penutup.
    // "\033" adalah karakter ESC: nomor 27, ditulis dalam oktal (basis 8), jadi 033 = 3 x 8 + 3 = 27.
    // constexpr membuat nilai tetap yang sudah pasti sejak kompilasi dan tidak bisa diubah.
    // const char* adalah penunjuk: nilai berisi alamat tempat teks itu disimpan di memori, bukan teksnya sendiri.
    // Di sini ia menunjuk teks tetap (string gaya bahasa C). Cocok, dikarenakan kode warna tidak berubah.
    constexpr const char* ANSI_RESET  = "\033[0m";     // 0 = kembali ke tampilan normal
    // Prompt adalah teks pertanyaan yang menunggu ketikan pengguna, misalnya "Pilihan Anda [0-3] : ".
    constexpr const char* ANSI_BOLD  = "\033[1m";     // 1 = tebal: prompt dan nama menu
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // 1;36 = cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // 1;32 = hijau tebal: pesan penutup program
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // 1;31 = merah tebal: pesan kesalahan
    constexpr const char* ANSI_HINT = "\033[33m";    // 33 = kuning: ajakan "Tekan Enter untuk kembali ke menu..."
    constexpr const char* ANSI_DIM    = "\033[90m";    // 90 = abu-abu: garis kotak

    // Simbol π, ², ³, garis kotak, dan sketsa Braille dikirim ke layar sebagai byte UTF-8. Masalahnya, konsol
    // Windows bawaan (Command Prompt, PowerShell) masih memakai code page lama, misalnya 437, sehingga simbol
    // tadi tampil sebagai huruf acak. Code page adalah tabel huruf: aturan byte mana menjadi huruf apa.
    // Jadi sebelum keluaran pertama, fungsi ini mengganti konsol Windows ke UTF-8 (code page 65001).
    // Dipanggil di main() di bagian paling bawah berkas.
    void enableUtf8Console() {
    // Garis kotak berasal dari drawBox() di bawah. Simbol dan sketsa Braille berasal dari berkas tugas 2 dan 3
    // (³ hanya di tugas 2). Pengaturan ini berlaku untuk jendela konsol yang sedang dipakai, sama seperti
    // perintah chcp 65001 di berkas peluncur jalankan.bat (folder proyek).
    // SetConsoleOutputCP dan SetConsoleCP hanya ada di windows.h, jadi dua baris di bawah hanya ikut
    // dikompilasi di Windows. Di macOS/Linux terminal sudah UTF-8, jadi isi fungsi ini kosong.
    #ifdef _WIN32
        // Untuk keluaran: byte UTF-8 ditampilkan sebagai karakter yang benar.
        // CP_UTF8 adalah konstanta Windows bernilai 65001, nomor code page UTF-8.
        SetConsoleOutputCP(CP_UTF8);
        // Untuk masukan: ketikan dengan huruf non-ASCII juga dibaca sebagai UTF-8, sama dengan keluarannya.
        // ASCII adalah kumpulan 128 karakter dasar: A-Z dan a-z tanpa aksen, angka 0-9, tanda baca, spasi,
        // dan karakter kendali seperti pindah baris. Huruf seperti é atau π termasuk non-ASCII.
        SetConsoleCP(CP_UTF8);
    #endif  // akhir bagian khusus Windows
    }

    // Boleh pakai warna atau tidak? Aturannya mengikuti kebiasaan program terminal:
    // kalau pengguna menyetel NO_COLOR, warna mati; kalau FORCE_COLOR, warna dipaksa hidup;
    // selain itu warna hanya dipakai bila keluaran benar-benar terminal, bukan berkas.
    // Di Windows, fungsi ini sekaligus menyalakan mode "virtual terminal" agar Command Prompt memahami kode ANSI.
    // Hasilnya bool: true berarti boleh berwarna.
    bool colorsEnabled() {
        // Pemeriksaannya cukup sekali seumur program, jadi hasilnya disimpan di variabel
        // static (dibuat sekali, diingat terus). Nilai awalnya dihitung oleh lambda, yaitu
        // fungsi kecil tanpa nama berbentuk []() { ... }, yang langsung dijalankan oleh
        // tanda () di baris "}();" paling bawah.
        static const bool enabled = []() {
            // getenv membaca variabel lingkungan, pengaturan yang diwariskan terminal ke
            // program. Hasil nullptr (penunjuk kosong) berarti variabel itu tidak ada.
            // return langsung mengakhiri lambda dan memberikan hasilnya.
            if (std::getenv("NO_COLOR") != nullptr) return false;   // pengguna minta tanpa warna
            if (std::getenv("FORCE_COLOR") != nullptr) return true; // pengguna memaksa warna
        // Cara memeriksa terminal berbeda di Windows dan macOS/Linux, jadi kita memilih lagi dengan #ifdef _WIN32.
        #ifdef _WIN32
            // Di Windows, kode ANSI baru dimengerti setelah mode "virtual terminal" dinyalakan.
            // Caranya: ambil pegangan (HANDLE) ke layar, baca mode konsol yang sedang aktif,
            // lalu tulis kembali mode itu dengan tambahan satu bendera.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;  // DWORD = bilangan bulat milik Windows, tempat mode konsol disimpan
            // Kalau pegangannya tidak sah, atau keluaran ternyata bukan konsol (misalnya
            // dialihkan ke berkas), menyerah saja: tanpa warna.
            // Tanda || berarti "atau", dan tanda ! berarti "tidak" (di sini: GetConsoleMode gagal).
            // &mode adalah alamat variabel mode, supaya GetConsoleMode bisa mengisinya.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // Tanda | menambahkan bendera ke mode lama tanpa menghapus yang sudah ada.
            // SetConsoleMode mengembalikan 0 bila gagal, jadi "!= 0" artinya berhasil.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else   // bukan Windows: macOS atau Linux
            // Di macOS/Linux cukup satu pertanyaan: apakah keluaran layar (stdout) sebuah terminal?
            // fileno(stdout) memberi nomor salurannya, lalu isatty menjawab. Hasil selain 0 berarti ya.
            return isatty(fileno(stdout)) != 0;
        #endif  // akhir pemeriksaan sesuai sistem operasi
        }();  // tanda () ini menjalankan lambda sekali; hasilnya jadi isi variabel enabled
        return enabled;  // pemanggilan berikutnya tinggal memakai hasil yang tersimpan
    }

    // Fungsi kecil ini membungkus teks dengan kode warna, tetapi hanya bila warna boleh dipakai.
    // Kalau tidak, teks dikembalikan apa adanya. Parameter code diisi salah satu konstanta ANSI_... di atas,
    // sedangkan text dibaca lewat std::string_view tanpa disalin. Hasilnya std::string baru, siap dicetak.
    std::string colorize(const char* code, std::string_view text) {
        if (!colorsEnabled()) return std::string(text);  // warna mati: kembalikan salinan teks apa adanya
        // Tiga potong teks disambung dengan +: kode warna, teks, lalu ANSI_RESET. ANSI_RESET di ujung berfungsi
        // agar warna tidak terbawa ke teks berikutnya. code dan text diubah dulu ke std::string supaya bisa disambung.
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Tidak semua karakter selebar satu kolom. Aksara CJK (Cina, Jepang, Korea), Hangul (huruf Korea), dan emoji
    // digambar terminal selebar dua huruf biasa. Kalau ini diabaikan, lebar kotak dan pembungkusan baris jadi
    // salah. Fungsi ini menjawab: apakah code point ini termasuk karakter lebar?
    // Code point adalah nomor unik sebuah karakter di Unicode, daftar baku semua karakter di dunia.
    // unsigned berarti bilangan bulat tanpa tanda negatif.
    // Dipakai di displayWidth() di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Angka berawalan 0x ditulis dalam heksadesimal (basis 16). Setiap pasangan (>= awal && <= akhir) adalah
        // satu rentang karakter lebar. Tanda && berarti "dan". Rentangnya:
        //   0x1100-0x115F                     Hangul Jamo, huruf penyusun Hangul
        //   0x2E80-0xA4CF                     CJK dan aksara Asia Timur lain
        //   0xAC00-0xD7A3                     suku kata Hangul
        //   0xF900-0xFAFF dan 0xFE30-0xFE4F   CJK kompatibilitas
        //   0xFF00-0xFF60 dan 0xFFE0-0xFFE6   huruf lebar penuh
        //   0x1F300-0x1F64F dan 0x1F900-0x1F9FF   emoji
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berapa kolom yang dipakai sebuah teks di layar? Jawabannya tidak selalu sama dengan jumlah byte-nya.
    // UTF-8 menyimpan satu karakter dalam 1 sampai 4 byte, padahal garis kotak tetap selebar satu kolom.
    // Aksara lebar (CJK, emoji) memakai dua kolom, dan kode warna ANSI tidak memakai kolom sama sekali.
    // Fungsi ini menghitungnya byte demi byte. Hasilnya bertipe std::size_t.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah terhitung
        bool inEscape = false;  // true selama berada di dalam kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu
        // Ini perulangan for "untuk setiap" (range-based for): mengambil setiap byte di text satu per satu ke
        // variabel c, dari awal sampai akhir, tanpa perlu indeks.
        // unsigned char adalah satu byte bernilai 0-255. const berarti c tidak diubah di dalam perulangan.
        for (const unsigned char c : text) {
            // Di dalam kode warna, byte dilewati sampai huruf 'm' penutupnya.
            // continue berarti langsung lanjut ke byte berikutnya, tanpa menjalankan baris di bawahnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC, pembuka kode warna
            // Satu byte = 8 bit, masing-masing 0 atau 1. Byte lanjutan UTF-8 selalu berpola 10xxxxxx: dua bit
            // pertamanya 1 lalu 0, sisanya (x) adalah data. Operator & (AND per bit) dengan 0xC0 mengambil 2 bit
            // teratas byte. Hasil 0x80 berarti ini byte lanjutan: 6 bitnya ditambahkan ke code point.
            if ((c & 0xC0) == 0x80) {
                // << 6 menggeser code point 6 bit ke kiri, lalu | menempelkan 6 bit terbawah byte ini (c & 0x3F).
                // --pending mengurangi sisa byte. Bila sisanya 0, code point sudah lengkap. Karakter lebar lalu
                // mendapat satu kolom tambahan (++width), dikarenakan kolom pertamanya sudah dihitung di byte awal.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            // Pola bit byte awal menentukan berapa byte lanjutan yang menyusul: 110xxxxx = 1, 1110xxxx = 2,
            // 11110xxx = 3. Bit x milik byte awal disimpan sebagai awal code point.
            // Byte ASCII biasa (0xxxxxxx) tidak punya lanjutan.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }
        return width;
    }

    // Seberapa lebar jendela terminal saat ini, dalam kolom? Fungsi ini menanyakannya setiap kali dipanggil,
    // jadi tampilan ikut menyesuaikan bila jendela diubah ukurannya. Kalau terminal tidak bisa ditanya
    // (keluaran bukan terminal), kita coba variabel lingkungan COLUMNS. Kalau itu juga tidak ada, pakai
    // 80 kolom, ukuran bawaan terminal pada umumnya. Hasilnya int dan tidak pernah di bawah 50,
    // dikarenakan di bawah 50 kolom kotak menu tidak bisa rapi.
    int terminalWidth() {
        int cols = 0;  // 0 = lebar belum diketahui
    // Cara bertanya ke terminal berbeda di Windows dan macOS/Linux, jadi kita memilih lagi dengan #ifdef _WIN32.
    #ifdef _WIN32
        // info bertipe struct milik Windows. Struct adalah wadah berisi beberapa data bernama.
        // GetConsoleScreenBufferInfo mengisi info lewat alamatnya (&info). Bagian srWindow menyimpan nomor kolom
        // kiri (Left) dan kanan (Right) jendela, jadi lebarnya = Right - Left + 1.
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // Di sini ukuran jendela ditampung struct winsize dari sys/ioctl.h; bagian ws_col berisi jumlah kolom.
        struct winsize ws;
        // ioctl dengan TIOCGWINSZ meminta ukuran jendela dari saluran keluaran layar (STDOUT_FILENO),
        // lalu mengisinya ke ws lewat alamatnya (&ws). Hasil 0 berarti berhasil.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // Masih belum tahu? Coba COLUMNS. env menunjuk ke teksnya, atau bernilai nullptr bila COLUMNS tidak ada.
        // std::atoi lalu mengubah teks itu (misalnya "100") menjadi angka int 100.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // tetap belum diketahui: pakai 80 kolom
        return std::max(50, cols);  // ambil yang lebih besar, jadi hasilnya tidak pernah di bawah 50
    }

    // Bagaimana kalau sebuah baris lebih lebar dari jendela? Jangan biarkan terminal memotongnya sembarangan.
    // Fungsi ini memotongnya sendiri di spasi, menjadi beberapa baris yang masing-masing muat dalam maxWidth kolom.
    // Baris lanjutan diberi indentasi "gantung" sebanyak hangingIndent spasi, agar sejajar dengan isi setelah label.
    // Parameter text bertipe const std::string&, yaitu referensi tetap. Referensi berarti fungsi memakai teks
    // aslinya tanpa menyalin; const berarti teks itu tidak boleh diubah.
    // Hasilnya std::vector<std::string>, satu elemen untuk satu baris. Dipakai drawBox() dan printError() di bawah.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil, mula-mula kosong
        // Jendela terlalu sempit untuk indentasi gantung + 10 kolom isi? Pakai indentasi 2 spasi saja.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // sisa teks yang belum dipotong; disalin dikarenakan isinya akan diganti
        // Selama sisa teks masih lebih lebar dari maxWidth, potong satu baris lagi dari depannya.
        while (displayWidth(remaining) > maxWidth) {
            // col menghitung kolom layar, dan cutPos menyimpan posisi spasi tempat memotong.
            // std::string::npos adalah nilai khusus yang berarti "tidak ditemukan"; di sini: belum ada spasi.
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama berada di dalam kode warna (tidak memakai kolom)
            // Cari spasi terakhir sebelum batas lebar. Perulangan for biasa ini mulai dari i = 0, berulang
            // selama i < remaining.size() (jumlah byte), dan menambah i satu (++i) setiap putaran.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char> mengubah char menjadi unsigned char (0-255) secara tertulis.
                // Ini perlu dikarenakan byte UTF-8 di atas 127 bisa terbaca sebagai angka negatif pada char biasa.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // lewati isi kode warna sampai 'm'
                if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = ESC: awal kode warna
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8 bukan karakter baru, jadi tidak menambah kolom
                if (col >= maxWidth) break;  // batas lebar tercapai; break keluar dari perulangan for ini
                // Spasi menjadi calon titik potong, asalkan letaknya setelah indentasi gantung.
                if (c == ' ' && col > hangingIndent) cutPos = i;
                ++col;  // karakter ini memakai satu kolom
            }
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            // substr(0, cutPos) mengambil teks dari awal sampai sebelum spasi; push_back menambahkannya ke result.
            result.push_back(remaining.substr(0, cutPos));
            // Sisanya, mulai setelah spasi, diberi awalan hangingIndent spasi. std::string(n, ' ') = n buah spasi.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }
        result.push_back(remaining);  // sisa terakhir sudah muat, jadi ia menjadi baris terakhir
        return result;
    }

    // Fungsi ini menggambar kotak berjudul berisi baris-baris teks. Kotak membuat informasi (petunjuk, menu,
    // hasil) terpisah jelas dari pertanyaan, dikarenakan mata langsung menangkap batasnya.
    // Lebar kotak mengikuti baris terpanjang, tetapi tidak pernah melebihi lebar terminal. Baris yang terlalu
    // panjang dibungkus dulu dengan wrapText() di atas. Bila baris berpola "Label : isi", lanjutannya dibuat
    // sejajar dengan isi setelah label.
    // Garis kotak ditulis sebagai kode Unicode di dalam string: "\u250c" = ┌, "\u2510" = ┐, "\u2514" = └,
    // "\u2518" = ┘, "\u2500" = ─ (garis datar), dan "\u2502" = │ (garis tegak).
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // maxWidth adalah lebar isi terbesar: lebar terminal dikurangi 2 tepi kotak, 2 margin, dan 1 kolom
        // cadangan. Kolom cadangan perlu dikarenakan sebagian terminal menambah baris kosong bila sebuah baris
        // persis selebar jendela. static_cast<std::size_t> mengubah hasil int menjadi std::size_t,
        // supaya setipe dengan ukuran teks lainnya.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris isi setelah dibungkus, siap digambar
        // Untuk setiap baris b di lines: tentukan dulu indentasi gantungnya, lalu bungkus dengan wrapText().
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // bawaan: baris lanjutan menjorok 2 spasi
            const std::size_t labelPos = b.find(" : ");  // posisi " : " pertama, atau npos bila tidak ada
            // Ada " : " dan label di depannya paling lebar 28 kolom? Berarti baris ini berpola "Label : isi".
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // sejajar dengan isi setelah " : "
            }
            // Setiap potongan (piece) hasil wrapText() dimasukkan ke contentLines.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Perlebar sampai baris terpanjang + 2, yaitu margin kiri dan kanan.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // tetapi tidak boleh lebih dari maxWidth + 2 margin
        std::string filler;  // garis pengisi di kanan judul
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // Garis atas: ┌─ judul ───┐. std::cout << ... mencetak bagian-bagiannya ke layar secara berurutan.
        // '\n' adalah karakter pindah baris.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Baris isi: │, teks, spasi pengisi sampai lebar kotak, lalu │ lagi.
        // Operator ternary (a ? b : c) berarti "bila a benar pakai b, bila tidak pakai c". Di sini ia menjaga
        // jumlah spasi pengisi agar tidak negatif, jadi paling sedikit 0.
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;  // satu spasi margin kiri
            const std::size_t contentWidth = displayWidth(rowText);
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }
        std::string bottomLine;  // garis bawah: ─ sebanyak lebar kotak
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';  // garis bawah: └───┘
    }

    // Pesan kesalahan dicetak merah dan dibungkus sesuai lebar terminal, dikarenakan pesan panjang tidak boleh
    // terpotong jendela sempit. Pesannya selalu diawali "  [!] ", jadi baris lanjutan menjorok 6 spasi agar
    // sejajar dengan teks setelah tanda "[!]". Dipakai readMenuChoice() di bawah.
    void printError(const std::string& text) {
        // Lebar bungkus = lebar terminal dikurangi 1 kolom cadangan, dan 6 = indentasi gantung.
        // Setiap baris hasilnya dicetak merah.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Banner identitas ditampilkan sekali di awal program, di dalam kotak (dipanggil dari main() di bawah).
    // Daftar teks di dalam { ... } langsung menjadi parameter lines milik drawBox().
    void printBanner() {
        drawBox("Tugas Praktikum Algoritma dan Struktur Data", {
            "Modul 1 : Input dan Output",
            "Nama    : Fadhlillah",
            "NIM     : 2531070011"
        });
    }

    // Menampilkan daftar pilihan program dalam kotak, lengkap dengan keterangan singkat tiap program.
    // [0] Keluar sengaja diletakkan terakhir, dikarenakan menu konsol biasanya menaruh "keluar" di akhir.
    void printMenu() {
        std::cout << '\n';  // satu baris kosong sebagai pemisah dari tampilan sebelumnya
        // Jendela sempit atau lebar? Di bawah 100 kolom, keterangan diturunkan ke baris berikutnya.
        // Kalau lebar, keterangan sejajar di kanan nama program. Dua tata letak ini menjaga menu tetap rapi.
        const bool isNarrow = terminalWidth() < 100;
        // menuItems adalah std::vector berisi std::pair; setiap pasangan = { nama menu, keterangannya }.
        // Nama menu diberi spasi di akhir agar sama panjang, sehingga keterangan di kanannya sejajar lurus.
        const std::vector<std::pair<std::string, std::string>> menuItems = {
            { "[1] Tools Nilai Mahasiswa", "Rekap Nama, Nilai, Total, dan Rata-Rata satu mata kuliah" },
            { "[2] Volume Tabung        ", "V = Pi x R x R x T; satuan, nilai Pi, jari-jari/diameter dipilih" },
            { "[3] Luas Bangun Diarsir  ", "Persegi dikurangi lingkaran di dalamnya; satuan dan Pi dipilih" },
            { "[0] Keluar               ", "Menutup program" },
        };
        std::vector<std::string> lines;  // baris-baris yang akan digambar di dalam kotak
        // Ambil setiap item di menuItems. auto berarti tipenya ditentukan compiler, di sini std::pair.
        // const& memakai elemen aslinya tanpa menyalin. item.first adalah nama menu (dicetak tebal),
        // dan item.second keterangannya. Jendela sempit: dua baris, keterangan menjorok 4 spasi.
        // Jendela lebar: satu baris, keduanya dipisah 3 spasi.
        for (const auto& item : menuItems) {
            if (isNarrow) { lines.push_back(colorize(ANSI_BOLD, item.first)); lines.push_back("    " + item.second); }
            else        { lines.push_back(colorize(ANSI_BOLD, item.first) + "   " + item.second); }
        }
        drawBox("Menu Program", lines);
        std::cout << "Ketik nomor menu lalu tekan Enter.\n";  // \n di dalam string juga berarti pindah baris
    }

    // Membaca nomor menu 0-3. Ketikan lain, misalnya 9, abc, atau 2.5, ditolak dan pertanyaannya diulang.
    // Hasilnya int: nomor pilihan yang sah.
    int readMenuChoice() {
        while (true) {  // while (true) berulang tanpa henti sampai pilihan sah; jalan keluarnya lewat return
            std::string text;  // menampung satu baris ketikan pengguna
            std::cout << colorize(ANSI_BOLD, "Pilihan Anda [0-3] : ");
            // std::getline(std::cin, text) membaca satu baris ketikan utuh ke text.
            // getline gagal bila input sudah ditutup, artinya tidak ada ketikan lagi yang bisa dibaca. Ini terjadi
            // bila pengguna menekan Ctrl+D (macOS/Linux) atau Ctrl+Z lalu Enter (Windows), atau bila input
            // dialihkan dari berkas (misalnya ./tugas_alpro < masukan.txt) dan isinya sudah habis.
            // Tanda ! membalik hasilnya, jadi isi blok if ini dijalankan saat getline gagal.
            if (!std::getline(std::cin, text)) {
                std::cerr << "\nInput berakhir. Program dihentikan.\n";  // std::cerr = saluran pesan galat
                std::exit(EXIT_FAILURE);  // hentikan program dengan kode keluar EXIT_FAILURE ("gagal")
            }
            // Spasi di awal dan akhir dibuang dulu, agar " 2 " tetap diterima; isi lainnya diperiksa ketat di bawah.
            // find_first_not_of dan find_last_not_of mencari posisi karakter pertama dan terakhir yang bukan spasi,
            // tab (\t), atau \r. Karakter \r adalah sisa akhir baris Windows pada input dari berkas.
            const auto start  = text.find_first_not_of(" \t\r");
            const auto end = text.find_last_not_of(" \t\r");
            // Operator ternary lagi: bila isinya spasi semua (start bernilai npos), text menjadi kosong.
            // Bila tidak, text diganti potongan mulai posisi start sepanjang end - start + 1 karakter.
            text = (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
            // Sah hanya bila teksnya tepat satu digit 0-3; bentuk lain (+1, 1.0, 1 2, abc, kosong) ditolak.
            // text.size() == 1 memastikan tepat satu karakter, std::isdigit memastikan karakter itu angka 0-9,
            // dan text[0] <= '3' memastikan paling besar 3. static_cast dipakai dikarenakan isdigit hanya aman
            // untuk nilai 0-255.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0])) && text[0] <= '3') {
                // Karakter disimpan sebagai nomor kode berurutan: '0' = 48, '2' = 50. Jadi '2' - '0' berfungsi
                // mengubah karakter digit menjadi angka 2, dikarenakan selisihnya sama dengan nilai angkanya.
                return text[0] - '0';
            }
            // Sampai di sini berarti ketikan tidak sah: tampilkan pesan, lalu while (true) mengulang pertanyaan.
            printError("  [!] Pilihan tidak dikenal. Ketik 1, 2, 3, atau 0 lalu tekan Enter.");
        }
    }

    // Menahan layar sebentar, supaya hasil sempat dibaca sebelum menu muncul lagi.
    void waitForEnter() {
        std::cout << '\n' << colorize(ANSI_HINT, "Tekan Enter untuk kembali ke menu...");
        std::string discard;             // isinya diabaikan, dikarenakan yang dibutuhkan hanya tombol Enter
        std::getline(std::cin, discard);  // tunggu sampai pengguna menekan Enter
    }

}  // akhir namespace tanpa nama: semua di atas hanya terlihat di berkas ini

// Di sinilah program dimulai: main() adalah fungsi yang pertama kali dijalankan saat program dibuka.
// Standar C++ mewajibkan main bertipe int, artinya main mengembalikan bilangan bulat ke sistem operasi.
// Nilai 0 (lihat return 0 di bawah) menandakan program selesai tanpa error.
int main() {
    enableUtf8Console();  // siapkan konsol Windows untuk UTF-8 sebelum ada keluaran apa pun
    printBanner();  // tampilkan identitas sekali di awal

    while (true) {  // tampilkan menu berulang-ulang sampai pengguna memilih keluar
        printMenu();  // tampilkan kotak menu
        const int choice = readMenuChoice();  // tunggu nomor menu yang sah (0-3); nilainya tidak diubah lagi

        // switch mengarahkan satu nilai pilihan ke satu program. Bentuk ini lebih rapi dibaca daripada
        // rangkaian if-else, dikarenakan pilihannya sedikit dan tetap. "case N:" dijalankan bila choice
        // bernilai N. task1::run() berarti fungsi run() di dalam namespace task1 (dideklarasikan di atas).
        // break di ujung tiap case menghentikan switch, agar case berikutnya tidak ikut jalan.
        switch (choice) {
            case 1: task1::run(); break;  // Program 1: Tools Nilai Mahasiswa
            case 2: task2::run(); break;  // Program 2: Volume Tabung
            case 3: task3::run(); break;  // Program 3: Luas Bangun yang Diarsir
            case 0:  // Keluar: cetak salam penutup berwarna hijau, lalu akhiri program
                std::cout << '\n' << colorize(ANSI_SUCCESS, "Terima kasih, program selesai.") << '\n';
                return 0;  // return di sini mengakhiri program sekaligus keluar dari perulangan
            default:  // dijalankan bila tidak ada case yang cocok
                break;  // tidak akan terjadi, dikarenakan pilihan sudah dibatasi 0-3; ditulis agar switch lengkap
        }
        waitForEnter();  // tahan hasil di layar sebelum menu muncul lagi
    }
}  // akhir main()
