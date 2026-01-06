#include "control.h"
#include "display.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void control_expose(
  Display *display, Window window, GC gc, XEvent *event, char *input, int *sel) {
  if (event->type == ConfigureNotify)
    calculate_dimensions(event->xconfigure.width, event->xconfigure.height);
  filterdisplay(display, window, gc, input, *sel);
}

void control_keypress(
  Display *display, Window window, GC gc, XEvent *event, char *input, int *sel) {
  char buf[32];
  KeySym keysym;
  int len = XLookupString(&event->xkey, buf, sizeof(buf) - 1, &keysym, NULL);

  if (keysym == XK_Up) {
    if (*sel > 0) {
      (*sel)--;
      if (*sel < topidx) topidx--;
    }
  } else if (keysym == XK_Down) {
    int filteredcount = filtercount(input);
    if (*sel < filteredcount - 1) {
      (*sel)++;
      if (*sel >= topidx + display_items) topidx++;
    }
  } else if (keysym == XK_Page_Up) {
    if (*sel > 0) {
      *sel -= display_items;
      if (*sel < topidx) topidx -= display_items;
    }
  } else if (keysym == XK_Page_Down) {
    int filteredcount = filtercount(input);
    if (*sel < filteredcount - 1) {
      *sel += display_items;
      if (*sel >= topidx + display_items) topidx += display_items;
    }
  } else if (keysym == XK_BackSpace && strlen(input) > 0) {
    input[strlen(input) - 1] = '\0';
    *sel = 0;
    topidx = 0;
  } else if (keysym == XK_Return) {
    int visible_index = 0;
    int found = 0;
    for (int i = 0; i < programcount; i++) {
      if (!strcasestr(programs[i].name, input)) continue;
      if (visible_index == *sel) {
        launch_program(programs[i].exec, programs[i].term);
        found = 1;
        break;
      }
      visible_index++;
    }

    if (found == 0 && launch_path(input)) {
      launch_program(input, "false");
    }
    exit(0);
  } else if (keysym == XK_Escape) {
    exit(0);
  } else if (len > 0 && len < MAX_NAME_LEN - 1) {
    strncat(input, buf, len);
    *sel = 0;
    topidx = 0;
  }

  filterdisplay(display, window, gc, input, *sel);
}

void control_buttonpress(XEvent *event, Program *programs, int item_height) {
  int x = event->xbutton.x;
  int y = event->xbutton.y;

  for (int i = 0; i < programcount; i++) {
    if (
      x >= programs[i].x &&
      x <= programs[i].x + programs[i].width &&
      y >= programs[i].y - item_height &&
      y <= programs[i].y
    ) {
      launch_program(programs[i].exec, programs[i].term);
      break;
    }
  }
}
