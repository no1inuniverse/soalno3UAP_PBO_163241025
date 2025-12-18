# soalno3UAP_PBO_163241025
**Ujian Akhir Praktikum Soal no 3:** Simulasi Sistem Manajemen Parkir Multilapis (Advanced Parking Management System)
**Mata Kuliah:** SIR201 – Pemrograman Berorientasi Objek (PBO)
**Program Studi:** Teknik Robotika & Kecerdasan Buatan – Universitas Airlangga

---
## Deskripsi Program
Program **Parking Management System** adalah aplikasi berbasis terminal yang dibuat
menggunakan bahasa **C++** dengan konsep **Object-Oriented Programming (OOP)**,
**inheritance**, **polymorphism**, **exception handling**, dan **smart pointer**.

Program ini mensimulasikan sistem manajemen parkir yang mendukung:
- Kendaraan **Motor**, **Mobil**, dan **Electric Vehicle (EV)**
- Slot parkir **Regular**, **EV**, dan **VIP**
- Reservasi slot VIP
- Perhitungan tarif otomatis
- Laporan harian dan pendapatan

--- 

## Repository Project 
https://github.com/no1inuniverse/soalno3UAP_PBO_163241025

---

## Cara Compile dan Menjalankan Program

Pastikan compiler **g++** sudah terinstall.

Cek dengan:
```bash
g++ --version

g++ main.cpp -o parking_system
parking_system.exe

---

## Asumsi Program
- Waktu parkir dihitung dalam satuan jam.
- Jika durasi parkir kurang dari 1 jam, tetap dihitung 1 jam.
- Input pengguna dianggap valid (tidak ada validasi input non-numerik).
- Slot parkir telah ditentukan di dalam kode program (hardcoded).
- Setiap kendaraan mendapatkan ticket unik secara otomatis.
- Pembayaran dilakukan saat kendaraan keluar (unpark).
- Diskon 10% diberikan jika durasi parkir lebih dari 24 jam.
- Slot VIP dikenakan biaya tambahan (surcharge).

## Konfigurasi Tarif Parkir
- Motor (Motorcycle)
double rate = durationHours * 2000;

- Mobil (Car)
double rate = durationHours * 5000;

- Electric Vehicle (EV)
double rate = durationHours * 4000;
if (chargingRequired) {
    rate += 3000;
}

- VIP Slot
double vipRate = baseRate * 1.5;
