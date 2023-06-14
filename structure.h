#ifndef _STRUCTURE_H
#define _STRUCTURE_H
#include "panda.h"
#include "vectorInt.h"
struct _node;
struct _side;
struct _piece;
struct _port;

enum TEAM {
    PUBLIC,
    PLAYER1,
    PLAYER2,  // com1
    PLAYER3,  // com2
    PLAYER4,  // com3
};

enum nodeType { NONE, SWTTLEMENT, CITY };
enum developType {
    KNIGHT,
    MONOPOLY,
    YEAROFPLENTY,
    ROADBUILDING,
    CHAPEL,
    GREATHALL,
    MARKET,
    LIBRARY,
    UNIVERSITY
};
enum sideType { None, ROAD };
enum pieceType { DESERT, FOREST, HILL, FARM, ARABLE, MINING };
enum resourceType { WOOD = 1, BRICKS, WOOL, WHEAT, METAL };

typedef struct _node {
    struct _node *linkedNode[3];
    struct _side *linkedSide[3];
    struct _piece *linkedPiece[3];
    struct _port *nearPort;
    enum nodeType type;
    enum TEAM belong;
    int index;
} node;
void initNode(node *n);
typedef struct _side {
    struct _node *linkedNode[2];
    enum sideType type;
    enum TEAM belong;
    int index;
} side;
void initSide(side *n);

typedef struct _piece {
    enum pieceType type;
    int robber;
    struct _side *linkedSide[6];
    struct _node *linkedNode[6];
    int number;
} piece;
void initPiece(piece *n);
typedef struct _port {
    int type;
    int request;
} port;
typedef struct _player {
    vectorInt *haveNode;
    vectorInt *haveSide;
    vectorInt *havePort;
    int Score;
    int knight;
    int road;
    enum TEAM type;
    vectorInt *card;
    int resource[6];
} player;
void initGame(piece *p, node *n, side *s);
int rollDice();
void shuffle(piece *p, int n, int times);
void shufflePort(port *p, int n, int times);
void shuffleInt(int *p, int n, int times);
void printMap(const piece *p, int n, const port *t, int size, int space);
void trade(int i, player *gamePlayer, port *tradePort);
void robber(int current_player, player *gamePlayer, int playerNumber,
                       piece *land);
void robberK(int current_player, player *gamePlayer, int playerNumber,
             piece *land);
void giveResource(int dice_number, piece *land, player *gamePlayer);
void knight_king(player *gamePlayer, int current_player, int playerNumber, int *knight_owner);

void add_resources(player *p,enum resourceType resource, int num); //玩家, 資源種類, 數量
void remove_resources(player *p,enum resourceType resource, int num); //玩家, 資源種類, 數量
int can_afford(player *p,enum resourceType resource, int num); //玩家, 資源種類, 數量 (回傳是否能夠支付)
//差建造流程的道路條件更改
int build_road(player *p, int land_pos, int linkedSide_pos, piece land[19] ); //玩家, 地塊位置, 連接邊位置, 地塊陣列
//差建造流程的道路條件更改
int build_road_free(player *p, int land_pos, int linkedSide_pos, piece land[19] ); //玩家, 地塊位置, 連接邊位置, 地塊陣列
int build_village(player *p, int land_pos, int linkedNode_pos, piece land[19] );
int draw_devCard(player *p ,int developCard[25], int num); //玩家, 發展卡陣列, 現在抽到第幾張發展
int monopoly_card(player gamePlayer[6], int playerSum, int playerNum);
int yearofplenty_card(player *p);  //拿取兩張任意資源
int roadbuilding_card(player *p, piece land[19]); //免費建造兩條路 //輸入檢查
int knight_card(player *p, player gamePlayer[6], int playerSum, piece land[19]); //移動強盜,從到達地周圍的一位玩家奪取一個資源（不用棄牌）
int someone_win(player *gamePlayer, int playerNumber, int knight_owner);
#endif