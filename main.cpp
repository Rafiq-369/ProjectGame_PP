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
