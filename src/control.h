#ifndef CONTROL_H
#define CONTROL_H

#include "program.h"

void control_expose(
  Display *display, Window window, GC gc, XEvent *event, char *input, int *sel);
void control_keypress(
  Display *display, Window window, GC gc, XEvent *event, char *input, int *sel);
void control_buttonpress(XEvent *event, Program *programs, int item_height);

#endif
