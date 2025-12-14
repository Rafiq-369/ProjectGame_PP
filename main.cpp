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


