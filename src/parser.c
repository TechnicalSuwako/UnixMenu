#include "parser.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

bool foundName = false;
bool foundKey = false;
bool foundExec = false;
bool isDesktopEntry = false;

void parse_name(char *line, Program *program) {
  if (foundName) return;
  if (strncmp(line, "Name[", 5) == 0) {
    char *locale = line + 5;
    char *end = strchr(locale, ']');
    if (!end) return;
    *end = '\0';
    if (strncmp(locale, getenv("LANG"), 2) != 0) return;
    if (!isdup(end + 2)) {
      strncpy(program->name, end + 2, MAX_NAME_LEN - 1);
      program->name[strcspn(program->name, "\n")] = '\0';
    }
    add_to_dup(end + 2);
    foundName = true;
  } else {
    if (!isdup(line + 5)) {
      strncpy(program->name, line + 5, MAX_NAME_LEN - 1);
      program->name[strcspn(program->name, "\n")] = '\0';
    }
    add_to_dup(line + 5);
    foundName = true;
  }
}

void parse_keywords(char *line, Program *program) {
  if (foundKey) return;
  size_t len = strnlen(line + 9, MAX_NAME_LEN - 1);
  strncpy(program->keys, line + 9, len);
  program->keys[len] = '\0';

  program->keys[strcspn(program->keys, "\n")] = '\0';
  foundKey = true;
}

void parse_exec(char *line, Program *program) {
  if (foundExec) return;
  size_t len = strnlen(line + 5, MAX_NAME_LEN - 1);
  strncpy(program->exec, line + 5, len);
  program->exec[strcspn(program->exec, "\n")] = '\0';

  char *p = program->exec;
  while ((p = strpbrk(p, "%")) != NULL) {
    *p = '\0';

    size_t rspace = MAX_NAME_LEN - strlen(program->exec) - 1;
    if (rspace > 0) {
      strncat(program->exec, p + 2, rspace);
    }
  }
  foundExec = true;
}

void parse_term(char *line, Program *program) {
  size_t len = strnlen(line + 9, MAX_NAME_LEN - 1);
  strncpy(program->term, line + 9, len);
  program->term[strcspn(program->term, "\n")] = '\0';

  char *p = program->term;

  while ((p = strpbrk(p, "%")) != NULL) {
    *p = '\0';

    size_t rspace = MAX_NAME_LEN - strlen(program->term) - 1;
    if (rspace > 0) {
      strncat(program->term, p + 2, rspace);
    }
  }
}

void parse_desktop_file(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (!file) return;

  char line[MAX_NAME_LEN * 2];
  Program program = {0};

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "Name=", 5) == 0 || strncmp(line, "Name[", 5) == 0) {
      parse_name(line, &program);
    } else if (strncmp(line, "Keywords=", 9) == 0) {
      parse_keywords(line, &program);
    } else if (strncmp(line, "Exec=", 5) == 0) {
      parse_exec(line, &program);
    } else if (strncmp(line, "Terminal=", 9) == 0) {
      parse_term(line, &program);
    }
  }

  fclose(file);

  if (program.name[0] && program.exec[0]) {
    programs[programcount++] = program;
  }
}

void scan_desktop_files(const char *directory) {
  DIR *dir = opendir(directory);
  if (!dir) return;

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcasestr(entry->d_name, ".desktop")) {
      size_t filepath_len = strlen(directory) + strlen(entry->d_name) + 2;
      char *filepath = malloc(filepath_len);
      if (!filepath) {
        closedir(dir);
        return;
      }

      snprintf(filepath, filepath_len, "%s/%s", directory, entry->d_name);
      if (isdis(filepath)) parse_desktop_file(filepath);

      isDesktopEntry = false;
      foundName = false;
      foundKey = false;
      foundExec = false;

      free(filepath);
    }
  }

  closedir(dir);
}
