@echo off
chcp 65001 >nul
rem chcp 65001 berfungsi mengaktifkan UTF-8 agar simbol pi, cm2, cm3 tampil benar di Command Prompt
rem jalankan.bat - peluncur untuk Windows: klik dua kali, jendela Command Prompt terbuka, program langsung jalan.
rem Skrip ini mengompilasi ulang (butuh g++ dari MSYS2 yang sudah ada di PATH), lalu menjalankan program.

cd /d "%~dp0"

where g++ >nul 2>nul
if errorlevel 1 (
    echo Compiler g++ tidak ditemukan. Pasang MSYS2 lalu tambahkan C:\msys64\ucrt64\bin ke PATH.
    pause
    exit /b 1
)

echo Mengompilasi program...
g++ -std=c++17 -Wall -Wextra -DBUILD_WITH_MENU -o tugas_alpro.exe src\main.cpp src\tugas1_nilai_mahasiswa.cpp src\tugas2_volume_tabung.cpp src\tugas3_luas_arsir.cpp
if errorlevel 1 (
    echo Kompilasi gagal. Periksa pesan error di atas.
    pause
    exit /b 1
)

tugas_alpro.exe
echo.
echo Program selesai.
pause
