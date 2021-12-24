#include <stdio.h>
#include <stdlib.h>
#include "board.h"


int main(void){

    const int rows;
    int cols;
    int steps;


    scanf("%d %d", &rows, &cols);
    scanf("%d\n", &steps);

    Board* b = new_board(rows, cols);
    b->rows = rows;
    b->cols = cols;
    
    for (int i = 0; i < rows*cols; i++){
        char c;
        scanf("%c ", &c);
        *(b->grid+i) = c;
    }
    
    for (int i = 0; i < steps; i++){
        b = update_board(b);
    }
    
    int gcount = get_count(b, 'g');
    int rcount = get_count(b, 'r');
    free(b);
    free(b->grid);
    
    printf("green: %d, red: %d\n", gcount, rcount);
    return 0;
}
