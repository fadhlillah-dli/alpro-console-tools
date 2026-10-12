# Makefile - build lewat terminal: ketik  make  lalu  ./tugas_alpro  (program bermenu)
# Build per tugas (berkas tugas lengkap, bisa berdiri sendiri):  make tugas1  /  make tugas2  /  make tugas3
# Windows dengan MSYS2: mingw32-make; hasilnya berakhiran .exe

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: tugas_alpro

tugas_alpro: src/main.cpp src/tugas1_nilai_mahasiswa.cpp src/tugas2_volume_tabung.cpp src/tugas3_luas_arsir.cpp
	$(CXX) $(CXXFLAGS) -DBUILD_WITH_MENU -o $@ $^

tugas1: src/tugas1_nilai_mahasiswa.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

tugas2: src/tugas2_volume_tabung.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

tugas3: src/tugas3_luas_arsir.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: tugas_alpro
	./tugas_alpro

clean:
	rm -f tugas_alpro tugas1 tugas2 tugas3 tugas_alpro.exe tugas1.exe tugas2.exe tugas3.exe

.PHONY: all run clean
