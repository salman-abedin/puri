/*
 * puri
 * minimal CLI URL launcher
 */

#include <ncurses.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urls.h"

#define HEADER "puri: Puny URL Launcher"
#define FOOTER "h:Quit   j:Down   k:Up   l:launch"

int count, mark, height, width;
char** urls;
WINDOW* win;

void cleanup() {
   for (int i = 0; i < count; ++i) free(urls[i]);
   free(urls);
   endwin();
}

void drawitems() {
   for (int i = 0; i < count; ++i) {
      if (i == mark) wattron(win, A_REVERSE);
      mvwprintw(win, i + 1, 1, urls[i]);
      wattroff(win, A_REVERSE);
   }
   wrefresh(win);
}

void handleinput() {
   int key;
   char* cmd;
   char cmdhead[] = "$BROWSER";
   while (1) {
      key = getch();
      if (key == 'j' && mark < count - 1) {
         ++mark;
         drawitems();
      } else if (key == 'k' && mark > 0) {
         --mark;
         drawitems();
      } else if (key == 'l') {
         cmd = calloc(strlen(cmdhead) + strlen(urls[mark]) + 2, sizeof(char));
         sprintf(cmd, "%s %s", cmdhead, urls[mark]);
         system(cmd);
      } else if (key == 'h')
         break;
   }
}

void drawui() {
   mvprintw(0, width / 2 - 10, HEADER);
   mvprintw(height - 1, width / 2 - 15, FOOTER);
   win = newwin(height - 2, width, 1, 0);
   box(win, 0, 0);
   refresh();
}

void init() {
   initscr();
   cbreak();
   noecho();
   curs_set(0);
   getmaxyx(stdscr, height, width);
}

int main(int argc, char* argv[]) {
   urls_t urlss = get_urls(argv[1]);
   urls = urlss.links;
   count = urlss.count;
   init();
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}

