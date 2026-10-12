/*
 * Tugas Praktikum 1 — Modul 1: Input dan Output
 * Program 1 : Tools Nilai Student
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
 * Bila digabung ke program bermenu (main.cpp), kompilasi dengan tambahan -DBUILD_WITH_MENU
 * agar main() di bagian paling bawah berkas ini tidak ikut (lihat Bagian 7).
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// #include memuat pustaka standar C++ yang berisi fungsi dan tipe siap pakai.
#include <algorithm>    // menggunakan <algorithm> berfungsi untuk std::max saat menghitung lebar kolom nama dan kotak
#include <cctype>       // menggunakan <cctype> berfungsi untuk std::isalpha dan std::isdigit (memeriksa jenis karakter)
#include <cstddef>      // menggunakan <cstddef> berfungsi untuk tipe std::size_t (tipe ukuran string dan vector)
#include <cstdlib>      // menggunakan <cstdlib> berfungsi untuk std::exit, EXIT_FAILURE, dan std::getenv
#include <iomanip>      // menggunakan <iomanip> berfungsi untuk setw, left, right, fixed, setprecision
#include <iostream>     // menggunakan <iostream> berfungsi untuk std::cin, std::cout, std::cerr
#include <sstream>      // menggunakan <sstream> berfungsi untuk std::istringstream/ostringstream (teks <-> angka)
#include <string>       // menggunakan <string> berfungsi untuk std::string (Nama Mata Kuliah, nama mahasiswa)
#include <string_view>  // menggunakan <string_view> berfungsi untuk teks prompt yang hanya dibaca, tanpa disalin
#include <vector>       // menggunakan <vector> berfungsi untuk daftar mahasiswa yang ukurannya ditentukan saat berjalan
// Pustaka khusus sistem operasi untuk mendeteksi terminal dan mengaktifkan warna. Menggunakan #ifdef _WIN32
// dikarenakan Windows dan macOS/Linux menyediakan fungsi yang berbeda untuk keperluan yang sama.
#ifdef _WIN32
    #ifndef NOMINMAX    // MinGW sudah mendefinisikannya lewat pustaka standar; definisi ulang akan jadi peringatan
    #define NOMINMAX 1  // berfungsi mencegah windows.h mendefinisikan makro min/max yang merusak std::max
    #endif
    #include <windows.h>    // GetStdHandle, GetConsoleMode, SetConsoleMode: mengaktifkan kode warna di Command Prompt
#else
    #include <cstdio>       // fileno, stdout: dipakai bersama isatty
    #include <sys/ioctl.h>  // ioctl, TIOCGWINSZ, struct winsize: membaca lebar jendela terminal
    #include <unistd.h>     // isatty, STDOUT_FILENO: memeriksa apakah keluaran adalah terminal (bukan berkas)
#endif

// Menggunakan namespace tanpa nama (anonymous) berfungsi untuk menjadikan semua konstanta dan fungsi bantu
// di bawah milik berkas ini saja, dikarenakan berkas tugas 2 dan 3 juga punya fungsi bantu bernama sama;
// tanpa ini, penggabungan ke program bermenu akan gagal dengan error "multiple definition".
namespace {

    // ===== BAGIAN 2: KONSTANTA DAN TIPE DATA =====
    // Menggunakan tipe data int (bilangan bulat, 4 byte) berfungsi untuk batas jumlah mahasiswa,
    // dikarenakan jumlah orang selalu bilangan bulat.
    constexpr int STUDENT_COUNT_MIN = 1;     // batas bawah 1 dikarenakan rekap tanpa mahasiswa tidak bermakna
    constexpr int STUDENT_COUNT_MAX = 1000;  // batas atas 1000 berfungsi untuk menolak salah ketik seperti 30000

    // Menggunakan tipe data double (bilangan desimal, 8 byte, presisi sekitar 15 digit) berfungsi untuk batas
    // nilai, dikarenakan nilai mahasiswa bisa desimal seperti 87.5; bila memakai int, 87.5 terpotong menjadi 87.
    constexpr double SCORE_MIN = 0.0;    // batas bawah 0 dikarenakan nilai tidak mungkin negatif
    constexpr double SCORE_MAX = 100.0;  // batas atas 100 dikarenakan skala nilai mata kuliah 0-100

    // Lebar kolom tabel rekap, dalam satuan karakter.
    constexpr int         NO_COLUMN_WIDTH    = 5;   // menggunakan int dikarenakan std::setw menerima lebar bertipe int
    constexpr std::size_t NAME_COLUMN_MIN_WIDTH    = 20;  // menggunakan size_t dikarenakan dibandingkan dengan panjang std::string
    constexpr int         SCORE_COLUMN_WIDTH = 8;   // menggunakan int dikarenakan std::setw menerima lebar bertipe int
    constexpr int         TITLE_WIDTH       = 60;  // lebar garis judul
    constexpr std::size_t NAME_MAX_LENGTH = 60;  // batas panjang nama berfungsi untuk menolak tempelan teks yang bukan nama
    constexpr std::size_t COURSE_NAME_MAX_LENGTH = 60;  // batas panjang nama mata kuliah, alasan yang sama

    // Menggunakan struct berfungsi untuk mengemas nama dan nilai satu mahasiswa menjadi satu kesatuan,
    // sehingga keduanya selalu berpasangan dan tidak tertukar antar mahasiswa.
    struct Student {
        std::string name;     // menggunakan std::string dikarenakan nama berisi huruf dan spasi dengan panjang tidak pasti
        double      score{};  // menggunakan double dikarenakan nilai boleh desimal; {} berfungsi agar nilai awal 0
    };

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, pilihan yang ditegaskan, hasil akhir, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // merah tebal: pesan kesalahan
    constexpr const char* ANSI_DIM    = "\033[90m";    // abu-abu: garis kotak dan garis pembatas

    // Berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama, dikarenakan simbol
    // π, ², ³, garis kotak, dan sketsa Braille dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan
    // masih memakai code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku untuk
    // jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur Jalankan.bat. Di macOS/Linux
    // terminal sudah UTF-8, sehingga fungsi ini tidak melakukan apa-apa.
    void enableUtf8Console() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // keluaran: byte UTF-8 ditampilkan sebagai karakter yang benar
        SetConsoleCP(CP_UTF8);        // masukan: ketikan nama dengan huruf non-ASCII dibaca sebagai UTF-8
    #endif
    }

    // Berfungsi untuk menentukan apakah warna boleh dipakai. Warna dimatikan bila variabel lingkungan NO_COLOR
    // ada (konvensi umum program CLI) atau keluaran bukan terminal (mis. dialihkan ke berkas), dan dipaksa
    // aktif bila FORCE_COLOR ada. Di Windows, mode "virtual terminal" diaktifkan agar Command Prompt memahami
    // kode ANSI. Menggunakan static dikarenakan pemeriksaan cukup dilakukan sekali untuk seluruh program.
    bool colorsEnabled() {
        static const bool enabled = []() {  // lambda berfungsi menghitung nilai awal variabel static satu kali
            if (std::getenv("NO_COLOR") != nullptr) return false;
            if (std::getenv("FORCE_COLOR") != nullptr) return true;
        #ifdef _WIN32
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else
            return isatty(fileno(stdout)) != 0;
        #endif
        }();
        return enabled;
    }

    // Berfungsi untuk membungkus teks dengan kode warna bila warna aktif; bila tidak, teks dikembalikan apa adanya.
    std::string colorize(const char* code, std::string_view text) {
        if (!colorsEnabled()) return std::string(text);
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Berfungsi untuk menentukan apakah sebuah code point Unicode memakai dua kolom di terminal (aksara CJK, Hangul,
    // emoji), dikarenakan tanpa ini kolom tabel bergeser untuk nama berhuruf Jepang/Tionghoa/Korea atau beremoji.
    bool isWideCodePoint(unsigned codePoint) {
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berfungsi untuk menghitung lebar teks di layar: karakter UTF-8 multibyte (π, ², ³, garis kotak) dihitung
    // satu kolom, aksara lebar (CJK, emoji) dua kolom, dan kode warna ANSI nol kolom, dikarenakan ketiganya
    // tidak memakai ruang sebanyak byte-nya. Byte awal UTF-8 menentukan jumlah byte lanjutan yang dirangkai.
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;
        bool inEscape = false;  // true selama berada di dalam urutan kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu untuk code point tersebut
        for (const unsigned char c : text) {
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = karakter ESC pembuka kode warna
            if ((c & 0xC0) == 0x80) {                      // byte lanjutan (10xxxxxx): tambahkan 6 bit ke code point
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;                                       // setiap byte awal = satu karakter = minimal satu kolom
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }
        return width;
    }

    // Berfungsi membaca lebar terminal saat ini (jumlah kolom) setiap kali dipanggil, sehingga tampilan ikut
    // menyesuaikan bila jendela diubah ukurannya. Bila tidak diketahui (keluaran bukan terminal), dipakai 80 kolom,
    // ukuran bawaan terminal pada umumnya. Menggunakan batas bawah 50 dikarenakan di bawah itu tabel tidak bisa rapi.
    int terminalWidth() {
        int cols = 0;
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;  // ioctl TIOCGWINSZ = tanya ukuran jendela
    #endif
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;
        return std::max(50, cols);
    }

    // Berfungsi memotong satu baris teks yang lebih panjang dari 'maxWidth' kolom menjadi beberapa baris, memotong
    // pada spasi, dan memberi baris lanjutan indentasi 'gantung' agar tetap sejajar dengan isi setelah label.
    // Dipakai drawBox() dikarenakan teks tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;  // indentasi terlalu besar untuk jendela sempit: pakai 2 spasi saja
        std::string remaining = text;
        while (displayWidth(remaining) > maxWidth) {
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;
            for (std::size_t i = 0; i < remaining.size(); ++i) {  // cari spasi terakhir sebelum batas lebar (dihitung per kolom)
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }
                if (c == 0x1B) { inEscape = true; continue; }
                if ((c & 0xC0) == 0x80) continue;
                if (col >= maxWidth) break;
                if (c == ' ' && col > hangingIndent) cutPos = i;
                ++col;
            }
            if (cutPos == std::string::npos) break;        // tidak ada spasi untuk memotong: biarkan apa adanya
            result.push_back(remaining.substr(0, cutPos));
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }
        result.push_back(remaining);
        return result;
    }

    // Berfungsi untuk menggambar kotak berjudul berisi baris-baris teks. Lebar kotak mengikuti baris terpanjang,
    // tetapi tidak pernah melebihi lebar terminal: baris yang terlalu panjang dibungkus lebih dulu (bungkus), dengan
    // indentasi gantung sejajar label bila baris berpola "Label : isi". Menggunakan kotak dikarenakan informasi
    // (petunjuk, menu, hasil) jadi terpisah jelas dari pertanyaan.
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Dikurangi 2 tepi kotak + 2 margin + 1 kolom cadangan, dikarenakan sebagian terminal menambah baris kosong
        // bila sebuah baris persis selebar jendela.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;
        for (const std::string& b : lines) {
            std::size_t hangingIndent = 2;
            const std::size_t labelPos = b.find(" : ");
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // sejajar dengan isi setelah " : "
            }
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);  // + 2 = margin kiri dan kanan
        width = std::min(width, maxWidth + 2);
        std::string filler;
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;
            const std::size_t contentWidth = displayWidth(rowText);
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }
        std::string bottomLine;
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }

    // Berfungsi mencetak pesan kesalahan (merah) yang dibungkus sesuai lebar terminal, dengan baris lanjutan
    // menjorok sejajar teks setelah tanda "[!]", dikarenakan pesan panjang tidak boleh terpotong jendela sempit.
    void printError(const std::string& text) {
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';
        }
    }

    // Berfungsi untuk mencetak satu baris garis pembatas abu-abu sepanjang TITLE_WIDTH.
    void printRule(char ch) {
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Menggunakan fungsi-fungsi terpisah berfungsi untuk menghindari penulisan kode yang sama berulang,
    // dikarenakan pembacaan input dipakai di banyak tempat (mata kuliah, jumlah, nama, nilai).

    // Berfungsi untuk menampilkan prompt (pertanyaan, dicetak tebal) lalu membaca satu baris ketikan.
    std::string readLine(std::string_view prompt) {
        std::string line;                   // menggunakan std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt); // tebal berfungsi agar pertanyaan mudah dibedakan dari keterangan
        // Menggunakan std::getline berfungsi untuk membaca satu baris penuh termasuk spasi (std::cin >>
        // berhenti di spasi pertama). Pemeriksaan ! (NOT) berfungsi untuk menangkap kegagalan getline,
        // dikarenakan getline bernilai gagal bila input ditutup (Ctrl+D/Ctrl+Z), agar program tidak terjebak.
        if (!std::getline(std::cin, line)) {
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";  // cerr dikarenakan ini pesan error
            std::exit(EXIT_FAILURE);  // kode gagal dikarenakan program berhenti tanpa data lengkap
        }
        return line;
    }

    // Berfungsi untuk membuang spasi/tab di awal dan akhir teks ("  Budi  " menjadi "Budi") dan merapatkan
    // spasi beruntun di tengah menjadi satu spasi.
    std::string normalizeSpaces(const std::string& text) {  // const& dikarenakan teks hanya dibaca dan tidak perlu disalin
        std::string result;                  // menggunakan std::string berfungsi untuk menampung teks yang sudah dirapikan
        bool pendingSpace = false;         // menggunakan bool berfungsi untuk menandai ada spasi yang belum ditulis
        for (const char c : text) {         // memeriksa setiap karakter satu per satu
            if (c == ' ' || c == '\t' || c == '\r') {  // spasi/tab/CR tidak langsung ditulis, hanya ditandai (CR = sisa akhir baris Windows)
                pendingSpace = !result.empty();  // spasi di awal diabaikan dikarenakan hasil masih kosong
            } else {
                if (pendingSpace) result += ' ';  // beberapa spasi beruntun ditulis sebagai satu spasi saja
                result += c;
                pendingSpace = false;
            }
        }
        return result;                       // spasi di akhir otomatis terbuang dikarenakan tidak pernah ditulis
    }

    // Berfungsi untuk memeriksa apakah teks hanya berisi angka 0-9 (tanpa tanda, titik, huruf, atau spasi).
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;     // teks kosong bukan bilangan
        for (const char c : text) {         // setiap karakter harus digit
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;  // unsigned char dikarenakan isdigit hanya aman untuk 0-255
        }
        return true;
    }

    // Berfungsi untuk memeriksa format bilangan desimal: angka, boleh diikuti SATU pemisah titik atau koma lalu
    // angka lagi (87, 87.5, 87,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 87 5 ditolak, dikarenakan
    // bukan cara menulis nilai yang diharapkan dari pengguna.
    bool isDecimalFormat(const std::string& text) {
        const auto separator = text.find_first_of(".,");         // posisi titik/koma pertama; npos bila tidak ada
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // bagian sesudahnya juga; pemisah kedua ikut tertolak di sini
    }

    // Berfungsi untuk memeriksa apakah teks adalah UTF-8 yang sah: setiap karakter non-ASCII terdiri dari satu byte awal
    // (0xC2-0xF4) diikuti 1-3 byte lanjutan (0x80-0xBF). Menggunakan pemeriksaan ini dikarenakan byte acak seperti
    // 0xFF (bukan huruf, bukan UTF-8) harus ditolak, sementara huruf seperti é atau ö (UTF-8 sah) tetap diterima.
    bool isValidUtf8(const std::string& text) {
        std::size_t i = 0;
        while (i < text.size()) {
            const unsigned char lead = static_cast<unsigned char>(text[i]);
            std::size_t extra = 0;                               // jumlah byte lanjutan yang harus mengikuti
            if (lead < 0x80)                       extra = 0;    // ASCII biasa
            else if (lead >= 0xC2 && lead <= 0xDF) extra = 1;    // 2 byte, mis. é ö ü
            else if (lead >= 0xE0 && lead <= 0xEF) extra = 2;    // 3 byte, mis. aksara Asia
            else if (lead >= 0xF0 && lead <= 0xF4) extra = 3;    // 4 byte, mis. emoji
            else return false;                                   // 0x80-0xC1 dan 0xF5-0xFF bukan byte awal yang sah
            if (i + extra >= text.size()) return false;             // byte lanjutan terpotong di akhir teks
            for (std::size_t k = 1; k <= extra; ++k) {
                const unsigned char cont = static_cast<unsigned char>(text[i + k]);
                if (cont < 0x80 || cont > 0xBF) return false;    // byte lanjutan harus 10xxxxxx
            }
            i += extra + 1;
        }
        return true;
    }

    // Berfungsi untuk memeriksa apakah teks layak sebagai nama mata kuliah: harus mengandung huruf, dan hanya boleh
    // berisi huruf, angka (Kalkulus 2), spasi, atau tanda . , & / ( ) - ' ; tidak boleh terlalu panjang.
    bool isValidCourseName(const std::string& text) {
        if (text.empty() || displayWidth(text) > COURSE_NAME_MAX_LENGTH || !isValidUtf8(text)) return false;
        bool hasLetter = false;  // menggunakan bool berfungsi untuk menandai sudah ada minimal satu huruf
        for (const char c : text) {
            const unsigned char uc = static_cast<unsigned char>(c);  // unsigned char dikarenakan isalpha/isdigit hanya aman untuk 0-255
            // Byte >= 0x80 adalah bagian huruf UTF-8 non-ASCII (mis. é, ö), dikarenakan std::isalpha hanya mengenal ASCII.
            if (std::isalpha(uc) || uc >= 0x80) { hasLetter = true; continue; }
            if (std::isdigit(uc) || c == ' ' || c == '.' || c == ',' || c == '&' || c == '/'
                || c == '(' || c == ')' || c == '-' || c == '\'') continue;
            return false;  // karakter lain (mis. #, @, !) bukan bagian nama mata kuliah
        }
        return hasLetter;  // "123" ditolak dikarenakan tidak mengandung huruf sama sekali
    }

    // Berfungsi untuk meminta nama mata kuliah dan mengulanginya sampai lolos isValidCourseName.
    std::string readCourseName(std::string_view prompt) {
        while (true) {  // menggunakan while (true) berfungsi untuk mengulang permintaan sampai input sah; keluar lewat return
            const std::string text = normalizeSpaces(readLine(prompt));  // const dikarenakan hasilnya tidak diubah lagi
            if (isValidCourseName(text)) return text;
            printError("  [!] Nama Mata Kuliah harus mengandung huruf dan hanya boleh berisi huruf, angka, spasi,"
                                        " atau tanda . , & / ( ) - ' (maksimal " + std::to_string(COURSE_NAME_MAX_LENGTH)
                                        + " karakter). Silakan ulangi.");
        }
    }

    // Berfungsi untuk memeriksa apakah teks layak sebagai nama orang: hanya huruf, spasi, titik (A. Rizky),
    // tanda hubung (Anne-Marie), atau tanda petik (Nur'aini); harus diawali huruf dan tidak terlalu panjang.
    // Dengan ini ketikan seperti "Luthfi 87.5" atau "123" ditolak, dikarenakan angka dan simbol bukan bagian nama.
    bool isValidPersonName(const std::string& name) {
        if (name.empty() || displayWidth(name) > NAME_MAX_LENGTH || !isValidUtf8(name)) return false;  // kosong, terlalu panjang, atau bukan UTF-8 sah
        // Menggunakan static_cast<unsigned char> dikarenakan std::isalpha hanya aman untuk nilai 0-255 (char bisa negatif).
        const unsigned char first = static_cast<unsigned char>(name.front());
        if (!std::isalpha(first) && first < 0x80) return false;  // karakter pertama harus huruf (ASCII atau UTF-8 non-ASCII)
        for (const char c : name) {  // memeriksa setiap karakter; const dikarenakan hanya dibaca
            const unsigned char uc = static_cast<unsigned char>(c);
            const bool isLetter = std::isalpha(uc) != 0 || uc >= 0x80;  // >= 0x80: byte huruf UTF-8 non-ASCII (mis. é, ö)
            const bool isAllowedMark = (c == ' ' || c == '.' || c == '-' || c == '\'');
            if (!isLetter && !isAllowedMark) return false;  // ada angka atau simbol lain -> bukan nama
        }
        return true;
    }

    // Berfungsi untuk meminta nama mahasiswa dan mengulanginya sampai lolos isValidPersonName.
    std::string readPersonName(std::string_view prompt) {
        while (true) {  // diulang dikarenakan nama yang salah harus diminta lagi
            const std::string name = normalizeSpaces(readLine(prompt));
            if (isValidPersonName(name)) return name;
            printError("  [!] Nama hanya boleh berisi huruf, spasi, titik, tanda hubung (-), atau tanda petik ('),"
                                        " diawali huruf, maksimal " + std::to_string(NAME_MAX_LENGTH)
                                        + " karakter. Silakan ulangi.");
        }
    }

    // Berfungsi untuk meminta bilangan bulat dalam rentang [minimal, maksimal]; dipakai untuk jumlah mahasiswa.
    int readInteger(std::string_view prompt, int minValue, int maxValue) {
        while (true) {  // diulang dikarenakan input salah harus diminta lagi
            // Menggunakan pembacaan sebagai teks lebih dulu berfungsi untuk menolak ketikan salah (abc, 2.5)
            // tanpa merusak kondisi std::cin.
            const std::string text = normalizeSpaces(readLine(prompt));
            std::istringstream parser(text);  // menggunakan istringstream berfungsi untuk membaca angka dari teks
            int number = 0;  // menggunakan int dikarenakan yang diminta bilangan bulat; 0 agar tidak berisi nilai acak
            // Empat syarat dengan && (AND) berfungsi untuk memastikan input sah: teks hanya berisi digit 0-9 (menolak
            // +3, 3.0, 1e3, "3 orang"), terbaca sebagai bilangan bulat tanpa sisa karakter, dan berada dalam rentang.
            if (isAllDigits(text) && parser >> number && (parser >> std::ws).eof() && number >= minValue && number <= maxValue) {
                return number;
            }
            printError("  [!] Harus bilangan bulat " + std::to_string(minValue) + "-" + std::to_string(maxValue)
                                        + " (hanya angka). Silakan ulangi.");
        }
    }

    // Berfungsi untuk mengubah teks menjadi bilangan desimal; mengembalikan true bila teks sah.
    // Parameter double& hasil berfungsi untuk mengirim angka kembali, dikarenakan nilai kembalian (bool)
    // sudah dipakai untuk status sah/tidak.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang diterima
        for (char& c : text) {        // menggunakan reference (char&) berfungsi untuk mengubah karakter langsung pada teksnya
            if (c == ',') c = '.';    // koma diganti titik dikarenakan C++ hanya mengenal penulisan 87.5
        }
        std::istringstream parser(text);  // menggunakan istringstream berfungsi untuk membaca angka dari teks
        return (parser >> result) && (parser >> std::ws).eof();  // sah bila terbaca sebagai angka tanpa sisa karakter
    }

    // Berfungsi untuk meminta nilai desimal dalam rentang [minimal, maksimal]; koma (87,5) diterima.
    double readDecimal(std::string_view prompt, double minValue, double maxValue) {
        while (true) {
            double score = 0.0;  // menggunakan double dikarenakan nilai boleh desimal; 0.0 sebagai nilai awal yang jelas
            if (parseDecimal(normalizeSpaces(readLine(prompt)), score) && score >= minValue && score <= maxValue) {
                return score;
            }
            // Menggunakan ostringstream terpisah berfungsi agar batas tampil "0-100", bukan "0.00-100.00",
            // dikarenakan format std::cout mungkin sudah diubah menjadi 2 desimal.
            std::ostringstream range;
            range << minValue << "-" << maxValue;
            printError("  [!] Nilai harus angka " + range.str() + ", hanya digit dengan satu pemisah desimal"
                                        " (mis. 87.5 atau 87,5). Silakan ulangi.");
        }
    }

    // ===== BAGIAN 5: JUDUL, PETUNJUK, DAN TABEL REKAP =====

    // Berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas.
    void printTitle(std::string_view text) {
        std::cout << '\n';
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';
        printRule('=');
    }

    // Berfungsi untuk menampilkan petunjuk pengisian dalam kotak sebelum pertanyaan pertama, dikarenakan pengguna
    // perlu tahu bentuk input yang diterima (huruf/angka, rentang, cara menulis desimal) agar tidak salah ketik.
    void printInstructions() {
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
        std::cout << '\n';
    }

    // Berfungsi untuk membuat satu baris tabel (nomor, nama, nilai) dengan lebar kolom tetap, siap dimasukkan ke kotak.
    // Menggunakan ostringstream berfungsi menyusun teks tanpa mengubah format std::cout.
    std::string formatTableRow(std::string_view no, std::string_view name, std::string_view score, int nameWidth) {
        // Kolom nama diisi spasi berdasarkan lebar tampil (displayWidth), bukan std::setw yang menghitung byte,
        // dikarenakan nama dengan huruf non-ASCII (é, ö) memakai 2 byte per huruf dan akan membuat kolom bergeser.
        const std::size_t shownWidth = displayWidth(name);
        const std::size_t targetWidth = static_cast<std::size_t>(nameWidth);
        const std::string namePadded = std::string(name) + std::string(targetWidth > shownWidth ? targetWidth - shownWidth : 0, ' ');
        std::ostringstream line;
        line << std::left  << std::setw(NO_COLUMN_WIDTH)    << no
             << namePadded
             << std::right << std::setw(SCORE_COLUMN_WIDTH) << score;
        return line.str();
    }

    // Berfungsi untuk mengubah angka menjadi teks dengan tepat 2 desimal (87.5 -> "87.50").
    std::string formatTwoDecimals(double number) {
        std::ostringstream text;
        text << std::fixed << std::setprecision(2) << number;
        return text.str();
    }

}  // namespace

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 1 =====
// Menggunakan namespace task1 berfungsi untuk memberi nama yang khas pada fungsi run(), dikarenakan
// program bermenu (main.cpp) memanggilnya sebagai tugas1::run() dan tugas lain punya run() juga.
namespace task1 {

    void run() {
        enableUtf8Console();  // dipanggil di sini agar program mandiri (tanpa menu) juga benar; aman dipanggil berulang
        printTitle("PROGRAM 1 : TOOLS NILAI MAHASISWA");
        std::cout << '\n';
        printInstructions();

        // --- Pengaturan awal: mata kuliah dan jumlah mahasiswa ditentukan oleh pengguna ---
        // Menggunakan std::string dikarenakan Nama Mata Kuliah adalah teks; const dikarenakan setelah diisi
        // tidak diubah lagi.
        const std::string courseName = readCourseName("Nama Mata Kuliah : ");
        // Menggunakan int dikarenakan jumlah mahasiswa bilangan bulat; dibatasi 1-1000 berfungsi untuk menolak salah ketik.
        const int studentCount = readInteger("Jumlah Mahasiswa : ", STUDENT_COUNT_MIN, STUDENT_COUNT_MAX);

        // Menggunakan std::vector berfungsi untuk membuat daftar mahasiswa sebesar jumlah yang diketik,
        // dikarenakan jumlah elemen baru diketahui saat program berjalan sehingga tidak bisa memakai array
        // berukuran tetap. Menggunakan static_cast<std::size_t> dikarenakan ukuran vector bertipe size_t.
        std::vector<Student> students(static_cast<std::size_t>(studentCount));

        // --- Tahap input: isi nama dan nilai setiap mahasiswa ---
        std::cout << '\n' << colorize(ANSI_TITLE, "Input Nilai " + courseName) << '\n';
        int rowNumber = 1;  // menggunakan int dikarenakan nomor urut bilangan bulat; mulai 1 dikarenakan penomoran manusia dari 1
        // Menggunakan range-based for berfungsi untuk mengunjungi setiap mahasiswa tanpa mengurus indeks;
        // Student& (reference) berfungsi agar data asli yang terisi, bukan salinannya.
        for (Student& student : students) {
            std::cout << colorize(ANSI_BOLD, "Mahasiswa ke-" + std::to_string(rowNumber) + " dari " + std::to_string(studentCount)) << '\n';
            student.name  = readPersonName   ("  Nama Lengkap  : ");                       // readPersonName dikarenakan nama harus berupa huruf; operator . mengakses anggota struct
            student.score = readDecimal("  Nilai (0-100) : ", SCORE_MIN, SCORE_MAX);  // berfungsi untuk menyimpan nilai (double)
            ++rowNumber;  // menggunakan ++ berfungsi untuk menambah nomor urut sebesar 1
        }

        // --- Tahap perhitungan: total, rata-rata, dan lebar kolom nama ---
        double      total     = 0.0;             // menggunakan double dikarenakan penjumlahan nilai desimal harus tetap desimal
        std::size_t nameWidth = NAME_COLUMN_MIN_WIDTH;  // menggunakan size_t dikarenakan dibandingkan dengan m.nama.size()
        for (const Student& student : students) {  // const& dikarenakan data hanya dibaca, tidak diubah, dan tidak disalin
            total += student.score;  // menggunakan += berfungsi untuk menjumlahkan nilai ke total (singkatan total = total + m.nilai)
            nameWidth = std::max(nameWidth, displayWidth(student.name) + 2);  // std::max berfungsi menyimpan lebar terbesar; + 2 = jarak ke kolom nilai
        }
        // Menggunakan pembagian double dikarenakan hasilnya harus desimal (pada int, 11/2 = 5; pada double 5.5).
        const double average = total / static_cast<double>(students.size());

        // --- Tahap output: tabel rekap disusun ke vector, lalu digambar dalam satu kotak ---
        const int nameColumnWidth = static_cast<int>(nameWidth);  // static_cast<int> dikarenakan std::setw hanya menerima int
        const std::string tableRule(NO_COLUMN_WIDTH + nameColumnWidth + SCORE_COLUMN_WIDTH, '-');  // garis selebar tabel

        std::vector<std::string> summaryLines;  // menggunakan vector dikarenakan jumlah baris mengikuti jumlah mahasiswa
        summaryLines.push_back("Mata Kuliah      : " + courseName);
        summaryLines.push_back("Jumlah Mahasiswa : " + std::to_string(studentCount));
        summaryLines.push_back("");
        summaryLines.push_back(colorize(ANSI_BOLD, formatTableRow("No", "Nama", "Nilai", nameColumnWidth)));  // judul kolom tebal
        summaryLines.push_back(tableRule);
        rowNumber = 1;  // diisi 1 lagi dikarenakan penomoran tabel dimulai ulang dari 1
        for (const Student& student : students) {  // berfungsi untuk membuat satu baris tabel per mahasiswa
            summaryLines.push_back(formatTableRow(std::to_string(rowNumber), student.name, formatTwoDecimals(student.score), nameColumnWidth));
            ++rowNumber;
        }
        summaryLines.push_back(tableRule);
        // Menggunakan "" selebar kolom No berfungsi agar label total dan rata-rata tetap sejajar dengan kolom nama;
        // keduanya hijau tebal dikarenakan merupakan hasil akhir yang paling ditunggu pengguna.
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Total",     formatTwoDecimals(total),    nameColumnWidth)));
        summaryLines.push_back(colorize(ANSI_SUCCESS, formatTableRow("", "Nilai Rata-Rata", formatTwoDecimals(average), nameColumnWidth)));
        std::cout << '\n';
        drawBox("Rekap Nilai Mahasiswa", summaryLines);
    }

}  // namespace task1

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU berfungsi untuk menyertakan main() hanya saat berkas ini dikompilasi
// sendiri, dikarenakan program bermenu (main.cpp) sudah punya main() dan C++ hanya mengizinkan satu main().
// Saat digabung, kompilasi memakai -DBUILD_WITH_MENU sehingga bagian ini dilewati oleh preprocessor.
#ifndef BUILD_WITH_MENU
int main() {  // menggunakan int main() dan return 0 dikarenakan standar C++ mewajibkan main bertipe int
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 1 - Fadhlillah (2531070011)") << '\n';
    task1::run();
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif
