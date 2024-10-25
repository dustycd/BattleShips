#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

typedef struct 
{
    char name[20];
    int size;
    int hits;
} Ship;


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

int If_sunk_(Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int * Ships)
{
    if(Carrier->size == Carrier->hits)
    {
        Ships--;
        return 1;
    }
    else if(Battleship->size == Battleship->hits)
    {
        Ships--;
        return 1;
    }
    else if(Destroyer->size == Destroyer->hits)
    {
        Ships--;
        return 1;
    }
    else if(Submarine->size == Submarine->hits)
    {
        Ships--;
        return 1;
    }
    else
    {
        return 0;
    }
}

void fire(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode) {
    char coord[3];
    printf("Enter coordinates: ");
    scanf("%s", coord);
    
    int x = coord[0] - 'A';
    int y;
    if(coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') {// B3
        y = coord[1] - '1';
    } else if(coord[1] >= '0' && coord[1] <= '9' && coord[2] >= '0' && coord[2] <= '9') {// A10
        y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
    } else {
        printf("Invalid coordinates");
        return;
    }

        if(oppGrid[x][y] == 'S') { //submarine
            oppGrid[x][y] = '*';
            Submarine->hits++;
            printf("Hit!");
        }
         else if(oppGrid[x][y] == 'C') { //carrier
            oppGrid[x][y] = '*';
            Carrier->hits++;
            printf("Hit!");
        }
        else if(oppGrid[x][y] == 'D') { // destroyer
            oppGrid[x][y] = '*';
            Destroyer->hits++;
            printf("Hit!");
        }
        else if(oppGrid[x][y] == 'B') { // battleship
            oppGrid[x][y] = '*';
            Battleship->hits++;
            printf("Hit!");
        }
        else if (mode == 0) { // else missed
            oppGrid[x][y] = 'o';
            printf("Miss!");
        }
        else {
            printf("Miss!");
        }
    

}

// check if this move is playable in main (playable when opponent sinks a ship)
void artillery(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode) {
    char coord[3];
    printf("Enter coordinates: ");
    scanf("%s", coord);
    
    int x = coord[0] - 'A';
    int y;
    if(coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') {// B3
        y = coord[1] - '1';
    } else if(coord[1] >= '0' && coord[1] <= '9' && coord[2] >= '0' && coord[2] <= '9') {// A10
        y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
    } else {
        printf("Invalid coordinates");
        return;
    }

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            if(oppGrid[i][j] == 'S') { //submarine
            oppGrid[i][j] = '*';
            Submarine->hits++;
            printf("Hit!");
        }
         else if(oppGrid[i][j] == 'C') { //carrier
            oppGrid[i][j] = '*';
            Carrier->hits++;
            printf("Hit!");
        }
        else if(oppGrid[i][j] == 'D') { // destroyer
            oppGrid[i][j] = '*';
            Destroyer->hits++;
            printf("Hit!");
        }
        else if(oppGrid[i][j] == 'B') { // battleship
            oppGrid[i][j] = '*';
            Battleship->hits++;
            printf("Hit!");
        }
        else if(mode == 0){ // else missed easy
            oppGrid[i][j] = 'o';
            printf("Miss!");
        }
        else {
            printf("Miss!");
        }
        }
    }
}

int main() {
    int p1_ships = 4;
    int p2_ships = 4;
    int rows = SIZE;
    int columns = SIZE;
    int** GridOne = (int**)malloc(rows * sizeof(int*));
    Ship P1Carrier= {
        .name = "Carrier",
        .size = 5,
        .hits = 0,
    };     
    Ship P1Battleship= {
        .name = "Battleship",
        .size = 4,
        .hits = 0,
    };       
    Ship P1Destroyer= {
        .name = "Destroyer",
        .size = 3,
        .hits = 0,
    };
    Ship P1Submarine= {
        .name = "Submarine",
        .size = 2,
        .hits = 0,
    };
    Ship P2Carrier= {
        .name = "Carrier",
        .size = 5,
        .hits = 0,
    };     
    Ship P2Battleship= {
        .name = "Battleship",
        .size = 4,
        .hits = 0,
    };       
    Ship P2Destroyer= {
        .name = "Destroyer",
        .size = 3,
        .hits = 0,
    };
    Ship P2Submarine= {
        .name = "Submarine",
        .size = 2,
        .hits = 0,
    };
    if(GridOne == NULL)
    {
        printf("Allocation for rows in grid1 has failed!");
        return -1;
    }
    for(int i= 0; i< rows; i++)
    {
        GridOne[i]= (int*)malloc(columns *sizeof(int));
        if(GridOne[i] == NULL)
        {
            printf("Allocation for columns in grid1 has failed!");
            return -1;
        }
    }

    int** GridTwo = (int**)malloc(rows * sizeof(int*));
    if(GridTwo == NULL)
    {
        printf("Allocation for rows in grid2 has failed!");
        return -1;
    }
    for(int i= 0; i< rows; i++)
    {
        GridTwo[i]= (int*)malloc(columns *sizeof(int));
        if(GridTwo[i] == NULL)
        {
            printf("Allocation for columns in grid2 has failed!");
            return -1;
        }
    }

    int** Hiddenp1 = (int**)malloc(rows * sizeof(int*));
    if(Hiddenp1 == NULL)
    {
        printf("Allocation for rows in Hiddenp1 has failed!");
        return -1;
    }
    for(int i= 0; i< rows; i++)
    {
        GridOne[i]= (int*)malloc(columns *sizeof(int));
        if(GridOne[i] == NULL)
        {
            printf("Allocation for columns in Hiddenp1 has failed!");
            return -1;
        }
    }

    int** Hiddenp2 = (int**)malloc(rows * sizeof(int*));
    if(Hiddenp2 == NULL)
    {
        printf("Allocation for rows in Hiddenp2 has failed!");
        return -1;
    }
    for(int i= 0; i< rows; i++)
    {
        GridOne[i]= (int*)malloc(columns *sizeof(int));
        if(GridOne[i] == NULL)
        {
            printf("Allocation for columns in Hiddenp2 has failed!");
            return -1;
        }
    }

    int difficulty; 
    char player1[10], player2[10];

    srand(time(0));

    initializeGrid(GridOne);
    initializeGrid(GridTwo);
    initializeGrid(Hiddenp1);
    initializeGrid(Hiddenp2);

    printf("Choose difficulty level (0 for Easy, 1 for Hard): ");
    scanf("%d", &difficulty);

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
