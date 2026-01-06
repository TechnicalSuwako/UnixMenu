#ifndef DISPLAY_H
#define DISPLAY_H

#include "program.h"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

void drawtext(
  Display *display, Window window, GC gc, int x, int y, const char *text, int sel);
void filterdisplay(
  Display *display, Window window, GC gc, const char *input, int sel);

extern XftColor color, selcolor;
extern Colormap colormap;
extern XftDraw *draw;
extern XftFont *font;
extern Visual *visual;
extern int topidx;

#endif
