/*
 * puri
 * minimal CLI URL launcher
 */

#include "ui.h"
#include "urls.h"

int main(int argc, char** argv) {
   Urls urls = get_urls(argv[1]);
   init(urls.count, urls.links);
   drawui();
   drawitems();
   handleinput();
   cleanup();
   return 0;
}
