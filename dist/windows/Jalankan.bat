@echo off
rem Peluncur paket rilis Windows: klik dua kali untuk menjalankan program yang sudah dikompilasi.
rem Program tidak ditandatangani Microsoft, sehingga berkas yang diunduh membawa tanda "Mark of the Web" dan
rem SmartScreen menolaknya. Baris Unblock-File di bawah melepas tanda itu hanya dari tugas_alpro.exe, atas
rem persetujuan Anda saat membuka peluncur ini (SmartScreen: "More info" -> "Run anyway", cukup sekali).
chcp 65001 >nul
cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -Command "Unblock-File -LiteralPath '%~dp0tugas_alpro.exe'" >nul 2>&1
tugas_alpro.exe
echo.
pause
