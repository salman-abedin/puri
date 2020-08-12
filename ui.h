#ifndef _UI_H
#define _UI_H

#define HEADER "puri: Puny URL Launcher"
#define FOOTER "h:Quit   j:Down   k:Up   l:launch"

void init(char** in_items, int in_count);
void drawui();
void drawitems();
void handleinput();
void cleanup();

#endif
