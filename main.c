#include <stdio.h>      // Library standar untuk input-output
#include <stdlib.h>     // Library standar untuk fungsi umum (seperti malloc, exit)
#include <string.h>     // Library untuk operasi string (seperti strcpy, strlen)

#define MAKS_BARANG 100      // Jumlah maksimal barang yang bisa dibeli
#define BATAS_DISKON 3       // Batas minimal pembelian untuk mendapatkan diskon
#define PERSEN_DISKON 0.10   // Besar diskon yang diberikan (10%)

// Struktur data untuk daftar barang yang tersedia di toko
typedef struct {
    char nama[50];   // Nama barang
    float harga;     // Harga satuan barang
} BarangTersedia;

// Struktur data untuk barang yang dibeli pelanggan
typedef struct {
    char nama[50];    // Nama barang yang dibeli
    int jumlah;       // Jumlah unit barang yang dibeli
    float harga;      // Harga satuan barang
    float diskon;     // Jumlah potongan diskon yang didapatkan
    float total;      // Harga total setelah diskon
} BarangDibeli;

// Inisialisasi daftar barang yang tersedia di toko
BarangTersedia daftarBarang[] = {
    {"Sabun", 5000},
    {"Shampoo", 15000},
    {"Pasta Gigi", 10000},
    {"Minyak Goreng", 25000},
    {"Gula", 12000},
    {"Beras", 50000}
};
int jumlahBarangTersedia = sizeof(daftarBarang) / sizeof(daftarBarang[0]); // Hitung banyaknya item di daftarBarang

