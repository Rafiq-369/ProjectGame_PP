#include <ncurses/curses.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

// ===============================
// PETA DASAR GAME
// '#' = tembok
// 'P' = pemain
// 'A', 'B' = kotak
// 'X', 'Y' = tujuan
// ===============================
vector<string> petaDasar = {
    "####################",
    "#     #          X #",
    "# ### # ####### ####",
    "# #  A             #",
    "# #   #######   ####",
    "#     B            #",
    "# #######  ### ### #",
    "#          # Y   P #",
    "####################"
};

// ===============================
// STRUKTUR POSISI (BARIS & KOLOM)
// ===============================
struct Posisi {
    int r, c;
};

// ===============================
// HELPER: CEK APAKAH POSISI TEMBOK
// ===============================
bool tembok(const vector<string>& peta, int r, int c) {
    // Di luar peta dianggap tembok
    if (r < 0 || r >= (int)peta.size()) return true;
    if (c < 0 || c >= (int)peta[0].size()) return true;
    return peta[r][c] == '#';
}

// ===============================
// HELPER: CARI KOTAK DI POSISI (r,c)
// Mengembalikan "A", "B", atau "" jika kosong
// ===============================
string cariKotak(map<string, Posisi>& kotak, int r, int c) {
    for (auto& k : kotak)
        if (k.second.r == r && k.second.c == c)
            return k.first;
    return "";
}

// ===============================
// MEMPERBARUI PETA UNTUK DITAMPILKAN
// (menggabungkan peta dasar + pemain + kotak)
// ===============================
vector<string> perbaruiPeta(
    const vector<string>& petaAsli,
    Posisi pemain,
    map<string, Posisi>& kotak
) {
    vector<string> peta = petaAsli;

// Hapus posisi lama pemain dan kotak
    for (int r = 0; r < peta.size(); r++)
        for (int c = 0; c < peta[r].size(); c++)
            if (peta[r][c] == 'P' || peta[r][c] == 'A' || peta[r][c] == 'B')
                peta[r][c] = ' ';

    // Gambar ulang kotak
    for (auto& k : kotak)
        peta[k.second.r][k.second.c] = k.first[0];

// Gambar ulang pemain
    peta[pemain.r][pemain.c] = 'P';
    return peta;
}

// ===============================
// STATUS GAME
// Digunakan untuk fitur UNDO
// ===============================
struct StatusGame {
    Posisi pemain;
    map<string, Posisi> kotak;
};
int main() {

    // ===============================
    // INISIALISASI NCURSES
    // ===============================
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    Posisi pemain;
    map<string, Posisi> kotak;

    // ===============================
    // DETEKSI POSISI AWAL PEMAIN & KOTAK
    // ===============================
    for (int r = 0; r < petaDasar.size(); r++) {
        for (int c = 0; c < petaDasar[r].size(); c++) {
            if (petaDasar[r][c] == 'P') pemain = {r, c};
            if (petaDasar[r][c] == 'A') kotak["A"] = {r, c};
            if (petaDasar[r][c] == 'B') kotak["B"] = {r, c};
        }
    }
    // ===============================
    // RIWAYAT GERAKAN (UNDO)
    // ===============================
    vector<StatusGame> riwayat;

    // ===============================
    // GAME LOOP UTAMA
    // ===============================
    while (true) {
        clear();

        auto petaTampil = perbaruiPeta(petaDasar, pemain, kotak);
        for (auto& baris : petaTampil)
            printw("%s\n", baris.c_str());

        printw("\nW A S D = bergerak | U = undo | Q = keluar\n");

        int input = getch();

        // ===============================
        // KELUAR GAME
        // ===============================
        if (input == 'q' || input == 'Q') break;

        // ===============================
        // UNDO GERAKAN
        // ===============================
        if (input == 'u' || input == 'U') {
            if (!riwayat.empty()) {
                StatusGame sebelumnya = riwayat.back();
                riwayat.pop_back();
                pemain = sebelumnya.pemain;
                kotak = sebelumnya.kotak;
            }
            continue;
        }

        // ===============================
        // INPUT ARAH GERAK
        // ===============================
        int dr = 0, dc = 0;
        if (input == 'w') dr = -1;
        if (input == 's') dr = 1;
        if (input == 'a') dc = -1;
        if (input == 'd') dc = 1;

        if (dr == 0 && dc == 0) continue;

        int nr = pemain.r + dr;
        int nc = pemain.c + dc;
