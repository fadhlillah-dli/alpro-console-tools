/*
 * Tugas Praktikum 3 — Modul 1: Input dan Output
 * Program 3 : Luas Bangun yang Diarsir
 * Alur : pengguna memilih satuan panjang (mm/cm/dm/m) dan nilai π (3.14 sesuai modul atau 22/7 seperti pada
 *        soal dengan kelipatan 7), lalu memasukkan sisi persegi (bujur sangkar). Output: luas daerah yang
 *        diarsir, yaitu persegi dikurangi lingkaran di dalamnya yang menyinggung keempat sisi (r = s / 2).
 *        Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2)); Pi = π (modul menulisnya phi).
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas3 tugas3_luas_arsir.cpp && ./tugas3
 * Bila digabung ke program bermenu (main.cpp), kompilasi dengan tambahan -DBUILD_WITH_MENU
 * agar main() di bagian paling bawah berkas ini tidak ikut (lihat Bagian 7).
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
#include <algorithm>    // menggunakan <algorithm> berfungsi untuk std::max saat menghitung lebar kotak
#include <cctype>       // menggunakan <cctype> berfungsi untuk std::isdigit, memeriksa apakah karakter adalah angka
#include <cmath>        // menggunakan <cmath> berfungsi untuk std::hypot dan std::fabs saat menghitung lingkaran sketsa
#include <cstddef>      // menggunakan <cstddef> berfungsi untuk std::size_t (tipe ukuran/lebar teks)
#include <cstdlib>      // menggunakan <cstdlib> berfungsi untuk std::exit, EXIT_FAILURE, dan std::getenv
#include <iomanip>      // menggunakan <iomanip> berfungsi untuk setw, left, fixed, setprecision
#include <initializer_list>  // menggunakan <initializer_list> berfungsi untuk menerima daftar baris opsi { ... } pada printOptions
#include <iostream>     // menggunakan <iostream> berfungsi untuk std::cin, std::cout, std::cerr
#include <sstream>      // menggunakan <sstream> berfungsi untuk std::istringstream/ostringstream (teks <-> angka)
#include <string>       // menggunakan <string> berfungsi untuk std::string (baris ketikan dan baris tampilan)
#include <string_view>  // menggunakan <string_view> berfungsi untuk teks prompt yang hanya dibaca, tanpa disalin
#include <vector>       // menggunakan <vector> berfungsi untuk daftar baris yang digambar di dalam kotak
// Pustaka khusus sistem operasi untuk mendeteksi terminal dan mengaktifkan warna. Menggunakan #ifdef _WIN32
// dikarenakan Windows dan macOS/Linux menyediakan fungsi yang berbeda untuk keperluan yang sama.
#ifdef _WIN32
    #define NOMINMAX        // berfungsi mencegah windows.h mendefinisikan makro min/max yang merusak std::max
    #include <windows.h>    // GetStdHandle, GetConsoleMode, SetConsoleMode: mengaktifkan kode warna di Command Prompt
#else
    #include <cstdio>       // fileno, stdout: dipakai bersama isatty
    #include <sys/ioctl.h>  // ioctl, TIOCGWINSZ, struct winsize: membaca lebar jendela terminal
    #include <unistd.h>     // isatty, STDOUT_FILENO: memeriksa apakah keluaran adalah terminal (bukan berkas)
#endif

// Menggunakan namespace tanpa nama berfungsi untuk menjadikan konstanta dan fungsi bantu di bawah milik berkas
// ini saja, dikarenakan berkas tugas lain punya fungsi bantu bernama sama (mencegah error "multiple definition").
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // Menggunakan π = 3.14 sebagai nilai bawaan berfungsi agar konsisten dengan program 2 dan modul (modul
    // menulis "phi"; yang dimaksud adalah π / pi, bukan φ / phi rasio emas 1,618). Menggunakan tipe double
    // dikarenakan π bilangan desimal; constexpr berfungsi untuk menjadikannya konstanta yang pasti sejak kompilasi.
    constexpr double PI_MODULE   = 3.14;        // nilai bawaan, sesuai modul
    constexpr double PI_FRACTION = 22.0 / 7.0;  // nilai yang dipakai soal sekolah bila sisi/jari-jari kelipatan 7
    constexpr int    TITLE_WIDTH = 60;         // lebar garis judul, dalam karakter
    constexpr int    LABEL_WIDTH = 24;         // lebar kolom label pertanyaan pilihan, agar semua opsi sejajar
    constexpr double DIMENSION_MAX = 1000000.0;  // batas atas ukuran berfungsi untuk menolak salah ketik yang tidak masuk akal
    constexpr const char* ANSI_HATCH = "\033[31m";  // merah: daerah yang diarsir pada sketsa (meniru warna gambar soal)

    // Daftar satuan panjang yang lazim pada soal; urutannya = nomor pilihan 1-4. Menggunakan const char*
    // dikarenakan simbol satuan adalah teks tetap yang tidak diubah.
    constexpr const char* UNITS[] = { "mm", "cm", "dm", "m" };
    constexpr int         UNIT_COUNT   = 4;
    constexpr int         DEFAULT_UNIT_INDEX   = 2;  // cm, dikarenakan hampir semua soal luas daerah arsiran memakai cm

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, pilihan yang ditegaskan, hasil akhir, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // merah tebal: pesan kesalahan
    constexpr const char* ANSI_HINT = "\033[33m";    // kuning: keterangan nilai bawaan [Enter = ...]
    constexpr const char* ANSI_DIM    = "\033[90m";    // abu-abu: garis kotak dan garis pembatas

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

    // Berfungsi untuk menampilkan prompt (pertanyaan, dicetak tebal) lalu membaca satu baris ketikan.
    std::string readLine(std::string_view prompt) {
        std::string line;                   // menggunakan std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt); // tebal berfungsi agar pertanyaan mudah dibedakan dari keterangan
        // Menggunakan std::getline berfungsi untuk membaca satu baris penuh. Pemeriksaan ! (NOT) berfungsi untuk
        // menangkap kegagalan getline, dikarenakan getline gagal bila input ditutup (Ctrl+D/Ctrl+Z).
        if (!std::getline(std::cin, line)) {
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";  // cerr dikarenakan ini pesan error
            std::exit(EXIT_FAILURE);  // kode gagal dikarenakan program berhenti tanpa data lengkap
        }
        return line;
    }

    // Berfungsi untuk membuang spasi/tab di awal dan akhir teks, dan merapatkan spasi beruntun menjadi satu.
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
    // angka lagi (14, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, dikarenakan
    // bukan cara menulis ukuran yang diharapkan dari pengguna.
    bool isDecimalFormat(const std::string& text) {
        const auto separator = text.find_first_of(".,");         // posisi titik/koma pertama; npos bila tidak ada
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus seluruhnya digit
        return isAllDigits(text.substr(0, separator))             // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));           // bagian sesudahnya juga; pemisah kedua ikut tertolak di sini
    }

    // Berfungsi untuk mengubah teks menjadi bilangan desimal; mengembalikan true bila teks sah.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // format diperiksa dulu agar hanya tulisan angka yang diterima
        for (char& c : text) {        // menggunakan reference (char&) berfungsi untuk mengubah karakter langsung pada teksnya
            if (c == ',') c = '.';    // koma diganti titik dikarenakan C++ hanya mengenal penulisan 7.5
        }
        std::istringstream parser(text);  // menggunakan istringstream berfungsi untuk membaca angka dari teks
        return (parser >> result) && (parser >> std::ws).eof();  // sah bila terbaca sebagai angka tanpa sisa karakter
    }

    // Berfungsi untuk meminta bilangan desimal yang harus lebih besar dari 0; diulang sampai sah.
    double readPositiveDecimal(std::string_view prompt) {
        while (true) {  // menggunakan while (true) berfungsi untuk mengulang permintaan sampai input sah; keluar lewat return
            double value = 0.0;  // menggunakan double dikarenakan ukuran boleh desimal; 0.0 sebagai nilai awal yang jelas
            // Syarat nilai > 0.0 berfungsi untuk menolak nol dan negatif, dikarenakan panjang sisi tidak mungkin
            // nol atau negatif.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }
    }

    // Berfungsi untuk meminta pilihan berupa satu digit 1..maksimal; Enter tanpa isi berarti pilihan bawaan.
    // Menggunakan nilai bawaan dikarenakan pilihan paling umum (cm, π = 3.14) cukup ditekan Enter.
    int readChoice(std::string_view prompt, int maxValue, int defaultValue) {
        while (true) {  // diulang dikarenakan pilihan salah harus diminta lagi
            const std::string text = normalizeSpaces(readLine(prompt));
            if (text.empty()) return defaultValue;  // Enter saja berfungsi untuk memilih bawaan
            // Sah bila tepat satu digit dan berada di rentang; '1' - '0' berfungsi mengubah karakter digit menjadi angka.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) {
                const int choice = text[0] - '0';
                if (choice >= 1 && choice <= maxValue) return choice;
            }
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue)
                                        + ", atau tekan Enter untuk memakai bawaan.");
        }
    }

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // Berfungsi untuk menampilkan sebuah pertanyaan pilihan dengan daftar opsinya rapi: baris pertama berisi
    // label (tebal) dan opsi pertama, baris berikutnya opsi lain yang disejajarkan di bawahnya, baris terakhir
    // nilai bawaan (kuning). Menggunakan fungsi ini dikarenakan semua pertanyaan pilihan harus tampil dengan
    // bentuk yang sama agar pengguna langsung mengenali pola "[nomor] pilihan" dan "[Enter = bawaan]".
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) {
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);
        bool isFirst = true;  // menggunakan bool berfungsi menandai baris pertama (yang memuat label)
        for (const std::string_view b : line) {
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter");  // baris "[Enter = ...]" diberi warna kuning
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b);
            // Baris pertama memuat label (tebal) lalu ": "; baris berikutnya diberi spasi selebar label + ": " agar
            // sejajar. Keduanya dibungkus (bungkus) agar opsi yang panjang tidak terpotong di jendela sempit.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;
        }
    }

    // Berfungsi untuk menampilkan daftar satuan, membaca pilihan, lalu menegaskan simbol satuan yang terpilih.
    const char* chooseUnit() {
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        const char* unit = UNITS[selection - 1];  // - 1 dikarenakan nomor pilihan mulai dari 1, indeks array mulai dari 0
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit) << "\n\n";  // penegasan agar pilihan pasti terbaca
        return unit;
    }

    // Berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan sisi persegi kelipatan 14)",
                                      "[Enter = 3.14]" });
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // operator ternary berfungsi memilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";
        return (selection == 2) ? PI_FRACTION : PI_MODULE;
    }

    // Berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas.
    void printTitle(std::string_view text) {
        std::cout << '\n';
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';
        printRule('=');
    }

    // Berfungsi untuk mengulang sebuah teks (termasuk karakter UTF-8 seperti ─) sebanyak n kali,
    // dikarenakan std::string(n, c) hanya bisa mengulang satu byte, bukan karakter multibyte.
    std::string repeatText(std::string_view text, int n) {
        std::string result;
        for (int i = 0; i < n; ++i) result += text;
        return result;
    }

    // ===== KANVAS BRAILLE: GAMBAR BERESOLUSI TINGGI DI TERMINAL =====
    // Menggunakan karakter Braille Unicode (U+2800 s.d. U+28FF) berfungsi untuk menggambar dengan resolusi 2 x 4 titik
    // per sel terminal, delapan kali lebih halus daripada satu karakter blok, dikarenakan setiap karakter Braille
    // mewakili kombinasi 8 titik yang dapat dinyalakan satu per satu (teknik yang dipakai pustaka seperti drawille).
    struct BrailleCanvas {
        int cols, rows;                 // ukuran kanvas dalam sel terminal
        std::vector<unsigned char> layers;  // untuk tiap titik: 0 = kosong, >0 = nomor lapisan gambar (menentukan warna)

        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {}
        int pixelWidth()  const { return 2 * cols; }    // 2 titik per sel ke samping
        int pixelHeight() const { return 4 * rows; } // 4 titik per sel ke bawah

        // Berfungsi menyalakan satu titik; lapisan bernomor lebih besar menimpa yang kecil (garis di atas arsiran).
        void setPixel(int x, int y, unsigned char layer) {
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // di luar kanvas diabaikan
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x];
            if (layer > ref) ref = layer;
        }

        // Berfungsi mengubah 8 titik di satu sel menjadi satu karakter Braille UTF-8 berwarna sesuai lapisannya.
        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const {
            // Peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40; kolom kanan 0x08, 0x10, 0x20, 0x80.
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} };
            unsigned char value = 0, maxLayer = 0;
            for (int dy = 0; dy < 4; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx];
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); }
                }
            }
            if (!value) return " ";                       // sel kosong = spasi biasa
            const unsigned codePoint = 0x2800u + value;          // kode Unicode karakter Braille yang dimaksud
            std::string s;                                // disandikan ke UTF-8 tiga byte
            s += static_cast<char>(0xE0 | (codePoint >> 12));
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
            s += static_cast<char>(0x80 | (codePoint & 0x3F));
            return colorize(layerColors[maxLayer], s);
        }

        // Berfungsi mengubah seluruh kanvas menjadi baris-baris teks siap cetak.
        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const {
            std::vector<std::string> lines;
            for (int cy = 0; cy < rows; ++cy) {
                std::string row;
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);
                lines.push_back(row);
            }
            return lines;
        }
    };

    // Berfungsi menggambar sketsa persegi dengan lingkaran di dalamnya pada kanvas Braille: lingkaran dihitung dari
    // persamaannya (titik berjarak R dari pusat), daerah di luar lingkaran diarsir garis diagonal merah seperti pada
    // buku cetak, dan garis jari-jari R dibubuhkan di baris tengah. Menggunakan perhitungan dikarenakan hasilnya
    // benar-benar bulat, dan Braille dikarenakan resolusinya jauh lebih halus daripada karakter blok.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal: dikurangi tepi kotak, margin, dan indentasi (8 kolom), kanvas
        // maksimal 48 sel dan minimal 20 sel; jumlah baris = setengah kolom agar bidangnya persegi (2 x 4 titik per sel).
        const int available = terminalWidth() - 8;
        int canvasCols = std::max(20, std::min(48, available - 2));
        if (canvasCols % 2 != 0) --canvasCols;
        const int canvasRows = canvasCols / 2;
        // Keterangan diletakkan di samping hanya bila baris terlebar (gambar + keterangan terpanjang) masih muat di
        // dalam kotak (lebar terminal - 5), dikarenakan drawBox akan membungkus baris yang lebih panjang dan itu
        // merusak gambar. 3 = indentasi, 2 = tepi persegi, 3 = jarak ke keterangan.
        const std::vector<std::string> legend = {
            "R = jari-jari lingkaran = S / 2",
            "S = sisi persegi = 2 x R (lingkaran menyinggung sisi)",
            colorize(ANSI_HATCH, "merah") + " : daerah yang diarsir (persegi - lingkaran)",
            colorize(ANSI_TITLE, "cyan") + "  : garis lingkaran",
            "Luas Arsir = (S x S) - (Pi x R x R)",
        };
        std::size_t longestLegend = 0;
        for (const std::string& k : legend) longestLegend = std::max(longestLegend, displayWidth(k));
        const bool legendOnRight = (3 + canvasCols + 2 + 3 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);

        BrailleCanvas canvas(canvasCols, canvasRows);
        const double cx = canvas.pixelWidth() / 2.0, cy = canvas.pixelHeight() / 2.0;  // pusat dalam satuan titik
        const double radiusPx  = std::min(cx, cy) - 0.5;                                     // lingkaran menyinggung sisi persegi
        for (int y = 0; y < canvas.pixelHeight(); ++y) {
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double dist = std::hypot(x + 0.5 - cx, y + 0.5 - cy);  // jarak titik ke pusat
                if (std::fabs(dist - radiusPx) <= 0.9)          canvas.setPixel(x, y, 2);  // lapisan 2: garis lingkaran
                else if (dist > radiusPx && (x + y) % 6 == 0)   canvas.setPixel(x, y, 1);  // lapisan 1: arsiran diagonal di luar lingkaran
            }
        }
        const std::vector<const char*> layerColors = { "", ANSI_HATCH, ANSI_TITLE };  // indeks = nomor lapisan
        std::vector<std::string> figureLines = canvas.toLines(layerColors);

        // Garis jari-jari "●──R──►" ditimpakan pada baris tengah, dari pusat ke tepi kanan lingkaran.
        {
            const int r = canvasRows / 2, centerCol = canvasCols / 2;
            std::string radiusLine = "\u25cf";                                    // ● pusat lingkaran
            for (int c = centerCol + 1; c < canvasCols - 1; ++c) radiusLine += (c == (centerCol + canvasCols - 1) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";                                               // ► ujung jari-jari
            std::string row;
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, r, layerColors);
            figureLines[r] = row + colorize(ANSI_HINT, radiusLine);
        }

        // Keterangan simbol: di samping kanan bila jendela lebar, di bawah gambar bila sempit.
        std::vector<std::string> sideNotes(canvasRows, "");
        if (legendOnRight) {
            const int positions[5] = { canvasRows / 8, canvasRows / 8 + 2, canvasRows / 8 + 5, canvasRows / 8 + 6, canvasRows / 2 };  // baris tempat tiap keterangan
            for (int i = 0; i < 5; ++i) if (positions[i] < canvasRows) sideNotes[positions[i]] = "   " + legend[i];
        }

        std::vector<std::string> lines;
        lines.push_back("   \u25c4" + repeatText("\u2500", canvasCols - 2) + "\u25ba  S");          // ◄────► panah sisi S
        lines.push_back("   " + colorize(ANSI_DIM, "\u250c" + repeatText("\u2500", canvasCols) + "\u2510"));  // ┌────┐ sisi atas persegi
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + colorize(ANSI_DIM, "\u2502") + figureLines[r] + colorize(ANSI_DIM, "\u2502") + sideNotes[r]);
        lines.push_back("   " + colorize(ANSI_DIM, "\u2514" + repeatText("\u2500", canvasCols) + "\u2518"));  // └────┘ sisi bawah persegi
        if (!legendOnRight) for (const std::string& k : legend) lines.push_back("   " + k);
        drawBox("Sketsa Bangun yang Diarsir", lines);
        std::cout << '\n';
    }

    // Berfungsi untuk menampilkan petunjuk pengisian dalam kotak sebelum pertanyaan pertama, dikarenakan pengguna
    // perlu tahu bentuk input yang diterima (pilihan, angka > 0, cara menulis desimal) agar tidak salah ketik.
    void printInstructions() {
        drawBox("Petunjuk Pengisian", {
            "- Satuan Panjang   : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)",
            "                     (contoh: Pilih [2] untuk satuan \"cm\")",
            "- Nilai Pi (\u03c0)     : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila sisi persegi kelipatan 14",
            "                     (contoh: Pilih [2] untuk nilai \"22/7\")",
            "- Sisi Persegi     : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                     (contoh: 14, 14,5 atau 14.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)",
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang."
        });
        std::cout << '\n';
    }

    // Berfungsi untuk membuat satu baris hasil "Label : nilai satuan" dengan 2 desimal, siap dimasukkan ke kotak.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit) {
        std::ostringstream line;  // menggunakan ostringstream berfungsi menyusun teks tanpa mengubah format std::cout
        line << std::left << std::setw(25) << label << ": " << std::fixed << std::setprecision(2) << value << ' ' << unit;
        return line.str();
    }

}  // namespace

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 3 =====
// Menggunakan namespace task3 berfungsi untuk memberi nama khas pada run(), dikarenakan program bermenu
// memanggilnya sebagai tugas3::run().
namespace task3 {

    void run() {
        printTitle("PROGRAM 3 : LUAS BANGUN YANG DIARSIR (PERSEGI - LINGKARAN)");
        std::cout << "Rumus : Luas Arsir = (S x S) - (Pi x (S/2) x (S/2))\n"
                  << "        Pi = \u03c0 (modul menulisnya phi)\n\n";
        printSketch();    // berfungsi menampilkan sketsa bangun agar pengguna tahu bagian mana yang dimaksud R, D, T
        printInstructions();

        // --- Tahap pengaturan: satuan dan nilai π ---
        const char* unit = chooseUnit();          // simbol satuan yang dipilih, mis. "cm"
        std::string piLabel;                    // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double pi = choosePi(piLabel);     // nilai π yang dipakai perhitungan

        // --- Tahap input ---
        // Menggunakan tipe data double berfungsi untuk menyimpan sisi persegi, dikarenakan sisi boleh desimal.
        // Menggunakan readPositiveDecimal dikarenakan panjang sisi tidak mungkin nol atau negatif.
        const double side = readPositiveDecimal(std::string("Masukkan Panjang Sisi Persegi (") + unit + ") : ");

        // --- Tahap perhitungan ---
        const double radius      = side / 2.0;                   // / berfungsi membagi: lingkaran menyinggung sisi, jadi r = s/2
        const double squareArea   = side * side;                  // * berfungsi mengalikan: luas persegi = s x s
        const double circleArea = pi * radius * radius;     // luas lingkaran = π x r x r
        const double shadedArea     = squareArea - circleArea;  // - berfungsi mengurangi: daerah merah = persegi - lingkaran

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit) + "\u00b2";  // satuan luas, mis. cm²
        std::vector<std::string> resultLines;
        resultLines.push_back("Pi (\u03c0) yang Digunakan    : " + piLabel);
        resultLines.push_back(formatResultLine("Sisi Persegi (S)", side, unit));
        resultLines.push_back(formatResultLine("Jari-Jari Lingkaran (R)", radius, unit) + "  (R = S / 2, lingkaran menyinggung sisi)");
        resultLines.push_back(formatResultLine("Luas Persegi", squareArea, areaUnit));
        resultLines.push_back(formatResultLine("Luas Lingkaran", circleArea, areaUnit));
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Luas Bangun yang Diarsir", shadedArea, areaUnit)));  // hasil akhir ditonjolkan hijau tebal
        std::cout << '\n';
        drawBox("Hasil Perhitungan", resultLines);
    }

}  // namespace task3

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU berfungsi untuk menyertakan main() hanya saat berkas ini dikompilasi sendiri,
// dikarenakan program bermenu sudah punya main() dan C++ hanya mengizinkan satu main().
#ifndef BUILD_WITH_MENU
int main() {
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 3 - Fadhlillah (2531070011)") << '\n';
    task3::run();
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif
