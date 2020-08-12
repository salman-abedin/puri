#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int mark, height, width, wwidth, count;
char** items;
WINDOW* win;

void init(int in_count, char** in_items) {
   items = in_items;
   count = in_count;
   initscr();
   cbreak();
   noecho();
   curs_set(0);
   getmaxyx(stdscr, height, width);
   wwidth = width / 1.5;
}

void drawui() {
   mvprintw(0, (width - strlen(HEADER)) / 2, HEADER);
   mvprintw(height - 1, (width - strlen(FOOTER)) / 2, FOOTER);
   win = newwin(height - 2, wwidth, 1, (width - wwidth) / 2);
   box(win, 0, 0);
   refresh();
}

void drawitems() {
   for (int i = 0; i < count; ++i) {
      if (i == mark) wattron(win, A_REVERSE);
      mvwaddnstr(win, i + 1, 1, items[i], wwidth - 2);
      wattroff(win, A_REVERSE);
   }
}

void handleinput() {
   int key;
   char* cmd;
   char* cmdhead = "$BROWSER";
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

void cleanup() {
   for (int i = 0; i < count; ++i) free(items[i]);
   free(items);
   endwin();
}
