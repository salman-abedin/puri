#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "urls.h"

char* _get_file_string(char* path) {
   int writei1, writei2, spacecount, newlinecount;
   char letter;
   long size;
   char* filestring;
   FILE* file;

   writei1 = writei2 = spacecount = newlinecount = 0;
   file = fopen(path, "r");

   fseek(file, 0L, SEEK_END);
   size = ftell(file);
   filestring = malloc(size);

   fseek(file, 0L, SEEK_SET);
   while ((letter = fgetc(file)) != EOF) {
      if (isprint(letter) != 0 && letter != '|')
         filestring[writei1++] = letter;
      else if (letter == '\n') {
         if (newlinecount > 1) {
            filestring[writei1++] = ' ';
            newlinecount = 0;
         } else {
            ++newlinecount;
            continue;
         }
      }
   }

   /* for (int i = 0; i < writei1; ++i) { */
   /*    if (filestring[i] == ' ') { */
   /*       ++spacecount; */
   /*       continue; */
   /*    } */
   /*    if (spacecount == 1) */
   /*       filestring[writei2++] = ' '; */
   /*    else */
   /*       filestring[writei2++] = filestring[i]; */
   /*    spacecount = 0; */
   /* } */
   /* filestring[writei2] = '\0'; */

   fclose(file);
   return filestring;
}

urls_t get_urls(char* path) {
   int match_len, url_count;
   char* cursor;
   urls_t urls;
   regex_t regx;
   regmatch_t matches[1];

   char* filestring = _get_file_string(path);

   regcomp(&regx, URL_PATTERN, REG_EXTENDED);
   cursor = filestring;
   while (regexec(&regx, cursor, 1, matches, 0) == 0) {
      ++url_count;
      match_len = matches[0].rm_eo - matches[0].rm_so;
      if (url_count == 1)
         urls.links = calloc(match_len + 1, sizeof(char*));
      else
         urls.links = realloc(urls.links, sizeof(char*) * url_count);
      urls.links[url_count - 1] = calloc(match_len + 1, sizeof(char));
      strncpy(urls.links[url_count - 1], &cursor[matches[0].rm_so], match_len);

      cursor += matches[0].rm_eo;
   }
   urls.count = url_count;

   regfree(&regx);
   return urls;
}
