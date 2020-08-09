/*
 * puri
 * minimal CLI URL launcher
 */

#include <ncurses.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER "puri: Puny URL Launcher"
#define FOOTER "h:Quit   j:Down   k:Up   l:launch"
#define URL_PATTERN "http[s]?://[^[:space:]]*"

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

void geturls(char* path) {
   FILE* file = fopen(path, "r");
   char *buffer, *cursor, *line;
   int bytes, len;
   regex_t regx;
   regmatch_t matches[1];

   if (file == NULL) exit(EXIT_FAILURE);
   fseek(file, 0L, SEEK_END);
   bytes = ftell(file);
   fseek(file, 0L, SEEK_SET);

   buffer = calloc(bytes, sizeof(char));
   line = calloc(width, sizeof(char));
   while (fgets(line, width + 2, file) != NULL) {
      if (strlen(line) - 1 > 1)
         line[strlen(line) - 1] = '\0';
      else
         line[strlen(line) - 1] = ' ';
      strcat(buffer, line);
   }
   fclose(file);
   free(line);

   regcomp(&regx, URL_PATTERN, REG_EXTENDED);

   cursor = buffer;
   while (regexec(&regx, cursor, 1, matches, 0) == 0) {
      ++count;
      len = matches[0].rm_eo - matches[0].rm_so;
      urls = realloc(urls, sizeof(char*) * count);
      urls[count - 1] = calloc(len + 1, sizeof(char));
      strncpy(urls[count - 1], &cursor[matches[0].rm_so], len);
      cursor += matches[0].rm_eo;
   }
   regfree(&regx);
   free(buffer);
}

void init() {
   initscr();
   cbreak();
   noecho();
   curs_set(0);
   getmaxyx(stdscr, height, width);
}


int main(int argc, char* argv[]) {
   init();
   geturls(argv[1]);
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}
