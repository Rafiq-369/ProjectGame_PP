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

