/*
 * Tugas Praktikum 2 — Modul 1: Input dan Output
 * Program 2 : Volume Tabung
 * Alur : nilai bawaan π adalah 3.14 (modul menulis "phi", padahal simbol konstanta lingkaran yang benar
 *        adalah π / pi). Pengguna memilih satuan panjang (mm/cm/dm/m), nilai π (3.14 sesuai modul, atau
 *        22/7 seperti pada soal dengan kelipatan 7), dan ukuran yang diketahui (jari-jari atau diameter).
 *        Setelah itu pengguna mengisi ukuran dan tinggi. Program menampilkan luas alas,
 *        volume V = π x r x r x t, dan padanannya dalam liter.
 * Nama : Fadhlillah
 * NIM  : 2531070011
 *
 * Berkas ini lengkap dan bisa dikompilasi sendiri:
 *   g++ -std=c++17 -Wall -Wextra -o tugas2 tugas2_volume_tabung.cpp && ./tugas2
 * Cara membaca perintah itu, dari kiri ke kanan:
 *   g++            = compiler C++, pengubah kode menjadi program yang bisa dijalankan
 *   -std=c++17     = pakai aturan bahasa C++ versi 2017
 *   -Wall -Wextra  = nyalakan peringatan tambahan, supaya kesalahan kecil cepat ketahuan
 *   -o tugas2      = beri nama program hasilnya "tugas2"
 *   && ./tugas2    = kalau kompilasi berhasil, langsung jalankan programnya
 * Berkas ini juga bisa digabung ke program bermenu (main.cpp di folder yang sama). Untuk itu tambahkan
 * -DBUILD_WITH_MENU saat kompilasi, supaya main() di bagian paling bawah tidak ikut (lihat BAGIAN 7).
 * Opsi -D sama saja dengan menulis #define di awal berkas: ia membuat makro BUILD_WITH_MENU dari perintah
 * kompilasi (arti makro dijelaskan di BAGIAN 1).
 */

// ===== BAGIAN 1: PUSTAKA (LIBRARY) YANG DIPAKAI =====
// Sebelum menulis program, kita "meminjam" dulu alat-alat yang sudah disediakan C++.
// Tulisan setelah // sampai akhir baris adalah komentar. Compiler mengabaikannya; ini hanya untuk manusia.
// #include menyalin isi sebuah header ke berkas ini sebelum dikompilasi.
// Header adalah berkas berisi daftar fungsi dan tipe data milik sebuah pustaka.
// Fungsi pustaka baru bisa dipakai setelah header-nya disertakan.
// Tanda <...> berarti header yang sudah tersedia di sistem, bawaan compiler atau sistem operasi,
// bukan buatan kita sendiri.
// Baris yang diawali # diproses lebih dulu oleh preprocessor, yaitu tahap persiapan sebelum kompilasi.
// Nama dari pustaka standar diawali std::. Bayangkan std sebagai "wadah nama" (namespace) milik C++,
// dan :: dibaca "milik". Jadi std::max artinya max milik pustaka standar.

// Untuk std::max dan std::min, yang mengambil nilai terbesar atau terkecil dari dua nilai.
// Kita memakainya untuk membatasi lebar kotak dan ukuran sketsa.
#include <algorithm>
// Untuk std::isdigit, yang memeriksa apakah satu karakter adalah angka 0-9 (dipakai di BAGIAN 4).
#include <cctype>
// Untuk std::cos dan std::sin. Keduanya menghitung titik-titik elips saat menggambar sketsa tabung.
#include <cmath>
// Untuk std::size_t: bilangan bulat tanpa tanda (tidak bisa negatif) yang dipakai C++ untuk ukuran dan posisi.
// Di sini ia menyimpan lebar teks dan posisi karakter di dalam teks.
#include <cstddef>
// Alat serbaguna. std::exit dan EXIT_FAILURE menghentikan program dengan kode gagal.
// Keduanya dipakai saat input terputus di readLine() (BAGIAN 4).
// std::getenv membaca variabel lingkungan, yaitu pengaturan bernama yang diwariskan sistem operasi ke
// program, misalnya NO_COLOR=1. std::atoi mengubah teks seperti "100" menjadi bilangan bulat 100.
#include <cstdlib>
// Untuk mengatur rupa angka saat dicetak: lebar kolom (std::setw) dan rata kiri (std::left).
// Juga jumlah angka di belakang titik desimal (std::fixed dan std::setprecision). Dipakai di BAGIAN 5 lanjutan.
#include <iomanip>
// Untuk std::initializer_list: daftar nilai yang ditulis langsung di dalam kurung kurawal { ... }.
// Fungsi printOptions() (BAGIAN 5) menerima daftar opsinya dalam bentuk ini.
#include <initializer_list>
// Untuk std::cin (membaca ketikan dari keyboard) dan std::cout (mencetak ke layar).
// Juga std::cerr, saluran khusus untuk mencetak pesan kesalahan.
#include <iostream>
// Untuk membaca dan menyusun teks, mirip membaca keyboard atau mencetak ke layar.
// std::istringstream membaca angka dari teks (BAGIAN 4); std::ostringstream menyusun teks berformat (BAGIAN 5).
#include <sstream>
// Untuk std::string, tipe data teks yang panjangnya bisa berubah.
// Ia menyimpan ketikan pengguna dan baris-baris tampilan.
#include <string>
// Untuk std::string_view, semacam "jendela baca" ke sebuah teks tanpa menyalinnya.
// Cocok untuk prompt dan judul yang hanya dibaca, dikarenakan menyalin teks yang tidak diubah buang waktu saja.
#include <string_view>
// Untuk std::vector, daftar nilai bertipe sama yang panjangnya bisa bertambah sesuai kebutuhan.
// Kita memakainya untuk baris-baris di dalam kotak dan titik-titik sketsa.
#include <vector>
// Program ini harus jalan di Windows maupun macOS/Linux, padahal cara keduanya "berbicara" dengan
// terminal berbeda: mendeteksi terminal, membaca lebar jendela, menyalakan warna, dan mengatur UTF-8.
// Jadi di sini kita bertanya ke compiler: "ini sedang dikompilasi untuk Windows atau bukan?"
// Itulah arti #ifdef _WIN32 (if defined: apakah nama _WIN32 ada? di Windows nama itu otomatis ada).
// Baris sampai #else hanya ikut di Windows, baris setelah #else hanya di macOS/Linux.
// Cara memilih baris seperti ini disebut kompilasi bersyarat.
#ifdef _WIN32
    // Sebelum memanggil windows.h, kita pastikan dulu makro NOMINMAX sudah ada.
    // Makro itu seperti catatan tempel untuk compiler: sebuah nama yang diganti teks lain.
    // NOMINMAX artinya "jangan buat min dan max", karena windows.h punya kebiasaan
    // membuat makro min/max sendiri yang merusak std::min dan std::max kita.
    // #ifndef = "if not defined": blok ini hanya jalan bila NOMINMAX belum ada,
    // dikarenakan MinGW (g++ untuk Windows) sudah mendefinisikannya lebih dulu, dan
    // mendefinisikan ulang nama yang sama menghasilkan peringatan.
    #ifndef NOMINMAX
    #define NOMINMAX 1  // bentuknya: #define NAMA NILAI; mulai baris ini NOMINMAX berarti 1
    #endif  // selesai mengecek NOMINMAX
    // windows.h adalah pustaka resmi Windows. Dari sini kita memakai fungsi untuk warna, UTF-8,
    // dan lebar jendela; semuanya dipanggil di fungsi-fungsi BAGIAN 3.
    #include <windows.h>
#else  // bukan Windows, berarti macOS atau Linux
    // Untuk stdout dan fileno: stdout adalah "saluran" keluaran ke layar, fileno memberi nomor saluran itu.
    #include <cstdio>
    // Untuk ioctl dan TIOCGWINSZ: bertanya ke terminal seberapa lebar jendelanya.
    // Jawabannya ditampung di winsize, tipe data bawaan sistem (dipakai di BAGIAN 3).
    #include <sys/ioctl.h>
    // Untuk isatty, yang memeriksa apakah keluaran benar-benar terminal, bukan berkas.
    // Juga STDOUT_FILENO, yaitu nomor 1, nomor milik keluaran standar (layar).
    #include <unistd.h>
#endif  // selesai memilih pustaka sesuai sistem operasi

// Semua alat bantu dari sini sampai sebelum BAGIAN 6 kita masukkan ke "namespace tanpa nama".
// Namespace adalah wadah nama. Wadah tanpa nama membuat isinya hanya terlihat di berkas ini.
// Kenapa perlu? Berkas tugas lain punya fungsi bantu bernama sama. Tanpa wadah ini, penggabungan ke
// program bermenu gagal dengan error "multiple definition" (satu nama dibuat dua kali).
namespace {

