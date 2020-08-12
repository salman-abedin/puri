#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

WINDOW* win;
int mark, height, width;
int count;
char** items;

void cleanup() {
   for (int i = 0; i < count; ++i) free(items[i]);
   free(items);
   endwin();
}

void drawitems() {
   for (int i = 0; i < count; ++i) {
      if (i == mark) wattron(win, A_REVERSE);
      mvwprintw(win, i + 1, 1, items[i]);
      wattroff(win, A_REVERSE);
   }
}

void handleinput() {
   int key;
   char* cmd;
   char cmdhead[] = "$BROWSER";
   while ((key = wgetch(win)) != 'h') {
      if (key == 'j' && mark < count - 1) {
         ++mark;
         drawitems();
      } else if (key == 'k' && mark > 0) {
         --mark;
         drawitems();
      } else if (key == 'l') {
         cmd = calloc(strlen(cmdhead) + strlen(items[mark]) + 2, sizeof(char));
         sprintf(cmd, "%s %s", cmdhead, items[mark]);
         system(cmd);
      }
   }
}

void drawui() {
   mvprintw(0, width / 2 - 10, HEADER);
   mvprintw(height - 1, width / 2 - 15, FOOTER);
   win = newwin(height - 2, width, 1, 0);
   box(win, 0, 0);
   refresh();
}

void init(char** in_items, int in_count) {
   items = in_items;
   count = in_count;
   initscr();
   cbreak();
   noecho();
   curs_set(0);
   getmaxyx(stdscr, height, width);
}
