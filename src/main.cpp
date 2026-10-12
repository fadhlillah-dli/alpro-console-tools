// ===== main.cpp — program bermenu yang menggabungkan Tugas 1, 2, dan 3 =====
// Berkas ini berisi semua kode pendukung yang tidak bisa dimasukkan ke berkas tugas: banner identitas,
// menu, pembacaan pilihan, jeda, dan perulangan menu. Ketiga berkas tugas (tugas1_*.cpp, tugas2_*.cpp,
// tugas3_*.cpp, semuanya di folder src/ bersama berkas ini) masing-masing sudah lengkap dan bisa dikompilasi sendiri.
// Baris yang diawali // adalah komentar: catatan untuk pembaca yang diabaikan oleh compiler.
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Kompilasi gabungan (dari folder proyek). -DBUILD_WITH_MENU berfungsi untuk mematikan main() di tiap berkas tugas:
// opsi -D membuat makro (penanda) bernama BUILD_WITH_MENU, dan setiap berkas tugas melewati main()-nya bila penanda
// itu ada, sehingga yang dipakai hanya main() di bagian paling bawah berkas ini.
//   g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
//   (Windows PowerShell: .\tugas_alpro.exe).
//   Cara lain di VS Code (editor kode): buka menu Terminal > Run Task..., lalu pilih task (perintah siap pakai)
//   "Build & Run", yang mengompilasi lalu menjalankan program; daftar task ada di berkas .vscode/tasks.json.
//   Pintasan Cmd/Ctrl+Shift+B (Cmd di macOS, Ctrl di Windows/Linux) hanya menjalankan task "Build (g++)", yaitu
//   mengompilasi saja tanpa menjalankan program.
// Arti bagian perintah di atas: -std=c++17 = memakai aturan bahasa C++17; -Wall -Wextra = menampilkan peringatan
// tambahan; -o tugas_alpro = nama program hasil kompilasi; src/*.cpp = semua berkas .cpp di folder src;
// && ./tugas_alpro = menjalankan program hasilnya, hanya bila kompilasi berhasil.

// Menggunakan #include berfungsi untuk menyalin isi sebuah header (berkas berisi deklarasi fungsi dan tipe milik
// pustaka) ke berkas ini sebelum dikompilasi, dikarenakan compiler baru mengenal sebuah fungsi pustaka setelah
// deklarasinya disalin. Tanda <...> berarti header milik pustaka standar C++ atau milik sistem operasi.
// Awalan std:: pada sebuah nama (mis. std::cout) berarti nama itu milik pustaka standar C++.
//
// Menggunakan <algorithm> berfungsi untuk std::max dan std::min (memilih nilai terbesar/terkecil dari dua nilai),
// dipakai fungsi terminalWidth() dan drawBox() di bawah saat menghitung lebar terminal dan kotak.
#include <algorithm>
// Menggunakan <cctype> berfungsi untuk std::isdigit (memeriksa apakah sebuah karakter adalah angka 0-9), dipakai
// fungsi readMenuChoice() di bawah untuk memastikan ketikan menu hanya angka.
#include <cctype>
// Menggunakan <cstddef> berfungsi untuk std::size_t, yaitu tipe bilangan bulat tanpa tanda negatif yang dipakai
// C++ untuk ukuran dan panjang (di berkas ini: ukuran/lebar teks).
#include <cstddef>
// Menggunakan <cstdlib> berfungsi untuk std::exit dan EXIT_FAILURE (menghentikan program dengan kode gagal, di
// readMenuChoice()), std::getenv (membaca variabel lingkungan = pengaturan bernama yang diwariskan terminal ke
// program, mis. NO_COLOR atau COLUMNS; di colorsEnabled() dan terminalWidth()), dan std::atoi (mengubah teks angka
// menjadi int, di terminalWidth()); fungsi readMenuChoice(), colorsEnabled(), dan terminalWidth() ada di bawah.
#include <cstdlib>
// Menggunakan <iostream> berfungsi untuk std::cin (masukan dari keyboard), std::cout (keluaran ke layar), dan
// std::cerr (keluaran khusus pesan galat).
#include <iostream>
// Menggunakan <string> berfungsi untuk std::string (teks yang panjangnya bisa berubah) untuk teks ketikan dan baris
// tampilan, serta std::getline (membaca satu baris ketikan utuh).
#include <string>
// Menggunakan <string_view> berfungsi untuk std::string_view, yaitu "jendela" ke sebuah teks yang hanya dibaca,
// tanpa disalin, dikarenakan menyalin teks yang tidak diubah hanya membuang waktu dan memori.
#include <string_view>
// Menggunakan <utility> berfungsi untuk std::pair (pasangan dua nilai), di sini pasangan nama menu dan keterangannya
// pada fungsi printMenu() di bawah.
#include <utility>
// Menggunakan <vector> berfungsi untuk std::vector (daftar berurutan yang panjangnya bisa bertambah), di sini daftar
// baris yang digambar di dalam kotak.
#include <vector>