    // ===== BAGIAN 2: KONSTANTA =====
    // Konstanta adalah nilai tetap yang tidak bisa diubah selama program berjalan.
    // constexpr membuat nilainya sudah pasti sejak kompilasi. Ini pengganti modern #define,
    // dan lebih aman dikarenakan punya tipe data. double adalah tipe untuk bilangan desimal.
    // Modul menulis "phi", padahal yang dimaksud adalah π (pi): keliling lingkaran dibagi diameternya.
    // φ (phi) sendiri adalah rasio emas 1,618, bukan konstanta ini.
    // PI_MODULE = 3.14, nilai bawaan sesuai modul (pilihan [1] di choosePi(), BAGIAN 5).
    constexpr double PI_MODULE   = 3.14;
    // PI_FRACTION = 22/7, nilai yang dipakai soal sekolah bila jari-jari atau diameter kelipatan 7 (pilihan [2]).
    // Kita menulis 22.0 / 7.0, bukan 22 / 7. Pembagian dua bilangan bulat di C++ membuang sisanya,
    // jadi 22 / 7 hasilnya 3.
    constexpr double PI_FRACTION = 22.0 / 7.0;
    // Lebar garis judul, dalam karakter (dipakai printRule(), BAGIAN 3). int adalah tipe bilangan bulat.
    constexpr int    TITLE_WIDTH = 60;
    // Lebar kolom label pada pertanyaan pilihan, supaya semua opsi sejajar (dipakai printOptions(), BAGIAN 5).
    constexpr int    LABEL_WIDTH = 24;
    // Batas atas ukuran. Angka yang lebih besar hampir pasti salah ketik, jadi ditolak (BAGIAN 4).
    constexpr double DIMENSION_MAX = 1000000.0;
    // Warna cyan biasa untuk arsiran permukaan tutup tabung pada sketsa.
    // const char* adalah penunjuk ke teks tetap, cara menyimpan teks gaya bahasa C.
    // "\033[36m" adalah kode warna ANSI: terminal tidak mencetaknya, tapi mengganti warna teks sesudahnya.
    // Penjelasan lengkap kode ANSI ada di BAGIAN 3.
    constexpr const char* ANSI_SURFACE = "\033[36m";

    // Nama satuan dan faktor konversinya ke liter selalu dipakai berpasangan, jadi kita kemas jadi satu.
    // Alatnya adalah struct: tipe data buatan sendiri yang mengelompokkan beberapa data di bawah satu nama.
    // Data di dalamnya disebut anggota, dan dibaca dengan titik, misalnya unit.symbol.
    struct LengthUnit {
        const char* symbol;   // simbol satuan panjang, misalnya "cm"
        // Isi 1 satuan kubik dalam liter. Dasarnya: 1 L = 1 dm³ (satuan SI turunan).
        double      litersPerCubicUnit;
    };  // titik koma setelah } penutup struct wajib ditulis
    // Daftar satuan yang lazim di soal; urutannya sama dengan nomor pilihan 1-4.
    // Karena 1 dm³ = 1 L, maka 1 cm³ = 0,001 L (= 1 mL), 1 mm³ = 0,000001 L, dan 1 m³ = 1000 L.
    // UNITS[] adalah array: deretan data bertipe sama yang diakses dengan nomor urut (indeks) mulai dari 0.
    // Jadi UNITS[0] = mm dan UNITS[3] = m. Kurung siku kosong [] artinya compiler sendiri yang menghitung
    // jumlah isinya. Setiap {"cm", 0.001} mengisi symbol lalu litersPerCubicUnit, sesuai urutan.
    constexpr LengthUnit UNITS[] = { {"mm", 0.000001}, {"cm", 0.001}, {"dm", 1.0}, {"m", 1000.0} };
    // Banyaknya satuan di UNITS; dipakai sebagai nomor pilihan terbesar di chooseUnit() (BAGIAN 5).
    constexpr int    UNIT_COUNT   = 4;
    // Pilihan bawaan bila pengguna hanya menekan Enter: 2 = cm, dikarenakan hampir semua soal tabung memakai cm.
    // Hati-hati, ini nomor pilihan (mulai 1), bukan indeks array. chooseUnit() mengambil UNITS[2 - 1].
    constexpr int    DEFAULT_UNIT_INDEX   = 2;

    // ===== BAGIAN 3: WARNA DAN KOTAK TAMPILAN =====
    // Supaya judul, pilihan yang ditegaskan, hasil akhir, dan pesan kesalahan mudah dibedakan, kita beri warna.
    // Caranya dengan kode warna ANSI, yang dipahami terminal macOS, Linux, VS Code, dan Windows Terminal.
    // Kode ANSI adalah teks khusus: karakter ESC, lalu "[", angka, dan huruf "m".
    // ESC ditulis "\033": angka 033 dalam bilangan oktal (basis 8) sama dengan 27, nomor karakter ESC.
    // Terminal tidak mencetak kode ini; ia hanya mengganti warna teks sesudahnya.
    // Arti angkanya: 0 = normal, 1 = tebal, 31 = merah, 32 = hijau, 33 = kuning, 36 = cyan, 90 = abu-abu.
    // Angka bisa digabung dengan titik koma: "1;36" = tebal sekaligus cyan.
    constexpr const char* ANSI_RESET  = "\033[0m";     // kembali ke tampilan normal
    constexpr const char* ANSI_BOLD  = "\033[1m";     // tebal: prompt dan label penting
    constexpr const char* ANSI_TITLE  = "\033[1;36m";  // cyan tebal: judul, judul kotak, dan garis sketsa
    constexpr const char* ANSI_SUCCESS  = "\033[1;32m";  // hijau tebal: pilihan yang ditegaskan dan hasil akhir
    constexpr const char* ANSI_ERROR  = "\033[1;31m";  // merah tebal: pesan kesalahan
    // Kuning: keterangan [Enter = ...], serta tanda R, panah T, dan keterangan di samping sketsa.
    constexpr const char* ANSI_HINT = "\033[33m";
    // Abu-abu: garis kotak, garis pembatas, serta tepi alas putus-putus dan keterangannya pada sketsa.
    constexpr const char* ANSI_DIM    = "\033[90m";

    // Simbol π, ², ³, garis kotak, dan sketsa Braille dikirim ke layar sebagai byte UTF-8.
    // UTF-8 adalah cara menyimpan setiap huruf atau simbol sebagai 1 sampai 4 byte (huruf biasa cukup 1 byte).
    // Byte adalah satuan data di memori yang terdiri atas 8 bit. Bit adalah satu angka 0 atau 1,
    // jadi satu byte bisa bernilai 0 sampai 255.
    // Masalahnya, Command Prompt dan PowerShell bawaan masih memakai code page lama (misalnya 437).
    // Code page adalah tabel yang dipakai konsol untuk menerjemahkan nomor byte menjadi huruf.
    // Dengan tabel yang salah, simbol tadi tampil sebagai huruf acak. Jadi sebelum mencetak apa pun,
    // kita pindahkan konsol Windows ke UTF-8 (code page 65001). Efeknya sama dengan perintah chcp 65001
    // di peluncur jalankan.bat (folder proyek), dan hanya berlaku untuk jendela konsol yang sedang dipakai.
    // Di macOS/Linux terminal sudah UTF-8, jadi fungsi ini tidak melakukan apa-apa.
    // void artinya fungsi ini tidak mengembalikan nilai; () kosong artinya ia tidak menerima masukan.
    void enableUtf8Console() {
    #ifdef _WIN32  // hanya ikut dikompilasi di Windows (lihat BAGIAN 1)
        // SetConsoleOutputCP mengatur code page untuk KELUARAN konsol. CP_UTF8 adalah nomor 65001.
        SetConsoleOutputCP(CP_UTF8);
        // SetConsoleCP adalah pasangannya untuk MASUKAN, supaya ketikan non-ASCII juga dibaca sebagai UTF-8.
        // ASCII adalah tabel 128 karakter dasar: huruf Latin tanpa aksen, angka 0-9, tanda baca, dan spasi.
        // π, ², dan ³ bukan ASCII.
        SetConsoleCP(CP_UTF8);
    #endif  // selesai bagian khusus Windows
    }

    // Boleh pakai warna atau tidak? Aturannya mengikuti kebiasaan program terminal:
    // kalau pengguna menyetel NO_COLOR, warna mati; kalau FORCE_COLOR, warna dipaksa hidup;
    // selain itu warna hanya dipakai bila keluaran benar-benar terminal, bukan berkas.
    // Hasilnya bool, tipe yang hanya bernilai true (benar) atau false (salah). true berarti boleh berwarna.
    bool colorsEnabled() {
        // Pemeriksaannya cukup sekali seumur program, jadi hasilnya disimpan di variabel
        // static (dibuat sekali, diingat terus). const artinya nilainya tidak bisa diubah lagi.
        // Nilai awalnya dihitung oleh lambda, yaitu fungsi kecil tanpa nama berbentuk []() { ... },
        // yang langsung dijalankan oleh tanda () di baris "}();" paling bawah.
        static const bool enabled = []() {
            // getenv mengembalikan nullptr (penunjuk kosong) bila variabel lingkungan itu tidak ada.
            // return mengakhiri lambda dan menyerahkan nilainya.
            if (std::getenv("NO_COLOR") != nullptr) return false;   // pengguna minta tanpa warna
            if (std::getenv("FORCE_COLOR") != nullptr) return true; // pengguna memaksa warna
        #ifdef _WIN32  // Windows: nyalakan pemahaman kode ANSI di konsol
            // Di Windows, kode ANSI baru dimengerti setelah mode "virtual terminal" dinyalakan.
            // Caranya: ambil pegangan (HANDLE) ke layar, baca mode konsol yang sedang aktif,
            // lalu tulis kembali mode itu dengan tambahan satu bendera.
            HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD  mode     = 0;  // DWORD = bilangan bulat milik Windows, tempat mode konsol disimpan
            // Kalau pegangannya tidak sah, atau keluaran ternyata bukan konsol (misalnya
            // dialihkan ke berkas), menyerah saja: tanpa warna.
            // || berarti "atau", ! berarti "tidak". &mode adalah alamat mode, supaya GetConsoleMode bisa mengisinya.
            if (output == INVALID_HANDLE_VALUE || !GetConsoleMode(output, &mode)) return false;
            // Tanda | menambahkan bendera ke mode lama tanpa menghapus yang sudah ada (OR per bit).
            // SetConsoleMode mengembalikan 0 bila gagal, jadi "!= 0" artinya berhasil.
            return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
        #else
            // Di macOS/Linux cukup satu pertanyaan: apakah stdout sebuah terminal (tty)?
            // isatty menjawab bukan-0 bila nomor saluran dari fileno(stdout) adalah terminal.
            return isatty(fileno(stdout)) != 0;
        #endif  // selesai memeriksa sesuai sistem operasi
        }();  // tanda () ini menjalankan lambda sekali; hasilnya jadi isi variabel enabled
        return enabled;  // pemanggilan berikutnya tinggal memakai hasil yang tersimpan
    }

    // Membungkus teks dengan kode warna, asalkan warna boleh dipakai. Kalau tidak, teks dikembalikan apa adanya.
    // code = kode warna (misalnya ANSI_TITLE), text = teks yang diwarnai. Hasilnya std::string baru.
    std::string colorize(const char* code, std::string_view text) {
        if (!colorsEnabled()) return std::string(text);  // warna mati: salin teksnya saja
        // Pada std::string, tanda + menyambung teks: kode warna + teks + ANSI_RESET (kembali normal).
        return std::string(code) + std::string(text) + ANSI_RESET;
    }

    // Ada karakter yang memakan dua kolom di terminal, misalnya aksara Cina/Jepang/Korea (disingkat CJK)
    // dan emoji.
    // Kalau tidak diperhitungkan, lebar kotak dan pemotongan baris jadi meleset.
    // Fungsi ini menjawab: apakah karakter bernomor codePoint termasuk yang lebar?
    // Code point adalah nomor urut karakter di tabel Unicode, misalnya π bernomor 960, ditulis 0x3C0.
    // Angka berawalan 0x ditulis dalam heksadesimal (basis 16); nomor karakter lazim ditulis begitu.
    // unsigned adalah bilangan bulat tanpa tanda (tidak bisa negatif).
    bool isWideCodePoint(unsigned codePoint) {
        // && berarti "dan". Hasilnya true bila codePoint jatuh di salah satu rentang ini:
        // 0x1100-0x115F (Hangul Jamo, huruf dasar aksara Korea), 0x2E80-0xA4CF (aksara CJK),
        // 0xAC00-0xD7A3 (suku kata Hangul), 0xF900-0xFAFF dan 0xFE30-0xFE4F (bentuk kompatibilitas CJK),
        // 0xFF00-0xFF60 dan 0xFFE0-0xFFE6 (huruf dan tanda lebar penuh),
        // 0x1F300-0x1F64F dan 0x1F900-0x1F9FF (emoji).
        return (codePoint >= 0x1100 && codePoint <= 0x115F) || (codePoint >= 0x2E80 && codePoint <= 0xA4CF)
            || (codePoint >= 0xAC00 && codePoint <= 0xD7A3) || (codePoint >= 0xF900 && codePoint <= 0xFAFF)
            || (codePoint >= 0xFE30 && codePoint <= 0xFE4F) || (codePoint >= 0xFF00 && codePoint <= 0xFF60)
            || (codePoint >= 0xFFE0 && codePoint <= 0xFFE6) || (codePoint >= 0x1F300 && codePoint <= 0x1F64F)
            || (codePoint >= 0x1F900 && codePoint <= 0x1F9FF);
    }

    // Berapa kolom yang dipakai sebuah teks di layar? Tidak selalu sama dengan jumlah byte-nya.
    // Karakter UTF-8 multibyte (π, ², ³, garis kotak) tetap dihitung satu kolom, aksara lebar (CJK, emoji)
    // dua kolom, dan kode warna ANSI nol kolom. Byte awal UTF-8 memberi tahu berapa byte lanjutan yang
    // menyusul. Hasilnya bertipe std::size_t (lihat BAGIAN 1).
    std::size_t displayWidth(std::string_view text) {
        std::size_t width = 0;  // jumlah kolom sejauh ini
        bool inEscape = false;  // true selama kita berada di dalam kode warna "\033[...m"
        unsigned codePoint = 0; // code point yang sedang dirangkai dari byte-byte UTF-8
        int pending = 0;        // sisa byte lanjutan yang masih ditunggu
        // Ini perulangan for berbasis rentang (range-based for): c berisi setiap byte text, satu per satu.
        // unsigned char adalah satu byte bernilai 0-255. const berarti c tidak diubah di dalam perulangan.
        for (const unsigned char c : text) {
            // Di dalam kode warna, lewati byte sampai huruf 'm' penutupnya. continue = lompat ke byte berikutnya.
            if (inEscape) { if (c == 'm') inEscape = false; continue; }
            if (c == 0x1B) { inEscape = true; continue; }  // 0x1B = ESC ("\033"), pembuka kode warna
            // Byte lanjutan UTF-8 selalu berpola bit 10xxxxxx. & (AND per bit) dengan 0xC0 mengambil
            // 2 bit teratas, dan hasil 0x80 berarti polanya 10. Byte ini tidak menambah kolom.
            // Enam bit isinya (c & 0x3F) disambung ke codePoint: geser codePoint 6 bit ke kiri (<< 6),
            // lalu gabungkan dengan | (OR). --pending mengurangi sisa byte. Kalau sisanya habis (0)
            // dan karakternya lebar, tambah satu kolom lagi, jadi totalnya 2 kolom.
            if ((c & 0xC0) == 0x80) {
                if (pending > 0) { codePoint = (codePoint << 6) | (c & 0x3F); if (--pending == 0 && isWideCodePoint(codePoint)) ++width; }
                continue;
            }
            ++width;  // setiap byte awal = satu karakter = minimal satu kolom
            // Byte awal menentukan panjang karakter: 110xxxxx = 2 byte (1 lanjutan), 1110xxxx = 3 byte
            // (2 lanjutan), 11110xxx = 4 byte (3 lanjutan). Bit x-nya menjadi awal codePoint.
            // Selain itu berarti huruf ASCII biasa (1 byte), jadi tidak ada byte lanjutan yang ditunggu.
            if      ((c & 0xE0) == 0xC0) { codePoint = c & 0x1F; pending = 1; }
            else if ((c & 0xF0) == 0xE0) { codePoint = c & 0x0F; pending = 2; }
            else if ((c & 0xF8) == 0xF0) { codePoint = c & 0x07; pending = 3; }
            else                         { pending = 0; }
        }  // selesai memeriksa setiap byte
        return width;
    }

