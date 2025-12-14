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

// ===============================
// STRUKTUR GAME STATE
// Digunakan untuk fitur UNDO
// ===============================
struct GameState {
    Pos player;
    map<string, Pos> boxes;
};

int main() {
    // ===============================
    // INISIALISASI NCURSES
    // ===============================
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    Pos player;
    map<string, Pos> boxes;

    // ===============================
    // DETEKSI POSISI AWAL PLAYER & BOX
    // ===============================
    for (int r = 0; r < baseMap.size(); r++) {
        for (int c = 0; c < baseMap[r].size(); c++) {
            if (baseMap[r][c] == 'P') player = {r, c};
            if (baseMap[r][c] == 'A') boxes["A"] = {r, c};
            if (baseMap[r][c] == 'B') boxes["B"] = {r, c};
        }
    }

    // ===============================
    // HISTORY UNTUK FITUR UNDO
    // ===============================
    vector<GameState> history;

    // ===============================
    // GAME LOOP UTAMA
    // ===============================
    while (true) {
        clear();

        // Tampilkan map
        auto displayMap = updateMap(baseMap, player, boxes);
        for (auto& row : displayMap)
            printw("%s\n", row.c_str());

        printw("\nWASD = bergerak | U = undo | Q = keluar\n");

        int ch = getch();

        // ===============================
        // KELUAR GAME
        // ===============================
        if (ch == 'q' || ch == 'Q') break;

        // ===============================
        // UNDO GERAKAN
        // ===============================
        if (ch == 'u' || ch == 'U') {
            if (!history.empty()) {
                GameState prev = history.back();
                history.pop_back();
                player = prev.player;
                boxes = prev.boxes;
            }
            continue;
        }

        // ===============================
        // INPUT GERAKAN PLAYER
        // ===============================
        int dr = 0, dc = 0;
        if (ch == 'w') dr = -1;
        if (ch == 's') dr = 1;
        if (ch == 'a') dc = -1;
        if (ch == 'd') dc = 1;

        if (dr == 0 && dc == 0) continue;

        int nr = player.r + dr;
        int nc = player.c + dc;

        // Tabrak tembok
        if (isWall(baseMap, nr, nc)) continue;

        string boxName = findBoxAt(boxes, nr, nc);

        // Simpan state sebelum bergerak (UNDO)
        history.push_back({player, boxes});

        // ===============================
        // JIKA MENABRAK BOX
        // ===============================
        if (boxName != "") {

            // Box terkunci jika sudah di goal benar
            bool A_locked = (baseMap[boxes["A"].r][boxes["A"].c] == 'X');
            bool B_locked = (baseMap[boxes["B"].r][boxes["B"].c] == 'Y');

            if (boxName == "A" && A_locked) { history.pop_back(); continue; }
            if (boxName == "B" && B_locked) { history.pop_back(); continue; }

            int br = boxes[boxName].r + dr;
            int bc = boxes[boxName].c + dc;

            // Validasi dorongan box
            if (isWall(baseMap, br, bc)) { history.pop_back(); continue; }
            if (findBoxAt(boxes, br, bc) != "") { history.pop_back(); continue; }

            // Box hanya boleh ke goal yang benar
            char targetTile = baseMap[br][bc];
            if (boxName == "A" && targetTile == 'Y') { history.pop_back(); continue; }
            if (boxName == "B" && targetTile == 'X') { history.pop_back(); continue; }

            // Geser box & player
            boxes[boxName] = {br, bc};
            player = {nr, nc};
        }
        else {
            // Gerak biasa
            player = {nr, nc};
        }

        // ===============================
        // CEK KONDISI MENANG
        // ===============================
        bool A_correct = (baseMap[boxes["A"].r][boxes["A"].c] == 'X');
        bool B_correct = (baseMap[boxes["B"].r][boxes["B"].c] == 'Y');

        if (A_correct && B_correct) {
            clear();
            auto dm = updateMap(baseMap, player, boxes);
            for (auto& row : dm)
                printw("%s\n", row.c_str());

            printw("\nSELAMAT! Semua box sudah di goal yang benar!\n");
            printw("Tekan tombol apa saja untuk keluar...\n");
            getch();
            break;
        }
    }

    endwin();
    return 0;
}
