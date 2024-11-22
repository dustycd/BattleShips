
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

void print_hidden_grid(char grid[SIZE][SIZE], int difficulty)
{
    if(difficulty == 0)
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
                if(grid[i][j] == 'B' || grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'x')     /*Function to print the hidden grid*/
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
    else
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
                if(grid[i][j] == 'B' || grid[i][j] == 'C' || grid[i][j] == 'D' || grid[i][j] == 'S' || grid[i][j] == 'o' || grid[i][j] == 'x')     /*Function to print the hidden grid*/
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
}


int isValidShipPlacement(char grid[SIZE][SIZE], int x, int y, int shipSize, char orientation){
    if(orientation == 'h' || orientation == 'H'){
        if(y + shipSize > SIZE) return 0;
        for (int i = 0 ; i < shipSize ; i++)
        {
            if(grid[x][y + i] != '~') return 1;
        }
    }else if (orientation == 'v' || orientation == 'V'){
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

void fire(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode ,char coord[], Player *Player) {

    int x ;
    int y =convertCoordinatesY(coord);

    if (coord[1] >= '1' && coord[1] <= '9' && coord[2] == '\0') { // B3
        x = convertCoordinatesX(coord);
    } else if (coord[1] == '1' && coord[2] == '0') { // A10
        x = 9;  // Since '10' corresponds to index 9 in a 0-based system
    } else {
        printf("Invalid coordinates. You lose your turn.");
        Player->AllowedSmokeScreen = 0;
        Player->AllowedArtilery = 0;
        return;
    } // line 67-76 changes coordinates of user (B3/ A10) to x and y so we can use them to acess the right place in the grid

        if(oppGrid[x][y] == 'S') { //submarine
            oppGrid[x][y] = '*';
            Submarine->hits++;
            printf("Hit!\n");
            if (If_sunk(*Submarine, Player)) {
                printf("You sunk the %s!\n", Submarine->name);
                Player->AllowedTorpedo = 1;
                Player->AllowedArtilery = 1;
                Player->AllowedSmokeScreen++;
            }
            else
            {
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
        }
         else if(oppGrid[x][y] == 'C') { //carrier
            oppGrid[x][y] = '*';
            Carrier->hits++;
            printf("Hit!\n");
            if (If_sunk(*Carrier, Player)) {
                printf("You sunk the %s!\n", Carrier->name);
                Player->AllowedTorpedo = 1;
                Player->AllowedArtilery = 1;
                Player->AllowedSmokeScreen++;
            }
            else
            {
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
        }
        else if(oppGrid[x][y] == 'D') { // destroyer
            oppGrid[x][y] = '*';
            Destroyer->hits++;
            printf("Hit!\n");
            if (If_sunk(*Destroyer, Player)) {
                printf("You sunk the %s!\n", Destroyer->name);
                Player->AllowedTorpedo = 1;
                Player->AllowedArtilery = 1;
                Player->AllowedSmokeScreen++;
            }
            else
            {
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
        }
        else if(oppGrid[x][y] == 'B') { // battleship
            oppGrid[x][y] = '*';
            Battleship->hits++;
            printf("Hit!\n");
            if (If_sunk(*Battleship, Player)) {
                printf("You sunk the %s!\n", Battleship->name);
                Player->AllowedTorpedo = 1;
                Player->AllowedArtilery = 1;
                Player->AllowedSmokeScreen++;
            }
            else
            {
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
        }else if(oppGrid[x][y] == '*'){
            printf("This coordinates has already been hit. You loose turn!");
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
        else if (mode == 0 && oppGrid[x][y] != 'S' && oppGrid[x][y] != 'C' && oppGrid[x][y] != 'D' && oppGrid[x][y] != 'B') { // else missed
            oppGrid[x][y] = 'o';
            printf("Miss!\n");
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
        else {
            printf("Miss!\n");
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
        
}


void artilleryHelper(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode, int x, int y) {
    if(oppGrid[x][y] == 'S') { //submarine
        oppGrid[x][y] = '*';
        Submarine->hits++;
        if (If_sunk(*Submarine, Player)) {
            printf("You sunk the %s!\n", Submarine->name);
            Player->AllowedTorpedo = 1;
            Player->AllowedArtilery = 1;
            Player->AllowedSmokeScreen++;
        }
        else
        {
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
        printf("Hit!");
    } else if(oppGrid[x][y] == 'C') { //carrier
        oppGrid[x][y] = '*';
        Carrier->hits++;
        printf("Hit!\n");
        if (If_sunk(*Carrier, Player)) {
            printf("You sunk the %s!\n", Carrier->name);
            Player->AllowedTorpedo = 1;
            Player->AllowedArtilery = 1;
            Player->AllowedSmokeScreen++;
        }
        else
        {
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
    } else if(oppGrid[x][y] == 'D') { // destroyer
        oppGrid[x][y] = '*';
        Destroyer->hits++;
        printf("Hit!\n");
        if (If_sunk(*Destroyer, Player)) {
            printf("You sunk the %s!\n", Destroyer->name);
            Player->AllowedTorpedo = 1;
            Player->AllowedArtilery = 1;
            Player->AllowedSmokeScreen++;
        }
        else
        {
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
    } else if(oppGrid[x][y] == 'B') { // battleship
        oppGrid[x][y] = '*';
        Battleship->hits++;
        printf("Hit!\n");
        if (If_sunk(*Battleship, Player)) {
            printf("You sunk the %s!\n", Battleship->name);
            Player->AllowedTorpedo = 1;
            Player->AllowedArtilery = 1;
            Player->AllowedSmokeScreen++;
        }
        else
        {
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
        }
        
    }
    else if(oppGrid[x][y] == '*'){
            printf("This coordinates has already been hit. You loose turn!");
            Player->AllowedTorpedo = 0;
            Player->AllowedArtilery = 0;
    } 
    else if(mode == 0 && oppGrid[x][y] != 'S' && oppGrid[x][y] != 'C' && oppGrid[x][y] != 'D' && oppGrid[x][y] != 'B'){ // else missed easy
        oppGrid[x][y] = 'o';
        printf("Miss!\n");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
    } else {
        printf("Miss!\n");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
    }
}


void artillery(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode , char coord[]) { // NEEDS TESTING 

    
    
    int x ;
    int y =convertCoordinatesY(coord);

    if (coord[1] >= '1' && coord[1] <= '9' && coord[2] == '\0') { // B3
        x = convertCoordinatesX(coord);
    } else if (coord[1] == '1' && coord[2] == '0') { // A10
        x = 9;  // Since '10' corresponds to index 9 in a 0-based system
    } else {
        printf("Invalid coordinates. You lose your turn.");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
        return;
    } 

    if(y == 10 && x < 10) {
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, Player , mode, x, y);
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine , Player, mode, x+1, y);
    } else if(x == 10 && y < 10) {
        // remove up and down
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine, Player, mode, x, y);
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine , Player, mode, x, y+1);
    } else if(x == 10 && y == 10) {
        artilleryHelper(oppGrid, Carrier, Battleship, Destroyer, Submarine , Player, mode, x, y);
    } else {
        for(int i = x; i <= x+1; i++) {
            for(int j = y; j <= y+1; j++) {
                if(oppGrid[i][j] == 'S') { //submarine
                oppGrid[i][j] = '*';
                Submarine->hits++;
                printf("Hit!\n");
                if (If_sunk(*Submarine, Player)) {
                    printf("You sunk the %s!\n", Submarine->name);
                    Player->AllowedTorpedo = 1;
                    Player->AllowedArtilery = 1;
                    Player->AllowedSmokeScreen++;
                }
                else
                {
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                }
            }
            else if(oppGrid[i][j] == 'C') { //carrier
                oppGrid[i][j] = '*';
                Carrier->hits++;
                printf("Hit!\n");
                if (If_sunk(*Carrier, Player)) {
                    printf("You sunk the %s!\n", Carrier->name);
                    Player->AllowedTorpedo = 1;
                    Player->AllowedArtilery = 1;
                    Player->AllowedSmokeScreen++;
                }
                else
                {
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                }
            }
            else if(oppGrid[i][j] == 'D') { // destroyer
                oppGrid[i][j] = '*';
                Destroyer->hits++;
                printf("Hit!\n");
                if (If_sunk(*Destroyer, Player)) {
                    printf("You sunk the %s!\n", Destroyer->name);
                    Player->AllowedTorpedo = 1;
                    Player->AllowedArtilery = 1;
                    Player->AllowedSmokeScreen++;
                }
                else
                {
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                }
            }
            else if(oppGrid[i][j] == 'B') { // battleship
                oppGrid[i][j] = '*';
                Battleship->hits++;
                printf("Hit!\n");
                if (If_sunk(*Battleship, Player)) {
                    printf("You sunk the %s!\n", Battleship->name);
                    Player->AllowedSmokeScreen++;
                    Player->AllowedArtilery = 1;
                    Player->AllowedTorpedo =1;
                }
                else
                {
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                }
            }
            else if(mode == 0){ // else missed easy
                oppGrid[i][j] = 'o';
                printf("Miss!\n");
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
            else {
                printf("Miss!\n");
                Player->AllowedTorpedo = 0;
                Player->AllowedArtilery = 0;
            }
            }
        }
    }
}

int Radar_Sweep(char oppGrid[SIZE][SIZE],char coord[], Player* Player)
{
    int found =0;
    int x = convertCoordinatesX(coord);
    int y = convertCoordinatesY(coord);

    if (coord[1] >= '0' && coord[1] <= '9' && coord[2] == '\0') { //"B3"
        y = convertCoordinatesY(coord);
    } else if (coord[1] == '1' && coord[2] == '0') { //"A10"
        y = 9; 
    } else {
        printf("Invalid coordinates. You lose your turn.\n");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
        return 0;
    }


    if(Player->radarSweeps == 0)
    {
        printf("No radar sweeps left. You lose your turn.\n");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
        return 1;
    }
    for(int i=x; i<(x+3); i++)
    {
        for(int j=y; j<(y+3); j++)
        {
            if(oppGrid[i][j] != 'o' && oppGrid[i][j] != '*' && oppGrid[i][j] != '~')
            {
                found =1;
                break;
            }
        }
    }
    if(found == 1)
    {
        printf("Enemy Ships Found.");
        Player->radarSweeps--;
    }
    else
    {
        printf("No enemy ships found");
    }
    Player->AllowedTorpedo = 0;
    Player->AllowedArtilery = 0;
    return 0;
}

int SmokeScreen(char grid[SIZE][SIZE], Player *player, char coord[]) {
    if (player->UsedSmoke >= player->AllowedSmokeScreen) {
        printf("No smoke screens left. You lose your turn.\n");
        return 1; // Turn lost
    }

    int x = convertCoordinatesX(coord);
    int y = convertCoordinatesY(coord);
    if (x == -1 || y < 0 || y >= SIZE) {
        printf("Invalid coordinates for smoke screen. You lose your turn.\n");
        return 1; // Turn lost
    }

    for (int i = x; i <= x + 1 && i < SIZE; i++) {
        for (int j = y; j <= y + 1 && j < SIZE; j++) {
            grid[i][j] = 'x'; // Mark cell as obscured
        }
    }

    printf("Smoke screen deployed at %c%d!\n", y + 'A', x + 1);
    player->UsedSmoke++;
    // Clear screen to preserve secrecy
    system("clear"); // For UNIX-based systems; use "cls" for Windows
    return 0; 
}

int Torpedo(char oppGrid[SIZE][SIZE], Ship *carrier, Ship *battleship, Ship *destroyer, Ship *submarine, Player *player, char target[]) {
    if (player->AllowedTorpedo <= 0) {
        printf("Torpedo is not unlocked yet or has already been used. You lose your turn.\n");
        return 1; // Turn lost
    }

    int hit = 0; // To track whether the torpedo hits any ship

    // Check if targeting a column (e.g., 'A' to 'J')
    if (strlen(target) == 1 && target[0] >= 'A' && target[0] <= 'J') {
        int col = target[0] - 'A'; // Convert column letter to index
        printf("Torpedo fired at column %c!\n", target[0]);

        // Iterate over all rows in the specified column
        for (int i = 0; i < SIZE; i++) {
            char cell = oppGrid[i][col];
            if (cell == carrier->letter || cell == battleship->letter || cell == destroyer->letter || cell == submarine->letter) {
                oppGrid[i][col] = '*'; // Mark hit
                hit = 1;

                // Update ship hits
                if (cell == carrier->letter) carrier->hits++;
                else if (cell == battleship->letter) battleship->hits++;
                else if (cell == destroyer->letter) destroyer->hits++;
                else if (cell == submarine->letter) submarine->hits++;

                // Check if the ship is sunk
                if (If_sunk(*carrier, player)) {
                    printf("You sunk the %s!\n", carrier->name);
                }
                if (If_sunk(*battleship, player)) {
                    printf("You sunk the %s!\n", battleship->name);
                }
                if (If_sunk(*destroyer, player)) {
                    printf("You sunk the %s!\n", destroyer->name);
                }
                if (If_sunk(*submarine, player)) {
                    printf("You sunk the %s!\n", submarine->name);
                }
            }
        }
    }
    // Check if targeting a row (e.g., "1" to "10")
    else if (atoi(target) >= 1 && atoi(target) <= SIZE) {
        int row = atoi(target) - 1; // Convert row number to index
        printf("Torpedo fired at row %d!\n", row + 1);

        // Iterate over all columns in the specified row
        for (int j = 0; j < SIZE; j++) {
            char cell = oppGrid[row][j];
            if (cell == carrier->letter || cell == battleship->letter || cell == destroyer->letter || cell == submarine->letter) {
                oppGrid[row][j] = '*'; // Mark hit
                hit = 1;

                // Update ship hits
                if (cell == carrier->letter) carrier->hits++;
                else if (cell == battleship->letter) battleship->hits++;
                else if (cell == destroyer->letter) destroyer->hits++;
                else if (cell == submarine->letter) submarine->hits++;

                // Check if the ship is sunk
                if (If_sunk(*carrier, player)) {
                    printf("You sunk the %s!\n", carrier->name);
                }
                if (If_sunk(*battleship, player)) {
                    printf("You sunk the %s!\n", battleship->name);
                }
                if (If_sunk(*destroyer, player)) {
                    printf("You sunk the %s!\n", destroyer->name);
                }
                if (If_sunk(*submarine, player)) {
                    printf("You sunk the %s!\n", submarine->name);
                }
            }
        }
    } else {
        printf("Invalid torpedo input. You lose your turn.\n");
        return 1; // Turn lost
    }

    if (hit) {
        printf("Torpedo hit enemy ships!\n");
    } else {
        printf("Torpedo missed.\n");
    }

    player->AllowedTorpedo = 0; // Torpedo can only be used once until re-unlocked
    return 0; // Successful turn
}

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
            if(orientation == 'h' || orientation == 'H'){
                for(int i = 0 ; i < ship.size; i++){
                    grid[x][y + i] = ship.letter;
                }
                //displayGrid(grid);
            }else if (orientation == 'v' || orientation == 'V')
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
int whoWins(Player  Player1, Player  Player2)
{
    if(Player1.numShips == 0 && Player2.numShips != 0)
    {
        return 2;
    }
    else if(Player2.numShips == 0 && Player1.numShips != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ShowAvailableMoves(Player Player)
{
    printf("\nAvailable Moves:\n");
    printf("1. Fire\n");
    printf("2. Radar (Available: %d)\n", Player.radarSweeps);
    printf("3. Smoke (Available: %d)\n", Player.AllowedSmokeScreen);
    printf("4. Artillery (Available: %d)\n", Player.AllowedArtilery);
    printf("5. Torpedo (Available: %d)\n", Player.AllowedTorpedo);
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
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");   
        fflush(stdout);
        placeShip(GridOne, P1Battleship);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridOne, P1Destroyer);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridOne, P1Submarine);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridTwo, P2Carrier);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Battleship);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Destroyer);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Submarine);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
    }else{
        placeShip(GridTwo, P2Carrier);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Battleship);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Destroyer);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridTwo, P2Submarine);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);

        printf("now its %s's turn to place his ships\n", stp);

        placeShip(GridOne, P1Carrier);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridOne, P1Battleship);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridOne, P1Destroyer);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
        placeShip(GridOne, P1Submarine);
        printf("\033c");    
        printf("\033[2J");  
        printf("\033[H");  
        fflush(stdout);
    }


    char command[20];
    char coordi[3];
    int ifwon = 0;
    int wonthegame;
    while (!ifwon){

            printf("\nits %s's turn to play!",ftp);
            if(firstPlayer == 0){
                ShowAvailableMoves(Player1);
                print_hidden_grid(GridTwo, difficulty);//TO DO
                printf("Enter your move (e.g.: Fire-B3) : ");
                scanf("%s", command);
                wonthegame = whoWins(Player1 , Player2);
                
                printf("\033c");    
                printf("\033[2J");  
                printf("\033[H");  
                fflush(stdout);

                switch (command[0])
                {
                case 'F':
                    coordi[0] = command[5];
                    coordi[1] = command[6];
                    coordi[2] = command[7];
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi , &Player1);
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[9];
                    Torpedo(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , Player2);
                    if(wonthegame == 1)
                    {
                        printf("%s won the game!!!", Player1.name);
                        ifwon =1;
                        break;
                    }
                    else if(wonthegame ==2)
                    {
                        printf("%s won the game!!!", Player2.name);
                        ifwon=1;
                        break;
                    }
                    
                printf("its %s's turn to play!",stp);

                

                ShowAvailableMoves(Player2);
                print_hidden_grid(GridOne, difficulty);//TO DO
                printf("Enter your move (e.g.: Fire-B3) : ");
                scanf_s("%s", command);

                printf("\033c");    
                printf("\033[2J");  
                printf("\033[H");  
                fflush(stdout);

                switch (command[0])
                {
                case 'F':
                    coordi[0] = command[5];
                    coordi[1] = command[6];
                    coordi[2] = command[7];
                    fire(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi , &Player2 );
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[9];
                    Torpedo(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , Player2);
                    if(wonthegame == 1){
                            printf("%s won the game!!!", Player1.name);
                            ifwon =1;
                            break;
                        }else if(wonthegame == 2){
                            printf("%s won the game!!!", Player2.name);
                            ifwon =1;
                            break;
                        }
                    
                }else  if(firstPlayer == 1){
                ShowAvailableMoves(Player2);
                print_hidden_grid(GridOne, difficulty);//TO DO
                printf("Enter your move (e.g.: Fire-B3) : ");
                scanf("%s", command);

                printf("\033c");    
                printf("\033[2J");  
                printf("\033[H"); 
                fflush(stdout);

                switch (command[0])
                {
                case 'F':
                    coordi[0] = command[5];
                    coordi[1] = command[6];
                    coordi[2] = command[7];
                    fire(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi , &Player2);
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[9];
                    Torpedo(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , Player2);
                    if(wonthegame == 1){
                            printf("%s won the game!!!", Player1.name);
                            ifwon =1;
                            break;
                        }else if(wonthegame == 2){
                            printf("%s won the game!!!", Player2.name);
                            ifwon =1;
                            break;
                        }


                printf("its %s's turn to play!",stp);

                ShowAvailableMoves(Player1);
                print_hidden_grid(GridTwo, difficulty);//TO DO
                printf("Enter your move (e.g.: Fire-B3) : ");
                scanf_s("%s", command);

                printf("\033c");    
                printf("\033[2J");  
                printf("\033[H"); 
                fflush(stdout);

                switch (command[0])
                {
                case 'F':
                    coordi[0] = command[5];
                    coordi[1] = command[6];
                    coordi[2] = command[7];
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi, &Player1);
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[9];
                    Torpedo(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , Player2);
                    if(wonthegame == 1){
                            printf("%s won the game!!!", Player1.name);
                            ifwon =1;
                            break;
                        }else if(wonthegame == 2){
                            printf("%s won the game!!!", Player2.name);
                            ifwon=1;
                            break;
                        }  
        }

    }
    
    return 0;
}

