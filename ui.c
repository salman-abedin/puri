#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int mark, start, end, width, height, wwidth, wheight, count, i, j;
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
   wheight = height - 2;
   end = count > wheight - 2 ? wheight - 2 : count;
}

void drawui() {
   mvprintw(0, (width - strlen(HEADER)) / 2, HEADER);
   mvprintw(height - 1, (width - strlen(FOOTER)) / 2, FOOTER);
   win = newwin(wheight, wwidth, 1, (width - wwidth) / 2);
}

void drawitems() {
   werase(win);
   box(win, 0, 0);
   for (i = 0, j = start; j < end; ++i, ++j) {
      if (i == mark) wattron(win, A_REVERSE);
      mvwaddnstr(win, i + 1, 1, items[j], wwidth - 2);
      wattroff(win, A_REVERSE);
   }
   refresh();
}

void handleinput() {
   int key;
   char* cmd;
   char* cmdhead = "$BROWSER";
   while ((key = wgetch(win)) != 'h') {
      if (key == 'j') {
         if (mark == wheight - 3 && end < count) {
            ++end;
            ++start;
         } else {
            if (mark < wheight - 3) {
               ++mark;
            } else {
               mark = start = 0;
               end = count > wheight - 2 ? wheight - 2 : count;
            }
         }
         drawitems();
      } else if (key == 'k') {
         if (mark == 0 && start > 0) {
            --end;
            --start;
         } else {
            if (mark > 0) {
               --mark;
            } else {
                mark = mark;
               /* mark = end = count - 1; */
               /* start = count > wheight - 2 ? wheight - 2 : 0; */
            }
         }
         drawitems();
      } else if (key == 'l') {
         cmd = calloc(strlen(cmdhead) + strlen(items[mark]) + 2, (sizeof *cmd));
         sprintf(cmd, "%s %s", cmdhead, items[mark]);
         system(cmd);
      }
   }
}

void cleanup() {
   for (i = 0; i < count; ++i) free(items[i]);
   free(items);
   endwin();
}
