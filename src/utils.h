#ifndef UTILS_H
#define UTILS_H

#include "program.h"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
  char name[MAX_NAME_LEN];
} Dup;

extern Dup dupn[MAX_NAME_LEN * 2];
extern int dupcnt;

#if defined(__linux__)
char *strcasestr(const char *haystack, const char *needle);
#endif

int isdup(const char *name);
int isdis(const char *filepath);
void add_to_dup(const char *name);
int launch_path(const char *exec);
void cleanup(Display *display, Window window, GC gc,
    XftColor *color, XftColor *selcolor, XftDraw *draw, XftFont *font,
    Colormap colormap, Visual *visual);

#endif
