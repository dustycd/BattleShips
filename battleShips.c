
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

void initializeGrid(char grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '~'; 
        }
    }
}

/*two functions to be able to convert coordinates from letters to number to be able to use them in our array*/
int convertCoordinatesY(char coord[]){
    return coord[0] - 'A';
}
int convertCoordinatesX(char coord[]){
    return coord[1] - '1';
}
void displayGrid(char grid[SIZE][SIZE]) {
    printf("   A B C D E F G H I J\n"); 
    for (int i = 0; i < SIZE; i++) {
        if(i < 9)
        {
            printf(" %d ", i + 1);
        }
        else
        {
            printf("%d ", i + 1);
        }
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", grid[i][j]); 
        }
        printf("\n");
    }
}

void print_hidden_grid(char grid[SIZE][SIZE])
{
    printf("   A B C D E F G H I J\n"); 
    for(int i=0; i< SIZE; i++)
    {
        if(i < 9)
        {
            printf(" %d ", i + 1);
        }
        else
        {
            printf("%d ", i + 1);
        }
        for(int j=0; j<SIZE; j++)
        {
            if(grid[i][j] == 'B' || grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S')     /*Function to print the hidden grid*/
            {
                printf("%c ", '~');
            }
            else
            {
                printf("%c ", grid[i][j]);
            }
        }
        printf("\n");
    }
}


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

void fire(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode ,char coord[]) {
    
    int x ;
    int y =convertCoordinatesY(coord);

    printf("%d\n is y in fire", y);

    if (coord[1] >= '1' && coord[1] <= '9' && coord[2] == '\0') { // B3
        x = convertCoordinatesX(coord);
    } else if (coord[1] == '1' && coord[2] == '0') { // A10
        x = 9;  // Since '10' corresponds to index 9 in a 0-based system
    } else {
        printf("Invalid coordinates. You lose your turn.");
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


void artilleryHelper(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode, int x, int y) {
    if(oppGrid[x][y] == 'S') { //submarine
        oppGrid[x][y] = '*';
        Submarine->hits++;
        printf("Hit!");
    } else if(oppGrid[x][y] == 'C') { //carrier
        oppGrid[x][y] = '*';
        Carrier->hits++;
        printf("Hit!");
    } else if(oppGrid[x][y] == 'D') { // destroyer
        oppGrid[x][y] = '*';
        Destroyer->hits++;
        printf("Hit!");
    } else if(oppGrid[x][y] == 'B') { // battleship
        oppGrid[x][y] = '*';
        Battleship->hits++;
        printf("Hit!");
    } else if(mode == 0){ // else missed easy
        oppGrid[x][y] = 'o';
        printf("Miss!");
    } else {
        printf("Miss!");
    }
}

// check if this move is playable in main (playable when opponent sinks a ship)
void artillery(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode , char coord[]) { // NEEDS TESTING 

    /*hon b hal function lezem l 3emela yzabit ossit l coordinates cz b radar sweeps the coordinate l lezem nhota is the top-left one so
    eza eja hada haf B10 it will go off grid w eza hek idk eza bet bayin bs B10 aw its counted as invalid coordinate w b rouh dawro lezem
    nehke w nshouf kif badna nzabeta*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM
    // fixed for now
    
    int x = convertCoordinatesX(coord);
    int y;

    if(coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') {// B3
        y = coord[1] - '1';
    } else if(coord[1] >= '0' && coord[1] <= '9' && coord[2] >= '0' && coord[2] <= '9') {// A10
        y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
    } else {
        printf("Invalid coordinates.  You lose your turn.");
        return;
    }

    if(y == 10 && x < 10) {
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, x, y);
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, x+1, y);
    } else if(x == 10 && y < 10) {
        // remove up and down
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, x, y);
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, x, y+1);
    } else if(x == 10 && y == 10) {
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, x, y);
    } else {
        for(int i = x; i < x+2; i++) { // this part might be wrong. NEEDS TESTING
            for(int j = y; j < y+2; j++) {
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
}

int Radar_Sweep(char oppGrid[SIZE][SIZE],char coord[], Player* Player)
{

    int x = convertCoordinatesX(coord);
    int y;

    if(coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') {// B3
        y = coord[1] - '1';
    } else if(coord[1] >= '0' && coord[1] <= '9' && coord[2] >= '0' && coord[2] <= '9') {// A10
        y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
    } else {
        printf("Invalid coordinates.  You lose your turn.");
        return 0;
    }

    /*hon b hal function lezem l 3emela yzabit ossit l coordinates cz b radar sweeps the coordinate l lezem nhota is the top-left one so
    eza eja hada haf B10 it will go off grid w eza hek idk eza bet bayin bs B10 aw its counted as invalid coordinate w b rouh dawro lezem
    nehke w nshouf kif badna nzabeta*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
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

int SmokeScreen(char smokeGrid[][SIZE], Player *Player, char coord[]) {
    if (Player->UsedSmoke >= Player->AllowedSmokeScreen) {
        printf("No smoke screens left. You lose your turn.\n");
        return 1;
    }

    int x = convertCoordinatesX(coord);
    int y;

    if(coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') {// B3
        y = coord[1] - '1';
    } else if(coord[1] >= '0' && coord[1] <= '9' && coord[2] >= '0' && coord[2] <= '9') {// A10
        y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
    } else {
        printf("Invalid coordinates.  You lose your turn.");
        return 1;
    }

    // Validate coordinates
    if (x < 0 || x >= SIZE - 1 || y < 0 || y >= SIZE - 1) {
        printf("Invalid coordinates for smoke screen. You lose your turn.\n");
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

int Torpedo(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode, char input[]) {
    if (Player->AllowedTorpedo == 0) {
        printf("Torpedo has already been used. You lose your turn.\n");
        return 1;
    }

    int hit = 0;

    if ((input[0] >= 'A' && input[0] <= 'J') && input[1] == '\0') {
        // Column attack
        int x = input[0] - 'A';
        if (x < 0 || x >= SIZE) {
            printf("Invalid column. You lose your turn.\n");
            return 1;
        }
        for (int y = 0; y < SIZE; y++) {
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
                if (If_sunk(*ship, Player)) {
                    printf("You sunk the %s!\n", ship->name);
                }
                hit = 1;
            } else {
                if (mode == 0) {
                    oppGrid[y][x] = 'o'; // Mark miss in easy mode
                }
            }
        }
    } else {
        int y = atoi(input) - 1;
        if (y >= 0 && y < SIZE) {
            // Row attack
            for (int x = 0; x < SIZE; x++) {
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
                    if (If_sunk(*ship, Player)) {
                        printf("You sunk the %s!\n", ship->name);
                    }
                    hit = 1;
                } else {
                    if (mode == 0) {
                        oppGrid[y][x] = 'o'; // Mark miss in easy mode
                    }
                }
            }
        } else {
            printf("Invalid input. You lose your turn.\n");
            return 1;
        }
    }

    if (!hit) {
        printf("Torpedo missed!\n");
    }

    Player->AllowedTorpedo--; // Mark torpedo as used

    return 0;
}


/*the function place ships, ask the user to place coordinates of where he wanna put his ships*/
/*attention to coordinates same error with placement in case it was 10 or above and also here x and y are the same*///fixed for now
void placeShip(char grid[SIZE][SIZE] , Ship ship){
    char coord[3],orientation;
    int valid = 0;

    while(!valid){
        displayGrid(grid);
        printf("Enter starting coordinates for %s (ex: A3) :", ship.name);
        scanf("%s",coord);
        printf("\n");
        printf("Enter orientation (h for horizontal , v for vertical):");
        scanf(" %c", &orientation);

        int x = convertCoordinatesX(coord);
        int y;
        if(coord[1] >= '1' && coord[1] <= '9' && coord[2] == '\0') {
            y = convertCoordinatesY(coord);
            } 
            else if(coord[1] == '1' && coord[2] == '0' ) {// A10
            y = (coord[1] - '0') * 10 + (coord[2] - '1' - '0');
            }

        if(isValidShipPlacement(grid , x , y , ship.size , orientation) == 2){
            if(orientation == 'h'){
                for(int i = 0 ; i < ship.size; i++){
                    grid[x][y + i] = ship.letter;
                }
                //displayGrid(grid);
            }else if (orientation == 'v')
            {
                for(int i = 0 ; i < ship.size ; i++){
                    grid[x + i][y] = ship.letter;
                }
                //displayGrid(grid);
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
    displayGrid(grid);
}
/*the function isValidShipPlacement take the coordinates given by the user for a ship and its orientation to check
if its valid so to see if it exceeds grid size or no and if its overlaps with other ships*/


void ShowAvailableMoves(Player Player)
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
    char GridOne[SIZE][SIZE] = {'~'};
    char GridTwo[SIZE][SIZE] = {'~'};

    int difficulty; 
    char player1[20], player2[20];
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

    Player Player1 = { .numShips = 4, .radarSweeps = 3, .AllowedSmokeScreen = 0, . UsedSmoke = 0, .AllowedArtilery = 0, .AllowedTorpedo =0};
    Player Player2 = { .numShips = 4, .radarSweeps = 3, .AllowedSmokeScreen = 0, . UsedSmoke = 0, .AllowedArtilery = 0, .AllowedTorpedo =0};
    strcpy(Player1.name, player1);
    strcpy(Player2.name, player2);

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
        placeShip(GridOne, P1Battleship);
        placeShip(GridOne, P1Destroyer);
        placeShip(GridOne, P1Submarine);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridTwo, P2Carrier);
        placeShip(GridTwo, P2Battleship);
        placeShip(GridTwo, P2Destroyer);
        placeShip(GridTwo, P2Submarine);
    }else{
        placeShip(GridTwo, P2Carrier);
        placeShip(GridTwo, P2Battleship);
        placeShip(GridTwo, P2Destroyer);
        placeShip(GridTwo, P2Submarine);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridOne, P1Carrier);

        placeShip(GridOne, P1Battleship);
        placeShip(GridOne, P1Destroyer);
        placeShip(GridOne, P1Submarine);
    }


    char command[20];
    char coordi[3];
    int ifwon = 0;
    while (!ifwon){

            printf("its %s's turn to play!",ftp);
            if(firstPlayer == 0){
                ShowAvailableMoves(Player1);
                print_hidden_grid(GridOne);//TO DO
                printf("Enter your move (e.g.: Fire B3) : ");
                scanf("%s", command);


                /*hon we have many problems, first concerning the struct of ships with pointers and second with the players*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
                switch (command[0])
                {
                case 'F':
                    coordi[0]=command[4];
                    coordi[1]=command[5];
                    coordi[2]=command[6];
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi);
                    break;
                case 'R':
                    coordi[0]=command[5];
                    coordi[1]=command[6];
                    coordi[2]=command[7];
                    Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0]=command[5];
                    coordi[1]= command[6];
                    coordi[2]=command[7];
                    SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0]=command[9];
                    coordi[1]=command[10];
                    coordi[2]=command[11];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0]=command[7];
                    coordi[1]=command[8];
                    coordi[2]=command[9];
                    //Torpedo(GridTwo , )
                    /*till now i dont know how i should inclune the torpedo function here so after we fix it i will -ali saad*/
                default:
                    break;
                    }
                    /*here first players turn is done so we should add the grid for him to see the updates + i think we should track if 
                    any ships where sunk if yes we should say it here*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 


                printf("its %s's turn to play!",stp);

                ShowAvailableMoves(Player2);
                print_hidden_grid(GridTwo);//TO DO
                printf("Enter your move (e.g.: Fire B3) : ");
                scanf_s("%s", command);

                /*hon we have many problems, first concerning the struct of ships with pointers and second with the players*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
                switch (command[0])
                {
                case 'F':
                    coordi[0]=command[5];
                    coordi[1]=command[6];
                    coordi[2]=command[7];
                    fire( GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi);
                    break;
                case 'R':
                    coordi[0]=command[6];
                    coordi[1]=command[7];
                    coordi[2]=command[8];
                    Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0]=command[6];
                    coordi[1]= command[7];
                    coordi[2]=command[8];
                    SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0]=command[10];
                    coordi[1]=command[11];
                    coordi[2]=command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0]=command[8];
                    coordi[1]=command[9];
                    coordi[2]=command[10];
                    //Torpedo(GridTwo , )
                    /*till now i dont know how i should inclune the torpedo function here so after we fix it i will -ali saad*/
                default:
                    break;
                    }
                    /*here second players turn is done so we should add the grid for him to see the updates + i think we should track if 
                    any ships where sunk if yes we should say it here*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
                }else  if(firstPlayer == 1){
                ShowAvailableMoves(Player2);
                print_hidden_grid(GridTwo);//TO DO
                printf("Enter your move (e.g.: Fire B3) : ");
                scanf_s("%s", command);


                /*hon we have many problems, first concerning the struct of ships with pointers and second with the players*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
                switch (command[0])
                {
                case 'F':
                    coordi[0]=command[6];
                    coordi[1]=command[7];
                    coordi[2]=command[8];
                    fire(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi);
                    break;
                case 'R':
                    coordi[0]=command[6];
                    coordi[1]=command[7];
                    coordi[2]=command[8];
                    Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0]=command[6];
                    coordi[1]= command[7];
                    coordi[2]=command[8];
                    SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0]=command[10];
                    coordi[1]=command[11];
                    coordi[2]=command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0]=command[8];
                    coordi[1]=command[9];
                    coordi[2]=command[10];
                    //Torpedo(GridTwo , )
                    /*till now i dont know how i should inclune the torpedo function here so after we fix it i will -ali saad*/
                default:
                    break;
                    }
                    /*here first players turn is done so we should add the grid for him to see the updates + i think we should track if 
                    any ships where sunk if yes we should say it here*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 


                printf("its %s's turn to play!",stp);

                ShowAvailableMoves(Player1);
                print_hidden_grid(GridOne);//TO DO
                printf("Enter your move (e.g.: Fire B3) : ");
                scanf_s("%s", command);

                /*hon we have many problems, first concerning the struct of ships with pointers and second with the players*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
                switch (command[0])
                {
                case 'F':
                    coordi[0]=command[5];
                    coordi[1]=command[6];
                    coordi[2]=command[7];
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi);
                    break;
                case 'R':
                    coordi[0]=command[6];
                    coordi[1]=command[7];
                    coordi[2]=command[8];
                    Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0]=command[6];
                    coordi[1]= command[7];
                    coordi[2]=command[8];
                    SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0]=command[10];
                    coordi[1]=command[11];
                    coordi[2]=command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0]=command[8];
                    coordi[1]=command[9];
                    coordi[2]=command[10];
                    //Torpedo(GridTwo , )
                    /*till now i dont know how i should inclune the torpedo function here so after we fix it i will -ali saad*/
                default:
                    break;
                    }
                    /*here second players turn is done so we should add the grid for him to see the updates + i think we should track if 
                    any ships where sunk if yes we should say it here*///-ALI SAAD IF U DIDNT GET THE PROBLEM ASK HIM 
        }

    }
    
    return 0;
}