    // Seberapa lebar jendela terminal sekarang (dalam kolom)? Kita tanya setiap kali fungsi dipanggil,
    // supaya tampilan ikut menyesuaikan bila jendela diubah ukurannya. Kalau tidak bisa diketahui
    // (keluaran bukan terminal), kita pakai variabel lingkungan COLUMNS, lalu 80 kolom, ukuran bawaan
    // terminal pada umumnya. Hasilnya paling kecil 50, dikarenakan di bawah itu kotak dan sketsa tidak bisa rapi.
    int terminalWidth() {
        int cols = 0;  // 0 = lebar belum diketahui
    #ifdef _WIN32  // Windows: tanya ukuran jendela konsol
        // info adalah struct milik Windows untuk menampung keadaan konsol. GetStdHandle memberi pegangan
        // ke layar, sama seperti di colorsEnabled() di atas. GetConsoleScreenBufferInfo lalu mengisi info
        // lewat alamatnya (&info) dan mengembalikan bukan-0 bila berhasil.
        // srWindow memuat tepi kiri (Left) dan kanan (Right) jendela yang terlihat, jadi lebarnya Right - Left + 1.
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) cols = info.srWindow.Right - info.srWindow.Left + 1;
    #else  // bukan Windows: macOS atau Linux
        // ws menampung ukuran jendela. ioctl dengan TIOCGWINSZ artinya "berapa ukuran jendelamu?".
        // ioctl mengisi ws lewat alamatnya (&ws) dan mengembalikan 0 bila berhasil; ws.ws_col = jumlah kolom.
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) cols = ws.ws_col;
    #endif  // selesai membaca lebar sesuai sistem operasi
        // Cadangan 1: variabel lingkungan COLUMNS. std::atoi mengubah teksnya menjadi int.
        if (cols <= 0) { const char* env = std::getenv("COLUMNS"); if (env != nullptr) cols = std::atoi(env); }
        if (cols <= 0) cols = 80;  // cadangan 2: 80 kolom
        return std::max(50, cols);  // std::max memilih yang lebih besar, jadi hasilnya minimal 50
    }

    // Teks yang terlalu panjang tidak boleh dipotong sembarangan oleh jendela terminal yang sempit.
    // Jadi kita potong sendiri: setiap baris paling lebar 'maxWidth' kolom, dan pemotongan dilakukan di spasi.
    // Baris lanjutan diberi indentasi "gantung" sebanyak 'hangingIndent' spasi, supaya sejajar dengan isi
    // setelah label. Fungsi ini dipakai oleh kotak, pesan kesalahan, dan daftar opsi.
    // const std::string& artinya fungsi memakai teks aslinya tanpa menyalin, dan berjanji tidak mengubahnya.
    // Tanda & di sini disebut reference, yaitu nama lain untuk variabel yang sama.
    // Hasilnya std::vector berisi std::string: daftar baris hasil potongan.
    std::vector<std::string> wrapText(const std::string& text, std::size_t maxWidth, std::size_t hangingIndent) {
        std::vector<std::string> result;  // daftar baris hasil, awalnya kosong
        // Kalau indentasinya terlalu besar untuk jendela sempit, pakai 2 spasi saja.
        if (hangingIndent + 10 > maxWidth) hangingIndent = 2;
        std::string remaining = text;  // sisa teks yang belum dipotong
        // while mengulang selama syaratnya benar; di sini, selama sisa teks masih lebih lebar dari batas.
        while (displayWidth(remaining) > maxWidth) {
            // col = kolom yang sedang diperiksa, cutPos = posisi spasi tempat memotong.
            // std::string::npos adalah nilai khusus yang berarti "posisi tidak ada", di sini: belum ketemu spasi.
            std::size_t col = 0, cutPos = std::string::npos;
            bool inEscape = false;  // true selama di dalam kode warna (tidak dihitung sebagai kolom)
            // Cari spasi terakhir sebelum batas lebar, dihitung per kolom. i = posisi byte, ++i = tambah 1.
            for (std::size_t i = 0; i < remaining.size(); ++i) {
                // static_cast<unsigned char>(...) mengubah char menjadi unsigned char secara tegas.
                // Ini perlu dikarenakan char bisa bernilai negatif untuk byte UTF-8, padahal pemeriksaan
                // di bawah ditulis untuk nilai byte 0-255.
                const unsigned char c = static_cast<unsigned char>(remaining[i]);
                if (inEscape) { if (c == 'm') inEscape = false; continue; }  // lewati isi kode warna
                if (c == 0x1B) { inEscape = true; continue; }  // ESC: awal kode warna
                // Byte lanjutan UTF-8 bukan kolom baru, jadi dilewati (penjelasannya di displayWidth() di atas).
                if ((c & 0xC0) == 0x80) continue;
                if (col >= maxWidth) break;  // break = keluar dari for, karena batas lebar sudah tercapai
                // Ingat posisi spasi ini, asal letaknya sudah melewati indentasi gantung.
                if (c == ' ' && col > hangingIndent) cutPos = i;
                ++col;
            }
            if (cutPos == std::string::npos) break;  // tidak ada spasi untuk memotong: biarkan apa adanya
            // push_back menambahkan satu isi di akhir vector. substr(0, cutPos) = teks dari awal sampai sebelum spasi.
            result.push_back(remaining.substr(0, cutPos));
            // Sisa teks = indentasi gantung + teks setelah spasi pemotong. std::string(n, ' ') = n buah spasi.
            remaining = std::string(hangingIndent, ' ') + remaining.substr(cutPos + 1);
        }  // selesai while
        result.push_back(remaining);  // potongan terakhir (atau seluruh teks bila sejak awal sudah muat)
        return result;
    }

    // Menggambar kotak berjudul berisi baris-baris teks. Dengan kotak, informasi (petunjuk, menu, hasil)
    // terpisah jelas dari pertanyaan. Lebar kotak mengikuti baris terpanjang, tapi tidak pernah melebihi
    // lebar terminal: baris yang kepanjangan dipotong dulu oleh wrapText() di atas. Bila baris berpola
    // "Label : isi", baris lanjutannya menjorok sejajar dengan isi setelah label.
    // title = judul kotak; lines = daftar baris isi (diterima lewat const& supaya tidak disalin).
    void drawBox(std::string_view title, const std::vector<std::string>& lines) {
        // Lebar terminal dikurangi 5: 2 tepi kotak, 2 margin, dan 1 kolom cadangan. Cadangan itu perlu
        // dikarenakan sebagian terminal menambah baris kosong bila sebuah baris persis selebar jendela.
        // static_cast<std::size_t> mengubah hasil int menjadi std::size_t.
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 5);
        std::vector<std::string> contentLines;  // baris isi setelah dipotong
        for (const std::string& b : lines) {  // b = setiap baris masukan, dibaca tanpa disalin
            std::size_t hangingIndent = 2;  // indentasi gantung bawaan: 2 spasi
            // find mencari posisi " : "; hasilnya std::string::npos bila tidak ketemu.
            const std::size_t labelPos = b.find(" : ");
            // Kalau ada label selebar paling banyak 28 kolom, baris lanjutan menjorok sejajar isi setelah label.
            if (labelPos != std::string::npos && displayWidth(b.substr(0, labelPos)) <= 28) {
                hangingIndent = displayWidth(b.substr(0, labelPos)) + 3;  // + 3 = lebar " : "
            }
            // Setiap potongan hasil wrapText() masuk ke contentLines.
            for (const std::string& piece : wrapText(b, maxWidth, hangingIndent)) contentLines.push_back(piece);
        }
        std::size_t width = displayWidth(title) + 4;  // lebar dalam minimal: judul + hiasan
        // Lebar dalam mengikuti baris terpanjang; + 2 untuk margin kiri dan kanan.
        for (const std::string& b : contentLines) width = std::max(width, displayWidth(b) + 2);
        width = std::min(width, maxWidth + 2);  // std::min: tapi tidak boleh melewati batas jendela
        // filler = garis ─ untuk mengisi sisa tepi atas setelah "┌─ " + judul + " ".
        // "\u2500" adalah cara menulis karakter Unicode lewat nomornya:
        // \u2500 = ─, \u250c = ┌, \u2510 = ┐, \u2502 = │, \u2514 = └, \u2518 = ┘.
        std::string filler;
        for (std::size_t i = displayWidth(title) + 3; i < width; ++i) filler += "\u2500";  // ─ sebanyak sisa lebar
        // Tepi atas: "┌─ " abu-abu, judul cyan tebal, lalu " " + filler + "┐" abu-abu. '\n' = pindah baris.
        std::cout << colorize(ANSI_DIM, "\u250c\u2500 ") << colorize(ANSI_TITLE, title) << colorize(ANSI_DIM, " " + filler + "\u2510") << '\n';
        // Setiap baris isi: "│" + spasi + teks + spasi pengisi + "│".
        // Operator ternary (kondisi ? a : b) memilih a bila kondisi benar, b bila salah. Di sini ia
        // menghitung spasi pengisi: width - contentWidth, atau 0 bila baris sudah selebar kotak.
        for (const std::string& b : contentLines) {
            const std::string rowText = " " + b;
            const std::size_t contentWidth = displayWidth(rowText);
            std::cout << colorize(ANSI_DIM, "\u2502") << rowText << std::string(width > contentWidth ? width - contentWidth : 0, ' ')
                      << colorize(ANSI_DIM, "\u2502") << '\n';
        }
        // Tepi bawah: "└" + garis ─ sebanyak width + "┘".
        std::string bottomLine;
        for (std::size_t i = 0; i < width; ++i) bottomLine += "\u2500";
        std::cout << colorize(ANSI_DIM, "\u2514" + bottomLine + "\u2518") << '\n';
    }

    // Mencetak pesan kesalahan berwarna merah. Pesan panjang dipotong sesuai lebar terminal, dan baris
    // lanjutannya menjorok sejajar teks setelah tanda "[!]", supaya tetap utuh terbaca di jendela sempit.
    void printError(const std::string& text) {
        // Lebarnya = lebar terminal - 1 kolom cadangan. Indentasi gantung 6 = lebar awalan "  [!] " tiap pesan.
        for (const std::string& b : wrapText(text, static_cast<std::size_t>(terminalWidth() - 1), 6)) {
            std::cout << colorize(ANSI_ERROR, b) << '\n';  // setiap potongan dicetak merah tebal
        }
    }

    // Mencetak satu garis pembatas abu-abu sepanjang TITLE_WIDTH (BAGIAN 2).
    // char adalah tipe untuk satu karakter, misalnya '='.
    // std::string(TITLE_WIDTH, ch) membuat teks berisi ch yang diulang TITLE_WIDTH kali.
    void printRule(char ch) {
        std::cout << colorize(ANSI_DIM, std::string(TITLE_WIDTH, ch)) << '\n';
    }

    // ===== BAGIAN 4: FUNGSI BANTU INPUT =====
    // Di bagian ini kita membaca ketikan pengguna, lalu memeriksa apakah ketikan itu sah.

    // Menampilkan pertanyaan (prompt) dengan huruf tebal, lalu membaca satu baris ketikan.
    // Hasilnya std::string berisi ketikan pengguna, tanpa tombol Enter-nya.
    std::string readLine(std::string_view prompt) {
        std::string line;  // std::string dikarenakan panjang ketikan tidak pasti
        std::cout << colorize(ANSI_BOLD, prompt);  // tebal, supaya pertanyaan beda dari keterangan
        // std::getline membaca satu baris penuh dari keyboard (std::cin), termasuk spasi, sampai Enter.
        // Tanda ! menangkap kegagalannya, dikarenakan getline gagal bila input ditutup (Ctrl+D atau Ctrl+Z).
        if (!std::getline(std::cin, line)) {
            // Ini pesan error, jadi dikirim ke std::cerr. "\n" di awal teks = pindah baris dulu.
            std::cerr << "\nInput berakhir sebelum data lengkap. Program dihentikan.\n";
            // Data tidak lengkap, jadi program berhenti saat itu juga dengan kode gagal (EXIT_FAILURE).
            std::exit(EXIT_FAILURE);
        }
        return line;  // serahkan baris yang dibaca ke pemanggil
    }

    // Merapikan spasi: buang spasi/tab di awal dan akhir, lalu rapatkan spasi beruntun menjadi satu.
    // Contoh: "  7   5 " menjadi "7 5". Parameter adalah masukan fungsi, yang ditulis di dalam kurung.
    // Di sini parameternya const& dikarenakan teks hanya dibaca, jadi tidak perlu disalin.
    std::string normalizeSpaces(const std::string& text) {
        // tempat menampung teks yang sudah rapi
        std::string result;
        bool pendingSpace = false;  // true bila ada spasi yang belum ditulis
        for (const char c : text) {  // periksa setiap karakter satu per satu
            // Spasi, tab ('\t'), dan CR ('\r', sisa akhir baris Windows) tidak langsung ditulis, cukup dicatat.
            if (c == ' ' || c == '\t' || c == '\r') {
                pendingSpace = !result.empty();  // spasi di awal diabaikan, karena hasil masih kosong
            } else {  // karakter selain spasi
                if (pendingSpace) result += ' ';  // spasi beruntun ditulis sebagai satu spasi saja
                result += c;  // += artinya tambahkan c di akhir hasil
                pendingSpace = false;
            }
        }
        return result;  // spasi di akhir otomatis terbuang, karena tidak pernah ditulis
    }

    // Apakah teks hanya berisi angka 0-9? Tanda, titik, huruf, atau spasi membuat jawabannya false.
    bool isAllDigits(const std::string& text) {
        if (text.empty()) return false;  // teks kosong bukan bilangan; empty() bernilai true bila teks kosong
        for (const char c : text) {  // setiap karakter harus digit
            // std::isdigit hanya aman untuk nilai 0-255, sedangkan char bisa negatif untuk huruf non-ASCII.
            // Karena itu c diubah dulu ke unsigned char. Satu saja yang bukan digit, langsung false.
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;  // semua karakter digit
    }

    // Memeriksa bentuk bilangan desimal: angka, boleh diikuti SATU titik atau koma, lalu angka lagi
    // (7, 7.5, 7,5). Bentuk lain seperti 1e2, +5, .5, 5., 8.7.5, atau 7 5 ditolak, dikarenakan
    // itu bukan cara menulis ukuran yang kita harapkan dari pengguna.
    bool isDecimalFormat(const std::string& text) {
        // find_first_of(".,") mencari titik atau koma pertama; hasilnya std::string::npos bila tidak ada.
        // auto artinya tipe variabel ditentukan compiler dari nilainya (di sini tipe posisi teks).
        const auto separator = text.find_first_of(".,");
        if (separator == std::string::npos) return isAllDigits(text);  // tanpa pemisah: harus digit semua
        // substr(0, separator) = teks sebelum pemisah, substr(separator + 1) = teks sesudahnya sampai akhir.
        return isAllDigits(text.substr(0, separator))  // bagian sebelum pemisah harus digit
            && isAllDigits(text.substr(separator + 1));  // bagian sesudahnya juga, jadi pemisah kedua tertolak
    }

    // Mengubah teks menjadi bilangan desimal. Hasilnya true bila teksnya sah.
    // text sengaja ditulis tanpa &, jadi yang diterima adalah salinan yang boleh diubah tanpa mengganggu aslinya.
    // double& result adalah reference: angka hasilnya ditulis langsung ke variabel milik pemanggil.
    bool parseDecimal(std::string text, double& result) {
        if (!isDecimalFormat(text)) return false;  // cek bentuknya dulu, supaya hanya tulisan angka yang lolos
        // char& membuat c menjadi nama lain untuk karakter di dalam text, jadi mengubah c berarti mengubah text.
        for (char& c : text) {
            if (c == ',') c = '.';  // koma diganti titik, karena C++ hanya mengenal penulisan 7.5
        }
        std::istringstream parser(text);  // parser membaca angka dari teks, mirip std::cin membaca keyboard
        // parser >> result membaca angka ke result (true bila berhasil). parser >> std::ws melewati spasi,
        // lalu .eof() memastikan sudah sampai akhir teks. Jadi sah hanya bila angkanya terbaca tanpa sisa.
        return (parser >> result) && (parser >> std::ws).eof();
    }

    // Meminta bilangan desimal yang lebih besar dari 0 dan paling besar DIMENSION_MAX (BAGIAN 2).
    // Permintaan diulang sampai jawabannya sah. prompt = teks pertanyaan yang ditampilkan.
    double readPositiveDecimal(std::string_view prompt) {
        // while (true) mengulang terus; satu-satunya jalan keluar adalah return saat input sah.
        while (true) {
            // double dikarenakan ukuran boleh desimal; 0.0 hanya nilai awal yang jelas.
            double value = 0.0;
            // Baca baris, rapikan spasinya, lalu ubah ke angka (tiga fungsi di atas).
            // Syarat value > 0.0 menolak nol dan negatif, dikarenakan ukuran bangun tidak mungkin begitu.
            // Syarat value <= DIMENSION_MAX menolak angka yang terlalu besar.
            if (parseDecimal(normalizeSpaces(readLine(prompt)), value) && value > 0.0 && value <= DIMENSION_MAX) {
                return value;
            }
            // Dua teks "..." "..." yang ditulis berurutan otomatis digabung compiler menjadi satu teks.
            printError("  [!] Harus angka lebih besar dari 0 dan paling besar 1000000, hanya digit dengan"
                                        " satu pemisah desimal (mis. 7.5 atau 7,5). Silakan ulangi.");
        }  // kembali meminta input
    }

    // Meminta pilihan berupa satu digit, dari 1 sampai maxValue. Enter tanpa isi berarti pilihan bawaan
    // (defaultValue). Dengan begitu, pilihan paling umum (cm, π = 3.14, jari-jari) cukup dijawab dengan Enter.
    int readChoice(std::string_view prompt, int maxValue, int defaultValue) {
        while (true) {  // diulang, karena pilihan yang salah harus ditanyakan lagi
            const std::string text = normalizeSpaces(readLine(prompt));  // baca, lalu rapikan spasinya
            if (text.empty()) return defaultValue;  // Enter saja = pilih bawaan
            // Sah bila tepat satu digit (size() == 1) dan ada di rentang. text[0] = karakter pertama.
            // text[0] - '0' mengubah karakter digit menjadi angka. Ini bisa dikarenakan kode karakter
            // '0' sampai '9' berurutan, misalnya '7' - '0' = 7.
            if (text.size() == 1 && std::isdigit(static_cast<unsigned char>(text[0]))) {
                const int choice = text[0] - '0';
                if (choice >= 1 && choice <= maxValue) return choice;  // di rentang 1 sampai maxValue: sah
            }
            // std::to_string mengubah angka maxValue menjadi teks, supaya bisa disambung ke pesan dengan +.
            printError("  [!] Pilihan tidak dikenal. Ketik salah satu nomor 1-" + std::to_string(maxValue)
                                        + ", atau tekan Enter untuk memakai bawaan.");
        }
    }

    // ===== BAGIAN 5: TAMPILAN PERTANYAAN PILIHAN, JUDUL, DAN PETUNJUK =====

    // Semua pertanyaan pilihan tampil dengan bentuk yang sama, supaya pengguna langsung mengenali pola
    // "[nomor] pilihan" dan "[Enter = bawaan]". Baris pertama memuat label (tebal) dan opsi pertama,
    // baris berikutnya opsi lain yang disejajarkan di bawahnya, dan baris terakhir nilai bawaan (kuning).
    // Daftar opsinya ditulis langsung dalam { ... } saat fungsi dipanggil (std::initializer_list), misalnya
    // printOptions("Pilih Nilai Pi", { "[1] ...", "[2] ...", "[Enter = 3.14]" }).
    void printOptions(std::string_view label, std::initializer_list<std::string_view> line) {
        const std::size_t maxWidth = static_cast<std::size_t>(terminalWidth() - 1);  // 1 kolom cadangan
        bool isFirst = true;  // menandai baris pertama, yang memuat label
        for (const std::string_view b : line) {  // b = setiap teks opsi, berurutan
            // substr(0, 6) = 6 karakter pertama. Baris yang diawali "[Enter" adalah baris bawaan.
            const bool isDefaultLine = (b.substr(0, 6) == "[Enter");
            // Baris bawaan dicetak kuning, baris lain apa adanya.
            const std::string content = isDefaultLine ? colorize(ANSI_HINT, b) : std::string(b);
            // Baris pertama diawali label (tebal) dan ": ". Label yang lebih pendek dari LABEL_WIDTH
            // ditambah spasi sampai selebar LABEL_WIDTH. Baris berikutnya diawali LABEL_WIDTH + 2 spasi
            // (selebar label yang sudah dilebarkan + ": "), supaya opsinya sejajar.
            const std::string start = isFirst
                ? colorize(ANSI_BOLD, std::string(label) + std::string(label.size() < LABEL_WIDTH ? LABEL_WIDTH - label.size() : 0, ' ')) + ": "
                : std::string(LABEL_WIDTH + 2, ' ');
            // Opsi yang panjang dipotong dengan wrapText() (BAGIAN 3), supaya tidak terpotong di jendela sempit.
            // Baris lanjutannya menjorok LABEL_WIDTH + 2 spasi.
            for (const std::string& piece : wrapText(start + content, maxWidth, LABEL_WIDTH + 2)) std::cout << piece << '\n';
            isFirst = false;  // baris berikutnya bukan baris pertama lagi
        }
    }

    // Menampilkan daftar satuan, membaca pilihan, lalu menegaskan satuan yang terpilih.
    // Hasilnya const LengthUnit&: reference ke salah satu isi UNITS (BAGIAN 2), jadi tidak perlu disalin.
    const LengthUnit& chooseUnit() {
        printOptions("Pilih Satuan Panjang", { "[1] mm   [3] dm", "[2] cm   [4] m", "[Enter = cm]" });
        // nomor 1 sampai UNIT_COUNT (4); Enter saja = DEFAULT_UNIT_INDEX (2 = cm)
        const int selection = readChoice("Pilihan Anda            : ", UNIT_COUNT, DEFAULT_UNIT_INDEX);
        // dikurangi 1, karena nomor pilihan mulai dari 1 sedangkan indeks array mulai dari 0
        const LengthUnit& unit = UNITS[selection - 1];
        // Penegasan hijau tebal, supaya pilihan pasti terbaca. "\n\n" = pindah baris dua kali (satu baris kosong).
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Satuan panjang : ") + unit.symbol) << "\n\n";
        return unit;
    }

    // Menampilkan pilihan nilai π, membaca pilihan, lalu menegaskan nilai yang terpilih.
    // Nilai kembaliannya (double) adalah π untuk perhitungan.
    // Teks nilai π untuk tampilan dititipkan lewat legend, sebuah reference ke variabel milik pemanggil.
    double choosePi(std::string& legend) {
        printOptions("Pilih Nilai Pi", { "[1] 3.14  (sesuai modul)",
                                      "[2] 22/7  (soal dengan jari-jari atau diameter kelipatan 7)",
                                      "[Enter = 3.14]" });
        const int selection = readChoice("Pilihan Anda            : ", 2, 1);  // nomor 1-2; Enter saja = 1 (3.14)
        legend = (selection == 2) ? "22/7 (3.142857)" : "3.14";  // ternary: pilih teks sesuai nomor
        std::cout << colorize(ANSI_SUCCESS, "  -> Nilai Pi (\u03c0)   : " + legend) << "\n\n";  // \u03c0 = π
        return (selection == 2) ? PI_FRACTION : PI_MODULE;  // pilihan 2 = 22/7, selain itu 3.14 (BAGIAN 2)
    }

    // Menanyakan ukuran mana yang diketahui soal, jari-jari atau diameter, lalu menegaskannya.
    // Hasilnya true bila diameter yang diketahui, false bila jari-jari.
    bool chooseDiameterInput() {
        printOptions("Ukuran yang Diketahui", { "[1] Jari-jari (R)",
                                             "[2] Diameter (D), program menghitung R = D / 2",
                                             "[Enter = Jari-jari]" });
        const bool diameter = (readChoice("Pilihan Anda            : ", 2, 1) == 2);  // true hanya bila pilihan 2
        // Penegasan hijau tebal; ternary memilih teks "Diameter" atau "Jari-jari".
        std::cout << colorize(ANSI_SUCCESS, std::string("  -> Ukuran         : ") + (diameter ? "Diameter" : "Jari-jari")) << "\n\n";
        return diameter;
    }

    // Mencetak judul program (cyan tebal) yang diapit dua garis pembatas.
    void printTitle(std::string_view text) {
        std::cout << '\n';  // baris kosong sebelum judul
        printRule('=');  // garis "=" sepanjang TITLE_WIDTH (BAGIAN 3)
        std::cout << colorize(ANSI_TITLE, "  " + std::string(text)) << '\n';  // judul menjorok 2 spasi
        printRule('=');
    }

    // ===== BAGIAN 5 (LANJUTAN): KANVAS BRAILLE, SKETSA, DAN BARIS HASIL =====
    // Bagaimana menggambar tabung yang halus hanya dengan teks? Kita pinjam karakter Braille Unicode,
    // bernomor U+2800 s.d. U+28FF. "U+" adalah penulisan nomor Unicode dalam heksadesimal (= 0x2800 s.d. 0x28FF).
    // Setiap karakter Braille punya 8 titik (2 x 4) yang bisa dinyalakan satu per satu. Jadi satu sel
    // terminal memberi 8 titik gambar, delapan kali lebih halus daripada satu karakter blok.
    // Teknik ini juga dipakai pustaka seperti drawille.
    // struct BrailleCanvas adalah "kanvas" kita: ia menyimpan titik-titik yang menyala, lalu mengubahnya
    // menjadi baris teks. Selain data, struct ini juga punya fungsi anggota, yang dipanggil dengan titik,
    // misalnya canvas.setPixel(...).
    struct BrailleCanvas {
        int cols, rows;  // ukuran kanvas dalam sel terminal (kolom dan baris karakter)
        // layers menyimpan nomor lapisan tiap titik: 0 = kosong, lebih dari 0 = nomor lapisan (penentu warna).
        // Titik disimpan berderet baris demi baris, jadi titik (x, y) ada di indeks y * pixelWidth() + x.
        // pixelWidth() adalah lebar kanvas dalam titik (fungsi anggota beberapa baris di bawah).
        std::vector<unsigned char> layers;

        // Constructor adalah fungsi bernama sama dengan struct-nya, yang otomatis jalan saat kanvas dibuat,
        // misalnya BrailleCanvas canvas(46, 21). Bagian setelah titik dua mengisi anggota: cols = k,
        // rows = row, dan layers berisi (2 x k) x (4 x row) titik yang semuanya mulai dari 0 (kosong).
        BrailleCanvas(int k, int row) : cols(k), rows(row), layers(static_cast<std::size_t>(2 * k) * (4 * row), 0) {}
        // const setelah () berarti fungsi anggota ini hanya membaca, tidak mengubah isi kanvas.
        int pixelWidth()  const { return 2 * cols; }  // lebar dalam titik: 2 titik per sel ke samping
        int pixelHeight() const { return 4 * rows; }  // tinggi dalam titik: 4 titik per sel ke bawah

        // Menyalakan satu titik. Lapisan bernomor lebih besar menimpa yang lebih kecil (garis di atas arsiran).
        // x = posisi mendatar, y = posisi tegak (0 = paling atas), layer = nomor lapisan.
        void setPixel(int x, int y, unsigned char layer) {
            if (x < 0 || y < 0 || x >= pixelWidth() || y >= pixelHeight()) return;  // di luar kanvas diabaikan
            // ref adalah reference ke isi layers di posisi (x, y). Mengubah ref = mengubah isi layers itu.
            unsigned char& ref = layers[static_cast<std::size_t>(y) * pixelWidth() + x];
            if (layer > ref) ref = layer;  // simpan hanya bila lapisannya lebih tinggi dari yang sudah ada
        }

        // Mengubah 8 titik dalam satu sel menjadi satu karakter Braille UTF-8, berwarna sesuai lapisannya.
        // cx, cy = posisi sel (kolom, baris); layerColors = daftar kode warna, indeksnya = nomor lapisan.
        std::string cellString(int cx, int cy, const std::vector<const char*>& layerColors) const {
            // Peta bit Braille: kolom kiri dari atas ke bawah 0x01, 0x02, 0x04, 0x40; kolom kanan 0x08, 0x10,
            // 0x20, 0x80. bit[4][2] adalah array dua dimensi: 4 baris titik x 2 kolom titik.
            // static supaya tabel ini dibuat sekali saja, bukan setiap kali fungsi dipanggil.
            static const unsigned char bit[4][2] = { {0x01, 0x08}, {0x02, 0x10}, {0x04, 0x20}, {0x40, 0x80} };
            // value = gabungan bit titik yang menyala; maxLayer = lapisan tertinggi di sel ini (penentu warna).
            unsigned char value = 0, maxLayer = 0;
            for (int dy = 0; dy < 4; ++dy) {  // dy = baris titik di dalam sel (0-3)
                for (int dx = 0; dx < 2; ++dx) {  // dx = kolom titik di dalam sel (0-1)
                    // ambil nomor lapisan titik (cx * 2 + dx, cy * 4 + dy) dari layers
                    const unsigned char layer = layers[static_cast<std::size_t>(cy * 4 + dy) * pixelWidth() + cx * 2 + dx];
                    // Titik menyala (layer bukan 0)? Nyalakan bit-nya (|= artinya OR lalu simpan),
                    // lalu catat lapisan tertingginya.
                    if (layer) { value |= bit[dy][dx]; maxLayer = std::max(maxLayer, layer); }
                }
            }
            if (!value) return " ";  // sel kosong = spasi biasa
            // Nomor Unicode karakter Braille yang dimaksud. Akhiran u pada 0x2800u menandai bilangan unsigned.
            const unsigned codePoint = 0x2800u + value;
            // Karakter ini disandikan ke UTF-8 tiga byte berpola 1110xxxx 10xxxxxx 10xxxxxx.
            // >> menggeser bit ke kanan, & 0x3F mengambil 6 bit terbawah, dan | memasang pola awal byte.
            // static_cast<char> menjadikan hasilnya satu byte.
            std::string s;
            s += static_cast<char>(0xE0 | (codePoint >> 12));
            s += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
            s += static_cast<char>(0x80 | (codePoint & 0x3F));
            return colorize(layerColors[maxLayer], s);  // beri warna sesuai lapisan tertinggi
        }

        // Mengubah seluruh kanvas menjadi baris-baris teks siap cetak, satu std::string per baris sel.
        std::vector<std::string> toLines(const std::vector<const char*>& layerColors) const {
            std::vector<std::string> lines;
            for (int cy = 0; cy < rows; ++cy) {  // setiap baris sel, dari atas ke bawah
                std::string row;
                for (int cx = 0; cx < cols; ++cx) row += cellString(cx, cy, layerColors);  // sel kiri ke kanan
                lines.push_back(row);
            }
            return lines;
        }
    };  // akhir struct BrailleCanvas

    // Menggambar sketsa tabung di kanvas Braille, mirip gambar di buku cetak.
    // Tutup dan alas berupa elips yang dihitung dari persamaannya: x = a cos t, y = b sin t.
    // a dan b adalah setengah sumbu mendatar dan tegak elips (semiMajor dan semiMinor di bawah).
    // Dinding berupa dua garis tegak, dan permukaan tutup diarsir tipis. Tepi alas yang tertutup badan
    // tabung digambar putus-putus. Garis jari-jari R dan panah tinggi T ditulis sebagai teks kuning
    // supaya langsung terbaca.
    void printSketch() {
        // Ukuran gambar mengikuti lebar terminal: kanvas paling lebar 46 sel dan paling sempit 24 sel.
        // Tingginya mengikuti perbandingan gambar asli (21 baris untuk 46 sel), supaya tabung tidak gepeng.
        // available = lebar terminal dikurangi 8 (tepi dan margin kotak 5 + indentasi gambar 3).
        const int available = terminalWidth() - 8;
        // Sisakan 6 kolom di kanan untuk panah T, lalu batasi: paling besar 46 (std::min), paling kecil 24 (std::max).
        int canvasCols = std::max(24, std::min(46, available - 6));
        if (canvasCols % 2 != 0) --canvasCols;  // % = sisa bagi; kalau ganjil, kurangi 1 supaya genap
        const int canvasRows = std::max(12, canvasCols * 21 / 46);  // tinggi sebanding 21/46, minimal 12 baris
        // Empat baris keterangan gambar, ditulis langsung dengan { ... } (indeks 0 sampai 3).
        const std::vector<std::string> legend = {
            "R = jari-jari (pusat ke tepi), D = diameter = 2 x R",
            "T = tinggi tabung (jarak alas ke tutup)",
            "V = Luas alas x T = Pi x R x R x T",
            "garis putus-putus = tepi alas yang tersembunyi di belakang badan tabung",
        };
        // Cari keterangan terlebar di antara legend[0] sampai legend[2]. Hanya ketiganya yang bisa ditaruh
        // di samping gambar; legend[3] selalu di bawah.
        std::size_t longestLegend = 0;
        for (int i = 0; i < 3; ++i) longestLegend = std::max(longestLegend, displayWidth(legend[static_cast<std::size_t>(i)]));
        // Keterangan ditaruh di samping hanya bila baris terlebar (gambar + panah T + keterangan terpanjang)
        // masih muat di dalam kotak (lebar terminal - 5). Kalau tidak muat, drawBox() (BAGIAN 3) memotong
        // barisnya dan gambarnya rusak. Rinciannya: 3 = indentasi, 5 = kolom panah "    │", 6 = jarak ke keterangan.
        const bool legendOnRight = (3 + canvasCols + 5 + 6 + longestLegend) <= static_cast<std::size_t>(terminalWidth() - 5);
        // π yang lebih teliti, khusus untuk menggambar; volume tetap memakai π pilihan pengguna
        constexpr double PI_DRAWING = 3.14159265358979;

        BrailleCanvas canvas(canvasCols, canvasRows);  // kanvas kosong (constructor BrailleCanvas di atas)
        // Ukuran elips memakai perbandingan gambar asli yang tingginya 84 titik (21 baris x 4 titik),
        // jadi bentuknya sama untuk kanvas sebesar apa pun: setengah sumbu tegak 9/84, pusat tutup 11/84,
        // dan jarak tutup ke alas 58/84.
        const double semiMajor = canvas.pixelWidth() / 2.0 - 2.0;        // setengah sumbu mendatar (titik)
        const double semiMinor = canvas.pixelHeight() * 9.0 / 84.0;      // setengah sumbu tegak (titik)
        const double cx = canvas.pixelWidth() / 2.0;             // pusat mendatar
        // yTop = posisi tegak pusat elips tutup; yBottom = posisi tegak pusat elips alas.
        const double yTop = canvas.pixelHeight() * 11.0 / 84.0, yBottom = yTop + canvas.pixelHeight() * 58.0 / 84.0;

        // drawEllipse menggambar satu elips. Bila dashedBack bernilai true, bagian belakangnya (atas)
        // digambar putus-putus abu-abu. drawEllipse adalah lambda yang disimpan ke variabel (tipenya auto).
        // [&] artinya lambda boleh memakai variabel di sekitarnya (canvas, cx, semiMajor, semiMinor,
        // PI_DRAWING) secara langsung, tanpa disalin.
        auto drawEllipse = [&](double yCenter, unsigned char frontLayer, bool dashedBack) {
            const int N = 720;  // 720 langkah sudut, supaya garisnya tersambung rapat
            for (int i = 0; i < N; ++i) {
                const double t = 2.0 * PI_DRAWING * i / N;  // sudut t dari 0 sampai hampir 2π (satu putaran)
                // Titik elips: x = cx + a cos t, y = yCenter + b sin t, dengan a = semiMajor dan b = semiMinor.
                // static_cast<int> membuang angka desimal, dikarenakan posisi titik di kanvas harus bilangan bulat.
                const int x = static_cast<int>(cx + semiMajor * std::cos(t)), y = static_cast<int>(yCenter + semiMinor * std::sin(t));
                const bool isBack = std::sin(t) < 0;  // y lebih kecil = tepi belakang, tertutup badan tabung
                // Tepi belakang yang putus-putus hanya digambar pada kelompok 18 langkah yang berselang-seling
                // ((i / 18) % 2 == 0), dengan lapisan 4 (abu-abu). Selain itu digambar utuh dengan frontLayer.
                if (isBack && dashedBack) { if ((i / 18) % 2 == 0) canvas.setPixel(x, y, 4); }
                else canvas.setPixel(x, y, frontLayer);
            }
        };  // akhir lambda drawEllipse
        // Arsiran tipis di permukaan tutup. ex dan ey = jarak titik dari pusat tutup, dibagi setengah sumbu.
        // ex * ex + ey * ey < 0.9 berarti titik ada di dalam elips, sedikit ke dalam dari tepinya.
        // (x + y) % 8 == 0 hanya memilih titik pada garis-garis miring berjarak 8, supaya arsirannya tipis.
        // Arsiran memakai lapisan 1.
        for (int y = 0; y < canvas.pixelHeight(); ++y) {  // setiap baris titik kanvas
            for (int x = 0; x < canvas.pixelWidth(); ++x) {
                const double ex = (x + 0.5 - cx) / semiMajor, ey = (y + 0.5 - yTop) / semiMinor;
                if (ex * ex + ey * ey < 0.9 && (x + y) % 8 == 0) canvas.setPixel(x, y, 1);
            }
        }
        drawEllipse(yTop, 2, false);  // tutup: elips penuh, lapisan 2
        // xLeft dan xRight = posisi mendatar dinding kiri dan kanan (tepi kiri dan kanan elips).
        const int xLeft = static_cast<int>(cx - semiMajor), xRight = static_cast<int>(cx + semiMajor);
        // Dinding kiri dan kanan, dari tutup sampai alas: tebal 2 titik, lapisan 3.
        for (int y = static_cast<int>(yTop); y <= static_cast<int>(yBottom); ++y) {
            canvas.setPixel(xLeft, y, 3); canvas.setPixel(xLeft + 1, y, 3);
            canvas.setPixel(xRight, y, 3); canvas.setPixel(xRight + 1, y, 3);
        }
        drawEllipse(yBottom, 2, true);  // alas: depan utuh, belakang putus-putus
        // Warna tiap lapisan, indeksnya = nomor lapisan: 0 = kosong (tanpa warna), 1 = arsiran (ANSI_SURFACE),
        // 2 = elips tutup dan alas, 3 = dinding (keduanya ANSI_TITLE), 4 = tepi alas putus-putus (ANSI_DIM).
        const std::vector<const char*> layerColors = { "", ANSI_SURFACE, ANSI_TITLE, ANSI_TITLE, ANSI_DIM };
        std::vector<std::string> figureLines = canvas.toLines(layerColors);  // gambar sebagai baris-baris teks

        // Baris sel untuk tutup dan alas: posisi titik dibagi 4, dikarenakan tinggi satu sel 4 titik.
        const int rowTop = static_cast<int>(yTop) / 4, rowBottom = static_cast<int>(yBottom) / 4;
        // Kurung kurawal { ... } tanpa if/for membuat blok tersendiri. Variabel di dalamnya (misalnya row)
        // hanya berlaku di blok itu, jadi namanya boleh dipakai lagi di blok berikutnya.
        {   // garis jari-jari "●──R──►" di baris tengah tutup
            const int centerCol = canvasCols / 2;  // kolom tengah kanvas = pusat tutup
            std::string radiusLine = "\u25cf";  // \u25cf = ● (titik pusat)
            // Garis ─ dari kolom setelah pusat sampai sebelum ujung panah, dengan huruf R di tengahnya.
            for (int c = centerCol + 1; c < canvasCols - 2; ++c) radiusLine += (c == (centerCol + canvasCols - 2) / 2) ? "R" : "\u2500";
            radiusLine += "\u25ba";  // \u25ba = ► (ujung panah di tepi tutup)
            std::string row;
            // Baris tutup = sel kanvas di kiri pusat + garis jari-jari kuning + sel paling kanan.
            for (int c = 0; c < centerCol; ++c) row += canvas.cellString(c, rowTop, layerColors);
            figureLines[rowTop] = row + colorize(ANSI_HINT, radiusLine) + canvas.cellString(canvasCols - 1, rowTop, layerColors);
        }
        {   // tulisan TABUNG di tengah badan
            const int r = (rowTop + rowBottom) / 2;  // baris tengah antara tutup dan alas
            std::string row;
            for (int c = 0; c < canvasCols; ++c) {
                // Di kolom tengah - 3, sisipkan "TABUNG" (6 huruf), lalu lompati 5 kolom berikutnya.
                // Ditambah ++c dari for, totalnya 6 kolom, jadi lebar baris tetap sama.
                // Kolom lain diisi sel kanvas biasa.
                if (c == canvasCols / 2 - 3) { row += colorize(ANSI_TITLE, "TABUNG"); c += 5; }
                else row += canvas.cellString(c, r, layerColors);
            }
            figureLines[r] = row;  // ganti baris tengah gambar dengan baris bertulisan
        }
        // Panah tinggi T selalu di kanan gambar; keterangan di samping bila jendela lebar, di bawah bila sempit.
        // sideNotes = teks di kanan setiap baris gambar. (canvasRows, "") membuat canvasRows buah teks kosong.
        std::vector<std::string> sideNotes(canvasRows, "");
        // Baris tutup diberi "  ──▲", baris alas "  ──▼", dan baris di antaranya garis tegak "    │".
        // Ini ternary bersusun: baris tutup pilih ▲, baris alas pilih ▼, selain itu │.
        for (int r = rowTop; r <= rowBottom; ++r) {
            sideNotes[r] = (r == rowTop) ? "  \u2500\u2500\u25b2" : (r == rowBottom) ? "  \u2500\u2500\u25bc" : "    \u2502";
        }
        sideNotes[(rowTop + rowBottom) / 2] += "  T";  // huruf T di tengah panah
        if (legendOnRight) {  // jendela lebar: tiga keterangan pertama di samping panah
            sideNotes[rowTop + 1]              += "      " + legend[0];
            sideNotes[(rowTop + rowBottom) / 2]    += "   " + legend[1];
            sideNotes[rowBottom - 1]               += "      " + legend[2];
        }

        // Susun isi kotak: 3 spasi indentasi + baris gambar + catatan samping berwarna kuning.
        std::vector<std::string> lines;
        for (int r = 0; r < canvasRows; ++r) lines.push_back("   " + figureLines[r] + colorize(ANSI_HINT, sideNotes[r]));
        // jendela sempit: tiga keterangan pertama ditaruh di bawah gambar
        if (!legendOnRight) for (int i = 0; i < 3; ++i) lines.push_back("   " + legend[i]);
        lines.push_back("   " + colorize(ANSI_DIM, legend[3]));  // keterangan garis putus-putus, selalu di bawah
        drawBox("Sketsa Tabung", lines);  // semuanya digambar di dalam kotak berjudul (BAGIAN 3)
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Sebelum pertanyaan pertama, kita tunjukkan dulu bentuk jawaban yang diterima (nomor pilihan,
    // angka > 0, cara menulis desimal), supaya pengguna tidak salah ketik.
    // Daftar teks di dalam { ... } otomatis menjadi std::vector<std::string> untuk drawBox() (BAGIAN 3).
    // Di dalam teks, \" adalah tanda kutip yang ikut tercetak, dan \u03c0 tercetak sebagai π.
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
        std::cout << '\n';  // baris kosong setelah kotak
    }

    // Menyusun satu baris hasil "Label : nilai satuan" dengan 'decimals' angka desimal, siap masuk kotak.
    // int decimals = 2 adalah argumen bawaan: bila pemanggil tidak mengisinya, nilainya 2.
    std::string formatResultLine(std::string_view label, double value, std::string_view unit, int decimals = 2) {
        std::ostringstream line;  // teks disusun di sini, tanpa mengubah format std::cout
        // std::left = rata kiri. std::setw(22) memberi label lebar 22 kolom, supaya titik dua semua baris sejajar.
        // std::fixed dan std::setprecision(decimals) menulis angka dengan tepat 'decimals' angka di belakang titik.
        line << std::left << std::setw(22) << label << ": " << std::fixed << std::setprecision(decimals) << value;
        if (!unit.empty()) line << ' ' << unit;  // satuan ditambahkan hanya bila tidak kosong
        return line.str();  // str() mengambil teks yang sudah tersusun
    }

}  // akhir namespace tanpa nama (dibuka sebelum BAGIAN 2)

// ===== BAGIAN 6: PROGRAM UTAMA TUGAS 2 =====
// Program bermenu (berkas main.cpp) memanggil fungsi di bawah ini sebagai task2::run().
// Karena itu run() kita taruh di namespace bernama task2, supaya namanya khas dan tidak tertukar
// dengan run() milik tugas lain. task2::run dibaca "run milik task2".
namespace task2 {

    // Inilah alur lengkap Program 2: tampilan awal, tahap pengaturan, input, perhitungan, lalu output.
    // Fungsi ini sengaja tidak berada di namespace tanpa nama, dikarenakan main.cpp harus bisa memanggilnya.
    void run() {
        // Pindahkan konsol Windows ke UTF-8 (BAGIAN 3). Dipanggil di sini juga supaya program mandiri
        // (tanpa menu) tetap benar; memanggilnya berulang kali pun aman.
        enableUtf8Console();
        printTitle("PROGRAM 2 : VOLUME TABUNG");  // judul program (BAGIAN 5)
        // Cetak rumusnya. \u03c0 di dalam teks tercetak sebagai π, dan \n\n = pindah baris dua kali.
        std::cout << "Rumus: V = Pi x R x R x T   (Pi = \u03c0; modul menulisnya phi)\n\n";
        printSketch();  // sketsa, supaya pengguna tahu bagian mana yang dimaksud R, D, dan T
        printInstructions();  // kotak petunjuk pengisian (BAGIAN 5 lanjutan)

        // --- Tahap pengaturan: satuan, nilai π, dan ukuran yang diketahui ---
        // Cukup reference (&) ke salah satu isi UNITS (BAGIAN 2), tidak perlu disalin.
        const LengthUnit& unit = chooseUnit();
        std::string   piLabel;  // teks "3.14" atau "22/7 (...)" untuk ditampilkan di hasil
        const double  pi = choosePi(piLabel);  // π untuk perhitungan; choosePi() juga mengisi piLabel
        const bool    diameterGiven = chooseDiameterInput();  // true bila soal menyebut diameter

        // --- Tahap input ---
        // Ukuran disimpan sebagai double, dikarenakan jari-jari dan tinggi boleh desimal (misalnya 7.5).
        // readPositiveDecimal() (BAGIAN 4) menolak nol dan negatif, karena ukuran bangun tidak mungkin begitu.
        // Prompt menyertakan satuan, supaya pengguna tahu satuan yang dipakai.
        // label = awal prompt, dipilih dengan ternary sesuai ukuran yang diketahui.
        const std::string label = diameterGiven ? "Masukkan Diameter Alas (" : "Masukkan Jari-Jari Alas (";
        // prompt = label + simbol satuan + ") : ", misalnya "Masukkan Jari-Jari Alas (cm) : "
        const double inputSize = readPositiveDecimal(label + unit.symbol + ") : ");
        // std::string(...) diperlukan dikarenakan dua teks mentah (teks dalam tanda kutip dan unit.symbol
        // yang bertipe const char*) tidak bisa disambung langsung dengan +.
        const double height = readPositiveDecimal(std::string("Masukkan Tinggi Tabung (") + unit.symbol + ")  : ");
        // Kalau yang diketahui diameter, radius = inputSize / 2; kalau tidak, radius = inputSize.
        const double radius = diameterGiven ? inputSize / 2.0 : inputSize;

        // --- Tahap perhitungan ---
        // Dihitung dua langkah supaya rumusnya mudah dibaca: luas alas lingkaran = π x r x r,
        // lalu volume = luas alas x tinggi. Tanda * adalah perkalian.
        const double baseArea = pi * radius * radius;  // luas alas = π x r x r
        const double volume   = baseArea * height;  // volume = luas alas x tinggi
        // Padanan liter = volume x isi 1 satuan kubik dalam liter (litersPerCubicUnit, BAGIAN 2),
        // dikarenakan soal cerita sering meminta jawaban dalam liter.
        const double liters    = volume * unit.litersPerCubicUnit;

        // --- Tahap output: semua baris disusun dulu ke vector, lalu digambar dalam satu kotak ---
        const std::string areaUnit = std::string(unit.symbol) + "\u00b2";  // satuan luas, mis. cm² (\u00b2 = ²)
        const std::string volumeUnit = std::string(unit.symbol) + "\u00b3";  // satuan volume, mis. cm³ (\u00b3 = ³)
        // vector dikarenakan jumlah barisnya berbeda untuk jari-jari dan diameter; push_back = tambah di akhir
        std::vector<std::string> resultLines;
        resultLines.push_back("Pi (\u03c0) yang Digunakan : " + piLabel);
        if (diameterGiven) {  // diameter diketahui: tampilkan langkah r = d/2 seperti penyelesaian soal
            resultLines.push_back(formatResultLine("Diameter (D)", inputSize, unit.symbol));
            resultLines.push_back(formatResultLine("Jari-Jari (R) = D / 2", radius, unit.symbol));
        } else {  // jari-jari diketahui: cukup satu baris
            resultLines.push_back(formatResultLine("Jari-Jari (R)", radius, unit.symbol));
        }
        resultLines.push_back(formatResultLine("Tinggi (T)", height, unit.symbol));
        resultLines.push_back(formatResultLine("Luas Alas", baseArea, areaUnit));
        // hasil akhir ditonjolkan hijau tebal (ANSI_SUCCESS)
        resultLines.push_back(colorize(ANSI_SUCCESS, formatResultLine("Volume Tabung", volume, volumeUnit)));
        // padanan liter ditulis dengan 3 angka desimal (argumen terakhir = decimals)
        resultLines.push_back(formatResultLine("Setara", liters, "liter (1 liter = 1 dm\u00b3)", 3));
        std::cout << '\n';  // baris kosong sebelum kotak hasil
        drawBox("Hasil Perhitungan", resultLines);  // semua baris hasil dicetak di dalam kotak (BAGIAN 3)
    }  // akhir run()

}  // akhir namespace task2

// ===== BAGIAN 7: TITIK AWAL BILA BERKAS INI DIKOMPILASI SENDIRI =====
// C++ hanya mengizinkan satu main(), padahal program bermenu sudah punya main() sendiri.
// Jadi main() di bawah hanya kita sertakan saat berkas ini dikompilasi sendiri.
// #ifndef BUILD_WITH_MENU artinya "bila makro BUILD_WITH_MENU belum ada". Makro itu baru ada
// bila perintah kompilasi memakai -DBUILD_WITH_MENU.
#ifndef BUILD_WITH_MENU
// main() adalah titik awal: fungsi pertama yang dijalankan saat program dibuka.
// int berarti main() mengembalikan kode angka ke sistem operasi.
int main() {
    // Cetak identitas (tebal), lalu jalankan Program 2 lewat task2::run() (BAGIAN 6).
    std::cout << colorize(ANSI_BOLD, "Tugas Praktikum 2 - Fadhlillah (2531070011)") << '\n';
    task2::run();
    return 0;  // 0 menandakan program selesai tanpa error
}
#endif  // akhir bagian yang hanya ada bila berkas dikompilasi sendiri (tanpa BUILD_WITH_MENU)
