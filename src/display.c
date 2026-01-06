#include "display.h"

#include <string.h>

int topidx = 0;

void drawtext(
  Display *display, Window window, GC gc, int x, int y, const char *text, int sel) {
  if (sel) {
    XSetForeground(display, gc, BLACK);
    XFillRectangle(
        display, window, gc, 0, y - item_height + 5, window_width, item_height);
    XSetForeground(display, gc, BLACK);
  } else {
    XSetForeground(display, gc, WHITE);
  }

  XftDrawStringUtf8(draw, &color, font, x, y, (const FcChar8 *)text, strlen(text));
}

void filterdisplay(
  Display *display,
  Window window,
  GC gc,
  const char *input,
  int sel
) {
  XClearWindow(display, window);

  // 検索ボックス
  drawtext(display, window, gc, 10, item_height, input, 0);

  int y = item_height * 2;
  int idx = 0;

  for (int i = 0; i < programcount; i++) {
    if (!strcasestr(programs[i].name, input) && !strcasestr(programs[i].keys, input))
      continue;

    if (idx >= topidx && idx < topidx + display_items) {
      drawtext(display, window, gc, 10, y, programs[i].name, idx == sel);
      y += item_height;
    }
    idx++;
  }
}
