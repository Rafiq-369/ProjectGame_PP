#include <ncurses/curses.h>   // Library UI terminal
#include <vector>             // Untuk vector
#include <string>             // Untuk string
#include <map>                // Untuk menyimpan box dengan nama

using namespace std;

// ===============================
// MAP DASAR GAME
// '#' = tembok
// 'P' = player
// 'A', 'B' = box
// 'X', 'Y' = goal
// ===============================
vector<string> baseMap = {
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
struct Pos {
    int r, c;
};

// ===============================
// HELPER: CEK APAKAH TILE ADALAH TEMBOK
// ===============================
bool isWall(const vector<string>& m, int r, int c) {
    // Keluar map dianggap tembok
    if (r < 0 || r >= (int)m.size()) return true;
    if (c < 0 || c >= (int)m[0].size()) return true;

    return m[r][c] == '#';
}

// ===============================
// HELPER: CEK APAKAH ADA BOX DI POSISI (r,c)
// Mengembalikan nama box ("A"/"B") atau "" jika kosong
// ===============================
string findBoxAt(map<string, Pos>& boxes, int r, int c) {
    for (auto& b : boxes)
        if (b.second.r == r && b.second.c == c)
            return b.first;
    return "";
}

// ===============================
// UPDATE MAP UNTUK DITAMPILKAN
// (Menggabungkan map dasar + posisi player & box)
// ===============================
vector<string> updateMap(
    const vector<string>& original,
    Pos player,
    map<string, Pos>& boxes
) {
    vector<string> m = original;


 // Bersihkan posisi lama player & box
    for (int r = 0; r < m.size(); r++)
        for (int c = 0; c < m[r].size(); c++)
            if (m[r][c] == 'P' || m[r][c] == 'A' || m[r][c] == 'B')
                m[r][c] = ' ';

    // Gambar ulang box
    for (auto& b : boxes)
        m[b.second.r][b.second.c] = b.first[0];

 // Gambar ulang player
    m[player.r][player.c] = 'P';
    return m;
}

