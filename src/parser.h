#ifndef PARSER_H
#define PARSER_H

#include "program.h"

void parse_name(char *line, Program *program);
void parse_keywords(char *line, Program *program);
void parse_exec(char *line, Program *program);
void parse_term(char *line, Program *program);
void parse_desktop_file(const char *filepath);
void scan_desktop_files(const char *directory);

#endif
