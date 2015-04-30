#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <string.h>

const struct option long_options[] = {
  { "help", 0, NULL, 'h' },
  { "ievent", 1, NULL, 'i' },
  { "nevent", 1, NULL, 'n' },
  { "tail", 1, NULL, 't' },
  { "makeTree", 0, NULL, 'm' },
  { NULL, 0, NULL, 0 } /* Required at end of array. */
};

class options {
 public:
  options(int argc, char *argv[]);
  ~options();

  int get_ievent() {return ievent;};
  int get_nevent() {return nevent;};
  std::string get_tail() {return tail;};
  bool make_tree(){return makeTree;};

 private:
  void print_usage(FILE* stream, int exit_code);
  void doit();

  std::string program_name;
  int next_option;
  std::string short_options;

  int ievent;
  int nevent;
  std::string tail;
  bool makeTree;

  int argc_;
  char **argv_;
};

#endif
