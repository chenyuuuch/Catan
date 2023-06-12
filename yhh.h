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
