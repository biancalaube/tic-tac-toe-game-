#include "ttt.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    return(1);
  }
  int i = 0;
  int board;

  init_boards();
  init_board(START_BOARD);
  join_graph(START_BOARD);
  compute_score();

  for (i = 1; i < argc; i++) {
    board =  atoi(argv[i]);
    printf("BOARD: %d\n",board );
    print_node(htable[board]);
  }
  return(0);
}