// Fungsi untuk mengurutkan barang dibeli berdasarkan harga total dari paling mahal
void urutkanBarang(BarangDibeli barang[], int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {                // Loop luar
        for (int j = 0; j < jumlah - i - 1; j++) {         // Loop dalam untuk bubble sort
            if (barang[j].total < barang[j + 1].total) {   // Jika total harga barang ke-j lebih kecil dari ke-(j+1)
                BarangDibeli temp = barang[j];             // Tukar posisi barang[j] dan barang[j+1]
                barang[j] = barang[j + 1];
                barang[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk menyimpan struk transaksi ke dalam file teks
void simpanKeFile(const char *namaFile, BarangDibeli barang[], int jumlah, const char *namaPelanggan) {
    FILE *file = fopen(namaFile, "w"); // Buka file untuk ditulis (mode "w")
    if (file == NULL) { // Jika gagal membuka file
        printf("Gagal menyimpan laporan!\n");
        return;
    }

    fprintf(file, "Struk Pembelian - %s\n", namaPelanggan); // Tulis header struk
    fprintf(file, "----------------------------------\n");

    float totalKeseluruhan = 0; // Variabel untuk total seluruh pembayaran
    for (int i = 0; i < jumlah; i++) { // Tulis tiap barang yang dibeli ke file
        fprintf(file, "%s - %d x Rp%.2f = Rp%.2f (Diskon: Rp%.2f)\n",
                barang[i].nama, barang[i].jumlah, barang[i].harga, barang[i].total, barang[i].diskon);
        totalKeseluruhan += barang[i].total; // Akumulasi total keseluruhan
    }

    fprintf(file, "----------------------------------\n");
    fprintf(file, "Total Bayar: Rp%.2f\n", totalKeseluruhan); // Tulis total pembayaran

    fclose(file); // Tutup file setelah selesai
    printf("Struk berhasil disimpan ke %s\n", namaFile); // Informasikan ke user
}

int main() {
    BarangDibeli daftarBelanja[MAKS_BARANG]; // Array untuk daftar barang yang dibeli
    int jumlahBarangDibeli = 0; // Counter jumlah barang yang dibeli
    char namaPelanggan[50]; // Variabel untuk menyimpan nama pelanggan

    printf("Masukkan Nama Pelanggan: ");
    fgets(namaPelanggan, sizeof(namaPelanggan), stdin); // Membaca input nama pelanggan
    namaPelanggan[strcspn(namaPelanggan, "\n")] = 0; // Hapus karakter newline ('\n') dari input

    char pilihan; // Variabel untuk menyimpan pilihan user (lanjut belanja atau tidak)
    do {
        // Tampilkan daftar barang
        printf("\nDaftar Barang yang Tersedia:\n");
        for (int i = 0; i < jumlahBarangTersedia; i++) {
            printf("%d. %s - Rp%.2f\n", i + 1, daftarBarang[i].nama, daftarBarang[i].harga);
        }

        // Pilih barang yang akan dibeli
        int pilihanBarang;
        printf("Pilih barang (nomor): ");
        scanf("%d", &pilihanBarang);
        getchar(); // Bersihkan buffer input dari karakter newline '\n'

        // Validasi pilihan barang
        if (pilihanBarang < 1 || pilihanBarang > jumlahBarangTersedia) {
            printf("Pilihan tidak valid.\n");
            continue; // Kembali ke awal loop jika pilihan salah
        }

        // Salin nama dan harga barang yang dipilih ke daftarBelanja
        strcpy(daftarBelanja[jumlahBarangDibeli].nama, daftarBarang[pilihanBarang - 1].nama);
        daftarBelanja[jumlahBarangDibeli].harga = daftarBarang[pilihanBarang - 1].harga;

        // Minta jumlah barang yang ingin dibeli
        printf("Masukkan jumlah barang: ");
        scanf("%d", &daftarBelanja[jumlahBarangDibeli].jumlah);
        getchar(); // Bersihkan buffer

        // Hitung total sebelum diskon
        daftarBelanja[jumlahBarangDibeli].diskon = 0; // Awal, belum ada diskon
        daftarBelanja[jumlahBarangDibeli].total = daftarBelanja[jumlahBarangDibeli].jumlah * daftarBelanja[jumlahBarangDibeli].harga;

        // Cek apakah berhak mendapat diskon
        if (daftarBelanja[jumlahBarangDibeli].jumlah >= BATAS_DISKON) {
            daftarBelanja[jumlahBarangDibeli].diskon = daftarBelanja[jumlahBarangDibeli].total * PERSEN_DISKON; // Hitung diskon
            daftarBelanja[jumlahBarangDibeli].total -= daftarBelanja[jumlahBarangDibeli].diskon; // Kurangi total dengan diskon
        }

        jumlahBarangDibeli++; // Tambah jumlah barang yang dibeli

        // Tanya user apakah ingin belanja lagi
        printf("Tambah barang lagi? (y/n): ");
        scanf(" %c", &pilihan); // Spasi sebelum %c untuk menghindari masalah input buffer
    } while (pilihan == 'y' && jumlahBarangDibeli < MAKS_BARANG); // Ulangi jika pilih 'y' dan belum melebihi kapasitas

    // Urutkan daftar belanja berdasarkan harga total dari mahal ke murah
    urutkanBarang(daftarBelanja, jumlahBarangDibeli);

    // Cetak struk pembelian ke layar
    printf("\nStruk Pembelian:\n");
    printf("----------------------------------\n");

    float totalKeseluruhan = 0; // Total semua barang
    for (int i = 0; i < jumlahBarangDibeli; i++) {
        printf("%s - %d x Rp%.2f = Rp%.2f (Diskon: Rp%.2f)\n",
               daftarBelanja[i].nama, daftarBelanja[i].jumlah, daftarBelanja[i].harga, daftarBelanja[i].total, daftarBelanja[i].diskon);
        totalKeseluruhan += daftarBelanja[i].total; // Tambahkan harga barang ke total keseluruhan
    }

    printf("----------------------------------\n");
    printf("Total Bayar: Rp%.2f\n", totalKeseluruhan);

    // Simpan struk pembelian ke file "struk.txt"
    simpanKeFile("struk.txt", daftarBelanja, jumlahBarangDibeli, namaPelanggan);

    return 0; // Program selesai
}
