// ===== main.cpp — program bermenu yang menggabungkan Tugas 1, 2, dan 3 =====
// Berkas ini berisi semua kode pendukung yang tidak bisa dimasukkan ke berkas tugas: banner identitas,
// menu, pembacaan pilihan, jeda, dan perulangan menu. Ketiga berkas tugas (tugas1_*.cpp, tugas2_*.cpp,
// tugas3_*.cpp) masing-masing sudah lengkap dan bisa dikompilasi sendiri.
//
// Nama : Fadhlillah
// NIM  : 2531070011
//
// Kompilasi gabungan (dari folder proyek). -DBUILD_WITH_MENU berfungsi untuk mematikan main() di tiap berkas tugas:
//   g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro src/*.cpp && ./tugas_alpro
//   (Windows PowerShell: .\tugas_alpro.exe) — atau tekan Cmd/Ctrl+Shift+B, lalu Run Task "Build & Run".

#include <algorithm>    // menggunakan <algorithm> berfungsi untuk std::max saat menghitung lebar kotak
#include <cctype>       // menggunakan <cctype> berfungsi untuk std::isdigit, memastikan ketikan menu hanya angka
#include <cstddef>      // menggunakan <cstddef> berfungsi untuk std::size_t (tipe ukuran/lebar teks)
#include <cstdlib>      // menggunakan <cstdlib> berfungsi untuk std::exit, EXIT_FAILURE, dan std::getenv
#include <iostream>     // menggunakan <iostream> berfungsi untuk std::cin, std::cout, std::cerr
#include <string>       // menggunakan <string> berfungsi untuk teks ketikan dan baris tampilan
#include <string_view>  // menggunakan <string_view> berfungsi untuk teks yang hanya dibaca, tanpa disalin
#include <utility>      // menggunakan <utility> berfungsi untuk std::pair (pasangan nama menu dan keterangannya)
#include <vector>       // menggunakan <vector> berfungsi untuk daftar baris yang digambar di dalam kotak
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

// Menggunakan deklarasi berfungsi untuk memberi tahu compiler bahwa fungsi-fungsi ini ada di berkas lain,
// dikarenakan C++ mewajibkan fungsi dideklarasikan sebelum dipanggil. Definisinya ada di tiap berkas tugas.
namespace task1 { void run(); }  // Program 1 : Tools Nilai Student     (tugas1_nilai_mahasiswa.cpp)
namespace task2 { void run(); }  // Program 2 : Volume Tabung             (tugas2_volume_tabung.cpp)
namespace task3 { void run(); }  // Program 3 : Luas Bangun yang Diarsir  (tugas3_luas_arsir.cpp)

// Menggunakan namespace tanpa nama berfungsi untuk menjadikan fungsi tampilan dan pembaca menu milik berkas ini
// saja, dikarenakan nama seperti warna atau kotak juga ada di berkas tugas dan tidak boleh bentrok saat digabung.
namespace {

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

    // Berfungsi untuk mengatur konsol Windows ke UTF-8 (code page 65001) sebelum keluaran pertama, dikarenakan simbol
    // π, ², ³, garis kotak, dan sketsa Braille dikirim sebagai byte UTF-8, sedangkan Command Prompt dan PowerShell bawaan
    // masih memakai code page lama (mis. 437) yang menampilkannya sebagai huruf acak. Pengaturan ini berlaku untuk
    // jendela konsol yang sedang dipakai, sama seperti perintah chcp 65001 pada peluncur jalankan.bat. Di macOS/Linux
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

    // Berfungsi untuk menampilkan banner identitas dalam kotak, sekali di awal program.
    void printBanner() {
        drawBox("Tugas Praktikum Algoritma dan Struktur Data", {
            "Modul 1 : Input dan Output",
            "Nama    : Fadhlillah",
            "NIM     : 2531070011"
        });
    }

