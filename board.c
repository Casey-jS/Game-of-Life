#include <stdio.h>
#include <stdlib.h>
#include "board.h"

void print_board(Board* b, char* grid){

    int rows = b->rows;
    int cols = b->cols;

    for (int i = 0; i < rows*cols; i++){ //print board
        if (i % cols == 0){ putchar('\n'); }
        *(b->grid+i) = grid[i];
        printf("%c ", *(b->grid+i));
    }
}
Board* new_shell(Board *b){

    int shell_rows = b->rows + 2;
    int shell_cols = b->cols + 2;
    //printf("Shell cols: %d\nShell rows: %d\n", shell_cols, shell_rows);

    int cols = b->cols;
    int rows = b->rows;

    Board* shell = new_board(shell_rows, shell_cols);   
    //printf("\nShell has %d rows, %d cols\n", shell->rows, shell->cols); 
    
    int shell_bottom = shell_rows - 1;
    int shell_right = shell_cols - 1;

    int right = cols - 1;
    int bottom = rows - 1;

    *(shell->grid+0*shell_cols+0) = *(b->grid+(bottom)*cols+(right)); //Top left
    //printf("\nBottom right is %c, assigning to top left", *(b->grid+bottom*cols+right));
  
    for (int i = 0; i < shell_cols - 1; i++){ 
        *(shell->grid+0*shell_cols+i+1) = *(b->grid+(bottom)*cols+i);
    }

    *(shell->grid+0*shell_cols+shell_right) = *(b->grid+bottom*cols+0); //Top right
    //printf("\nBottom left is %c, assigning to top right", *(b->grid+bottom*cols+0));

    for (int row = 1; row < shell_bottom; row++){ // Everything except bottom row
        for (int col = 0; col < shell_cols; col++){
            if (col == 0){
                *(shell->grid+row*shell_cols+0) = *(b->grid+(row-1)*cols+right);
            }
            else if(col == shell_right){
                *(shell->grid+row*shell_cols+shell_right) = *(b->grid+(row-1)*cols+0);
            }
            else{
                *(shell->grid+row*shell_cols+col) = *(b->grid+(row-1)*cols+col-1);
            }
        }
    }

    *(shell->grid+shell_bottom*shell_cols+0) = *(b->grid+0*cols+(right)); //Bottom left
    for (int i = 0; i < shell_cols; i++){ 
        *(shell->grid+shell_bottom*shell_cols+i+1) = *(b->grid+0*cols+i); }

    *(shell->grid+shell_bottom*shell_cols+shell_right) = *(b->grid+0*cols+0);
    
    return shell;

}
char *neighbors(Board *shell, int row, int col, int ncols){
    char *status  = (char*)malloc(8*sizeof(char));
    status[0] = *(shell->grid+(row)*ncols+(col+1)); //N
    //printf("[%d][%d]'s N: %c\n", row, col, status[0]);
    status[1] = *(shell->grid+(row)*ncols+(col+2)); //NE
    //printf("NE: %c\n", status[1]);
    status[2] = *(shell->grid+(row+1)*ncols+(col+2)); //E
    //printf("E: %c\n", status[2]);
    status[3] = *(shell->grid+(row+2)*ncols+(col+2)); //SE
    //printf("SE: %c\n", status[3]);
    status[4] = *(shell->grid+(row+2)*ncols+(col+1)); //S
    //printf("S: %c\n", status[4]);
    status[5] = *(shell->grid+(row+2)*ncols+(col+0)); //SW
    //printf("SW: %c\n", status[5]);
    status[6] = *(shell->grid+(row+1)*ncols+(col+0)); //W
    ///printf("W: %c\n", status[6]);
    status[7] = *(shell->grid+(row)*ncols+(col+0)); //NW
    //printf("NW: %c\n", status[7]);
    
    return status;

}
char update_cell(char cell, char* nbs){
    
    int rcount = 0;
    int gcount = 0;
    int xcount = 0;

    for (int i = 0; i < 8; i++){
        if (nbs[i] == 'r'){ 
            rcount++;
            //printf("nbs[%d] = %c, r count = %d\n", i, nbs[i], rcount);
        }
        else if (nbs[i] == 'g'){
            gcount++;
            //printf("nbs[%d] = %c, g count = %d\n", i, nbs[i], gcount);
        }
        else{
            xcount++;
        }
        
    }
    //printf("xcount = %d\n", xcount);
    //printf("rcount = %d\n", rcount);
    //printf("gcount = %d\n", gcount);
    
    if (cell == 'r' || cell == 'g'){
        if ((rcount + gcount) > 3 || (rcount + gcount) < 2){
            return 'x';
        }
        else{
            return cell;
        }
    }
    
    if (cell == 'x'){
        if ((rcount + gcount == 3)){
            if (gcount >= rcount){
                return 'g';
            }
            else{
                return 'r';
            }
        }
        else{ return 'x';}
    }
    
}

Board* update_board(Board *b){
    int rows = b->rows;
    int cols = b->cols;
    
    Board* shell = new_shell(b);
    Board* new_b = new_board(rows, cols);
    
    
    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            char* nbs = neighbors(shell, row, col, shell->cols);
            char new_cell = update_cell(*(b->grid+row*cols+col), nbs);
            free(nbs);
            *(new_b->grid+row*cols+col) = new_cell;
            //printf("Updated [%d][%d] to %c\n", row, col, new_cell);
        }
    }
    free(b);
    free(b->grid);
    free(shell);
    free(shell->grid);
    
    return new_b;
}

Board* new_board(int rows, int cols){
    Board *board = (Board*)malloc(sizeof(Board));
    board->rows = rows;
    board->cols = cols;
    board->grid = (char*)malloc(sizeof(char)*rows*cols);

    return board;
}


 int get_count(Board *b, char color){

    int count = 0;

    for (int row = 0; row < b->rows; row++){
        for (int col = 0; col < b->cols; col++){
            if (*(b->grid+row*b->cols+col) == color){ count++; }
        }
    }
    return count;
}
/*
int main(void){
    
    int rows = 3;
    int cols = 10;

    char *directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    char grid[] = {'x', 'x', 'x', 'g', 'x', 'g', 'x', 'r', 'x', 'r',
                   'g', 'x', 'r', 'x', 'x', 'g', 'x', 'x', 'x', 'x',
                   'x', 'g', 'x', 'x', 'r', 'x', 'x', 'x', 'x', 'r'};
    
    Board* b = new_board(rows, cols);
    
    print_board(b, grid);

    Board* shell = new_shell(b);

    Board* new_b = new_board(rows, cols);
    //new_b = update_board(b);


    printf("\nShell: \n"); //print shell
    for (int row = 0; row < shell->rows; row++){
        for (int col = 0; col < shell->cols; col++){
            if (col == 0){ putchar('\n');}
            printf("%c ", *(shell->grid+row*shell->cols+col));
        }
    } 
    
    putchar('\n');

    

    for (int row = 0; row < rows; row++){
        for (int col = 0; col < cols; col++){
            char* nbs = neighbors(shell, row, col, shell->cols);
            char cell = update_cell(*(b->grid+row*cols+col), nbs);
            *(new_b->grid+row*cols+col) = cell;
            printf("Updated cel[%d][%d] to %c\n", row, col, cell);
        }
    }
    int redc = get_count(new_b, 'r');
    int greenc = get_count(new_b, 'g');

    printf("\nRed count: %d Green count: %d\n", redc, greenc);

    return 0;
} */