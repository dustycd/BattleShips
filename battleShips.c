#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

void initializeGrid(char grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '~'; 
        }
    }
}

void displayGrid(char grid[SIZE][SIZE]) {
    printf("  A B C D E F G H I J\n"); 
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i + 1); 
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", grid[i][j]); 
        }
        printf("\n");
    }
}

int main() {
    char grid1[SIZE][SIZE], grid2[SIZE][SIZE];
    char difficulty; 
    char player1[10], player2[10];

    srand(time(0));

    initializeGrid(grid1);
    initializeGrid(grid2);

    printf("Choose difficulty level (E for Easy, H for Hard): ");
    scanf("%s", &difficulty);

    printf("Enter name for Player 1: ");
    scanf("%s", player1);
    printf("Enter name for Player 2: ");
    scanf("%s", player2);

    int firstPlayer = rand() % 2; 
    printf("%s will go first!\n", firstPlayer == 0 ? player1 : player2);

    /*while loop where the first player has to put his ships when hes done the second player*/



    /*attack phase*/
    while (/*both still have ships*/)
    {
        fires
        switch (/*turn to play*/)
        {
        case /* constant-expression */:
            /* code */
            break;
        
        default:
            break;
        }
    }
    


    return 0;
}