// Blok #ifdef _WIN32 ... #endif di bawah memilih pustaka sesuai sistem operasi saat kompilasi (kompilasi bersyarat).
// #ifdef _WIN32 adalah pertanyaan ke compiler: "apakah kode ini sedang dikompilasi untuk Windows?" (#ifdef = "if
// defined", apakah sebuah nama sudah didefinisikan). _WIN32 adalah penanda yang otomatis didefinisikan compiler di
// Windows, sehingga baris sampai #else hanya ikut dikompilasi di Windows dan baris setelah #else hanya di
// macOS/Linux; #endif menutup pemilihan ini. Menggunakan pustaka khusus sistem operasi ini berfungsi untuk
// mendeteksi terminal, membaca lebar jendela, mengaktifkan warna, dan UTF-8 (aturan menyimpan setiap huruf dan
// simbol, termasuk π dan garis kotak, sebagai 1 sampai 4 byte; dijelaskan lagi di fungsi displayWidth() di bawah),
// dikarenakan Windows dan macOS/Linux menyediakan fungsi yang berbeda untuk keperluan yang sama.
#ifdef _WIN32
    // #ifndef NOMINMAX adalah pertanyaan ke compiler: "apakah makro NOMINMAX belum ada?" (#ifndef = "if not
    // defined", kebalikan dari #ifdef). Makro adalah nama pengganti teks yang dibuat dengan #define; nama NOMINMAX
    // dibaca "no min max" (jangan buat min dan max). Menggunakan #ifndef NOMINMAX berfungsi agar #define di
    // bawahnya hanya dijalankan bila NOMINMAX belum pernah didefinisikan, dikarenakan MinGW (compiler g++ untuk
    // Windows) sudah mendefinisikannya lewat pustaka standar dan mendefinisikan ulang makro yang sama menghasilkan
    // peringatan.
    #ifndef NOMINMAX
    // Menggunakan #define NOMINMAX 1 berfungsi untuk membuat makro NOMINMAX bernilai 1 (bentuk #define NAMA NILAI:
    // setiap NAMA di kode sesudahnya diganti NILAI sebelum kompilasi). Header windows.h (#include-nya ada di bawah
    // #endif berikut) memeriksa penanda ini dan, bila sudah ada, tidak membuat makro min/max miliknya sendiri,
    // dikarenakan makro min/max itu akan mengganti tulisan min/max pada std::min dan std::max dari <algorithm> di
    // atas (dipakai terminalWidth() dan drawBox() di bawah) sehingga kode gagal dikompilasi.
    #define NOMINMAX 1
    #endif  // akhir pengecekan NOMINMAX
    // Menggunakan <windows.h> berfungsi untuk fungsi-fungsi Windows yang dipanggil di fungsi colorsEnabled(),
    // terminalWidth(), dan enableUtf8Console() di bawah: GetStdHandle, GetConsoleMode dan SetConsoleMode (warna),
    // SetConsoleOutputCP dan SetConsoleCP (UTF-8), serta GetConsoleScreenBufferInfo (lebar jendela).
    #include <windows.h>
#else  // bukan Windows: macOS atau Linux
    // Menggunakan <cstdio> berfungsi untuk fileno dan stdout (saluran keluaran ke layar), yang dipakai bersama
    // isatty di fungsi colorsEnabled() di bawah.
    #include <cstdio>
    // Menggunakan <sys/ioctl.h> berfungsi untuk ioctl, TIOCGWINSZ, dan struct winsize (struct = wadah berisi
    // beberapa data bernama), dipakai fungsi terminalWidth() di bawah untuk membaca lebar jendela terminal.
    #include <sys/ioctl.h>
    // Menggunakan <unistd.h> berfungsi untuk isatty (memeriksa apakah keluaran adalah terminal, bukan berkas), dipakai
    // di colorsEnabled(), dan STDOUT_FILENO (nomor saluran keluaran layar), dipakai di terminalWidth() di bawah.
    #include <unistd.h>
#endif  // akhir pemilihan pustaka sesuai sistem operasi

// Tiga baris di bawah adalah deklarasi fungsi: hanya memberi tahu compiler bahwa fungsi run() ada, sedangkan isinya
// (definisinya) ditulis di berkas tugas masing-masing di folder src/. Menggunakan deklarasi berfungsi untuk
// mengizinkan main() di bawah memanggil fungsi-fungsi itu, dikarenakan C++ mewajibkan fungsi dideklarasikan sebelum
// dipanggil. Menggunakan namespace (wadah nama) task1, task2, dan task3 berfungsi untuk membedakan tiga fungsi yang
// sama-sama bernama run(), dikarenakan satu program tidak boleh punya dua fungsi bernama sama di wadah yang sama;
// memanggilnya ditulis task1::run(), task2::run(), dan task3::run(). Kata void berarti fungsi tidak mengembalikan
// nilai, dan tanda kurung () kosong berarti fungsi tidak menerima masukan.
namespace task1 { void run(); }  // Program 1 : Tools Nilai Mahasiswa, isinya di src/tugas1_nilai_mahasiswa.cpp
namespace task2 { void run(); }  // Program 2 : Volume Tabung, isinya di src/tugas2_volume_tabung.cpp
namespace task3 { void run(); }  // Program 3 : Luas Bangun yang Diarsir, isinya di src/tugas3_luas_arsir.cpp

