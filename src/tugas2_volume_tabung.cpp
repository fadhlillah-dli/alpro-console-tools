/*
 * Tugas Praktikum 2 — Modul 1: Input dan Output
 * Program 2 : Volume Tabung
 * Alur : mendefinisikan π = 3.14 (modul menulis "phi"; simbol konstanta lingkaran yang benar adalah π / pi),
 *        pengguna memilih satuan panjang (mm/cm/dm/m), nilai π (3.14 sesuai modul atau 22/7 seperti pada
 *        soal dengan kelipatan 7), dan ukuran yang diketahui (jari-jari atau diameter), lalu memasukkan
 *        ukuran dan tinggi. Program menampilkan luas alas, volume V = π x r x r x t, dan padanan liter.
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas2 tugas2_volume_tabung.cpp && ./tugas2
 * Bila digabung ke program bermenu (main.cpp), kompilasi dengan tambahan -DBUILD_WITH_MENU
 * agar main() di bagian paling bawah berkas ini tidak ikut (lihat Bagian 7).
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
#include <algorithm>    // menggunakan <algorithm> berfungsi untuk std::max dan std::min saat menghitung lebar kotak dan ukuran sketsa
#include <cctype>       // menggunakan <cctype> berfungsi untuk std::isdigit, memeriksa apakah karakter adalah angka
#include <cmath>        // menggunakan <cmath> berfungsi untuk std::cos dan std::sin saat menggambar elips sketsa
#include <cstddef>      // menggunakan <cstddef> berfungsi untuk std::size_t (tipe ukuran/lebar teks)
#include <cstdlib>      // menggunakan <cstdlib> berfungsi untuk std::exit, EXIT_FAILURE, std::getenv, dan std::atoi
#include <iomanip>      // menggunakan <iomanip> berfungsi untuk setw, left, fixed, setprecision
#include <initializer_list>  // menggunakan <initializer_list> berfungsi untuk menerima daftar baris opsi { ... } pada printOptions
#include <iostream>     // menggunakan <iostream> berfungsi untuk std::cin, std::cout, std::cerr
#include <sstream>      // menggunakan <sstream> berfungsi untuk std::istringstream/ostringstream (teks <-> angka)
#include <string>       // menggunakan <string> berfungsi untuk std::string (baris ketikan dan baris tampilan)
#include <string_view>  // menggunakan <string_view> berfungsi untuk teks prompt yang hanya dibaca, tanpa disalin
#include <vector>       // menggunakan <vector> berfungsi untuk daftar baris yang digambar di dalam kotak
// Pustaka khusus sistem operasi untuk mendeteksi terminal, membaca lebar jendela, mengaktifkan warna, dan UTF-8. Menggunakan #ifdef _WIN32
// dikarenakan Windows dan macOS/Linux menyediakan fungsi yang berbeda untuk keperluan yang sama.
#ifdef _WIN32
    #ifndef NOMINMAX    // menggunakan #ifndef dikarenakan MinGW sudah mendefinisikannya lewat pustaka standar; definisi ulang akan jadi peringatan
    #define NOMINMAX 1  // berfungsi mencegah windows.h mendefinisikan makro min/max yang merusak std::max
    #endif
    #include <windows.h>    // GetStdHandle, Get/SetConsoleMode (warna), SetConsoleOutputCP/SetConsoleCP (UTF-8), GetConsoleScreenBufferInfo (lebar jendela)
#else
    #include <cstdio>       // fileno, stdout: dipakai bersama isatty
    #include <sys/ioctl.h>  // ioctl, TIOCGWINSZ, struct winsize: membaca lebar jendela terminal
    #include <unistd.h>     // isatty, STDOUT_FILENO: memeriksa apakah keluaran adalah terminal (bukan berkas)
#endif

// Menggunakan namespace tanpa nama berfungsi untuk menjadikan konstanta dan fungsi bantu di bawah milik berkas
// ini saja, dikarenakan berkas tugas lain punya fungsi bantu bernama sama; tanpa ini, penggabungan ke program
// bermenu gagal dengan error "multiple definition".
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // Mendefinisikan π = 3.14 sesuai ketentuan tugas (modul menulis "phi"; yang dimaksud adalah π / pi,
    // konstanta keliling lingkaran dibagi diameter; φ / phi adalah rasio emas 1,618 dan bukan konstanta ini).
    // Menggunakan tipe data double dikarenakan π bilangan desimal; constexpr berfungsi untuk menjadikannya
    // konstanta yang pasti sejak kompilasi (pengganti modern #define, lebih aman dikarenakan punya tipe data).
    constexpr double PI_MODULE   = 3.14;        // nilai bawaan, sesuai modul
    constexpr double PI_FRACTION = 22.0 / 7.0;  // nilai yang dipakai soal sekolah bila jari-jari atau diameter kelipatan 7
    constexpr int    TITLE_WIDTH = 60;         // lebar garis judul, dalam karakter
    constexpr int    LABEL_WIDTH = 24;         // lebar kolom label pertanyaan pilihan, agar semua opsi sejajar
    constexpr double DIMENSION_MAX = 1000000.0;  // batas atas ukuran berfungsi untuk menolak salah ketik yang tidak masuk akal
    constexpr const char* ANSI_SURFACE = "\033[36m";  // cyan biasa: arsiran permukaan tutup tabung pada sketsa

    // Menggunakan struct berfungsi untuk mengemas nama satuan dan faktor konversinya ke liter menjadi satu
    // kesatuan, dikarenakan keduanya selalu dipakai berpasangan.
    struct LengthUnit {
        const char* symbol;   // simbol satuan panjang, mis. "cm"; const char* dikarenakan teks tetap, tidak diubah
        double      litersPerCubicUnit;  // isi 1 satuan kubik dalam liter, dikarenakan 1 L = 1 dm³ (satuan SI turunan)
    };
    // Daftar satuan yang lazim pada soal; urutannya = nomor pilihan 1-4. 1 dm³ = 1 L, maka 1 cm³ = 0,001 L
    // (= 1 mL), 1 mm³ = 0,000001 L, dan 1 m³ = 1000 L.
    constexpr LengthUnit UNITS[] = { {"mm", 0.000001}, {"cm", 0.001}, {"dm", 1.0}, {"m", 1000.0} };
    constexpr int    UNIT_COUNT   = 4;
    constexpr int    DEFAULT_UNIT_INDEX   = 2;  // cm, dikarenakan hampir semua soal tabung memakai cm

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Menggunakan kode warna ANSI berfungsi untuk membedakan judul, pilihan yang ditegaskan, hasil akhir, dan
    // pesan kesalahan secara visual, dikarenakan terminal macOS, Linux, VS Code, dan Windows Terminal memahaminya.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // cyan tebal: judul program dan judul kotak
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // merah tebal: pesan kesalahan
    constexpr const char* ANSI_HINT = "\033[33m";    // kuning: keterangan nilai bawaan [Enter = ...] serta tanda R, T, dan keterangan pada sketsa
    constexpr const char* ANSI_DIM    = "\033[90m";    // abu-abu: garis kotak dan garis pembatas

    // Berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama, dikarenakan simbol
    // π, ², ³, garis kotak, dan sketsa Braille dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan
    // masih memakai code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku untuk
    // jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur jalankan.bat. Di macOS/Linux
    // terminal sudah UTF-8, sehingga fungsi ini tidak melakukan apa-apa.
    void enableUtf8Console() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // keluaran: byte UTF-8 ditampilkan sebagai karakter yang benar
        SetConsoleCP(CP_UTF8);        // masukan: ketikan dengan huruf non-ASCII dibaca sebagai UTF-8, sama dengan keluarannya
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
    // emoji), dikarenakan tanpa ini lebar kotak dan pembungkusan baris salah bila teks memuat aksara lebar atau emoji.
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
    // menyesuaikan bila jendela diubah ukurannya. Bila tidak diketahui (keluaran bukan terminal), dipakai variabel COLUMNS, lalu 80 kolom,
    // ukuran bawaan terminal pada umumnya. Menggunakan batas bawah 50 dikarenakan di bawah itu kotak dan sketsa tidak bisa rapi.
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
    // Dipakai drawBox(), printError(), dan printOptions() dikarenakan teks tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
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
    // tetapi tidak pernah melebihi lebar terminal: baris yang terlalu panjang dibungkus lebih dulu (wrapText), dengan
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
    // angka lagi (7, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, dikarenakan
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

    // Berfungsi untuk meminta bilangan desimal yang harus lebih besar dari 0 dan paling besar DIMENSION_MAX; diulang sampai sah.
    double readPositiveDecimal(std::string_view prompt) {
        while (true) {  // menggunakan while (true) berfungsi untuk mengulang permintaan sampai input sah; keluar lewat return
            double value = 0.0;  // menggunakan double dikarenakan ukuran boleh desimal; 0.0 sebagai nilai awal yang jelas
            // Syarat nilai > 0.0 berfungsi untuk menolak nol dan negatif, dikarenakan ukuran bangun tidak mungkin
            // nol atau negatif.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }
    }

    // Berfungsi untuk meminta pilihan berupa satu digit 1..maxValue; Enter tanpa isi berarti pilihan bawaan (defaultValue).
    // Menggunakan nilai bawaan dikarenakan pilihan paling umum (cm, π = 3.14, jari-jari) cukup ditekan Enter.
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
            // sejajar. Keduanya dibungkus (wrapText) agar opsi yang panjang tidak terpotong di jendela sempit.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;
        }
    }

    // Berfungsi untuk menampilkan daftar satuan, membaca pilihan, lalu menegaskan satuan yang terpilih.
    const LengthUnit& chooseUnit() {
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        const LengthUnit& unit = UNITS[selection - 1];  // - 1 dikarenakan nomor pilihan mulai dari 1, indeks array mulai dari 0
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit.symbol) << "\n\n";  // penegasan agar pilihan pasti terbaca
        return unit;
    }

    // Berfungsi untuk menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan jari-jari atau diameter kelipatan 7)",
                                      "[Enter = 3.14]" });
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // operator ternary berfungsi memilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";
        return (selection == 2) ? PI_FRACTION : PI_MODULE;
    }

    // Berfungsi untuk menanyakan ukuran mana yang diketahui soal (jari-jari atau diameter) dan menegaskannya.
    bool chooseDiameterInput() {
        printOptions("Ukuran yang Diketahui", { "[1] Jari-jari (R)",
                                             "[2] Diameter (D), program menghitung R = D / 2",
                                             "[Enter = Jari-jari]" });
        const bool diameter = (readChoice("Pilihan Anda            : ", 2, 1) == 2);
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Ukuran         : ") + (diameter ? "Diameter" : "Jari-jari")) << "\n\n";
        return diameter;
    }

    // Berfungsi untuk mencetak judul program (cyan tebal) di antara dua garis pembatas.
    void printTitle(std::string_view text) {
        std::cout << '\n';
        printRule('=');
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';
        printRule('=');
    }

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
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

    // Berfungsi menggambar sketsa tabung pada kanvas Braille: tutup dan alas berupa elips yang dihitung dari
    // persamaannya (x = a cos t, y = b sin t), dinding berupa dua garis tegak, permukaan tutup diarsir tipis, dan
    // tepi alas yang tertutup badan tabung digambar putus-putus seperti pada buku cetak. Garis jari-jari R dan panah
    // tinggi T dibubuhkan sebagai teks berwarna kuning agar langsung terbaca.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal: kanvas maksimal 46 sel dan minimal 24 sel; tinggi mengikuti
        // perbandingan gambar aslinya (21 baris untuk 46 sel) agar bentuk tabung tidak gepeng.
        const int available = terminalWidth() - 8;
        int canvasCols = std::max(24, std::min(46, available - 6));
        if (canvasCols % 2 != 0) --canvasCols;
        const int canvasRows = std::max(12, canvasCols * 21 / 46);
        // Keterangan diletakkan di samping hanya bila baris terlebar (gambar + panah T + keterangan terpanjang) masih
        // muat di dalam kotak (lebar terminal - 5), dikarenakan drawBox akan membungkus baris yang lebih panjang dan
        // itu merusak gambar. 3 = indentasi, 5 = kolom panah "    │", 6 = jarak ke keterangan.
        const std::vector<std::string> legend = {
            "R = jari-jari (pusat ke tepi), D = diameter = 2 x R",
            "T = tinggi tabung (jarak alas ke tutup)",
            "V = Luas alas x T = Pi x R x R x T",
            "garis putus-putus = tepi alas yang tersembunyi di belakang badan tabung",
        };
        std::size_t longestLegend = 0;
        for (int i = 0; i < 3; ++i) longestLegend = std::max(longestLegend, displayWidth(legend[static_cast<std::size_t>(i)]));
        const bool legendOnRight = (3 + canvasCols + 5 + 6 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);
        constexpr double PI_DRAWING = 3.14159265358979;           // hanya untuk menggambar; volume tetap memakai pilihan pengguna

        BrailleCanvas canvas(canvasCols, canvasRows);
        const double semiMajor = canvas.pixelWidth() / 2.0 - 2.0;        // setengah sumbu mendatar elips (titik)
        const double semiMinor = canvas.pixelHeight() * 9.0 / 84.0;      // setengah sumbu tegak elips (titik)
        const double cx = canvas.pixelWidth() / 2.0;             // pusat mendatar
        const double yTop = canvas.pixelHeight() * 11.0 / 84.0, yBottom = yTop + canvas.pixelHeight() * 58.0 / 84.0;

        // Berfungsi menggambar satu elips; bila dashedBack, bagian belakang (atas) digambar putus-putus abu-abu.
        auto drawEllipse = [&](double yCenter, unsigned char frontLayer, bool dashedBack) {
            const int N = 720;  // 720 langkah sudut agar garis tersambung rapat
            for (int i = 0; i < N; ++i) {
                const double t = 2.0 * PI_DRAWING * i / N;
                const int x = static_cast<int>(cx + semiMajor * std::cos(t)), y = static_cast<int>(yCenter + semiMinor * std::sin(t));
                const bool isBack = std::sin(t) < 0;  // y lebih kecil = tepi belakang, tertutup badan tabung
                if (isBack && dashedBack) { if ((i / 18) % 2 == 0) canvas.setPixel(x, y, 4); }
                else canvas.setPixel(x, y, frontLayer);
            }
        };
        for (int y = 0; y < canvas.pixelHeight(); ++y) {           // arsiran tipis pada permukaan tutup
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double ex = (x + 0.5 - cx) / semiMajor, ey = (y + 0.5 - yTop) / semiMinor;
                if (ex * ex + ey * ey < 0.9 && (x + y) % 8 == 0) canvas.setPixel(x, y, 1);
            }
        }
        drawEllipse(yTop, 2, false);                                      // tutup: elips penuh
        const int xLeft = static_cast<int>(cx - semiMajor), xRight = static_cast<int>(cx + semiMajor);
        for (int y = static_cast<int>(yTop); y <= static_cast<int>(yBottom); ++y) {  // dinding kiri dan kanan, 2 titik tebal
            canvas.setPixel(xLeft, y, 3); canvas.setPixel(xLeft + 1, y, 3);
            canvas.setPixel(xRight, y, 3); canvas.setPixel(xRight + 1, y, 3);
        }
        drawEllipse(yBottom, 2, true);                                        // alas: depan utuh, belakang putus-putus
        const std::vector<const char*> layerColors = { "", ANSI_SURFACE, ANSI_TITLE, ANSI_TITLE, ANSI_DIM };  // indeks = nomor lapisan
        std::vector<std::string> figureLines = canvas.toLines(layerColors);

        const int rowTop = static_cast<int>(yTop) / 4, rowBottom = static_cast<int>(yBottom) / 4;  // baris sel tutup dan alas
        {   // garis jari-jari "●──R──►" pada baris tengah tutup
            const int centerCol = canvasCols / 2;
            std::string radiusLine = "\u25cf";
            for (int c = centerCol + 1; c < canvasCols - 2; ++c) radiusLine += (c == (centerCol + canvasCols - 2) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";
            std::string row;
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, rowTop, layerColors);
            figureLines[rowTop] = row + colorize(ANSI_HINT, radiusLine) + canvas.cellString(canvasCols - 1, rowTop, layerColors);
        }
        {   // tulisan TABUNG di tengah badan
            const int r = (rowTop + rowBottom) / 2;
            std::string row;
            for (int c = 0; c < canvasCols; ++c) {
                if (c == canvasCols / 2 - 3) { row += colorize(ANSI_TITLE, "TABUNG"); c += 5; }
                else row += canvas.cellString(c, r, layerColors);
            }
            figureLines[r] = row;
        }
        // Panah tinggi T selalu di samping kanan; keterangan di samping bila jendela lebar, di bawah bila sempit.
        std::vector<std::string> sideNotes(canvasRows, "");
        for (int r = rowTop; r <= rowBottom; ++r) {
            sideNotes[r] = (r == rowTop) ? "  \u2500\u2500\u25b2" : (r == rowBottom) ? "  \u2500\u2500\u25bc" : "    \u2502";
        }
        sideNotes[(rowTop + rowBottom) / 2] += "  T";
        if (legendOnRight) {
            sideNotes[rowTop + 1]              += "      " + legend[0];
            sideNotes[(rowTop + rowBottom) / 2]    += "   " + legend[1];
            sideNotes[rowBottom - 1]               += "      " + legend[2];
        }

        std::vector<std::string> lines;
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + figureLines[r] + colorize(ANSI_HINT, sideNotes[r]));
        if (!legendOnRight) for (int i = 0; i < 3; ++i) lines.push_back("   " + legend[i]);
        lines.push_back("   " + colorize(ANSI_DIM, legend[3]));
        drawBox("Sketsa Tabung", lines);
        std::cout << '\n';
    }

    // Berfungsi untuk menampilkan petunjuk pengisian dalam kotak sebelum pertanyaan pertama, dikarenakan pengguna
    // perlu tahu bentuk input yang diterima (pilihan, angka > 0, cara menulis desimal) agar tidak salah ketik.
    void printInstructions() {
        drawBox("Petunjuk Pengisian", {
            "- Satuan Panjang     : Nomor pilihan 1-4; Enter saja = cm (satuan yang paling umum)",
            "                       (contoh: Pilih [2] untuk satuan \"cm\")",
            "- Nilai Pi (\u03c0)       : Nomor pilihan 1-2; 1 = 3.14 sesuai modul, 2 = 22/7 bila jari-jari atau diameter kelipatan 7",
            "                       (contoh: Pilih [2] untuk nilai \"22/7\")",
            "- Ukuran Diketahui   : Nomor pilihan 1-2; 1 = jari-jari, 2 = diameter (program menghitung R = D / 2)",
            "                       (contoh: Pilih [2] bila soal menyebut \"diameter\")",
            "- Jari-Jari/Diameter : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                       (contoh: 7, 7,5 atau 7.5; tanpa pemisah ribuan: tulis 1000, bukan 1.000)",
            "- Tinggi Tabung      : Angka lebih dari 0; nilai desimal ditulis dengan tanda titik atau koma",
            "                       (contoh: 10, 10,5 atau 10.5)",
            "Ketik jawaban lalu tekan Enter (pada pilihan, Enter tanpa isi = bawaan). Input yang tidak sesuai akan diminta ulang."
        });
        std::cout << '\n';
    }

    // Berfungsi untuk membuat satu baris hasil "Label : nilai satuan" dengan 'decimals' angka desimal (bawaan 2), siap dimasukkan ke kotak.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit, int decimals = 2) {
        std::ostringstream line;  // menggunakan ostringstream berfungsi menyusun teks tanpa mengubah format std::cout
        line << std::left << std::setw(22) << label << ": " << std::fixed << std::setprecision(decimals) << value;
        if (!unit.empty()) line << ' ' << unit;
        return line.str();
    }

}  // namespace

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 2 =====
// Menggunakan namespace task2 berfungsi untuk memberi nama khas pada run(), dikarenakan program bermenu
// memanggilnya sebagai task2::run().
namespace task2 {

    void run() {
        enableUtf8Console();  // dipanggil di sini agar program mandiri (tanpa menu) juga benar; aman dipanggil berulang
        printTitle("PROGRAM 2 : VOLUME TABUNG");
        std::cout << "Rumus: V = Pi x R x R x T   (Pi = \u03c0; modul menulisnya phi)\n\n";
        printSketch();    // berfungsi menampilkan sketsa bangun agar pengguna tahu bagian mana yang dimaksud R, D, T
        printInstructions();

        // --- Tahap pengaturan: satuan, nilai π, dan ukuran yang diketahui ---
        const LengthUnit& unit = chooseUnit();        // reference dikarenakan cukup menunjuk ke daftar, tidak perlu disalin
        std::string   piLabel;                  // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double  pi = choosePi(piLabel);    // nilai π yang dipakai perhitungan
        const bool    diameterGiven = chooseDiameterInput();  // true bila soal menyebut diameter

        // --- Tahap input ---
        // Menggunakan tipe data double berfungsi untuk menyimpan ukuran tabung, dikarenakan jari-jari dan tinggi
        // boleh desimal (mis. 7.5). Menggunakan readPositiveDecimal dikarenakan ukuran bangun tidak mungkin nol
        // atau negatif. Prompt menyertakan satuan agar pengguna tahu satuan yang dipakai.
        const std::string label = diameterGiven ? "Masukkan Diameter Alas (" : "Masukkan Jari-Jari Alas (";
        const double inputSize = readPositiveDecimal(label + unit.symbol + ") : ");
        const double height = readPositiveDecimal(std::string("Masukkan Tinggi Tabung (") + unit.symbol + ")  : ");
        // Menggunakan operator ternary (kondisi ? a : b) berfungsi memilih: bila diameterGiven, radius = inputSize / 2; bila tidak, radius = inputSize.
        const double radius = diameterGiven ? inputSize / 2.0 : inputSize;

        // --- Tahap perhitungan ---
        // Menggunakan dua langkah (luas alas lalu volume) berfungsi agar rumusnya mudah dibaca:
        // luas alas lingkaran = π x r x r, volume = luas alas x tinggi. Operator * = perkalian.
        const double baseArea = pi * radius * radius;
        const double volume   = baseArea * height;
        const double liters    = volume * unit.litersPerCubicUnit;  // padanan liter, dikarenakan soal cerita sering meminta liter

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit.symbol) + "\u00b2";  // satuan luas, mis. cm²
        const std::string volumeUnit = std::string(unit.symbol) + "\u00b3";  // satuan volume, mis. cm³
        std::vector<std::string> resultLines;  // menggunakan vector dikarenakan jumlah baris berbeda (jari-jari vs diameter)
        resultLines.push_back("Pi (\u03c0) yang Digunakan : " + piLabel);
        if (diameterGiven) {  // bila diameter yang diketahui, tampilkan langkah r = d/2 seperti penyelesaian soal
            resultLines.push_back(formatResultLine("Diameter (D)", inputSize, unit.symbol));
            resultLines.push_back(formatResultLine("Jari-Jari (R) = D / 2", radius, unit.symbol));
        } else {
            resultLines.push_back(formatResultLine("Jari-Jari (R)", radius, unit.symbol));
        }
        resultLines.push_back(formatResultLine("Tinggi (T)", height, unit.symbol));
        resultLines.push_back(formatResultLine("Luas Alas", baseArea, areaUnit));
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Volume Tabung", volume, volumeUnit)));  // hasil akhir ditonjolkan hijau tebal
        resultLines.push_back(formatResultLine("Setara", liters, "liter (1 liter = 1 dm\u00b3)", 3));
        std::cout << '\n';
        drawBox("Hasil Perhitungan", resultLines);
    }

}  // namespace task2

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// Menggunakan #ifndef BUILD_WITH_MENU berfungsi untuk menyertakan main() hanya saat berkas ini dikompilasi sendiri,
// dikarenakan program bermenu sudah punya main() dan C++ hanya mengizinkan satu main().
#ifndef BUILD_WITH_MENU
int main() {
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 2 - Fadhlillah (2531070011)") << '\n';
    task2::run();
    return 0;  // 0 berfungsi untuk menandakan program selesai tanpa error
}
#endif