    // Berfungsi untuk menampilkan daftar pilihan program dalam kotak beserta keterangan singkat tiap program;
    // [0] diletakkan terakhir dikarenakan kebiasaan menu konsol menempatkan "keluar" di akhir.
    void printMenu() {
        std::cout << '\n';
        // Menggunakan dua bentuk tata letak berfungsi agar menu tetap rapi di jendela sempit: bila lebar terminal
        // kurang dari 100 kolom, keterangan diturunkan ke baris berikutnya; bila lebar, sejajar di kanan nama program.
        const bool isNarrow = terminalWidth() < 100;
        const std::vector<std::pair<std::string, std::string>> menuItems = {
            { "[1] Tools Nilai Mahasiswa", "Rekap Nama, Nilai, Total, dan Rata-Rata satu mata kuliah" },
            { "[2] Volume Tabung        ", "V = Pi x R x R x T; satuan, nilai Pi, jari-jari/diameter dipilih" },
            { "[3] Luas Bangun Diarsir  ", "Persegi dikurangi lingkaran di dalamnya; satuan dan Pi dipilih" },
            { "[0] Keluar               ", "Menutup program" },
        };
        std::vector<std::string> lines;
        for (const auto& item : menuItems) {
            if (isNarrow) { lines.push_back(colorize(ANSI_BOLD, item.first)); lines.push_back("    " + item.second); }
            else        { lines.push_back(colorize(ANSI_BOLD, item.first) + "   " + item.second); }
        }
        drawBox("Menu Program", lines);
        std::cout << "Ketik nomor menu lalu tekan Enter.\n";
    }

    // Berfungsi untuk membaca nomor menu 0-3; ketikan lain (9, abc, 2.5) ditolak dan diminta ulang.
    int readMenuChoice() {
        while (true) {  // menggunakan while (true) berfungsi untuk mengulang sampai pilihan sah; keluar lewat return
            std::string text;
            std::cout << colorize(ANSI_BOLD, "Pilihan Anda [0-3] : ");
            if (!std::getline(std::cin, text)) {  // getline gagal bila input ditutup (Ctrl+D/Ctrl+Z)
                std::cerr << "\nInput berakhir. Program dihentikan.\n";
                std::exit(EXIT_FAILURE);
            }
            // Menghapus spasi di awal/akhir berfungsi agar " 2 " tetap diterima; isi lain diperiksa ketat di bawah.
            const auto start  = text.find_first_not_of(" \t\r");  // \r = sisa akhir baris Windows pada input dari berkas
            const auto end = text.find_last_not_of(" \t\r");
            text = (start == std::string::npos) ? "" : text.substr(start, end - start + 1);
            // Sah bila teks tepat satu digit 0-3; bentuk lain (+1, 1.0, 1 2, abc, kosong) ditolak.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0])) && text[0] <= '3') {
                return text[0] - '0';  // '2' - '0' berfungsi mengubah karakter digit menjadi angka 2
            }
            printError("  [!] Pilihan tidak dikenal. Ketik 1, 2, 3, atau 0 lalu tekan Enter.");
        }
    }

    // Berfungsi untuk menahan layar agar hasil sempat dibaca sebelum menu ditampilkan lagi.
    void waitForEnter() {
        std::cout << '\n' << colorize(ANSI_HINT, "Tekan Enter untuk kembali ke menu...");
        std::string discard;             // isinya diabaikan dikarenakan yang dibutuhkan hanya penekanan Enter
        std::getline(std::cin, discard);
    }

}  // namespace

// Menggunakan int main() dan return 0 dikarenakan standar C++ mewajibkan main bertipe int;
// 0 menandakan program selesai tanpa error.
int main() {
    enableUtf8Console();  // berfungsi menyiapkan konsol Windows untuk UTF-8 sebelum ada keluaran apa pun
    printBanner();  // berfungsi untuk menampilkan identitas sekali di awal

    while (true) {  // menggunakan while (true) berfungsi untuk menampilkan menu berulang sampai pengguna memilih keluar
        printMenu();
        const int choice = readMenuChoice();

        // Menggunakan switch berfungsi untuk mengarahkan satu nilai pilihan ke satu program, dikarenakan lebih
        // rapi dibaca daripada rangkaian if-else untuk beberapa pilihan tetap.
        switch (choice) {
            case 1: task1::run(); break;  // break berfungsi menghentikan switch agar case lain tidak ikut jalan
            case 2: task2::run(); break;
            case 3: task3::run(); break;
            case 0:
                std::cout << '\n' << colorize(ANSI_SUCCESS, "Terima kasih, program selesai.") << '\n';
                return 0;  // return di sini berfungsi untuk mengakhiri program sekaligus keluar dari perulangan
            default:
                break;  // tidak akan terjadi dikarenakan rentang sudah dibatasi 0-3; ditulis agar switch lengkap
        }
        waitForEnter();  // berfungsi untuk menahan hasil di layar sebelum menu ditampilkan lagi
    }
}
