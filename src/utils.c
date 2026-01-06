#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(__linux__)
#include <string.h>
#include <ctype.h>
#endif

Dup dupn[MAX_NAME_LEN * 2];
int dupcnt = 0;

#if defined(__linux__)
#include <string.h>

char *strcasestr(const char *haystack, const char *needle) {
  size_t needle_len = strlen(needle);
  if (needle_len == 0) {
    return (char *)haystack;
  }

  while (*haystack) {
    if (strncasecmp(haystack, needle, needle_len) == 0) {
      return (char *)haystack;
    }
    haystack++;
  }
  return NULL;
}
#endif

int isdup(const char *name) {
  for (int i = 0; i < dupcnt; i++) {
    if (strncmp(dupn[i].name, name, strlen(name)) == 0) return 1;
  }

  return 0;
}

int isdis(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (!file) return 0;

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "NoDisplay=true")) {
      fclose(file);
      return 0;
    }
  }

  fclose(file);
  return 1;
}

void add_to_dup(const char *name) {
  if (dupcnt < (MAX_NAME_LEN * 2)) {
    strncpy(dupn[dupcnt].name, name, MAX_NAME_LEN - 1);
    dupn[dupcnt].name[MAX_NAME_LEN - 1] = '\0';
    dupcnt++;
  }
}

int launch_path(const char *exec) {
  char *path = getenv("PATH");
  if (!path) return 1;

  char *pdup = strdup(path);
  char *dir = strtok(pdup, ":");
  while (dir != NULL) {
    char fp[1024];
    snprintf(fp, sizeof(fp), "%s/%s", dir, exec);

    if (access(fp, X_OK) == 0) {
      free(pdup);
      return 1;
    }
    dir = strtok(NULL, ":");
  }

  free(pdup);
  return 0;
}

void cleanup(Display *display, Window window, GC gc, XftColor *color,
    XftColor *selcolor, XftDraw *draw, XftFont *font,
    Colormap colormap, Visual *visual) {
  if (color) XftColorFree(display, visual, colormap, color);
  if (selcolor) XftColorFree(display, visual, colormap, selcolor);
  if (font) XftFontClose(display, font);
  if (draw) XftDrawDestroy(draw);
  if (gc) XFreeGC(display, gc);
  if (window) XDestroyWindow(display, window);
  if (display) XCloseDisplay(display);
}
