#include "options.h"

options::options(int argc, char *argv[]) :
  program_name(argv[0]),
  short_options("hi:n:t:m"),
  ievent(0),
  nevent(-1),
  tail(""),
  makeTree(false),
  argc_(argc) {

  argv_ = new char*[argc+1];

  for (int i = 0; i < argc_; i++) {
    int len = strlen(argv[i] + 1);
    argv_[i] = new char[len];
    strcpy( argv_[i], argv[i] );
  }
  argv_[argc] = NULL;
  doit();
}

options::~options() {
  for(int i=0; i < argc_; i++) {
    delete [] argv_[i];
  }
  delete [] argv_;
}

void options::print_usage (FILE* stream, int exit_code) {
  fprintf (stream, "Usage: %s options [ ... ]\n", program_name.c_str());
  fprintf (stream,
           " -h --help Display this usage information.\n"
           " -i --ievent Start from event i.\n"
           " -n --nevent Run for n events.\n"
           " -t --tail Append tail to output files.\n"
           " -m --makeTree make new pattern tree with strip coordinates.\n");
  exit (exit_code);
}

void options::doit() {

  do {
    next_option = getopt_long (argc_, argv_, short_options.c_str(), long_options, NULL);
    switch (next_option)
      {
      case 'h':
        /* User has requested usage information. Print it to standard output, and exit with exit code zero (normal termination). */
        print_usage (stdout, 0);

      case 'i':
        ievent = atoi(optarg);
        std::cout << "starting from event " << ievent << std::endl;
        break;

      case 'n':
        nevent = atoi(optarg);
        std::cout << "running for " << nevent << " events" << std::endl;
        break;

      case 't':
        tail = optarg;
        std::cout << "appending " << tail << std::endl;
        break;

      case 'm':
        makeTree = true;
        std::cout << "make pattern tree " << std::endl;
        break;

      case '?': /* The user specified an invalid option. */
        /* Print usage information to standard error, and exit with exit code one (indicating abnormal termination). */
        print_usage (stderr, 1);

      case -1:  /* Done with options. */
        break;

      default: /* Something else: unexpected. */
        abort ();
      }
  }
  while (next_option != -1);

}
