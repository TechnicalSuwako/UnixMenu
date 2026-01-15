#ifndef PROGRAM_H
#define PROGRAM_H

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#define MAX_ITEMS 512
#define MAX_NAME_LEN 256
#define MAX_LINE_LENGTH 256

#define FGCOL 0xb61729
#define FGSEL 0x120f12
#define BGSEL 0xf35869
#define BGCOL 0x232023

typedef struct {
  char name[MAX_NAME_LEN];
  char keys[MAX_NAME_LEN];
  char exec[MAX_NAME_LEN];
  char term[MAX_NAME_LEN];
  int x, y, width, height;
} Program;

void fetch_programs();
void launch_program(const char *exec, const char *term);
int filtercount(const char *input);
void calculate_dimensions(int win_width, int win_height);

extern Program programs[MAX_ITEMS];
extern int programcount;
extern int window_width;
extern int window_height;
extern int item_height;
extern int display_items;

#endif
