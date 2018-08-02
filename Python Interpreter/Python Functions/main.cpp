#include <iostream>
#include <string>
#include "includes/symbolTable.h"
#include "includes/poolOfNodes.h"


extern int yyparse();

extern void init_scanner(FILE *);
extern void end_scanner();

static FILE * 
open_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Could not open file \"%s\"\n", filename);
    exit(EXIT_FAILURE);
  }
  return file;
}


int main(int argc, char * argv[]) {
  FILE *input_file = stdin;
  if (argc > 1) { /* user-supplied filename */
    input_file = open_file(argv[1]);
  }
  init_scanner(input_file);
  int parse_had_errors = yyparse();
  PoolOfNodes::getInstance().drainThePool();
  fclose(input_file);
  end_scanner();

  if (parse_had_errors) {
    fprintf(stderr,"Abnormal termination\n");
  }

  return (parse_had_errors ? EXIT_FAILURE : EXIT_SUCCESS);

}