// Menggunakan namespace tanpa nama (kata namespace langsung diikuti kurung kurawal, tanpa nama) berfungsi untuk
// menjadikan semua konstanta dan fungsi di dalamnya, sampai baris "}  // akhir namespace tanpa nama" tepat sebelum
// main(), milik berkas ini saja, dikarenakan nama seperti warna atau kotak juga ada di berkas tugas dan tidak boleh
// bentrok saat digabung.
namespace {

    // ===== WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, nomor menu, ajakan menekan Enter, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    // Kode warna ANSI adalah teks khusus yang tidak tercetak, melainkan dibaca terminal sebagai perintah ganti gaya:
    // "\033" adalah karakter ESC (nomor 27, ditulis dalam oktal), lalu "[", angka gaya, dan huruf m sebagai penutup.
    // Menggunakan constexpr berfungsi untuk membuat nilai tetap yang sudah pasti sejak kompilasi dan tidak bisa
    // diubah; const char* adalah penunjuk ke teks tetap (string gaya bahasa C), dikarenakan kode warna tidak berubah.
    constexpr const char* ANSI_RESET  = "\033[0m";     // 0 = kembali ke tampilan normal
    // ANSI_BOLD dipakai di bawah untuk nama menu (fungsi printMenu()) dan untuk prompt, yaitu teks pertanyaan
    // "Pilihan Anda [0-3] : " yang menunggu ketikan pengguna (fungsi readMenuChoice()).
    constexpr const char* ANSI_BOLD  = "\033[1m";     // 1 = tebal: prompt dan nama menu
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // 1;36 = cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // 1;32 = hijau tebal: pesan penutup program
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // 1;31 = merah tebal: pesan kesalahan
    constexpr const char* ANSI_HINT = "\033[33m";    // 33 = kuning: ajakan "Tekan Enter untuk kembali ke menu..."
    constexpr const char* ANSI_DIM    = "\033[90m";    // 90 = abu-abu: garis kotak di drawBox() di bawah

    // Fungsi enableUtf8Console() berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001) sebelum keluaran
    // pertama, dikarenakan garis kotak dari fungsi drawBox() di bawah, serta simbol π, ², ³ dan sketsa Braille
    // yang dicetak berkas tugas2_volume_tabung.cpp dan tugas3_luas_arsir.cpp di folder src/ (sketsa Braille hanya
    // di tugas2_volume_tabung.cpp), dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan
    // masih memakai code page (tabel huruf) lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini
    // berlaku untuk jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada berkas peluncur
    // jalankan.bat di folder proyek. Di macOS/Linux terminal sudah UTF-8, sehingga fungsi ini tidak melakukan
    // apa-apa. Kata void berarti fungsi tidak mengembalikan nilai. Dipanggil di main() di bagian paling bawah berkas.
    void enableUtf8Console() {
    // Menggunakan #ifdef _WIN32 (apakah sedang dikompilasi untuk Windows?) berfungsi agar dua baris sampai #endif
    // hanya ikut dikompilasi di Windows, dikarenakan kedua fungsi di bawah, SetConsoleOutputCP dan SetConsoleCP,
    // hanya ada di <windows.h> (lihat #include di bagian atas berkas); di macOS/Linux isi fungsi ini kosong.
    #ifdef _WIN32
        // SetConsoleOutputCP berfungsi untuk keluaran: byte UTF-8 ditampilkan sebagai karakter yang benar.
        // CP_UTF8 adalah konstanta Windows bernilai 65001 (nomor code page UTF-8).
        SetConsoleOutputCP(CP_UTF8);
        // SetConsoleCP berfungsi untuk masukan: ketikan dengan huruf non-ASCII dibaca sebagai UTF-8, sama dengan
        // keluarannya. ASCII adalah kumpulan 128 karakter dasar: huruf A-Z dan a-z tanpa aksen, angka 0-9, tanda
        // baca, spasi, dan karakter kendali seperti pindah baris; huruf seperti é atau π adalah non-ASCII.
        SetConsoleCP(CP_UTF8);
    #endif  // akhir bagian khusus Windows
    }

