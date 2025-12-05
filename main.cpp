#include <ncurses/curses.h>
#include <vector>

vector<string> baseMap = {
    "####################",
    "#     #       X    #",
    "# ### # ## #### ####",
    "#   A      #       #",
    "# #   #### ##  ######",
    "# #        #        #",
    "# ## ###   #  ####  #",
    "#     B    #     P  #",
    "####################"
};

int main() {
  initscr();
  noecho();

   while (true) {
        clear();

        auto displayMap = updateMap(baseMap, player, boxes);

        for (auto& row : displayMap)
            printw("%s\n", row.c_str());

        printw("\nGerak: W A S D | Q untuk keluar\n");

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        int dr = 0, dc = 0;
        if (ch == 'w') dr = -1;
        if (ch == 's') dr = 1;
        if (ch == 'a') dc = -1;
        if (ch == 'd') dc = 1;

        int nr = player.r + dr;
        int nc = player.c + dc;
     
  endwin();
return 0;
}
