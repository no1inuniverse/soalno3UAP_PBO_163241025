#ifndef CONFIG_PARKING_H
#define CONFIG_PARKING_H

#include <string>
#include <vector>

// ============================================
// KONFIGURASI TARIF PARKIR
// ============================================

namespace ParkingRate {
    // Tarif dasar per jam (dalam Rupiah)
    const double MOTORCYCLE_RATE_PER_HOUR = 2000.0;
    const double CAR_RATE_PER_HOUR = 5000.0;
    const double EV_RATE_PER_HOUR = 4000.0;
    
    // Biaya tambahan
    const double EV_CHARGING_FEE = 3000.0;
    const double VIP_SURCHARGE_PERCENTAGE = 50.0; // 50% tambahan dari tarif mobil
    
    // Diskon
    const double LONG_TERM_DISCOUNT_PERCENTAGE = 10.0; // 10% diskon
    const int LONG_TERM_DISCOUNT_HOURS = 24; // Minimal jam untuk diskon
    
    // Durasi minimum parkir (dalam jam)
    const int MINIMUM_PARKING_HOURS = 1;
    
    // Faktor konversi detik ke jam
    const int SECONDS_PER_HOUR = 3600;
}

// ============================================
// KONFIGURASI LAYOUT PARKIR
// ============================================

namespace ParkingLayout {
    // Struktur untuk definisi slot parkir
    struct SlotDefinition {
        std::string id;
        std::string type;  // "REGULAR", "EV", "VIP"
        int level;
        bool isActive;
    };
    
    // Default slot configuration
    const std::vector<SlotDefinition> DEFAULT_SLOTS = {
        // Lantai 1 - Regular Slots
        {"R1", "REGULAR", 1, true},
        {"R2", "REGULAR", 1, true},
        {"R3", "REGULAR", 1, true},
        
        // Lantai 1 - EV Slots
        {"E1", "EV", 1, true},
        {"E2", "EV", 1, true},
        
        // Lantai 2 - VIP Slots
        {"V1", "VIP", 2, true},
        {"V2", "VIP", 2, true},
        {"V3", "VIP", 2, true},
        
        // Lantai 2 - Regular Slots (tambahan)
        {"R4", "REGULAR", 2, true},
        {"R5", "REGULAR", 2, true}
    };
    
    // Kapasitas per lantai
    const int FLOOR_1_CAPACITY = 5;
    const int FLOOR_2_CAPACITY = 5;
    const int TOTAL_CAPACITY = FLOOR_1_CAPACITY + FLOOR_2_CAPACITY;
    
    // Kapasitas per tipe slot
    const int REGULAR_SLOT_COUNT = 5;
    const int EV_SLOT_COUNT = 2;
    const int VIP_SLOT_COUNT = 3;
    
    // Konfigurasi reservasi VIP
    const int MAX_VIP_RESERVATION_HOURS = 72; // Maksimal 3 hari
    const int MIN_VIP_RESERVATION_HOURS = 1;  // Minimal 1 jam
}

// ============================================
// KONFIGURASI SISTEM
// ============================================

namespace SystemConfig {
    // ID awal untuk tiket
    const int INITIAL_TICKET_ID = 1000;
    
    // Format tanggal/waktu
    const std::string TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
    const std::string DATE_FORMAT = "%Y-%m-%d";
    
    // Mode operasi
    const bool ENABLE_VIP_RESERVATION = true;
    const bool ENABLE_EV_CHARGING = true;
    const bool ENABLE_LONG_TERM_DISCOUNT = true;
    
    // Waktu operasional
    const int OPENING_HOUR = 6;   // Buka jam 6 pagi
    const int CLOSING_HOUR = 22;  // Tutup jam 10 malam
    
    // Interval laporan
    const int REPORT_GENERATION_INTERVAL_HOURS = 24;
}

// ============================================
// KONFIGURASI PESAN/SISTEM
// ============================================

namespace Messages {
    // Pesan error
    const std::string ERROR_NO_SLOT = "Tidak ada slot tersedia untuk tipe kendaraan ini";
    const std::string ERROR_INVALID_TICKET = "ID tiket tidak valid atau sudah dibayar";
    const std::string ERROR_RESERVATION_FAILED = "Gagal melakukan reservasi slot VIP";
    const std::string ERROR_PAYMENT = "Terjadi kesalahan dalam proses pembayaran";
    const std::string ERROR_SLOT_OCCUPIED = "Slot sudah terisi";
    
    // Pesan sukses
    const std::string SUCCESS_PARKED = "Kendaraan berhasil diparkir";
    const std::string SUCCESS_UNPARKED = "Kendaraan berhasil dikeluarkan";
    const std::string SUCCESS_RESERVED = "Slot VIP berhasil direservasi";
    
    // Pesan informasi
    const std::string INFO_TICKET_ID = "ID Tiket Anda: ";
    const std::string INFO_PARKING_FEE = "Biaya parkir: Rp ";
    const std::string INFO_RESERVATION_EXPIRY = "Reservasi berlaku hingga: ";
    
    // Label menu
    const std::string MENU_TITLE = "=== SISTEM MANAJEMEN PARKIR ===";
    const std::string MENU_OPTION_1 = "1. Daftar dan parkir kendaraan";
    const std::string MENU_OPTION_2 = "2. Keluarkan kendaraan";
    const std::string MENU_OPTION_3 = "3. Reservasi slot VIP";
    const std::string MENU_OPTION_4 = "4. Tampilkan status parkir";
    const std::string MENU_OPTION_5 = "5. Generate laporan harian";
    const std::string MENU_OPTION_6 = "6. Simpan data uji";
    const std::string MENU_OPTION_0 = "0. Keluar";
    const std::string MENU_PROMPT = "Pilihan: ";
    
    // Label laporan
    const std::string REPORT_TITLE = "=== LAPORAN HARIAN PARKIR ===";
    const std::string REPORT_TOTAL_VEHICLES = "Total kendaraan: ";
    const std::string REPORT_MOTORCYCLES = "Motor: ";
    const std::string REPORT_CARS = "Mobil: ";
    const std::string REPORT_EVS = "Kendaraan Listrik: ";
    const std::string REPORT_TOTAL_REVENUE = "Total pendapatan: Rp ";
    const std::string REPORT_UTILIZATION = "Utilisasi per Lantai:";
    
    // Label kendaraan
    const std::string VEHICLE_TYPE_MOTORCYCLE = "Motor";
    const std::string VEHICLE_TYPE_CAR = "Mobil";
    const std::string VEHICLE_TYPE_EV = "Kendaraan Listrik (EV)";
    
    // Label slot
    const std::string SLOT_TYPE_REGULAR = "Regular";
    const std::string SLOT_TYPE_EV = "EV";
    const std::string SLOT_TYPE_VIP = "VIP";
}

#endif // CONFIG_PARKING_H
