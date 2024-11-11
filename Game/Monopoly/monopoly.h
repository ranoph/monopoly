#ifndef MONOPOLY_H
#define MONOPOLY_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

// Constants
const int MAX_PLAYERS = 4;
const int BOARD_SIZE = 40;
const int MAX_NAME_LENGTH = 50;
const int STARTING_MONEY = 1500;
const int MAX_HOUSES = 4;
const int HOTEL = 5;
const int NUM_RAILROADS = 4;
const int NUM_UTILITIES = 2;
const int GET_OUT_OF_JAIL_COST = 50;
const int JAIL_POSITION = 10;

// Structure Definitions
struct Property {
    char name[MAX_NAME_LENGTH];
    int type;  // 0=SPECIAL, 1=REGULAR_PROPERTY, 2=RAILROAD, 3=TAX, 4=CHANCE, 5=COMMUNITY_CHEST
    int color; // -1=NONE, 0=BROWN, 1=LIGHT_BLUE, etc.
    int price;
    int baseRent;
    int rentWithSet;
    int rentWithHouses[4];
    int rentWithHotel;
    int houseCost;
    int owner;  // -1 for unowned
    int houses; // 0-4 for houses, 5 for hotel
    bool mortgaged;
};

struct Player {
    char name[MAX_NAME_LENGTH];
    int money;
    int position;
    bool inJail;
    int jailTurns;
    bool bankrupt;
    int getOutOfJailCards;
    int ownedProperties[BOARD_SIZE];
    int propertyCount;
};

struct Card {
    char text[MAX_NAME_LENGTH];
    int actionType;
    int actionValue;
};

struct GameState {
    Player players[MAX_PLAYERS];
    int numPlayers;
    int currentPlayer;
    bool gameOver;
    Card chanceCards[16];
    Card communityCards[16];
    int chanceIndex;
    int communityIndex;
    Property* board;
};

// Function declarations
void initializeBoard(Property[]);
void initializeCards(GameState*);
void initializePlayers(GameState*);
void rollDice(int*, int*);
bool isDouble(int, int);
void movePlayer(GameState*, Property[], int);
void handleProperty(GameState*, Property[], int);
void handleSpecialSpace(GameState*, Property[], int);
void handleChance(GameState*, Property[]);
void handleCommunityChest(GameState*, Property[]);
void displayGameState(const GameState*, const Property[]);
void displayProperty(const Property&);
void displayPlayerProperties(const GameState*, const Property[], int);
void tradeProperties(GameState*, Property[]);
bool hasMonopoly(const GameState*, const Property[], int, int);
void mortgageProperty(GameState*, Property[], int);
void unmortgageProperty(GameState*, Property[], int);
void buildHouse(GameState*, Property[], int);
void sellHouse(GameState*, Property[], int);
int calculateRent(const Property&, const GameState*, int, int);
bool canBuildHouse(const GameState*, const Property[], int);
void handleJailTurn(GameState*, Property[]);
void handleBankruptcy(GameState*, Property[]);
void goToJail(GameState*);
int countRailroadsOwned(const GameState* game, const Property* board, int playerNum);
int countUtilitiesOwned(const GameState* game, const Property* board, int playerNum);
bool hasMonopoly(const GameState* game, const Property* board, int playerNum, int propertyIndex);
void processPlayerTurn(GameState*, Property[]);
bool checkWinCondition(const GameState*);
void saveGame(const GameState*, const Property[]);
bool loadGame(GameState*, Property[]);
void displayMenu();
void goToJail(GameState* game);

#endif