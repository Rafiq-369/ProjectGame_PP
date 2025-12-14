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
