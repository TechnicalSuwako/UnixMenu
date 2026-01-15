#include <stdio.h>
#include <stdlib.h>

#include <X11/Xatom.h>

#include "src/control.h"
#include "src/program.h"
#include "src/display.h"
#include "src/parser.h"
#include "src/utils.h"

XftColor color, selcolor;
Colormap colormap;
XftDraw *draw;
XftFont *font;
Visual *visual;

const char *sofname = "um";
const char *version = "0.2.1";

int main() {
  Display *display;
  Window window;
  XEvent event;
  int screen;
  GC gc = NULL;
  XGCValues values;
  char input[MAX_NAME_LEN] = {0};
  int sel = 0;

  fetch_programs();

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "画面を開けられません。\n");
    exit(1);
  }

  screen = DefaultScreen(display);

  int screen_width = DisplayWidth(display, screen);
  int screen_height = DisplayHeight(display, screen);
  window_width = screen_width / 4;
  window_height = screen_height / 4;
  int window_x = (screen_width - window_width) / 2;
  int window_y = (screen_height - window_height) / 2;

  window = XCreateSimpleWindow(display, RootWindow(display, screen),
      window_x, window_y, window_width, window_height, 1, FGCOL, FGCOL);
  if (!window) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
  }

  Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom dialog = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);

  XChangeProperty(display, window, net_wm_window_type, XA_ATOM, 32,
      PropModeReplace, (unsigned char *)&dialog, 1);

  XSetWindowBackground(display, window, BGCOL);

  XSelectInput(display, window,
      ExposureMask | KeyPressMask | StructureNotifyMask | ButtonPressMask);

  gc = XCreateGC(display, window, 0, &values);
  if (!gc) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "GCを作れません。\n");
    exit(1);
  }

  visual = DefaultVisual(display, screen);

  colormap = XCreateColormap(display, window, visual, AllocNone);
  if (colormap == None) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "カラーマップを作れません。\n");
    exit(1);
  }

  draw = XftDrawCreate(display, window, visual, colormap);
  if (!draw) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "ドローを作れません。\n");
    exit(1);
  }

  font = XftFontOpenName(display, screen, "Noto Sans CJK-12");
  if (!font) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "フォントの読み込みに失敗。\n");
    exit(1);
  }

  if (!XftColorAllocName(display, visual, colormap, "#ee4030", &color)) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "色の役割に失敗。\n");
  }

  if (!XftColorAllocName(display, visual, colormap, "#120f12", &selcolor)) {
    cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);
    fprintf(stderr, "選択色の役割に失敗。\n");
  }

  XMapWindow(display, window);

  while (1) {
    XNextEvent(display, &event);

    if (event.type == Expose || event.type == ConfigureNotify) {
      control_expose(display, window, gc, &event, input, &sel);
    } else if (event.type == KeyPress) {
      control_keypress(display, window, gc, &event, input, &sel);
    } else if (event.type == ButtonPress) {
      control_buttonpress(&event, programs, item_height);
    }
  }

  cleanup(display, window, gc, &color, &selcolor, draw, font, colormap, visual);

  return 0;
}
