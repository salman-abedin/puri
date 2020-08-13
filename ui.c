#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int mark, start, end, limit, height, width, wwidth, count, i, j;
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
   limit = height - 4;
   end = count > limit ? limit : count;
}

void drawui() {
   mvprintw(0, (width - strlen(HEADER)) / 2, HEADER);
   mvprintw(height - 1, (width - strlen(FOOTER)) / 2, FOOTER);
   win = newwin(height - 2, wwidth, 1, (width - wwidth) / 2);
   box(win, 0, 0);
   refresh();
}

void drawitems() {
   for (i = start; i < end; ++i) {
      if (i == mark) wattron(win, A_REVERSE);
      /* mvwaddnstr(win, i + 1, 1, items[i], wwidth - 2); */
      for (j = 0; j < wwidth - 2; ++j) waddch(win, items[i][j]);
      wattroff(win, A_REVERSE);
   }
}

void handleinput() {
   int key;
   char* cmd;
   char* cmdhead = "$BROWSER";
   while ((key = wgetch(win)) != 'h') {
      if (key == 'j') {
         /* ++mark; */
         /* ++end; */
         /* ++start; */

         if (mark == limit - 1 && end < count) {
            ++end;
            ++start;
         } else {
            ++mark;
         }

         drawitems();
      } else if (key == 'k' && mark > 0) {
         if (mark == 0 && start > 0) {
            --end;
            --start;
         } else {
            --mark;
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