    // Fungsi colorsEnabled() berfungsi untuk menentukan apakah warna boleh dipakai; hasilnya bool (true = boleh,
    // false = tidak). Warna dimatikan bila variabel lingkungan (pengaturan yang diwariskan terminal ke program)
    // NO_COLOR ada (konvensi umum program CLI, yaitu program yang dijalankan dari terminal/baris perintah) atau
    // keluaran bukan terminal (mis. dialihkan ke berkas), dan dipaksa aktif bila FORCE_COLOR ada. Di Windows, mode
    // "virtual terminal" diaktifkan agar Command Prompt memahami kode ANSI. Menggunakan static (pada variabel
    // enabled di dalam fungsi ini) dikarenakan pemeriksaan cukup dilakukan sekali untuk seluruh program.
    bool colorsEnabled() {
        // Menggunakan static berfungsi agar variabel enabled dibuat dan diisi hanya sekali, pada pemanggilan pertama,
        // lalu nilainya diingat untuk pemanggilan berikutnya. Nilai awalnya berasal dari lambda, yaitu fungsi kecil
        // tanpa nama berbentuk []() { ... }; tanda () pada baris "}();" di bawah langsung menjalankannya. Lambda
        // berfungsi menghitung nilai awal variabel static satu kali, dikarenakan pemeriksaannya butuh beberapa
        // langkah if yang tidak muat dalam satu ekspresi.
        static const bool enabled = []() {
            // std::getenv("NO_COLOR") membaca variabel lingkungan NO_COLOR; hasil nullptr (penunjuk kosong) berarti
            // variabel itu tidak ada. Bila ada, return false langsung mengakhiri lambda dengan hasil "warna mati".
            if (std::getenv("NO_COLOR") != nullptr) return false;
            // Bila FORCE_COLOR ada, warna dipaksa aktif tanpa memeriksa terminal.
            if (std::getenv("FORCE_COLOR") != nullptr) return true;
        // Menggunakan #ifdef _WIN32 berfungsi memilih cara pemeriksaan: cara Windows sampai #else, cara macOS/Linux
        // setelah #else, dikarenakan fungsi yang tersedia di kedua sistem berbeda.
        #ifdef _WIN32
            // HANDLE adalah tipe Windows untuk "pegangan" ke sebuah saluran; GetStdHandle(STD_OUTPUT_HANDLE)
            // mengambil pegangan ke keluaran layar (konsol).
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;  // DWORD = bilangan bulat tanpa tanda milik Windows; menampung mode konsol saat ini
            // GetConsoleMode membaca mode konsol ke variabel mode (&mode = alamat variabel itu, agar bisa diisi).
            // Bila pegangannya tidak sah atau keluaran bukan konsol (mis. dialihkan ke berkas), warna dimatikan.
            // Operator || berarti "atau", tanda ! berarti "tidak" (GetConsoleMode gagal).
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // Operator | menambahkan bendera ENABLE_VIRTUAL_TERMINAL_PROCESSING ke mode lama, berfungsi agar konsol
            // memahami kode ANSI; SetConsoleMode mengembalikan 0 bila gagal, sehingga != 0 berarti warna aktif.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else  // bukan Windows: macOS atau Linux
            // fileno(stdout) memberi nomor saluran keluaran layar, lalu isatty memeriksa apakah saluran itu terminal
            // (bukan berkas); hasil selain 0 berarti terminal, sehingga warna dipakai.
            return isatty(fileno(stdout)) != 0;
        #endif  // akhir pemeriksaan sesuai sistem operasi
        }();  // () di sini menjalankan lambda satu kali; hasilnya menjadi nilai awal variabel enabled
        return enabled;  // mengembalikan hasil yang tersimpan, tanpa memeriksa ulang
    }

