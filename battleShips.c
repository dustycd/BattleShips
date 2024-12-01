
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 10
double probabilityGrid[SIZE][SIZE];
char probabilityGridOpp[SIZE][SIZE];

typedef struct 
{
    char name[20];
    int size;
    int hits;
    char letter;
    int sink;
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

typedef struct {
    int x;
    int y;
    char shipLetter;
    int direction;
    int isInitialized;
} Coordinate;


static int countBattleShip = 0;
static int countDestroyer = 0;
static int countSubmarine = 0;
static int countCarrier = 0;
static Coordinate botHitBattleShip[10]; // initalize to size of the ship
static Coordinate botHitDestroyer[10]; // initialize to size of the ship
static Coordinate botHitSubmarine[10]; // initialize to size of the ship
static Coordinate botHitCarrier[10]; // initialize to size of the ship


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

int If_sunk(Ship *Ship, Player * Player)
{
    if(Ship->size == Ship->hits && Ship->sink ==0)
    {
        Player->numShips--;
        Ship->sink =1;
        return 1;
    }
    else if(Ship->size == Ship->hits && Ship->sink ==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void fire(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, int mode ,char coord[], Player *Player) {
    printf("entered the fire function\n");

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
            if (If_sunk(Submarine, Player)) {
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
            if (If_sunk(Carrier, Player)) {
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
            probabilityGridOpp[x][y] = '*';
            Destroyer->hits++;
            printf("Hit!\n");
            if (If_sunk(Destroyer, Player)) {
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
            if (If_sunk(Battleship, Player)) {
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
        if (If_sunk(Submarine, Player)) {
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
        if (If_sunk(Carrier, Player)) {
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
        if (If_sunk(Destroyer, Player)) {
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
        if (If_sunk(Battleship, Player)) {
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
                if (If_sunk(Submarine, Player)) {
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
                if (If_sunk(Carrier, Player)) {
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
                if (If_sunk(Destroyer, Player)) {
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
                if (If_sunk(Battleship, Player)) {
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
                if (oppGrid[i][j] != '*') {
                    oppGrid[i][j] = 'o';
                    printf("Miss!\n");
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                } else {
                    printf("Already Hit!\n");
                    Player->AllowedTorpedo = 0;
                    Player->AllowedArtilery = 0;
                }
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

int Radar_Sweep(char oppGrid[SIZE][SIZE], int smokeGrid[SIZE][SIZE], char coord[], Player* Player)
{
    int found = 0;
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

    if (Player->radarSweeps == 0) {
        printf("No radar sweeps left. You lose your turn.\n");
        Player->AllowedTorpedo = 0;
        Player->AllowedArtilery = 0;
        return 1;
    }

    for (int i = x; i < x + 3 && i < SIZE; i++) {
        for (int j = y; j < y + 3 && j < SIZE; j++) {
            // Skip cells with smoke screens or empty water
            if (smokeGrid[i][j] == 1 || oppGrid[i][j] == '~') {
                continue;
            }
            if (oppGrid[i][j] != 'o' && oppGrid[i][j] != '*' && oppGrid[i][j] != '~') {
                found = 1;
                break;
            }
        }
    }

    if (found == 1) {
        printf("Enemy Ships Found.\n");
        Player->radarSweeps--;
    } else {
        printf("No enemy ships found.\n");
    }

    Player->AllowedTorpedo = 0;
    Player->AllowedArtilery = 0;
    return 0;
}

int SmokeScreen(char grid[SIZE][SIZE], int smokeGrid[SIZE][SIZE], Player *player, char coord[]) {
    if (player->UsedSmoke >= player->AllowedSmokeScreen) {
        printf("No smoke screens left. You lose your turn.\n");
        return 1; // Turn lost
    }

    int x = convertCoordinatesX(coord);
    int y = convertCoordinatesY(coord);
    if (x < 0 || y < 0 || x >= SIZE || y >= SIZE) {
        printf("Invalid coordinates for smoke screen. You lose your turn.\n");
        return 1; // Turn lost
    }

    for (int i = x; i <= x + 1 && i < SIZE; i++) {
        for (int j = y; j <= y + 1 && j < SIZE; j++) {
            smokeGrid[i][j] = 1; // Mark cell as having an active smoke screen
        }
    }

    printf("Smoke screen deployed at %c%d!\n", y + 'A', x + 1);
    player->UsedSmoke++;
    return 0;
}

void Torpedo(char oppGrid[SIZE][SIZE], Ship *carrier, Ship *battleship, Ship *destroyer, Ship *submarine, Player *player, int mode, char coordi[]) {
    int x;
    int y =convertCoordinatesY(coordi);

    if (coordi[1] >= '1' && coordi[1] <= '9' && coordi[2] == '\0') { // B3
        x = convertCoordinatesX(coordi);
    } else if (coordi[1] == '1' && coordi[2] == '0') { // A10
        x = 9;  // Since '10' corresponds to index 9 in a 0-based system
    }
    
    char newcoord[3];
    char c = 'A';
    if(coordi[0] >= '1' && coordi[0] <= '9'){

        if(coordi[0] >= '1' && coordi[0] <= '9' && coordi[1] == '\0'){
            newcoord[1] = coordi[0];
        }else if(coordi[0] == '1' && coordi[1] == '0'){
            newcoord[1] = coordi[0];
            newcoord[2] = coordi[1];
        }else{
            printf("Invalid coordinates. You lose your turn.");
            player->AllowedTorpedo = 0;
            player->AllowedArtilery = 0;
            return;
        }
        for(int i = 0 ; i < 10 ; i++){
            newcoord[0] = c + i;
            fire(oppGrid , carrier ,battleship , destroyer , submarine , mode , newcoord , player);
        }
    }else{
        for(int i = 0; i < 10; i++) {
            artilleryHelper(oppGrid, carrier, battleship, destroyer, submarine, player , mode, i, y);
        }
    }
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



int isEmpty(Coordinate x[]) {
    if(x[0].isInitialized == 0) {
        return 1;
    } else {
        return 0;
    }
}

int isOutOfBounds(int x, int y) {
    if(x >= 11 || x <= -1 || y >= 11 || y <= -1) {
        return 1; // out of bounds
    } else {
        return 0;
    }
}

char intToChar(int number) {
    if (number >= 0 && number <= 9) {
        return 'A' + number;
    } else {
        // Handle invalid input
        printf("Invalid input: %d. Input must be between 0 and 9.\n", number);
        return '\0'; // Return null character for invalid input
    }
}

void botFire(char oppGrid[SIZE][SIZE], Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode , char coord[] , Coordinate *current) {
    printf("entered botfire function\n");
    //Coordinate current has to be initialized in main as default or empty so we can use it here
    int x;
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

    // we have to insert here first a while loop that checks if all the arrays (botHitSub, ..) are empty.
    // If not, we go to that specific array and fire there.
    printf("%s" , coord);
    puts("\n");
    while(isEmpty(botHitBattleShip) && isEmpty(botHitSubmarine) && isEmpty(botHitCarrier) && isEmpty(botHitDestroyer)) {
        printf("entered the first while loop that checks if isEmpty for everything\n");
        if(oppGrid[x][y] == 'S') {
            fire(oppGrid , Carrier , Battleship , Destroyer , Submarine , mode , coord , Player);
            probabilityGridOpp[x][y] = '*';
            printf(" SUBMARINE\n");
            Coordinate originalHitBotCoord = {x, y, 'S', 0, 1};
            current->x = x;
            current->y = y;
            current->shipLetter = 'S';
            current->direction = 0 ;
            current->isInitialized = 1;
            botHitSubmarine[countSubmarine] = originalHitBotCoord;
            countSubmarine++;
            return;
        } else if(oppGrid[x][y] == 'D') {
            fire(oppGrid , Carrier , Battleship , Destroyer , Submarine , mode , coord , Player);
            probabilityGridOpp[x][y] = '*';
            printf(" DESTROYER\n");
            Coordinate originalHitBotCoord = {x, y, 'D', 0, 1};
            current->x = x;
            current->y = y;
            current->shipLetter = 'D';
            current->direction = 0 ;
            current->isInitialized = 1;
            botHitDestroyer[countDestroyer] = originalHitBotCoord;
            countDestroyer++;
            return;
        } else if(oppGrid[x][y] == 'C' ) {
            fire(oppGrid , Carrier , Battleship , Destroyer , Submarine , mode , coord , Player);
            probabilityGridOpp[x][y] = '*';
            printf(" carrier\n");
            Coordinate originalHitBotCoord = {x, y, 'C', 0, 1};
            current->x = x;
            current->y = y;
            current->shipLetter = 'C';
            current->direction = 0 ;
            current->isInitialized = 1;
            botHitCarrier[countCarrier] = originalHitBotCoord;
            countCarrier++;
            return;
        } else if(oppGrid[x][y] == 'B') { 
            fire(oppGrid , Carrier , Battleship , Destroyer , Submarine , mode , coord , Player);
            probabilityGridOpp[x][y] = '*';
            printf(" BATTLESHIP\n");
            Coordinate originalHitBotCoord = {x, y, 'B', 0, 1};
            current->x = x;
            current->y = y;
            current->shipLetter = 'B';
            current->direction = 0 ;
            current->isInitialized = 1;
            botHitBattleShip[countBattleShip] = originalHitBotCoord;
            countBattleShip++;
            return;
        }else{
            fire(oppGrid , Carrier , Battleship , Destroyer , Submarine , mode , coord , Player);
            probabilityGridOpp[x][y] = 'o';
            // current->x = x;
            // current->y = y;
            // current->shipLetter = 'o';
            // current->direction = 0 ;
            // current->isInitialized = 1;
            //i think its a good idea like that we can see if its a hit or miss in current->shipletter and if hit we can check which ship
            return;

        }
    }

    if(!isEmpty(botHitBattleShip) && If_sunk(Battleship , Player) == 0) { //if didnt sink
        printf("entered battleship while loop\n");
        

        if (countBattleShip >= 10 || countBattleShip < 0) {
            printf("Error: countBattleShip out of bounds\n");
            return; // Handle the error gracefully
        }
        printf("botHitBattleShip[%d]: x=%d, y=%d, shipLetter=%c, isInitialized=%d\n", countBattleShip, botHitBattleShip[countBattleShip].x, botHitBattleShip[countBattleShip].y, botHitBattleShip[countBattleShip].shipLetter, botHitBattleShip[countBattleShip].isInitialized);
        int x = current->x;
        int y = current->y;
        //here x and y are 0 and 0 it shouldnt be like that

        printf("X: %d , Y: %d\n" , x , y);
        char letter;

        //we need to check if our current is or miss or hit 
        //if its hit it will continue normally but if its miss what we have to do is that we return at index 0
        int h = current->x;
        int i = current->y;
        if(oppGrid[h][i] == 'o'){
            current->x = botHitBattleShip[0].x;
            current->y = botHitBattleShip[0].y;
            x = current->x;
            y = current->y;
        }
        coord[0] = intToChar(y);
        if(x == 9) {
            
            coord[1]= '1';
            coord[2] = '0';
        } else {
            coord[1] = x + '1' ;
            coord[2] = '\0';
        }
        //we need to check if oppgrid at x and y is water if it is we have to back track
        //we were thinking about if we need to back track we delete all the components in the array except the original hit so at index 0
        if(!isOutOfBounds(x, y+1) && oppGrid[x][y+1] != '*' && oppGrid[x][y+1] != 'o') {
            printf("entered THE Y+1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y+1\n");
                if(oppGrid[x][y+1] == 'S' || oppGrid[x][y+1] == 'D' || oppGrid[x][y+1] == 'B' || oppGrid[x][y+1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y+1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitBattleShip[countBattleShip].x = current->x;
                    botHitBattleShip[countBattleShip].y = current->y;
                    botHitBattleShip[countBattleShip].direction = current->direction;
                    botHitBattleShip[countBattleShip].isInitialized = current->isInitialized;
                    botHitBattleShip[countBattleShip].shipLetter = current->shipLetter;

                    countBattleShip++;
                    
            
        
                    if(If_sunk(Battleship , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitBattleShip[i].isInitialized = 0;
                            botHitBattleShip[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 1;
                    }
                    probabilityGridOpp[x][y+1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y+1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->direction = 1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
        } else if(!isOutOfBounds(x, y-1) && oppGrid[x][y-1] != '*' && oppGrid[x][y-1] != 'o'){ 
        printf("entered the Y-1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y-1\n");
                if(oppGrid[x][y-1] == 'S' || oppGrid[x][y-1] == 'D' || oppGrid[x][y-1] == 'B' || oppGrid[x][y-1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y-1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitBattleShip[countBattleShip].x = current->x;
                    botHitBattleShip[countBattleShip].y = current->y;
                    botHitBattleShip[countBattleShip].direction = current->direction;
                    botHitBattleShip[countBattleShip].isInitialized = current->isInitialized;
                    botHitBattleShip[countBattleShip].shipLetter = current->shipLetter;

                    countBattleShip++;
                    // if(Battleship->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Battleship , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitBattleShip[i].isInitialized = 0;
                            botHitBattleShip[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -1;
                    }
                    probabilityGridOpp[x][y-1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y-1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->direction = -1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
            } else if(!isOutOfBounds(x-1,y) && !(oppGrid[x-1][y] == '*') && !(oppGrid[x-1][y] == 'o')) {
                 printf("entered the X-1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '1' && coord[2] == '0')//ex A10 
                        {
                            coord[1] = 9;
                            coord[2] = '\0';
                        }else{
                            coord[1]--;
                            coord[2] = '\0';
                        }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X-1\n");
                if(oppGrid[x-1][y] == 'S' || oppGrid[x-1][y] == 'D' || oppGrid[x-1][y] == 'B' || oppGrid[x-1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x-1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitBattleShip[countBattleShip].x = current->x;
                    botHitBattleShip[countBattleShip].y = current->y;
                    botHitBattleShip[countBattleShip].direction = current->direction;
                    botHitBattleShip[countBattleShip].isInitialized = current->isInitialized;
                    botHitBattleShip[countBattleShip].shipLetter = current->shipLetter;

                    countBattleShip++;
                    // if(Battleship->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Battleship , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitBattleShip[i].isInitialized = 0;
                            botHitBattleShip[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -2;
                    }
                    probabilityGridOpp[x-1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x-1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = -2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
            else if(!isOutOfBounds(x+1,y) && !(oppGrid[x+1][y] == '*') && !(oppGrid[x+1][y] == 'o')) {
                 printf("entered the X+1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '9') { //A9
                        coord[1] = 1;
                        coord[2] = 0;
                    }else{
                        coord[1]++;
                        coord[2] = '\0';
                    }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X+1\n");
                if(oppGrid[x+1][y] == 'S' || oppGrid[x+1][y] == 'D' || oppGrid[x+1][y] == 'B' || oppGrid[x+1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x+1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitBattleShip[countBattleShip].x = current->x;
                    botHitBattleShip[countBattleShip].y = current->y;
                    botHitBattleShip[countBattleShip].direction = current->direction;
                    botHitBattleShip[countBattleShip].isInitialized = current->isInitialized;
                    botHitBattleShip[countBattleShip].shipLetter = current->shipLetter;

                    countBattleShip++;

                    // if(Battleship->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Battleship , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitBattleShip[i].isInitialized = 0;
                            botHitBattleShip[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 2;
                    }
                    probabilityGridOpp[x+1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x+1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = 2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
    }
    
    if(!isEmpty(botHitSubmarine) && If_sunk(Submarine , Player) == 0) { //if didnt sink
        printf("entered submarine while loop\n");

        if (countSubmarine >= 10 || countSubmarine < 0) {
            printf("Error: countSubmarine out of bounds\n");
            return; // Handle the error gracefully
        }
        printf("botHitSubmarine[%d]: x=%d, y=%d, shipLetter=%c, isInitialized=%d\n", countSubmarine, botHitSubmarine[countSubmarine].x, botHitSubmarine[countSubmarine].y, botHitSubmarine[countSubmarine].shipLetter, botHitSubmarine[countSubmarine].isInitialized);
        int x = current->x;
        int y = current->y;
        //here x and y are 0 and 0 it shouldnt be like that

        printf("X: %d , Y: %d\n" , x , y);
        char letter;

        //we need to check if our current is or miss or hit 
        //if its hit it will continue normally but if its miss what we have to do is that we return at index 0
        int h = current->x;
        int i = current->y;
        if(oppGrid[h][i] == 'o'){
            current->x = botHitSubmarine[0].x;
            current->y = botHitSubmarine[0].y;
            x = current->x;
            y = current->y;
        }
        coord[0] = intToChar(y);
        if(x == 9) {
            
            coord[1]= '1';
            coord[2] = '0';
        } else {
            coord[1] = x + '1' ;
            coord[2] = '\0';
        }
        //we need to check if oppgrid at x and y is water if it is we have to back track
        //we were thinking about if we need to back track we delete all the components in the array except the original hit so at index 0
        if(!isOutOfBounds(x, y+1) && oppGrid[x][y+1] != '*' && oppGrid[x][y+1] != 'o') {
            printf("entered THE Y+1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y+1\n");
                if(oppGrid[x][y+1] == 'S' || oppGrid[x][y+1] == 'D' || oppGrid[x][y+1] == 'B' || oppGrid[x][y+1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y+1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitSubmarine[countSubmarine].x = current->x;
                    botHitSubmarine[countSubmarine].y = current->y;
                    botHitSubmarine[countSubmarine].direction = current->direction;
                    botHitSubmarine[countSubmarine].isInitialized = current->isInitialized;
                    botHitSubmarine[countSubmarine].shipLetter = current->shipLetter;

                    countSubmarine++;
                    // if(Submarine->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Submarine , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitSubmarine[i].isInitialized = 0;
                            botHitSubmarine[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 1;
                    }
                    probabilityGridOpp[x][y+1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y+1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->direction = 1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
        } else if(!isOutOfBounds(x, y-1) && oppGrid[x][y-1] != '*' && oppGrid[x][y-1] != 'o'){ 
        printf("entered the Y-1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y-1\n");
                if(oppGrid[x][y-1] == 'S' || oppGrid[x][y-1] == 'D' || oppGrid[x][y-1] == 'B' || oppGrid[x][y-1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y-1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitSubmarine[countSubmarine].x = current->x;
                    botHitSubmarine[countSubmarine].y = current->y;
                    botHitSubmarine[countSubmarine].direction = current->direction;
                    botHitSubmarine[countSubmarine].isInitialized = current->isInitialized;
                    botHitSubmarine[countSubmarine].shipLetter = current->shipLetter;

                    countSubmarine++;

                    // if(Submarine->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Submarine , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitSubmarine[i].isInitialized = 0;
                            botHitSubmarine[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -1;
                    }
                    probabilityGridOpp[x][y-1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y-1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->direction = -1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
            } else if(!isOutOfBounds(x-1,y) && !(oppGrid[x-1][y] == '*') && !(oppGrid[x-1][y] == 'o')) {
                 printf("entered the X-1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '1' && coord[2] == '0')//ex A10 
                        {
                            coord[1] = 9;
                            coord[2] = '\0';
                        }else{
                            coord[1]--;
                            coord[2] = '\0';
                        }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X-1\n");
                if(oppGrid[x-1][y] == 'S' || oppGrid[x-1][y] == 'D' || oppGrid[x-1][y] == 'B' || oppGrid[x-1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x-1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitSubmarine[countSubmarine].x = current->x;
                    botHitSubmarine[countSubmarine].y = current->y;
                    botHitSubmarine[countSubmarine].direction = current->direction;
                    botHitSubmarine[countSubmarine].isInitialized = current->isInitialized;
                    botHitSubmarine[countSubmarine].shipLetter = current->shipLetter;

                    countSubmarine++;

                    // if(Submarine->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Submarine , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitSubmarine[i].isInitialized = 0;
                            botHitSubmarine[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -2;
                    }
                    probabilityGridOpp[x-1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x-1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = -2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
            else if(!isOutOfBounds(x+1,y) && !(oppGrid[x+1][y] == '*') && !(oppGrid[x+1][y] == 'o')) {
                 printf("entered the X+1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '9') { //A9
                        coord[1] = 1;
                        coord[2] = 0;
                    }else{
                        coord[1]++;
                        coord[2] = '\0';
                    }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X+1\n");
                if(oppGrid[x+1][y] == 'S' || oppGrid[x+1][y] == 'D' || oppGrid[x+1][y] == 'B' || oppGrid[x+1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x+1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitSubmarine[countSubmarine].x = current->x;
                    botHitSubmarine[countSubmarine].y = current->y;
                    botHitSubmarine[countSubmarine].direction = current->direction;
                    botHitSubmarine[countSubmarine].isInitialized = current->isInitialized;
                    botHitSubmarine[countSubmarine].shipLetter = current->shipLetter;

                    countSubmarine++;

                    // if(Submarine->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Submarine , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitSubmarine[i].isInitialized = 0;
                            botHitSubmarine[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 2;
                    }
                    probabilityGridOpp[x+1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x+1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = 2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
    }




    if(!isEmpty(botHitCarrier) && If_sunk(Carrier , Player) == 0) { //if didnt sink
        printf("entered carrier while loop\n");

        if (countCarrier >= 10 || countCarrier < 0) {
            printf("Error: countCarrier out of bounds\n");
            return; // Handle the error gracefully
        }
        printf("botHitCarrier[%d]: x=%d, y=%d, shipLetter=%c, isInitialized=%d\n", countCarrier, botHitCarrier[countCarrier].x, botHitCarrier[countCarrier].y, botHitCarrier[countCarrier].shipLetter, botHitCarrier[countCarrier].isInitialized);
        int x = current->x;
        int y = current->y;
        //here x and y are 0 and 0 it shouldnt be like that

        printf("X: %d , Y: %d\n" , x , y);
        char letter;

        //we need to check if our current is or miss or hit 
        //if its hit it will continue normally but if its miss what we have to do is that we return at index 0
        int h = current->x;
        int i = current->y;
        if(oppGrid[h][i] == 'o'){
            current->x = botHitCarrier[0].x;
            current->y = botHitCarrier[0].y;
            x = current->x;
            y = current->y;
        }
        coord[0] = intToChar(y);
        if(x == 9) {
            
            coord[1]= '1';
            coord[2] = '0';
        } else {
            coord[1] = x + '1' ;
            coord[2] = '\0';
        }
        //we need to check if oppgrid at x and y is water if it is we have to back track
        //we were thinking about if we need to back track we delete all the components in the array except the original hit so at index 0
        if(!isOutOfBounds(x, y+1) && oppGrid[x][y+1] != '*' && oppGrid[x][y+1] != 'o') {
            printf("entered THE Y+1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y+1\n");
                if(oppGrid[x][y+1] == 'S' || oppGrid[x][y+1] == 'D' || oppGrid[x][y+1] == 'B' || oppGrid[x][y+1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y+1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitCarrier[countCarrier].x = current->x;
                    botHitCarrier[countCarrier].y = current->y;
                    botHitCarrier[countCarrier].direction = current->direction;
                    botHitCarrier[countCarrier].isInitialized = current->isInitialized;
                    botHitCarrier[countCarrier].shipLetter = current->shipLetter;

                    countCarrier++;

                    // if(Carrier->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Carrier , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitCarrier[i].isInitialized = 0;
                            botHitCarrier[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 1;
                    }
                    probabilityGridOpp[x][y+1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y+1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->direction = 1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
        } else if(!isOutOfBounds(x, y-1) && oppGrid[x][y-1] != '*' && oppGrid[x][y-1] != 'o'){ 
        printf("entered the Y-1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y-1\n");
                if(oppGrid[x][y-1] == 'S' || oppGrid[x][y-1] == 'D' || oppGrid[x][y-1] == 'B' || oppGrid[x][y-1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y-1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitCarrier[countCarrier].x = current->x;
                    botHitCarrier[countCarrier].y = current->y;
                    botHitCarrier[countCarrier].direction = current->direction;
                    botHitCarrier[countCarrier].isInitialized = current->isInitialized;
                    botHitCarrier[countCarrier].shipLetter = current->shipLetter;

                    countCarrier++;

                    // if(Carrier->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Carrier , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitCarrier[i].isInitialized = 0;
                            botHitCarrier[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -1;
                    }
                    probabilityGridOpp[x][y-1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y-1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->direction = -1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
            } else if(!isOutOfBounds(x-1,y) && !(oppGrid[x-1][y] == '*') && !(oppGrid[x-1][y] == 'o')) {
                 printf("entered the X-1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '1' && coord[2] == '0')//ex A10 
                        {
                            coord[1] = 9;
                            coord[2] = '\0';
                        }else{
                            coord[1]--;
                            coord[2] = '\0';
                        }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X-1\n");
                if(oppGrid[x-1][y] == 'S' || oppGrid[x-1][y] == 'D' || oppGrid[x-1][y] == 'B' || oppGrid[x-1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x-1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitCarrier[countCarrier].x = current->x;
                    botHitCarrier[countCarrier].y = current->y;
                    botHitCarrier[countCarrier].direction = current->direction;
                    botHitCarrier[countCarrier].isInitialized = current->isInitialized;
                    botHitCarrier[countCarrier].shipLetter = current->shipLetter;

                    countCarrier++;

                    // if(Carrier->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Carrier , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitCarrier[i].isInitialized = 0;
                            botHitCarrier[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -2;
                    }
                    probabilityGridOpp[x-1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x-1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = -2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
            else if(!isOutOfBounds(x+1,y) && !(oppGrid[x+1][y] == '*') && !(oppGrid[x+1][y] == 'o')) {
                 printf("entered the X+1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '9') { //A9
                        coord[1] = 1;
                        coord[2] = 0;
                    }else{
                        coord[1]++;
                        coord[2] = '\0';
                    }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X+1\n");
                if(oppGrid[x+1][y] == 'S' || oppGrid[x+1][y] == 'D' || oppGrid[x+1][y] == 'B' || oppGrid[x+1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x+1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitCarrier[countCarrier].x = current->x;
                    botHitCarrier[countCarrier].y = current->y;
                    botHitCarrier[countCarrier].direction = current->direction;
                    botHitCarrier[countCarrier].isInitialized = current->isInitialized;
                    botHitCarrier[countCarrier].shipLetter = current->shipLetter;

                    countCarrier++;

                    // if(Carrier->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Carrier , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitCarrier[i].isInitialized = 0;
                            botHitCarrier[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 2;
                    }
                    probabilityGridOpp[x+1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x+1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = 2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
    }
    
    if(!isEmpty(botHitDestroyer) && If_sunk(Destroyer , Player) == 0) { //if didnt sink
        printf("entered destroyer while loop\n");

        if (countDestroyer >= 10 || countDestroyer < 0) {
            printf("Error: countDestroyer out of bounds\n");
            return; // Handle the error gracefully
        }
        printf("botHitDestroyer[%d]: x=%d, y=%d, shipLetter=%c, isInitialized=%d\n", countDestroyer, botHitDestroyer[countDestroyer].x, botHitDestroyer[countDestroyer].y, botHitDestroyer[countDestroyer].shipLetter, botHitDestroyer[countDestroyer].isInitialized);
        int x = current->x;
        int y = current->y;
        //here x and y are 0 and 0 it shouldnt be like that

        printf("X: %d , Y: %d\n" , x , y);
        char letter;

        //we need to check if our current is or miss or hit 
        //if its hit it will continue normally but if its miss what we have to do is that we return at index 0
        int h = current->x;
        int i = current->y;
        if(oppGrid[h][i] == 'o'){
            current->x = botHitDestroyer[0].x;
            current->y = botHitDestroyer[0].y;
            x = current->x;
            y = current->y;
        }
        coord[0] = intToChar(y);  
        if(x == 9) {
            coord[1]= '1';
            coord[2] = '0';
        } else {
            coord[1] = x + '1' ;
            coord[2] = '\0';
        }
        //we need to check if oppgrid at x and y is water if it is we have to back track
        //we were thinking about if we need to back track we delete all the components in the array except the original hit so at index 0
        if(!isOutOfBounds(x, y+1) && oppGrid[x][y+1] != '*' && oppGrid[x][y+1] != 'o') {
            printf("entered THE Y+1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y+1\n");
                if(oppGrid[x][y+1] == 'S' || oppGrid[x][y+1] == 'D' || oppGrid[x][y+1] == 'B' || oppGrid[x][y+1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y+1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitDestroyer[countDestroyer].x = current->x;
                    botHitDestroyer[countDestroyer].y = current->y;              
                    botHitDestroyer[countDestroyer].direction = current->direction;
                    botHitDestroyer[countDestroyer].isInitialized = current->isInitialized;
                    botHitDestroyer[countDestroyer].shipLetter = current->shipLetter;

                    countDestroyer++;

                    // if(Destroyer->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Destroyer , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitDestroyer[i].isInitialized = 0;
                            botHitDestroyer[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 1;
                    }
                    probabilityGridOpp[x][y+1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]++; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y+1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y+1;
                    current->shipLetter = letter;
                    current->direction = 1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
        } else if(!isOutOfBounds(x, y-1) && oppGrid[x][y-1] != '*' && oppGrid[x][y-1] != 'o'){ 
        printf("entered the Y-1 out of bounds if statment\n");
                printf("check if already hit and if hit or miss IN Y-1\n");
                if(oppGrid[x][y-1] == 'S' || oppGrid[x][y-1] == 'D' || oppGrid[x][y-1] == 'B' || oppGrid[x][y-1] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x][y-1];
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    //when checking the coordinates it not taking it from the current but from the highest in the probability grid and its adding to it
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitDestroyer[countDestroyer].x = current->x;
                    botHitDestroyer[countDestroyer].y = current->y;
                    botHitDestroyer[countDestroyer].direction = current->direction;
                    botHitDestroyer[countDestroyer].isInitialized = current->isInitialized;
                    botHitDestroyer[countDestroyer].shipLetter = current->shipLetter;

                    countDestroyer++;

                    // if(Destroyer->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Destroyer , Player) == 1){
                        printf("entered if sunk if statement");
                        for(int i = 0 ; i < 5 ; i++){
                            botHitDestroyer[i].isInitialized = 0;
                            botHitDestroyer[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -1;
                    }
                    probabilityGridOpp[x][y-1] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x; //to the right
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("in case its not a ship its water\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    coord[0]--; // if this causes error, make new coordinate array
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                    puts("");
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x][y-1] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x;
                    current->y = y-1;
                    current->shipLetter = letter;
                    current->direction = -1;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                }
            } else if(!isOutOfBounds(x-1,y) && !(oppGrid[x-1][y] == '*') && !(oppGrid[x-1][y] == 'o')) {
                 printf("entered the X-1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '1' && coord[2] == '0')//ex A10 
                        {
                            coord[1] = 9;
                            coord[2] = '\0';
                        }else{
                            coord[1]--;
                            coord[2] = '\0';
                        }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X-1\n");
                if(oppGrid[x-1][y] == 'S' || oppGrid[x-1][y] == 'D' || oppGrid[x-1][y] == 'B' || oppGrid[x-1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x-1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitDestroyer[countDestroyer].x = current->x;
                    botHitDestroyer[countDestroyer].y = current->y;
                    botHitDestroyer[countDestroyer].direction = current->direction;
                    botHitDestroyer[countDestroyer].isInitialized = current->isInitialized;
                    botHitDestroyer[countDestroyer].shipLetter = current->shipLetter;

                    countDestroyer++;

                    // if(Destroyer->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Destroyer , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitDestroyer[i].isInitialized = 0;
                            botHitDestroyer[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = -2;
                    }
                    probabilityGridOpp[x-1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x-1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x-1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = -2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
            else if(!isOutOfBounds(x+1,y) && !(oppGrid[x+1][y] == '*') && !(oppGrid[x+1][y] == 'o')) {
                 printf("entered the X+1 out of bounds if statment\n");
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        if (coord[1] == '9') { //A9
                        coord[1] = 1;
                        coord[2] = 0;
                    }else{
                        coord[1]++;
                        coord[2] = '\0';
                    }
                    printf("COORD 0:%c\n" , coord[0]);
                    printf("COORD 1:%c\n" , coord[1]);
                    printf("COORD 2:%c\n" , coord[2]);
                        printf("check if already hit and if hit or miss IN X+1\n");
                if(oppGrid[x+1][y] == 'S' || oppGrid[x+1][y] == 'D' || oppGrid[x+1][y] == 'B' || oppGrid[x+1][y] == 'C') {
                    //its not entering this if statement its not checking right
                    printf("checked of which type of ship it is\n");
                    letter = oppGrid[x+1][y];
                    printf("before using the fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);

                    botHitDestroyer[countDestroyer].x = current->x;
                    botHitDestroyer[countDestroyer].y = current->y;
                    botHitDestroyer[countDestroyer].direction = current->direction;
                    botHitDestroyer[countDestroyer].isInitialized = current->isInitialized;
                    botHitDestroyer[countDestroyer].shipLetter = current->shipLetter;

                    countDestroyer++;

                    // if(Destroyer->sink == 0){
                    //     Player->numShips++;
                    // }
                    if(If_sunk(Destroyer , Player) == 1){
                        for(int i = 0 ; i < 5 ; i++){
                            botHitDestroyer[i].isInitialized = 0;
                            botHitDestroyer[i].direction = 0;

                        }
                        current->direction = 0;
                    }else{
                        current->direction = 2;
                    }
                    probabilityGridOpp[x+1][y] = '*';
                    printf("after using the fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1; //to the right
                    current->y = y;
                    current->shipLetter = letter;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    //we have to store coordinates current in the array
                } else {
                    printf("before fire function\n");
                    fire(oppGrid, Carrier, Battleship, Destroyer, Submarine, mode, coord, Player);
                    probabilityGrid[x+1][y] = 'o';
                    printf("after fire function\n");
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    current->x = x+1;
                    current->y = y;
                    current->shipLetter = letter;
                    current->direction = 2;
                    current->isInitialized = 1;
                    printf("%d", current->x);
                    printf("%d\n", current->y);
                    }
            }
    }

}

void updateProbabilityGrid(Ship ships[], int numShips) {
    
    for (int i = 0; i < SIZE; i++) { // initialize to 0 everytime
        for (int j = 0; j < SIZE; j++) {
            probabilityGrid[i][j] = 0.0;
        }
    }

    for (int k = 0; k < numShips; k++) {
        Ship ship = ships[k];
        if (ship.hits < ship.size) { // Ship is not sunk
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (probabilityGridOpp[i][j] == '~') { // Unhit water
                        // Check horizontal fit
                        int fitsHorizontally = 1;
                        for (int l = 0; l < ship.size; l++) {
                            if (j + l >= SIZE || probabilityGridOpp[i][j + l] != '~') {
                                fitsHorizontally = 0;
                                break;
                            }
                        }
                        if (fitsHorizontally) {
                            for (int l = 0; l < ship.size; l++) {
                                probabilityGrid[i][j + l] += 1.0;
                            }
                        }

                        // Check vertical fit
                        int fitsVertically = 1;
                        for (int l = 0; l < ship.size; l++) {
                            if (i + l >= SIZE || probabilityGridOpp[i + l][j] != '~') {
                                fitsVertically = 0;
                                break;
                            }
                        }
                        if (fitsVertically) {
                            for (int l = 0; l < ship.size; l++) {
                                probabilityGrid[i + l][j] += 1.0;
                            }
                        }
                    }
                }
            }
        }
    }
}

void findBestTarget(int *bestX, int *bestY, Coordinate *current) {
    double maxProbability = -1.0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (probabilityGrid[i][j] > maxProbability) {
                maxProbability = probabilityGrid[i][j];
                *bestX = i;
                *bestY = j;
            }
        }
    }
}

void debugPrintProbabilityGrid() {
    printf("Current Probability Grid:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%.2f ", probabilityGrid[i][j]);
        }
        printf("\n");
    }
}
int MediumMode(char grid[SIZE][SIZE], Ship ships[], int numShips, Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode, Coordinate *current, int *i, int*j)
{
    char coord[3];
    printf("entered medium mde\n");
    if(If_sunk(Carrier, Player)== 0 && current->direction ==0)
    {
        (*j) +=5;
        if(*j>=9)
        {
            (*i)++;
            *j=0;
        }
        if(*i>9)
        {
            *i=0;
        }
        coord[0] = 'A' + *j;
        coord[1] = '1' + *i;
        botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
        printf("CARRIER");
        return 0;
    }
    else if(If_sunk(Battleship, Player)== 0 && current->direction ==0)
    {
        (*j)+=4;
        if(*j>=9)
        {
            (*i)++;
            *j=0;
        }
        if(*i>9)
        {
            *i=0;
        }
        coord[0] = 'A' + *j;
        coord[1] = '1' + *i;
        printf("BATTLESHIP");
        botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
        return 0;
    }
    else if(If_sunk(Destroyer, Player)== 0 && current->direction ==0)
    {
        (*j)+=3;
        if(*j>=9)
        {
            (*i)++;
            *j=0;
        }
        if(*i>9)
        {
            *i=0;
        }
        coord[0] = 'A' + *j;
        coord[1] = '1' + *i;
        printf("DESTROYER");
        botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
        return 0;
    }
    else if(If_sunk(Submarine, Player)== 0 && current->direction ==0)
    {
        (*j)+=2;
        if(*j>=9)
        {
            (*i)++;
            *j=0;
        }
        if(*i>9)
        {
            *i=0;
        }
        coord[0] = 'A' + *j;
        coord[1] = '1' + *i;
        botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
        return 0;
    }
    else if(current->direction !=0)
    {
        botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
        return 0;
    }
    else
    return 1;
}

void HardMode(char grid[SIZE][SIZE], Ship ships[], int numShips, Ship *Carrier, Ship *Battleship, Ship *Destroyer, Ship *Submarine, Player *Player, int mode, Coordinate *current) {
    printf("entered hard mde\n");
    updateProbabilityGrid(ships, numShips);
    // debugPrintProbabilityGrid();
    printf("updated the probality grid by using the function\n");

    int bestX;
    int bestY;
    findBestTarget(&bestX, &bestY, current);
    printf("found the best target by calling the function\n");
    printf("Best Y: %d " , bestY);
    printf("Best X: %d " , bestX);
    char coord[3];
    coord[0] = 'A' + bestY;
    coord[1] = '1' + bestX;
    if (bestX == 9) { // Handle row 10
        coord[1] = '1';
        coord[2] = '0';
    } else {
        coord[2] = '\0';
    }

    printf("%c", coord[0]);
    printf("%c", coord[1]);
    printf("%c", coord[2]);


    botFire(grid, Carrier, Battleship, Destroyer, Submarine, Player, mode, coord, current);
    updateProbabilityGrid(ships, numShips);
    debugPrintProbabilityGrid();
    printf("Bot fire call done\n");
    
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

void PlayGame(int difficulty, char oppgrid[SIZE][SIZE], Ship botships[], Ship *P1Carrier, Ship *P1Battleship, Ship *P1Destroyer, Ship *P1Submarine, Player *Player1, int mode, Coordinate *current, int *i, int*j){
    printf("entered play game function\n");
    int numships = Player1->numShips;
    switch (difficulty)
    {
    case 1:
        /* code */
        break;
    case 2:
        printf("entered case 2 in switch\n");
        MediumMode(oppgrid , botships , numships , P1Carrier , P1Battleship , P1Destroyer , P1Submarine , Player1 , mode, current, i, j);
        printf("finished mediummode\n");
        break;
    case 3:
    printf("entered case 3 in switch\n");
        HardMode(oppgrid , botships , numships , P1Carrier , P1Battleship , P1Destroyer , P1Submarine , Player1 , mode, current);
        printf("finished hardmode\n");
        break;
    case 4:
        /* code */
        break;

    default:
        break;
    }   
}

int main() {
    int rows = SIZE;
    int columns = SIZE;
    Ship P1Carrier= {"Carrier", 5, 0, 'C', 0};     
    Ship P1Battleship= {"Battleship", 4, 0,'B', 0};       
    Ship P1Destroyer= {"Destroyer", 3, 0,'D', 0};
    Ship P1Submarine= {"Submarine", 2, 0,'S', 0};
    Ship P2Carrier= {"Carrier", 5, 0,'C', 0};     
    Ship P2Battleship= {"Battleship", 4, 0,'B', 0};       
    Ship P2Destroyer= {"Destroyer", 3, 0,'D', 0};
    Ship P2Submarine= {"Submarine", 2, 0,'S', 0};
    Ship BotCarrier= {"Carrier", 5, 0,'C', 0};     
    Ship BotBattleship= {"Battleship", 4, 0,'B', 0};       
    Ship BotDestroyer= {"Destroyer", 3, 0,'D', 0};
    Ship BotSubmarine= {"Submarine", 2, 0,'S', 0};

    char GridOne[SIZE][SIZE] = {'~'};
    char GridTwo[SIZE][SIZE] = {'~'};
    int mediumi=0;
    int mediumj=-1;
    initializeGrid(probabilityGridOpp);

    for (int i = 0; i < 10; i++) {
        botHitCarrier[i] = (Coordinate){0, 0, '\0', 0, 0};  // Initialize all fields
        botHitBattleShip[i] = (Coordinate){0, 0, '\0', 0, 0};
        botHitDestroyer[i] = (Coordinate){0, 0, '\0', 0, 0};
        botHitSubmarine[i] = (Coordinate){0, 0, '\0', 0, 0};
    }

    //ask if single player or multiplayer
    int howtoplay;
    printf("Choose : SinglePlayer(1)   OR    MultiPlayer(2): ");
    scanf("%d" , &howtoplay);
    puts("\n");
    if(howtoplay == 1){


        Coordinate current ={0, 0, 'X', 0, 0}; //should be initialized
        int difficulty;
        char p1[20];
        char ftp[10], stp[10]; /*ftp = first to play and stp = second to play*/

        int chooselevel;
        printf("Choose level : Easy(1)\t\tMedium(2)\t\tHard(3)\t\tImpossible(4) : ");
        scanf("%d" , &chooselevel);
        printf("------------------------------------------------------------------------------\n");


        srand(time(0));

        initializeGrid(GridOne); //for player 1
        initializeGrid(GridTwo); //for bot

        printf("Choose difficulty (0 for Easy, 1 for Hard): ");
        scanf("%d", &difficulty);
        puts("\n");

        printf("Enter your name : ");
        scanf("%s" , &p1);
        puts("\n");

        Player Player1 = { .numShips = 4, .radarSweeps = 3, .AllowedSmokeScreen = 0, . UsedSmoke = 0, .AllowedArtilery = 0, .AllowedTorpedo =0};
        Player CPU = { .numShips = 4, .radarSweeps = 3, .AllowedSmokeScreen = 0, . UsedSmoke = 0, .AllowedArtilery = 0, .AllowedTorpedo =0};

        strcpy(Player1.name, p1);
        strcpy(CPU.name, "Bot");

        Ship p1ships[] = {P1Submarine , P1Destroyer , P1Battleship , P1Carrier};
        int firstPlayer = rand() % 2;
        if (firstPlayer == 0) {
            strcpy(ftp, p1);
            strcpy(stp, "Bot");
        } else {
            strcpy(ftp, "Bot");
            strcpy(stp, p1);
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
        printf("Bot placing his ships...\n");
        Ship botShips[] = {BotCarrier, BotBattleship, BotDestroyer, BotSubmarine};
        for (int i = 0; i < 4; i++) {
        int placed = 0;
        while (!placed) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';

            if (isValidShipPlacement(GridTwo, x, y, botShips[i].size, orientation) == 2) {
                if (orientation == 'H') {
                    for (int j = 0; j < botShips[i].size; j++) {
                        GridTwo[x][y + j] = botShips[i].letter;
                    }
                } else if (orientation == 'V') {
                    for (int j = 0; j < botShips[i].size; j++) {
                        GridTwo[x + j][y] = botShips[i].letter;
                    }
                }
                placed = 1;
                }
            }
        }
        printf("Bot placed his ships.");

    }else{
        printf("Bot placing his ships...\n");
        Ship botShips[] = {BotCarrier, BotBattleship, BotDestroyer, BotSubmarine};
        for (int i = 0; i < 4; i++) {
        int placed = 0;
        while (!placed) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            char orientation = (rand() % 2 == 0) ? 'H' : 'V';

            if (isValidShipPlacement(GridTwo, x, y, botShips[i].size, orientation) == 2) {
                if (orientation == 'H') {
                    for (int j = 0; j < botShips[i].size; j++) {
                        GridTwo[x][y + j] = botShips[i].letter;
                    }
                } else if (orientation == 'V') {
                    for (int j = 0; j < botShips[i].size; j++) {
                        GridTwo[x + j][y] = botShips[i].letter;
                    }
                }
                placed = 1;
                }
            }
        }
        printf("Bot placed his ships.\n\n");

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
    while (ifwon ==0){
            printf("\nits %s's turn to play!",ftp);
            if(firstPlayer == 0){
                ShowAvailableMoves(Player1);
                print_hidden_grid(GridTwo, difficulty);//TO DO
                printf("Enter your move (e.g.: Fire-B3) : ");
                scanf("%s", command);
                wonthegame = whoWins(Player1 , CPU);
                
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
                    // Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
                    Torpedo(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , CPU);
                    if(wonthegame == 1)
                    {
                        printf("%s won the game!!!", Player1.name);
                        ifwon =1;
                        break;
                    }
                    else if(wonthegame ==2)
                    {
                        printf("%s won the game!!!", CPU.name);
                        ifwon=1;
                        break;
                    }
                    
                printf("its %s's turn to play!",stp);
                printf("Before play game call\n");
                PlayGame(chooselevel , GridOne , p1ships , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player1 , difficulty, &current, &mediumi, &mediumj);

                
                wonthegame = whoWins(Player1 , CPU);
                if(wonthegame == 1){
                        printf("%s won the game!!!", Player1.name);
                        ifwon =1;
                        break;
                    }else if(wonthegame == 2){
                        printf("%s won the game!!!", CPU.name);
                        ifwon =1;
                        break;
                    }
                }else  if(firstPlayer == 1){
                    printf("Before play game call\n");
                    PlayGame(chooselevel , GridOne , p1ships , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player1 , difficulty, &current, &mediumi, &mediumj);

                    wonthegame = whoWins(Player1 , CPU);
                    if(wonthegame == 1){
                            printf("%s won the game!!!", Player1.name);
                            ifwon =1;
                            break;
                        }else if(wonthegame == 2){
                            printf("%s won the game!!!", CPU.name);
                            ifwon =1;
                            break;
                        }


                printf("its %s's turn to play!",stp);

                ShowAvailableMoves(Player1);
                print_hidden_grid(GridTwo, difficulty);//TO DO
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
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi, &Player1);
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
                    Torpedo(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                default:
                    break;
                    }
                    wonthegame = whoWins(Player1 , CPU);
                    if(wonthegame == 1){
                            printf("%s won the game!!!", Player1.name);
                            ifwon =1;
                            break;
                        }else if(wonthegame == 2){
                            printf("%s won the game!!!", CPU.name);
                            ifwon=1;
                            break;
                        }  
        }

    }
    }else if(howtoplay == 2){
    
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
                    // Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
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
                    fire(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , difficulty , coordi , &Player2 );
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
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
                    // Radar_Sweep(GridOne , coordi , &Player2);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridTwo , &Player2 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridOne , &P1Carrier , &P1Battleship , &P1Destroyer , &P1Submarine , &Player2 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
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
                    fire(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , difficulty , coordi, &Player1);
                    break;
                case 'R':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // Radar_Sweep(GridTwo , coordi , &Player1);
                    break;
                case 'S':
                    coordi[0] = command[6];
                    coordi[1] = command[7];
                    coordi[2] = command[8];
                    // SmokeScreen(GridOne , &Player1 ,coordi);
                    break;
                case 'A':
                    coordi[0] = command[10];
                    coordi[1] = command[11];
                    coordi[2] = command[12];
                    artillery(GridTwo , &P2Carrier , &P2Battleship , &P2Destroyer , &P2Submarine , &Player1 , difficulty , coordi);
                    break;
                case 'T':
                    coordi[0] = command[8];
                    coordi[1] = command[9];
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
    }
    return 0;
}

