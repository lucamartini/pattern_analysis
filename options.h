#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <string.h>

const struct option long_options[] = {
  { "help", 0, NULL, 'h' },
  { "file", 1, NULL, 'f' },
  { "ievent", 1, NULL, 'i' },
  { "nevent", 1, NULL, 'n' },
  { "tail", 1, NULL, 't' },
  { "SS5", 1, NULL, 's' },
  { "layer5", 0, NULL, '5'},
  { NULL, 0, NULL, 0 } /* Required at end of array. */
};

class options {
 public:
  options(int argc, char *argv[]);
  ~options();

  bool get_l5() {return l5;};
  int get_ievent() {return ievent;};
  int get_nevent() {return nevent;};
  int get_SS5() {return SS5;};
  std::string get_tail() {return tail;};
  std::string get_file() {return file;};

 private:
  void print_usage(FILE* stream, int exit_code);
  void doit();

  std::string program_name;
  int next_option;
  std::string short_options;

  bool l5;
  int ievent;
  int nevent;
  int SS5;
  std::string tail;
  std::string file;


  int argc_;
  char **argv_;
};

#endif
