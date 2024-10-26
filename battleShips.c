#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10

typedef struct 
{
    char name[20];
    int size;
    int hits;
    char letter;
} Ship;

typedef struct 
{
    char name[20];
    int numShips;
    int radarSweeps;
    int AllowedSmokeScreen;
    int UsedSmoke;
    int AllowedArtilery;
    int AllowedTorpedo;
} Player;

/*void initializeGrid(char grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '~'; 
        }
    }
}*/

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

int If_sunk(Ship Ship, Player * Player)
{
    if(Ship.size == Ship.hits)
    {
        Player->numShips--;
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
    } // line 67-76 changes coordinates of user (B3/ A10) to x and y so we can use them to acess the right place in the grid

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
void artillery(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode) { // NEEDS TESTING 
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

int Radar_Sweep(char oppGrid[SIZE][SIZE], int x, int y, Player* Player)
{
    if(Player->radarSweeps == 0)
    {
        printf("No radar sweeps left. You lose your turn.\n");
        return 1;
    }
    for(int i=x; i<(x+3); i++)
    {
        for(int j=y; j<(y+3); j++)
        {
            if(oppGrid[i][j] == 'o' || oppGrid[i][j] == '*' || oppGrid[i][j] == '~')
            {
                printf("No enemy ships found");
            }
            else
            {
                printf("Enemy ships found");
            }
        }
    }

    return 0;
}

int SmokeScreen(int smokeGrid[SIZE][SIZE], Player *Player) {
    if (Player->UsedSmoke >= Player->AllowedSmokeScreen) {
        printf("No smoke screens left. You lose your turn.\n");
        return 1;
    }

    char coord[3];
    printf("Enter top-left coordinate for smoke screen (e.g., B3): ");
    scanf("%s", coord);

    int x = coord[0] - 'A';
    int y = atoi(&coord[1]) - 1;

    // Validate coordinates
    if (x < 0 || x >= SIZE - 1 || y < 0 || y >= SIZE - 1) {
        printf("Invalid coordinates for smoke screen.\n");
        return 1;
    }

    // Apply smoke screen to a 2x2 area
    for (int i = x; i < x + 2; i++) {
        for (int j = y; j < y + 2; j++) {
            smokeGrid[i][j] = 1; // Mark cell as smoked
        }
    }

    Player->UsedSmoke++;

    return 0;
}

int Torpedo(char oppGrid[SIZE][SIZE], int smokeGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player) {
    if (Player->AllowedTorpedo == 0) {
        printf("Torpedo has already been used. You lose your turn.\n");
        return 1;
    }

    char input[3];
    printf("Enter row number (1-10) or column letter (A-J) for torpedo attack: ");
    scanf("%s", input);

    int hit = 0;

    if (input[0] >= 'A' && input[0] <= 'J') {
        // Column attack
        int x = input[0] - 'A';
        for (int y = 0; y < SIZE; y++) {
            if (smokeGrid[y][x] == 1) continue; // Skip smoked cells

            char cell = oppGrid[y][x];
            Ship *ship = NULL;

            // Check if the cell matches any ship
            if (cell == Carrier->letter) ship = Carrier;
            else if (cell == Battleship->letter) ship = Battleship;
            else if (cell == Destroyer->letter) ship = Destroyer;
            else if (cell == Submarine->letter) ship = Submarine;

            if (ship) {
                oppGrid[y][x] = 'X';
                ship->hits++;
                printf("Hit at %c%d!\n", x + 'A', y + 1);

                // Check if the ship is sunk
                if (If_sunk(*ship, Player->numShips)) {
                    printf("You sunk the %s!\n", ship->name);
                }
                hit = 1;
            }
        }
    } else {
        int y = atoi(input) - 1;
        if (y >= 0 && y < SIZE) {
            // Row attack
            for (int x = 0; x < SIZE; x++) {
                if (smokeGrid[y][x] == 1) continue; // Skip smoked cells

                char cell = oppGrid[y][x];
                Ship *ship = NULL;

                // Check if the cell matches any ship
                if (cell == Carrier->letter) ship = Carrier;
                else if (cell == Battleship->letter) ship = Battleship;
                else if (cell == Destroyer->letter) ship = Destroyer;
                else if (cell == Submarine->letter) ship = Submarine;

                if (ship) {
                    oppGrid[y][x] = '*';
                    ship->hits++;
                    printf("Hit at %c%d!\n", x + 'A', y + 1);

                    // Check if the ship is sunk
                    if (If_sunk(*ship, Player ->numShips)) {
                        printf("You sunk the %s!\n", ship->name);
                    }
                    hit = 1;
                }
            }
        } else {
            printf("Invalid input.\n");
            return 1;
        }

    }

    if (!hit) {
        printf("Torpedo missed!\n");
    }

    Player->AllowedTorpedo--; // Mark torpedo as used

    return 0;
}

/*two functions to be able to convert coordinates from letters to number to be able to use them in our array*/
int convertCoordinatesX(char coord[]){
    return coord[0] - 'A';
}
int convertCoordinatesY(char coord[]){
    return coord[1] - '1';
}


/*the function place ships, ask the user to place coordinates of where he wanna put his ships*/
void placeShip(char grid[SIZE][SIZE] , Ship ship){
    char coord[3],orientation;
    int valid = 0;

    while(!valid){
        printf("Enter starting coordinates for %s (ex: A3) :", ship.name);
        scanf("%s\n",coord);
        printf("Enter orientation (h for horizontal , v for vertical):");
        scanf("%c", &orientation);

        int x = convertCoordinatesX(coord);
        int y = convertCoordinatesY(coord);

        if(isValidShipPlacement(grid , x , y , ship.size , orientation) == 2){
            if(orientation == 'h'){
                for(int i = 0 ; i < ship.size; i++){
                    grid[x][y + i] = ship.letter;
                }
            }else if (orientation == 'v')
            {
                for(int i = 0 ; i < ship.size ; i++){
                    grid[x + i][y] = ship.letter;
                }
            }
            valid = 1;
        }else if (isValidShipPlacement(grid , x , y , ship.size , orientation) == 1)
        {
            printf("Invalid placement,Ships overlap! Try again.\n");
        }else if (isValidShipPlacement(grid , x , y , ship.size , orientation) == 0)
        {
            printf("Invalid placement,Ships exceed grid area! Try again.\n");
        }else{
            printf("Invalid placement! Try again.\n");
        }
    }
}
/*the function isValidShipPlacement take the coordinates given by the user for a ship and its orientation to check
if its valid so to see if it exceeds grid size or no and if its overlaps with other ships*/
int isValidShipPlacement(char grid[SIZE][SIZE], int x, int y, int shipSize, char orientation){
    if(orientation == 'h'){
        if(y + shipSize > SIZE) return 0;
        for (int i = 0 ; i < shipSize ; i++)
        {
            if(grid[x][y + i] != '~') return 1;
        }
    }else if (orientation == 'v'){
        if(x + shipSize > SIZE) return 0;
        for(int i = 0 ; i < shipSize ; i++){
            if(grid[x + i][y] != '~') return 1;
        }
    }
    return 2;
    
}

void print_hidden_grid(char grid[SIZE][SIZE])
{
    for(int i=0; i< SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            if(grid[i][j] == 'B' || grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S')     /*Function to print the hidden grid*/
            {
                printf("%c ", '~');
            }
            else
            {
                printf("%d ", grid[i][j]);
            }
        }
    }
}

void print_grid(char grid[SIZE][SIZE])
{
    for(int i=0; i< SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            printf("%d ", grid[i][j]);
        }
    }
}

void ShowAvailableMoves(Player *Player)
{
    printf("Available Moves:\n");
    printf("1. Fire\n");
    printf("2. Radar\n");
    printf("3. Smoke\n");
    printf("4. Artillery\n");
    printf("5. Torpedo\n");
}
int main() {
    int rows = SIZE;
    int columns = SIZE;
    Ship P1Carrier= {"Carrier", 5, 0, 'C'};     
    Ship P1Battleship= {"Battleship", 4, 0,'B'};       
    Ship P1Destroyer= {"Destroyer", 3, 0,'D'};
    Ship P1Submarine= {"Submarine", 2, 0,'S'};
    Ship P2Carrier= {"Carrier", 5, 0,'C'};     
    Ship P2Battleship= {"Battleship", 4, 0,'B'};       
    Ship P2Destroyer= {"Destroyer", 3, 0,'D'};
    Ship P2Submarine= {"Submarine", 2, 0,'S'};
    int GridOne[SIZE][SIZE] = {'~'};
    int GridTwo[SIZE][SIZE] = {'~'};

    int difficulty; 
    char player1[10], player2[10];
    char ftp[10], stp[10]; /*ftp = first to play and stp = second to play*/

    srand(time(0));

    initializeGrid(GridOne);
    initializeGrid(GridTwo);

    printf("Choose difficulty level (0 for Easy, 1 for Hard): ");
    scanf("%d", &difficulty);

   printf("Enter name for Player 1: ");
    scanf("%s", player1);
    printf("Enter name for Player 2: ");
    scanf("%s", player2);

    Player Player1 = {player1, 4, 3, 0, 0, 0, 0, 0};
    Player Player2 = {player2, 4, 3, 0, 0, 0, 0, 0};

    int firstPlayer = rand() % 2; 
    if (firstPlayer == 0) {
        strcpy(ftp, player1);
        strcpy(stp, player2);
    } else {
        strcpy(ftp, player2);
        strcpy(stp, player1);
    }

    printf("%s will place his ships first\n", ftp);

    if(firstPlayer == 0){
        placeShip(GridOne, P1Carrier);
        print_grid(GridOne);
        placeShip(GridOne, P1Battleship);
        print_grid(GridOne);
        placeShip(GridOne, P1Destroyer);
        print_grid(GridOne);
        placeShip(GridOne, P1Submarine);
        print_grid(GridOne);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridTwo, P2Carrier);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Battleship);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Destroyer);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Submarine);
        print_grid(GridTwo);
    }else{
        placeShip(GridTwo, P2Carrier);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Battleship);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Destroyer);
        print_grid(GridTwo);
        placeShip(GridTwo, P2Submarine);
        print_grid(GridTwo);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridOne, P1Carrier);
        print_grid(GridOne);
        placeShip(GridOne, P1Battleship);
        print_grid(GridOne);
        placeShip(GridOne, P1Destroyer);
        print_grid(GridOne);
        placeShip(GridOne, P1Submarine);
        print_grid(GridOne);
    }

    int ifwon = 0;
    while (!ifwon){
        printf("its %s's turn to play!",ftp);
        if(firstPlayer == 0){
            print_hidden_grid(GridOne);//TO DO

        }

    }
    
    return 0;
}
