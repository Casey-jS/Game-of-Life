#ifndef BOARD_H
#define BOARD_H
typedef struct Board{
    int rows;
    int cols;
    char *grid;
}Board;

Board* new_board(int, int);

Board* new_shell(Board*);

char update_cell(char, char*);

int get_count(Board*, char);

char *neighbors(Board*, int, int, int);

Board* update_board(Board *b);

#endif