#include "ttt.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int i = 0;
  int count = 0;

  init_boards();
  init_board(START_BOARD);
  join_graph(START_BOARD);
  compute_score();

  for (i = 0; i < HSIZE; i++) {
    if (htable[i].init == 1) {
      count++;
    }
  }
  printf("%d\n",count );
  return(0);
}
