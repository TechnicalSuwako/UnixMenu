#include "program.h"
#include "parser.h"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

Program programs[MAX_ITEMS];
int programcount = 0;
int window_width = 300;
int window_height = 240;
int item_height = 20;
int display_items = 10;

void fetch_programs() {
  char *xdg_data_home = getenv("XDG_DATA_HOME");
  if (!xdg_data_home) {
    xdg_data_home = getenv("HOME");
    if (xdg_data_home) {
      char path[MAX_NAME_LEN];
      snprintf(path, sizeof(path), "%s/.local/share/applications", xdg_data_home);
      scan_desktop_files(path);
    }
  } else {
    char path[MAX_NAME_LEN];
    snprintf(path, sizeof(path), "%s/applications", xdg_data_home);
    scan_desktop_files(path);
  }

  scan_desktop_files("/usr/share/applications");
  scan_desktop_files("/usr/local/share/applications");
#if defined(__NetBSD__)
  scan_desktop_files("/usr/pkg/share/applications");
#endif
}

void launch_program(const char *exec, const char *term) {
  if (fork() == 0) {
    setsid();
    if (strncmp(term, "true", 4) == 0) {
      char *terminal = getenv("TERMINAL");
      if (!terminal) terminal = "xterm";
      execlp(terminal, terminal, "-e", exec, NULL);
    } else {
      execl("/bin/sh", "sh", "-c", exec, NULL);
    }
    exit(0);
  }
}

int filtercount(const char *input) {
  int count = 0;

  for (int i = 0; i < programcount; i++) {
    if (strcasestr(programs[i].name, input) || strcasestr(programs[i].keys, input))
      count++;
  }

  return count;
}

void calculate_dimensions(int win_width, int win_height) {
  window_width = win_width;
  window_height = win_height;
  item_height = win_height / (display_items + 2);
}
