#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "panda.h"
#include "structure.h"
#include "vectorInt.h"
#define SEASIZE 2
#define MAPSIZE 4
const int ORDER[19] = {16, 17, 18, 15, 11, 6, 2, 1, 0, 3,
                       7,  12, 13, 14, 10, 5, 4, 8, 9};
const int NUMBER[18] = {5, 2, 6,  3, 8, 10, 9, 12, 11,
                        4, 8, 10, 9, 4, 5,  6, 3,  11};
// const int TEAMCOLOR[5] = {255, 9, 75, 82, 196};
extern int TEAMCOLOR[5];
extern char resourceStr[6][10];

static void initPlayer(player *p) {
    p->card = create_vector_vectorInt();
    for (int i = 1; i <= 5; ++i) p->resource[i] = 0;
    p->knight = p->road = p->Score = 0;
    p->haveNode = create_vector_vectorInt();
    p->haveSide = create_vector_vectorInt();
    p->havePort = create_vector_vectorInt();
    p->type = 0;
}
static void freePlayer(player *p) {
    p->haveNode->free(p->haveNode);
    p->haveSide->free(p->haveSide);
    p->havePort->free(p->havePort);
    p->card->free(p->card);
    free(p);
}
static void shufflePlayer(player *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        player tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
static int dicePiece[13][2] = {0};
static int desertLoc = -1;
static node corner[54];
static side edge[72];
static port tradePort[9];
static player gamePlayer[6];
static piece land[19];
static int playerNumber = 0;
static int developCard[25];

void add_resources(player *p,enum resourceType resource, int num) //玩家, 資源種類, 數量
{
    p->resource[resource] += num;
}

void remove_resources(player *p,enum resourceType resource, int num) //玩家, 資源種類, 數量 
{
    if (p->resource[resource] - num < 0)
    {
        printf("Error: player does not have enough resources\n");
        return;
    }
    p->resource[resource] -= num;
}

int can_afford(player *p,enum resourceType resource, int num) //玩家, 資源種類, 數量 (回傳是否能夠支付)
{
    return p->resource[resource] >= num;
}

//差建造流程的道路條件更改
int build_road(player *p, int land_pos, int linkedSide_pos, piece land[19] ) //玩家, 地塊位置, 連接邊位置, 地塊陣列
{
    if (!can_afford(p, BRICKS, 1) || !can_afford(p, WOOD, 1)) {
        printf("Error: player does not have enough resources to build road\n");
        return 0;
    }

    if (land[land_pos].linkedSide[linkedSide_pos]->belong == PUBLIC) 
    {
        int check = 0;
        for (int l = 0; l < 2; ++l) 
        {
            if (land[land_pos].linkedSide[linkedSide_pos]->linkedNode[l] != NULL &&
                land[land_pos].linkedSide[linkedSide_pos]->linkedNode[l]->belong == p->type) 
            {
                check = 1;
                break;
            }
        }
        if (check) 
        {
            land[land_pos].linkedSide[linkedSide_pos]->belong = p->type;
            land[land_pos].linkedSide[linkedSide_pos]->type = ROAD;
        } 
        else 
        {
            printf("there had no your swttlement near here, input again\n");
            return 0;
        }
    } 
    else 
    {
        printf("there had people here, input again\n");
        return 0;
    }

    remove_resources(p, BRICKS, 1);
    remove_resources(p, WOOD, 1);
    return 1;
}

//差建造流程的道路條件更改
int build_road_free(player *p, int land_pos, int linkedSide_pos, piece land[19] ) //玩家, 地塊位置, 連接邊位置, 地塊陣列
{
    if (land[land_pos].linkedSide[linkedSide_pos]->belong == PUBLIC) 
    {
        int check = 0;
        for (int l = 0; l < 2; ++l) 
        {
            if (land[land_pos].linkedSide[linkedSide_pos]->linkedNode[l] != NULL &&
                land[land_pos].linkedSide[linkedSide_pos]->linkedNode[l]->belong == p->type) 
            {
                check = 1;
                break;
            }
        }
        if (check) 
        {
            land[land_pos].linkedSide[linkedSide_pos]->belong = p->type;
            land[land_pos].linkedSide[linkedSide_pos]->type = ROAD;
        } 
        else 
        {
            printf("there had no your swttlement near here, input again\n");
            return 0;
        }
    } 
    else 
    {
        printf("there had people here, input again\n");
        return 0;
    }

    return 1;
}

int build_village(player *p, int land_pos, int linkedNode_pos, piece land[19] ) 
{
    if (!can_afford(p, BRICKS, 1) || !can_afford(p, WOOD, 1) || !can_afford(p, WOOL, 1) || !can_afford(p, WHEAT, 1)) 
    {
        printf("Error: player does not have enough resources to build village\n");
        return 0;
    }

    if (land[land_pos].linkedNode[linkedNode_pos]->belong == PUBLIC) 
    {
        int check = 1;
        for (int l = 0; l < 3; ++l) 
        {
            if (land[land_pos].linkedNode[linkedNode_pos]->linkedNode[l] != NULL &&
                land[land_pos].linkedNode[linkedNode_pos]->linkedNode[l]->belong != PUBLIC) 
            {
                check = 0;
                break;
            }
        }
        if (check) 
        {
            land[land_pos].linkedNode[linkedNode_pos]->belong = p->type;
            land[land_pos].linkedNode[linkedNode_pos]->type = SWTTLEMENT;
        } 
        else 
        {
            printf("there had swttlement near here, input again\n");
            return 0;
        }
    } 
    else 
    {
        printf("there had people here, input again\n");
        return 0;
    }

    // if (player->num_villages == 0) {
    //     printf("Error: player %d does not have enough villages\n", player->id);
    //     return 0;
    // }
    
    remove_resources(p, BRICKS, 1);
    remove_resources(p, WOOD, 1);
    remove_resources(p, WOOL, 1);
    remove_resources(p, WHEAT, 1);

    return 1;
}

int build_city(player *p, int land_pos, int linkedNode_pos, piece land[19] ) 
{
    if (!can_afford(p, METAL, 3)|| !can_afford(p, WHEAT, 2)) {
        printf("Error: player does not have enough resources to build citu\n");
        return 0;
    }   

    if (land[land_pos].linkedNode[linkedNode_pos]->belong == p->type && land[land_pos].linkedNode[linkedNode_pos]->type == SWTTLEMENT) 
    {
        land[land_pos].linkedNode[linkedNode_pos]->type = CITY;
    } 
    else 
    {
        printf("there had no SWTTLEMENT here or SWTTLEMENT not belong to you, input again\n");
        return 0;
    }
    
    remove_resources(p, METAL, 3);
    remove_resources(p, WHEAT, 2);

    return 1;
}

int draw_devCard(player *p ,int developCard[25], int num) //玩家, 發展卡陣列, 現在抽到第幾張發展卡
{
    if( num == 25)
    {
        printf("Error: no more development card\n");
        return 0;
    }

    if (!can_afford(p, METAL, 1)|| !can_afford(p, WHEAT, 1) || !can_afford(p, WOOL, 1)) {
        printf("Error: player does not have enough resources to draw development card\n");
        return 0;
    }   

    p->card->push(p->card, developCard[num]);
    num++;

    remove_resources(p, METAL, 1);
    remove_resources(p, WHEAT, 1);
    remove_resources(p, WOOL, 1);

    return num;
}

int monopoly_card(player gamePlayer[6], int playerSum, int playerNum) //拿取所有玩家的某一種資源 所有玩家,玩家數量,奪取資源的玩家編號
{
    int resourceType;
    while(1)
    {
        printf("請輸入要拿取的資源種類：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType);
        if (resourceType < 1 || resourceType > 5)
        {
            printf("輸入錯誤，請重新輸入\n");
        }
        else
        {
            break;
        }
    }
    int num = 0;
    for (int i = 1; i <= playerSum; i++)
    {
        num += gamePlayer[i].resource[resourceType];
        gamePlayer[i].resource[resourceType] = 0;
    }
    gamePlayer[playerNum].resource[resourceType] = num;
    return 1;
}

int yearofplenty_card(player *p)  //拿取兩張任意資源
{
    int resourceType1, resourceType2;
    while(1)
    {
        printf("請輸入要拿取的第一種資源種類：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType1);
        if (resourceType1 < 1 || resourceType1 > 5)
        {
            printf("輸入錯誤，請重新輸入\n");
        }
        else
        {
            break;
        }
    }
    while(1)
    {
        printf("請輸入要拿取的第二種資源種類：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType2);
        if (resourceType2 < 1 || resourceType2 > 5)
        {
            printf("輸入錯誤，請重新輸入\n");
        }
        else
        {
            break;
        }
    }
    
    p->resource[resourceType1]++;
    p->resource[resourceType2]++;
    return 1;
}

int roadbuilding_card(player *p, piece land[19]) //免費建造兩條路 //輸入檢查
{
    int land_pos1, land_pos2;
    int linkedNode_pos1, linkedNode_pos2;
    printf("請輸入要蓋路的第一個板塊編號及路編號：\n");
    scanf("%d %d", &land_pos1, &linkedNode_pos1);
    //build_road_free(p, land_pos1, linkedNode_pos1, land);
    while(!(build_road_free(p, land_pos1, linkedNode_pos1, land)))
    {
        printf("請輸入要蓋路的第一個板塊編號及路編號：\n");
        scanf("%d %d", &land_pos1, &linkedNode_pos1);
    }
    printf("請輸入要蓋路的第二個板塊編號及路編號：\n");
    scanf("%d %d", &land_pos2, &linkedNode_pos2);
    //build_road_free(p, land_pos2, linkedNode_pos2, land);
    while(!(build_road_free(p, land_pos2, linkedNode_pos2, land)))
    {
        printf("請輸入要蓋路的第二個板塊編號及路編號：\n");
        scanf("%d %d", &land_pos2, &linkedNode_pos2);
    }
    
    return 1;
} 

int knight_card(player *p, player gamePlayer[6], int playerSum, piece land[19]) //移動強盜,從到達地周圍的一位玩家奪取一個資源（不用棄牌）
{
    int robber_pos;
    int robber_pos_origin;
    int robbed_player;
    for (size_t i = 0; i < 19; i++)
    {
        if(land[i].robber == 1)
        {
            land[i].robber = 0;
            robber_pos_origin = i;
            break;
        }
    }
    while (1)
    {
        printf("請輸入要移動強盜的板塊編號：\n");
        scanf("%d", &robber_pos);
        if (robber_pos == robber_pos_origin)
        {
            printf("Error: robber can't move to the same place\n");
        }
        else
        {
            break;
        }
    }
    land[robber_pos].robber = 1;

    while(1)
    {
        int check1 = 1;
        int check2 = 1;
        int check3 = 0;
        printf("請輸入要奪取資源的玩家編號：\n");
        scanf("%d", &robbed_player);
        if (robbed_player <= 0 && robbed_player > playerSum )
        {
            printf("please enter correct player number\n");
            check1 = 0;
        }
        if (robbed_player == p->type )
        {
            printf("please dont enter yourself\n");
            check2 = 0;
        }
        for (size_t i = 0; i < 6; i++)
        {
            if (land[robber_pos].linkedNode[i]->belong == robbed_player)
            {
                check3 = 1;
                break;
            }
        }
        if (check3 == 0)
        {
            printf("Error: this player no around this land\n");
        }

        if (check1 == 1 && check2 == 1 && check3 == 1)
        {
            break;
        }
        else
        {
            printf("Error: please enter again\n");
        }
    }

    int resource_num = 0;
    for (size_t i = 1; i < 6; i++)
    {
        resource_num += gamePlayer[robbed_player-1].resource[i];
    }
    if (resource_num == 0)
    {
        printf("this player has no resource\n");
        return 1;
    }
    
    srand(time(NULL));
    int random = rand() % 5 + 1; //產生1到5的亂數
    if (gamePlayer[robbed_player-1].resource[random] > 0)
    {
        gamePlayer[robbed_player-1].resource[random]--;
        p->resource[random]++;
    }
    else
    {
        while (1)
        {
            random = rand() % 5 + 1;
            if (gamePlayer[robbed_player-1].resource[random] > 0)
            {
                gamePlayer[robbed_player-1].resource[random]--;
                p->resource[random]++;
                break;
            }
        }
    }

    return 1;
}

void setUpGame() {
    while (1) {
        printf("how mamy player?");
        scanf("%d", &playerNumber);
        if (playerNumber <= 4 && playerNumber >= 2) {
            break;
        } else {
            printf("it shoud in [2,4]\n");
        }
    }
    for (int i = 0; i < 14; ++i) developCard[i] = KNIGHT;
    for (int i = 14; i < 20; i += 2)
        developCard[i] = developCard[i + 1] = (i >> 1) - 6;
    for (int i = 20; i < 25; ++i) developCard[i] = i - 16;
    shuffleInt(developCard, 25, 1000);
    for (int i = 2; i <= 12; ++i) {
        dicePiece[i][0] = -1;
        dicePiece[i][1] = -1;
    }
    for (int i = 0; i < 19; ++i) {
        initPiece(&(land[i]));
        if (i < 4)
            land[i].type = FOREST;
        else if (i < 8)
            land[i].type = ARABLE;
        else if (i < 12)
            land[i].type = FARM;
        else if (i < 15)
            land[i].type = HILL;
        else if (i < 18) {
            land[i].type = MINING;
        } else {
            land[i].type = DESERT;
        }
    }
    shuffle(land, 19, 1000);
    int cnt = 0;
    for (int i = 0; i < 19; ++i) {
        if (land[ORDER[i]].type == DESERT) {
            desertLoc = ORDER[i];
            land[ORDER[i]].number = 7;
            land[ORDER[i]].robber = 1;
        } else {
            if (dicePiece[NUMBER[cnt]][0] == -1)
                dicePiece[NUMBER[cnt]][0] = ORDER[i];
            else
                dicePiece[NUMBER[cnt]][1] = ORDER[i];
            land[ORDER[i]].number = NUMBER[cnt];
            ++cnt;
        }
    }
    // printf("test");
    for (int i = 0; i < 54; ++i) {
        // printf("%d ", i);
        initNode(&(corner[i]));
        corner[i].index = i;
    }
    for (int i = 0; i < 72; ++i) {
        initSide(&(edge[i]));
        edge[i].index = i;
    }
    for (int i = 0; i < 4; ++i) {
        tradePort[i].type = 0;
        tradePort[i].request = 3;
    }
    for (int i = 1; i <= 5; ++i) {
        tradePort[i + 3].type = i;
        tradePort[i + 4].request = 2;
    }
    shufflePort(tradePort, 9, 1000);
    corner[0].nearPort = &(tradePort[0]);
    corner[1].nearPort = &(tradePort[0]);
    corner[3].nearPort = &(tradePort[1]);
    corner[4].nearPort = &(tradePort[1]);
    corner[14].nearPort = &(tradePort[2]);
    corner[15].nearPort = &(tradePort[2]);
    corner[7].nearPort = &(tradePort[3]);
    corner[17].nearPort = &(tradePort[3]);
    corner[26].nearPort = &(tradePort[5]);
    corner[37].nearPort = &(tradePort[5]);
    corner[28].nearPort = &(tradePort[4]);
    corner[38].nearPort = &(tradePort[4]);
    corner[45].nearPort = &(tradePort[7]);
    corner[46].nearPort = &(tradePort[7]);
    corner[47].nearPort = &(tradePort[6]);
    corner[48].nearPort = &(tradePort[6]);
    corner[50].nearPort = &(tradePort[8]);
    corner[51].nearPort = &(tradePort[8]);
    initGame(land, corner, edge);
    for (int i = 0; i < playerNumber; ++i) {
        initPlayer(&(gamePlayer[i]));
        gamePlayer[i].type = i + 1;
    }
    shufflePlayer(gamePlayer, playerNumber, 100);
    int i, j;
    for (int k = 0; k < playerNumber; ++k) {
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your first swttlement:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedNode[j]->belong = gamePlayer[k].type;
                    land[i].linkedNode[j]->type = SWTTLEMENT;

                    break;
                } else {
                    printf("there had swttlement near here, input again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your first road:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,edge)\n"
                "0/    \\1\n"
                "2|land|3\n"
                "4\\    /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedSide[j]->belong == PUBLIC) {
                int check = 0;
                for (int l = 0; l < 2; ++l) {
                    printf("%d %d\n",
                           land[i].linkedSide[j]->linkedNode[l]->index,
                           land[i].linkedSide[j]->linkedNode[l]->belong);
                    if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                        land[i].linkedSide[j]->linkedNode[l]->belong ==
                            gamePlayer[k].type) {
                        check = 1;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedSide[j]->belong = gamePlayer[k].type;
                    land[i].linkedSide[j]->type = ROAD;

                    break;
                } else {
                    printf(
                        "there had no your swttlement near here, input "
                        "again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
    for (int k = playerNumber - 1; k >= 0; --k) {
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your second swttlement:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedNode[j]->belong = gamePlayer[k].type;
                    land[i].linkedNode[j]->type = SWTTLEMENT;
                    printf("you got:");
                    for (int l = 0; l < 3; ++l) {
                        enum pieceType tmp =
                            land[i].linkedNode[j]->linkedPiece[l]->type;
                        if (tmp != DESERT) {
                            printf(" %s", resourceStr[tmp]);
                            gamePlayer[k].resource[tmp]++;
                        }
                    }
                    printf("\n");
                    break;
                } else {
                    printf("there had swttlement near here, input again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
        while (1) {
            printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
            printf("\e[38;5;%dmplayer %d\e[0m choose your second road:",
                   TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
            printf(
                "(land,edge)\n"
                "0/    \\1\n"
                "2|land|3\n"
                "4\\    /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedSide[j]->belong == PUBLIC) {
                int check = 0;
                for (int l = 0; l < 2; ++l) {
                    if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                        land[i].linkedSide[j]->linkedNode[l]->belong ==
                            gamePlayer[k].type) {
                        check = 1;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedSide[j]->belong = gamePlayer[k].type;
                    land[i].linkedSide[j]->type = ROAD;
                    break;
                } else {
                    printf(
                        "there had no your swttlement near here, input "
                        "again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
    // printMap(land, 19, tradePort);
}
int main() {
    setUpGame();
    for (int i = 0; i < playerNumber; ++i) {
        int state = 0;
        int step = 0;
        int number;
        int haveK = 0;
        for (int j = 0; j < gamePlayer[i].card->size; ++i) {
            if (gamePlayer[i].card->get(gamePlayer[i].card, j) == KNIGHT) {
                ++haveK;
            }
        }
        while (1) {
            if (state == 0) {  // doro
                printf("1.roll dice\n");
                if (haveK) {
                    printf("2. use Knight Card\n");
                }
                printf("your step:");
                scanf("%d", &step);

                if (step == 1) {
                    number = rollDice();
                    if (number == 7) {
                        robber();
                    } else {
                        giveResource(number);
                    }
                    state = 1;
                } else if (step == 2 && haveK) {
                    robber();
                }
            }
        }
    }
    // rollDice();
}