    // Fungsi colorize() berfungsi untuk membungkus teks dengan kode warna bila warna aktif; bila tidak, teks
    // dikembalikan apa adanya. Parameter code diisi salah satu konstanta ANSI_... di atas, dan text bertipe
    // std::string_view (hanya dibaca, tanpa disalin). Hasilnya std::string baru, siap dicetak.
    std::string colorize(const char* code, std::string_view text) {
        // Tanda ! berarti "tidak": bila warna tidak aktif, return mengakhiri fungsi dengan salinan teks apa adanya.
        if (!colorsEnabled()) return std::string(text);
        // Operator + menyambung teks: kode warna + teks + ANSI_RESET, agar warna tidak terbawa ke teks berikutnya.
        // std::string(code) dan std::string(text) mengubah keduanya menjadi std::string dulu agar bisa disambung.
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Fungsi isWideCodePoint() berfungsi untuk menentukan apakah sebuah code point Unicode (nomor unik sebuah
    // karakter) memakai dua kolom di terminal (aksara CJK = Cina, Jepang, Korea; Hangul = huruf Korea; dan emoji),
    // yaitu karakter yang digambar terminal selebar dua huruf biasa, dikarenakan tanpa ini lebar kotak dan
    // pembungkusan baris salah bila teks memuat aksara lebar atau emoji. Kata unsigned berarti bilangan bulat tanpa
    // tanda negatif. Dipakai fungsi displayWidth() di bawah.
    bool isWideCodePoint(unsigned codePoint) {
        // Angka berawalan 0x ditulis dalam heksadesimal (basis 16). Setiap pasangan (>= awal && <= akhir) adalah
        // satu rentang karakter lebar; && berarti "dan", || berarti "atau". Rentangnya: Hangul Jamo (huruf penyusun
        // Hangul, 0x1100-0x115F), CJK dan aksara Asia Timur lain (0x2E80-0xA4CF), suku kata Hangul (0xAC00-0xD7A3),
        // CJK kompatibilitas (0xF900-0xFAFF, 0xFE30-0xFE4F), huruf lebar penuh (0xFF00-0xFF60, 0xFFE0-0xFFE6), dan
        // emoji (0x1F300-0x1F64F, 0x1F900-0x1F9FF).
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Fungsi displayWidth() berfungsi untuk menghitung lebar teks di layar (jumlah kolom): karakter UTF-8 multibyte
    // (mis. garis kotak) dihitung satu kolom, aksara lebar (CJK, emoji) dua kolom, dan kode warna ANSI nol kolom,
    // dikarenakan ketiganya tidak memakai ruang sebanyak byte-nya. UTF-8 menyimpan satu karakter dalam 1 sampai
    // 4 byte; byte awal UTF-8 menentukan jumlah byte lanjutan yang dirangkai. Hasilnya bertipe std::size_t.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom yang sudah terhitung
        bool inEscape = false;  // true selama berada di dalam urutan kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point tersebut
        // Range-based for: perulangan yang mengambil setiap byte di text satu per satu ke variabel c, dari awal
        // sampai akhir. unsigned char = satu byte bernilai 0-255; const berarti c tidak diubah di dalam perulangan.
        for (const unsigned char c : text) {
            // Selama di dalam kode warna, byte dilewati sampai huruf 'm' penutupnya; continue = langsung lanjut ke
            // byte berikutnya tanpa menjalankan baris di bawahnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC pembuka kode warna
            // Operator & (AND per bit) dengan 0xC0 mengambil 2 bit teratas byte; hasil 0x80 berarti pola 10xxxxxx,
            // yaitu byte lanjutan (10xxxxxx): tambahkan 6 bit ke code point.
            if ((c & 0xC0) == 0x80) {
                // << 6 menggeser code point 6 bit ke kiri, lalu | menempelkan 6 bit terbawah byte ini (c & 0x3F).
                // --pending mengurangi sisa byte; bila sudah 0, code point lengkap, dan karakter lebar mendapat satu
                // kolom tambahan (++width), dikarenakan kolom pertamanya sudah dihitung saat byte awal.
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            // Pola bit byte awal menentukan jumlah byte lanjutan: 110xxxxx = 1, 1110xxxx = 2, 11110xxx = 3. Bit x
            // milik byte awal disimpan sebagai awal code point; byte ASCII biasa (0xxxxxxx) tidak punya lanjutan.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }
        return width;
    }

    // Fungsi terminalWidth() berfungsi membaca lebar terminal saat ini (jumlah kolom) setiap kali dipanggil,
    // sehingga tampilan ikut menyesuaikan bila jendela diubah ukurannya. Bila tidak diketahui (keluaran bukan
    // terminal), dipakai variabel lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan terminal pada umumnya.
    // Menggunakan batas bawah 50 dikarenakan di bawah itu kotak menu tidak bisa rapi. Hasilnya int (bilangan bulat).
    int terminalWidth() {
        int cols = 0;  // 0 = lebar belum diketahui
    // Menggunakan #ifdef _WIN32 berfungsi memilih cara membaca lebar: cara Windows sampai #else, cara macOS/Linux
    // setelah #else, dikarenakan fungsi yang tersedia di kedua sistem berbeda.
    #ifdef _WIN32
        // CONSOLE_SCREEN_BUFFER_INFO adalah struct milik Windows yang diisi GetConsoleScreenBufferInfo (&info =
        // alamat variabel info, agar bisa diisi). Bagian srWindow menyimpan nomor kolom kiri (Left) dan kanan
        // (Right) jendela, sehingga lebar = Right - Left + 1.
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // struct winsize dari <sys/ioctl.h> menampung ukuran jendela; bagiannya ws_col = jumlah kolom.
        struct winsize ws;
        // ioctl TIOCGWINSZ = tanya ukuran jendela ke saluran keluaran layar (STDOUT_FILENO) dan isi ws (&ws =
        // alamat ws); hasil 0 berarti berhasil.
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // akhir pembacaan lebar sesuai sistem operasi
        // Bila lebar belum diketahui, coba variabel lingkungan COLUMNS: env menunjuk ke teksnya (nullptr bila COLUMNS
        // tidak ada), lalu std::atoi mengubah teks itu (mis. "100") menjadi angka int 100.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // masih belum diketahui: pakai 80 kolom
        return std::max(50, cols);  // std::max memilih yang lebih besar, sehingga hasil tidak pernah di bawah 50
    }

    // Fungsi wrapText() berfungsi memotong satu baris teks yang lebih panjang dari 'maxWidth' kolom menjadi beberapa
    // baris, memotong pada spasi, dan memberi baris lanjutan indentasi 'gantung' (sebanyak 'hangingIndent' spasi)
    // agar tetap sejajar dengan isi setelah label. Dipakai fungsi drawBox() dan printError() di bawah, dikarenakan
    // teks tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
    // Parameter const std::string& text adalah referensi tetap (const&): fungsi memakai teks asli tanpa menyalinnya
    // dan tanpa boleh mengubahnya. Hasilnya std::vector<std::string>: daftar teks, satu elemen = satu baris.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil, mula-mula kosong
        // Bila indentasi gantung + 10 kolom isi tidak muat di maxWidth, indentasi terlalu besar untuk jendela sempit:
        // pakai 2 spasi saja.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // sisa teks yang belum dipotong (salinan, dikarenakan isinya akan diganti)
        // Selama sisa teks masih lebih lebar dari maxWidth, potong satu baris lagi dari depannya.
        while (displayWidth(remaining) > maxWidth) {
            // col = kolom layar yang sedang dihitung; cutPos = posisi spasi tempat memotong. std::string::npos adalah
            // nilai khusus yang berarti "tidak ditemukan", di sini tanda bahwa belum ada spasi.
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama berada di dalam kode warna (tidak memakai kolom)
            // Cari spasi terakhir sebelum batas lebar (dihitung per kolom). Perulangan for biasa: mulai i = 0,
            // ulangi selama i < remaining.size() (jumlah byte), dan tambah i satu (++i) setiap putaran.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char> mengubah tipe char menjadi unsigned char (0-255) secara tertulis,
                // dikarenakan byte UTF-8 di atas 127 bisa terbaca sebagai angka negatif pada char biasa.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // lewati isi kode warna sampai 'm'
                if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = ESC: awal kode warna
                if ((c & 0xC0) == 0x80) continue;  // byte lanjutan UTF-8: bukan karakter baru, tidak menambah kolom
                if (col >= maxWidth) break;  // batas lebar tercapai: break = keluar dari perulangan for ini
                // Simpan posisi spasi sebagai calon titik potong, asalkan letaknya setelah indentasi gantung.
                if (c == ' ' && col > hangingIndent) cutPos = i;
                ++col;  // karakter ini memakai satu kolom
            }
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            // substr(0, cutPos) = potongan teks dari awal sampai sebelum spasi; push_back menambahkannya ke result.
            result.push_back(remaining.substr(0, cutPos));
            // Sisa teks setelah spasi diberi awalan spasi sebanyak hangingIndent; std::string(n, ' ') = n buah spasi.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }
        result.push_back(remaining);  // sisa terakhir (sudah muat) menjadi baris terakhir
        return result;
    }

    // Fungsi drawBox() berfungsi untuk menggambar kotak berjudul berisi baris-baris teks. Lebar kotak mengikuti baris
    // terpanjang, tetapi tidak pernah melebihi lebar terminal: baris yang terlalu panjang dibungkus lebih dulu dengan
    // fungsi wrapText() di atas, dengan indentasi gantung sejajar label bila baris berpola "Label : isi".
    // Menggunakan kotak dikarenakan informasi (petunjuk, menu, hasil) jadi terpisah jelas dari pertanyaan.
    // Garis kotak ditulis sebagai kode Unicode di dalam string: "\u250c" = ┌, "\u2510" = ┐, "\u2514" = └,
    // "\u2518" = ┘, "\u2500" = ─ (garis datar), dan "\u2502" = │ (garis tegak).
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // maxWidth = lebar isi terbesar: lebar terminal dikurangi 2 tepi kotak + 2 margin + 1 kolom cadangan,
        // dikarenakan sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela.
        // static_cast<std::size_t> mengubah hasil int menjadi std::size_t agar setipe dengan ukuran teks lainnya.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris isi setelah dibungkus, siap digambar
        // Untuk setiap baris b di lines: tentukan indentasi gantungnya, lalu bungkus dengan wrapText().
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;  // bawaan: baris lanjutan menjorok 2 spasi
            const std::size_t labelPos = b.find(" : ");  // posisi " : " pertama; std::string::npos bila tidak ada
            // Bila ada " : " dan label di depannya paling lebar 28 kolom, baris dianggap berpola "Label : isi".
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // sejajar dengan isi setelah " : "
            }
            // Setiap potongan (piece) hasil wrapText() ditambahkan ke contentLines.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Perlebar sampai baris terpanjang + 2 = margin kiri dan kanan; std::max memilih nilai yang lebih besar.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min memilih yang lebih kecil: tidak melebihi maxWidth + 2 margin
        std::string filler;  // garis pengisi di kanan judul
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // Garis atas: ┌─ judul ───┐. std::cout << ... mencetak bagian-bagiannya ke layar secara
        // berurutan; '\n' = karakter pindah baris.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Baris isi: │ + teks + spasi pengisi sampai lebar kotak + │. Operator ternary (a ? b : c) berarti "bila a
        // benar pakai b, bila tidak pakai c": jumlah spasi pengisi tidak boleh negatif, jadi paling sedikit 0.
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

    // Fungsi printError() berfungsi mencetak pesan kesalahan (merah) yang dibungkus sesuai lebar terminal, dengan
    // baris lanjutan menjorok 6 spasi, sejajar teks setelah tanda "[!]" (pesannya diawali "  [!] "), dikarenakan
    // pesan panjang tidak boleh terpotong jendela sempit. Dipakai fungsi readMenuChoice() di bawah.
    void printError(const std::string& text) {
        // Lebar bungkus = lebar terminal - 1 kolom cadangan; 6 = indentasi gantung. Setiap baris dicetak merah.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Fungsi printBanner() berfungsi untuk menampilkan banner identitas dalam kotak, sekali di awal program
    // (dipanggil dari main() di bawah). Daftar teks di dalam { ... } menjadi parameter lines milik drawBox().
    void printBanner() {
        drawBox("Tugas Praktikum Algoritma dan Struktur Data", {
            "Modul 1 : Input dan Output",
            "Nama    : Fadhlillah",
            "NIM     : 2531070011"
        });
    }

    // Fungsi printMenu() berfungsi untuk menampilkan daftar pilihan program dalam kotak beserta keterangan singkat
    // tiap program; [0] diletakkan terakhir dikarenakan kebiasaan menu konsol menempatkan "keluar" di akhir.
    void printMenu() {
        std::cout << '\n';  // satu baris kosong sebagai pemisah dari tampilan sebelumnya
        // Menggunakan dua bentuk tata letak berfungsi agar menu tetap rapi di jendela sempit: bila lebar terminal
        // kurang dari 100 kolom, keterangan diturunkan ke baris berikutnya; bila lebar, sejajar di kanan nama program.
        const bool isNarrow = terminalWidth() < 100;
        // menuItems adalah std::vector berisi std::pair: setiap pasangan = { nama menu, keterangannya }. Nama menu
        // diberi spasi di akhir agar sama panjang, sehingga keterangan di sebelah kanannya sejajar lurus.
        const std::vector<std::pair<std::string, std::string>> menuItems = {
            { "[1] Tools Nilai Mahasiswa", "Rekap Nama, Nilai, Total, dan Rata-Rata satu mata kuliah" },
            { "[2] Volume Tabung        ", "V = Pi x R x R x T; satuan, nilai Pi, jari-jari/diameter dipilih" },
            { "[3] Luas Bangun Diarsir  ", "Persegi dikurangi lingkaran di dalamnya; satuan dan Pi dipilih" },
            { "[0] Keluar               ", "Menutup program" },
        };
        std::vector<std::string> lines;  // baris-baris yang akan digambar di dalam kotak
        // Untuk setiap item di menuItems: auto = tipenya ditentukan compiler (di sini std::pair), const& = memakai
        // elemen asli tanpa menyalin. item.first = nama menu (dicetak tebal), item.second = keterangannya.
        // Jendela sempit: nama dan keterangan menjadi dua baris (keterangan menjorok 4 spasi);
        // jendela lebar: satu baris, keduanya dipisah 3 spasi.
        for (const auto& item : menuItems) {
            if (isNarrow) { lines.push_back(colorize(ANSI_BOLD, item.first)); lines.push_back("    " + item.second); }
            else        { lines.push_back(colorize(ANSI_BOLD, item.first) + "   " + item.second); }
        }
        drawBox("Menu Program", lines);
        std::cout << "Ketik nomor menu lalu tekan Enter.\n";  // \n di dalam string = pindah baris
    }

    // Fungsi readMenuChoice() berfungsi untuk membaca nomor menu 0-3; ketikan lain (9, abc, 2.5) ditolak dan diminta
    // ulang. Hasilnya int: nomor pilihan yang sah.
    int readMenuChoice() {
        while (true) {  // menggunakan while (true) berfungsi untuk mengulang sampai pilihan sah; keluar lewat return
            std::string text;  // menampung satu baris ketikan pengguna
            std::cout << colorize(ANSI_BOLD, "Pilihan Anda [0-3] : ");
            // std::getline(std::cin, text) membaca satu baris ketikan utuh ke text. getline gagal bila input ditutup,
            // yaitu tidak ada ketikan lagi yang bisa dibaca: pengguna menekan Ctrl+D (macOS/Linux) atau Ctrl+Z lalu
            // Enter (Windows), atau input dialihkan dari berkas (mis. ./tugas_alpro < masukan.txt) dan isinya sudah
            // habis. Tanda ! membalik hasilnya, sehingga isi blok if ini dijalankan saat gagal.
            if (!std::getline(std::cin, text)) {
                std::cerr << "\nInput berakhir. Program dihentikan.\n";  // std::cerr = saluran pesan galat
                std::exit(EXIT_FAILURE);  // std::exit menghentikan program; EXIT_FAILURE = kode keluar "gagal"
            }
            // Menghapus spasi di awal/akhir berfungsi agar " 2 " tetap diterima; isi lain diperiksa ketat di bawah.
            // find_first_not_of dan find_last_not_of mencari posisi karakter pertama dan terakhir yang bukan spasi,
            // tab (\t), atau \r (\r = sisa akhir baris Windows pada input dari berkas).
            // auto = tipe variabel ditentukan compiler dari nilai di sebelah kanannya.
            const auto start  = text.find_first_not_of(" \t\r");
            const auto end = text.find_last_not_of(" \t\r");
            // Operator ternary: bila semua isinya spasi (start bernilai npos), text menjadi kosong; bila tidak, text
            // diganti potongan mulai posisi start sepanjang end - start + 1 karakter.
            text = (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
            // Sah bila teks tepat satu digit 0-3; bentuk lain (+1, 1.0, 1 2, abc, kosong) ditolak.
            // text.size() == 1 = tepat satu karakter; std::isdigit = karakter itu angka 0-9 (diberi static_cast
            // dikarenakan isdigit hanya aman untuk nilai 0-255); text[0] <= '3' = paling besar 3.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0])) && text[0] <= '3') {
                // '2' - '0' berfungsi mengubah karakter digit menjadi angka 2, dikarenakan karakter disimpan sebagai
                // nomor kode berurutan ('0' = 48, '2' = 50), sehingga selisihnya sama dengan nilai angkanya.
                return text[0] - '0';
            }
            // Sampai di baris ini berarti ketikan tidak sah: tampilkan pesan, lalu while (true) mengulang pertanyaan.
            printError("  [!] Pilihan tidak dikenal. Ketik 1, 2, 3, atau 0 lalu tekan Enter.");
        }
    }

    // Fungsi waitForEnter() berfungsi untuk menahan layar agar hasil sempat dibaca sebelum menu ditampilkan lagi.
    void waitForEnter() {
        std::cout << '\n' << colorize(ANSI_HINT, "Tekan Enter untuk kembali ke menu...");
        std::string discard;             // isinya diabaikan dikarenakan yang dibutuhkan hanya penekanan Enter
        std::getline(std::cin, discard);  // menunggu sampai pengguna menekan Enter
    }

}  // akhir namespace tanpa nama: semua di atas hanya terlihat di berkas ini

// Fungsi main() adalah titik awal program: isinya dijalankan pertama kali saat program dibuka.
// Menggunakan int main() dan return 0 dikarenakan standar C++ mewajibkan main bertipe int (mengembalikan
// bilangan bulat ke sistem operasi); 0 menandakan program selesai tanpa error.
int main() {
    enableUtf8Console();  // berfungsi menyiapkan konsol Windows untuk UTF-8 sebelum ada keluaran apa pun
    printBanner();  // berfungsi untuk menampilkan identitas sekali di awal

    while (true) {  // menggunakan while (true) berfungsi untuk menampilkan menu berulang sampai pengguna memilih keluar
        printMenu();  // menampilkan kotak menu
        const int choice = readMenuChoice();  // menunggu nomor menu yang sah (0-3); const = nilainya tidak diubah lagi

        // Menggunakan switch berfungsi untuk mengarahkan satu nilai pilihan ke satu program, dikarenakan lebih
        // rapi dibaca daripada rangkaian if-else untuk beberapa pilihan tetap. "case N:" dijalankan bila choice
        // bernilai N; task1::run() = fungsi run() di dalam namespace task1 (dideklarasikan di atas).
        switch (choice) {
            case 1: task1::run(); break;  // break berfungsi menghentikan switch agar case lain tidak ikut jalan
            case 2: task2::run(); break;  // Program 2 : Volume Tabung
            case 3: task3::run(); break;  // Program 3 : Luas Bangun yang Diarsir
            case 0:  // Keluar: cetak salam penutup berwarna hijau, lalu akhiri program
                std::cout << '\n' << colorize(ANSI_SUCCESS, "Terima kasih, program selesai.") << '\n';
                return 0;  // return di sini berfungsi untuk mengakhiri program sekaligus keluar dari perulangan
            default:  // default dijalankan bila tidak ada case yang cocok
                break;  // tidak akan terjadi dikarenakan rentang sudah dibatasi 0-3; ditulis agar switch lengkap
        }
        waitForEnter();  // berfungsi untuk menahan hasil di layar sebelum menu ditampilkan lagi
    }
}  // akhir main()
