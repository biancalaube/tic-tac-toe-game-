#include "ttt.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

void init_boards() {
  //set init = 0 for entire htable
  for (int i = 0; i < HSIZE; i++) {
    //htable[i].init = '0';
    htable[i].init = 0;
  }
}

int depth(Board board) {
  //returns number of plays so far
  int i = 0;
  int d = 0;
  for (i = 0; i < BSIZE; i++){
    if (board[i] == 'X' || board[i] == 'O') {
      d++;
    }
  }
  return(d);
}

char winner(Board board) {
  //checks for winner
  int d = depth(board);
  int i, j, pos1, pos2, pos3;
  char c = '?';
  int curWin[3];

    if (d == 9) {
      c = '-';
    } else if (d < 3) {
      c = '?';
    } else {
      for (i = 0; i < 8; i++) {
        for (j = 0; j < 3; j++) {
          curWin[j] = WINS[i][j];
        }
        pos1 = pos2idx[curWin[0]];
        pos2 = pos2idx[curWin[1]];
        pos3 = pos2idx[curWin[2]];
        //checks if X won
        if ((board[pos1] == 'X') && (board[pos2] == 'X') && (board[pos3] == 'X')) {
          c = 'X';
        }
        if ((board[pos1] == 'O') && (board[pos2] == 'O') && (board[pos3] == 'O')) {
          c = 'O';
        }
      }
    }
  return(c);
}

char turn(Board board) {
  int d = depth(board);
  int hash = board_hash(board);
  int check = d % 2;
  char t;

  if (d == 9) {
    t = '-';
  } else if (htable[hash].winner == 'X' || htable[hash].winner == 'O') {
    t = '-';
  } else {
    if (check == 0) {
      t = 'X';
    } else {
      t = 'O';
    }
  }
  return(t);
}

void init_board(Board board) {
  int hash = board_hash(board);

  htable[hash].init = 1;
  htable[hash].depth = depth(board);
  strcpy(htable[hash].board, board);
  htable[hash].winner = winner(board);
  htable[hash].turn = turn(board);
}

void join_graph(Board board) {
  int i, j;
  char c, t;
  int newHash;
  int hash = board_hash(board);
  char newboard[BSIZE];

  for (i = 0; i < 9; i++) {
    c = board[pos2idx[i]];
    if (c == 'X' || c == 'O') {
      htable[hash].move[i] = -1;
    } else if (htable[hash].winner == 'X' || htable[hash].winner == 'O') {
      for (j = 0; j < 9; j++) {
        htable[hash].move[j] = -1;
      }
    } else {
      strcpy(newboard, board);
      t = turn(board);
      newboard[pos2idx[i]] = t; //gonna return x or o
      newHash = board_hash(newboard);
      htable[hash].move[i] = newHash;
      if (htable[newHash].init != 1) {
        init_board(newboard);
        join_graph(newboard);
      }
    }
  }
}

void compute_score() {
  int i, j, tempHash;

    for (i = HSIZE-1; i > -1; i--) {
      if (htable[i].init == 1) {
        htable[i].score = 0;
        if (htable[i].winner == 'X') {
          htable[i].score = 1;
        } else if (htable[i].winner == 'O') {
          htable[i].score = -1;
        } else if (htable[i].winner == '-'){
          htable[i].score = 0;
        } else {
            if (htable[i].turn == 'X') {
              for (j = 0; j < 9; j++) {
                tempHash = htable[i].move[j];
                if (htable[tempHash].score > 0) {
                  htable[i].score = 1;
                  j = 9;
                }
              }
            } else if (htable[i].turn == 'O') {
              for (j = 0; j < 9; j++) {
                tempHash = htable[i].move[j];
                if (htable[tempHash].score < 0) {
                  htable[i].score = -1;
                  j = 9;
                }
              }
            }
        }
      }
    }

}

int best_move(int board) {
  int i, bestBoard = -1, tempHash;
  for (i=0; i < 9; i++) {
    tempHash = htable[board].move[i];
  //  printf("%d %d\n", tempHash, htable[tempHash].score);
    if (tempHash != -1) {
      if (htable[board].turn == 'X') {
        if (htable[tempHash].score > -1) {
          bestBoard = i;
        }
      } else if (htable[board].turn == 'O') {
        if (htable[tempHash].score < 1) {
          bestBoard = i;
        }
      }
    }
  }
  if (bestBoard == -1) {
    for (i = 0; i < 9; i++) {
      if (htable[board].move[i] != -1) {
        bestBoard = i;
        i = 9;
      }
    }
  }
  //printf("board being sent is %d\n",bestBoard );
  return(bestBoard);
}
