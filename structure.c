#include "structure.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "panda.h"
#include "vectorInt.h"
const int TEAMCOLOR[5] = {255, 93, 75, 82, 196};
const int PIECECOLOR[6] = {11, 28, 202, 145, 94, 237};
const int PORTCOLOR[6] = {241, 136, 255, 9, 226, 246};
const char PORTTEXT[6] = {'?', 'l', 'w', 'b', 'h', 'm'};
const char NODETYPE[3] = {' ', 's', 'c'};
const int NUMSHOW[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 0}, {1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 1}, {0, 1, 1, 1, 0, 1, 0}, {1, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1}, {1, 0, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1},
};

const char resourceStr[6][10] = {"None", "wood",  "bricks",
                                 "wool", "wheat", "metal"};
const char cardtoString[10][16] = {
    "None",   "Knight",    "Monopoly", "Year Of Plenty", "Road Building",
    "Chapel", "Greathall", "Market",   "Library",        "University"};
void initGame(piece *p, node *n, side *s) {
    // corner bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 3; ++j) {
            p[i].linkedNode[j] = &(n[(i << 1) + (i >= 3) + (i >= 7) +
                                     ((i >= 12) << 1) + (i >= 16) * 3 + j]);
            p[18 - i].linkedNode[j + 3] = &(n[51 -
                                              (i * 2 + (i >= 3) + (i >= 7) +
                                               (i >= 12) * 2 + (i >= 16) * 3) +
                                              j]);
        }
    }
    // side bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 2; ++j) {
            p[i].linkedSide[j] = &(s[i * 2 + (i >= 3) * 4 + (i >= 7) * 5 +
                                     (i >= 12) * 7 + (i >= 16) * 7 + j]);
            p[18 - i].linkedSide[j + 4] =
                &(s[70 -
                    (i * 2 + (i >= 3) * 4 + (i >= 7) * 5 + (i >= 12) * 7 +
                     (i >= 16) * 7) +
                    j]);
            p[i].linkedSide[j + 2] =
                &(s[i + j + 6 + (i >= 3) * 9 + (i >= 7) * 11 + (i >= 12) * 11 +
                    (i >= 16) * 9]);
        }
    }
    // bind node and node | side and side | node and piece
    for (int i = 0; i < 19; ++i) {
        p[i].linkedNode[0]->linkedNode[1] = p[i].linkedNode[1];
        p[i].linkedNode[0]->linkedNode[2] = p[i].linkedNode[3];
        p[i].linkedNode[1]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedNode[1]->linkedNode[2] = p[i].linkedNode[2];
        p[i].linkedNode[2]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedNode[2]->linkedNode[2] = p[i].linkedNode[5];
        p[i].linkedNode[3]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedNode[3]->linkedNode[2] = p[i].linkedNode[4];
        p[i].linkedNode[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedNode[4]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedNode[5]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedNode[5]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedNode[0]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[0]->linkedSide[2] = p[i].linkedSide[2];
        p[i].linkedNode[1]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[1]->linkedSide[2] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[0] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[2] = p[i].linkedSide[3];
        p[i].linkedNode[3]->linkedSide[0] = p[i].linkedSide[2];
        p[i].linkedNode[3]->linkedSide[2] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[0] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedNode[5]->linkedSide[0] = p[i].linkedSide[3];
        p[i].linkedNode[5]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedSide[0]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[0]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedSide[1]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[1]->linkedNode[1] = p[i].linkedNode[2];
        p[i].linkedSide[2]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedSide[2]->linkedNode[1] = p[i].linkedNode[3];
        p[i].linkedSide[3]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedSide[3]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedSide[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedSide[4]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedSide[5]->linkedNode[0] = p[i].linkedNode[5];
        p[i].linkedSide[5]->linkedNode[1] = p[i].linkedNode[4];

        for (int j = 0; j < 6; ++j) {
            p[i].linkedNode[j]->linkedPiece[2 - (int)(j / 2)] = &(p[i]);
        }
    }
}

void initPlayer(player *p) {
    p->card = create_vector_vectorInt();
    for (int i = 1; i <= 5; ++i) p->resource[i] = 0;
    p->knight = p->road = p->Score = 0;
    p->haveNode = create_vector_vectorInt();
    p->haveSide = create_vector_vectorInt();
    p->havePort = create_vector_vectorInt();
    p->type = 0;
}
void freePlayer(player *p) {
    p->haveNode->free(p->haveNode);
    p->haveSide->free(p->haveSide);
    p->havePort->free(p->havePort);
    p->card->free(p->card);
    free(p);
}
void shufflePlayer(player *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        player tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void initNode(node *n) {
    for (int i = 0; i < 3; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedPiece[i] = NULL;
        n->linkedPiece[i] = NULL;
    }
    n->nearPort = NULL;
    n->belong = 0;
    n->type = 0;
}
void initSide(side *n) {
    n->linkedNode[0] = NULL;
    n->linkedNode[1] = NULL;
    n->belong = 0;
    n->type = 0;
}
void initPiece(piece *n) {
    for (int i = 0; i < 6; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedSide[i] = NULL;
    }
    n->robber = 0;
    n->type = 0;
    n->number = 7;
}
int rollDice() {
    srand(time(NULL));
    int a, b;
    a = rand() % 6 + 1;
    b = rand() % 6 + 1;
    printf("roll %d and %d => %d\n", a, b, a + b);
    return a + b;
}

void shuffle(piece *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        piece tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shufflePort(port *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        port tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shuffleInt(int *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        int tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
static char numberofPiece(int x, int y, int num) {
    // printf("%d %d:%d\n", x, y, num);
    if (x == 0) {
        if (y == 0) {
            return ' ';
        } else if (y == 1) {
            return NUMSHOW[num][0] ? '_' : ' ';
        } else {
            return ' ';
        }
    } else if (x == 1) {
        if (y == 0) {
            return NUMSHOW[num][1] ? '|' : ' ';

        } else if (y == 1) {
            return NUMSHOW[num][3] ? '_' : ' ';
        } else {
            return NUMSHOW[num][2] ? '|' : ' ';
        }
    } else {
        if (y == 0) {
            return NUMSHOW[num][4] ? '|' : ' ';
        } else if (y == 1) {
            return NUMSHOW[num][6] ? '_' : ' ';
        } else {
            return NUMSHOW[num][5] ? '|' : ' ';
        }
    }
}
static void printpart(const piece *p, int l, int r, int t, int tl, int size,
                      int space) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 16)
                for (int j = 0; j < (size)*3; ++j) printf("\e[48;5;17m \e[0m");
            if (k == 16 || k == 12) {
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[0]
                                         ->linkedPiece[0]
                                         ->linkedNode[3]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[0]
                                        ->linkedPiece[0]
                                        ->linkedNode[3]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
                printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedNode[0]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[4]
                                                           ->belong]);
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
            }

            if (size - 1 == i)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[0]->belong],
                       NODETYPE[p[k].linkedNode[0]->type]);
            for (int j = 0; j < 1 + (size - i - 1) * 3 - (size - 1 == i); ++j) {
                if (p[k].linkedNode[1]->linkedPiece[0] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            }
            printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedSide[0]->belong]);

            for (int j = 0; j < 1 + 2 * i * 3; ++j)
                if (i)
                    printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[1]->belong],
                           NODETYPE[p[k].linkedNode[1]->type]);
                }
            printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedSide[1]->belong]);
            if (size - 1 == i && k == r)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[2]->belong],
                       NODETYPE[p[k].linkedNode[2]->type]);
            for (int j = 0; j < (size - i - 1) * 3 - (size - 1 == i) + (k == r);
                 ++j)
                if (p[k].linkedNode[1]->linkedPiece[1] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[1]->linkedPiece[1]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            if (k == 18 || k == 15) {
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[2]->linkedPiece[0]->type]);
                printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedNode[2]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[5]
                                                           ->belong]);
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[2]
                                         ->linkedPiece[0]
                                         ->linkedNode[5]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[2]
                                        ->linkedPiece[0]
                                        ->linkedNode[5]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
            }
            if (k == r) {
                if (r == 2)
                    for (int j = 0; j < (size)*6; ++j)
                        printf("\e[48;5;17m \e[0m");
                if (r == 6 || r == 18)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
                ;
            }
        }
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");

        printf("\n");
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0 || k == 16)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 12)
                for (int j = 0; j < (size)*3; ++j) printf("\e[48;5;17m \e[0m");
            printf("\e[38;5;%dm|\e[0m", TEAMCOLOR[p[k].linkedSide[2]->belong]);

            for (int j = 0; j < 2 * (size)*3 - 1; ++j) {
                if (j > (size)*3 - 1 && j < (size)*3 + 3 && i < 3) {
                    printf("\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                           numberofPiece(i, j - size * 3, p[k].number / 10));
                } else if (j > (size)*3 + 3 && j < (size)*3 + 7 && i < 3) {
                    printf(
                        "\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                        numberofPiece(i, j - size * 3 - 4, p[k].number % 10));
                } else {
                    if (j < (size)*3 - 3 && j >= (size)*3 - 5 && i >= 1 &&
                        i <= 2 && p[k].robber) {
                        printf("\e[48;5;240m \e[0m");
                    } else {
                        printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                    }
                }
            }
            if (k == r) {
                printf("\e[38;5;%dm|\e[0m",
                       TEAMCOLOR[p[k].linkedSide[3]->belong]);
                if (r == 2 || r == 18)
                    for (int j = 0; j < (size)*6; ++j)
                        printf("\e[48;5;17m \e[0m");
                if (r == 6 || r == 15)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
            }
        }
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");

        printf("\n");
    }
    if (l == 16) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < 6 * size; ++j) printf("\e[48;5;17m \e[0m");
            for (int k = l; k <= r; ++k) {
                if (i)
                    for (int j = 0; j < 3 * i + 1; ++j)
                        printf("\e[48;5;17m \e[0m");
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[3]->belong],
                           NODETYPE[p[k].linkedNode[3]->type]);
                }
                printf("\e[38;5;%dm＼\e[0m",
                       TEAMCOLOR[p[k].linkedSide[4]->belong]);
                if (i != size - 1)
                    for (int j = 0; j < (size - i - 1) * 6 + 1; ++j)
                        printf("\e[48;5;%dm \e[0m",
                               PIECECOLOR
                                   [p[k].linkedNode[4]->linkedPiece[0]->type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[4]->belong],
                           NODETYPE[p[k].linkedNode[4]->type]);
                }
                printf("\e[38;5;%dm／\e[0m",
                       TEAMCOLOR[p[k].linkedSide[5]->belong]);
                // if (i)
                for (int j = 0; j < 3 * i; ++j) printf("\e[48;5;17m \e[0m");
            }
            if (i == 0)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[r].linkedNode[5]->belong],
                       NODETYPE[p[r].linkedNode[5]->type]);
            else
                printf("\e[48;5;17m \e[0m");
            if (r == 18)
                for (int j = 0; j < (size)*6; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            printf("\n");
        }
    }
}
void printMap(const piece *p, int n, const port *t, int size, int space) {
    printpart(p, 0, 2, 5, t[3].type, size, space);
    printpart(p, 3, 6, 3, t[2].type, size, space);
    printpart(p, 7, 11, 1, t[5].type, size, space);
    printpart(p, 12, 15, 3, t[4].type, size, space);
    printpart(p, 16, 18, 5, t[7].type, size, space);
}

void trade(int i, player *gamePlayer, port *tradePort) {
  int give, get, give_amount;
  int best_trade_int[6] = {0, 4, 4, 4, 4, 4};

  // print resource
  printf("wood:%d bricks:%d wool:%d wheat:%d metal:%d\n",
         gamePlayer[i].resource[WOOD], gamePlayer[i].resource[BRICKS],
         gamePlayer[i].resource[WOOL], gamePlayer[i].resource[WHEAT],
         gamePlayer[i].resource[METAL]);
  // find the best trade among bank and port
  for (int port_idx = 0; port_idx < gamePlayer[i].havePort->size; port_idx++) {
    for (int resource_idx = 1; resource_idx < 6; resource_idx++) {
      int port_code =
          gamePlayer[i].havePort->get(gamePlayer[i].havePort, port_idx);
      if (tradePort[port_code].type == resource_idx ||
          tradePort[port_code].type == 0) {
        if (best_trade_int[resource_idx] > tradePort[port_code].request)
          best_trade_int[resource_idx] = tradePort[port_code].request;
      }
    }
  }

  // print best options
  printf("you can give %d %s or %d %s or %d %s or %d %s or %d %s or for one "
         "resource (1. wood 2. bricks 3. wool 4. wheat 5. metal): give, get",
         best_trade_int[1], resourceStr[1], best_trade_int[2], resourceStr[2],
         best_trade_int[3], resourceStr[3], best_trade_int[4], resourceStr[4],
         best_trade_int[5], resourceStr[5]);
  // choose resouce to trade
  scanf("%d %d", &give, &get);
  if (give > 5 || give < 1 || get > 5 || get < 1) {
    printf("\e[4;31m wrong input, try again\e[0m\n");
    return;
  }

  // check player resource amount
  if (gamePlayer[i].resource[give] < best_trade_int[give]) {
    printf("\e[4;31m not enough resources, try again\e[0m\n");
    return;
  }
  gamePlayer[i].resource[get] += 1;
  gamePlayer[i].resource[give] -= best_trade_int[give];
  return;
}
void robber(int current_player, player *gamePlayer, int playerNumber,
                       piece *land) {
  // give up resouces more than 7
  for (int i = 0; i < playerNumber; i++) {
    int give_amount = 0, total_input = 0;
    int array_give[6];
    for (int k = 1; k < 6; k++) {
      give_amount += gamePlayer[i].resource[k];
    }
    if (give_amount <= 7) {
      continue;
    }
    give_amount = (give_amount + (give_amount % 2)) / 2;
    printf("\e[38;5;%dmplayer %d \e[0m please choose resource to giveup\n", TEAMCOLOR[gamePlayer[i].type], i + 1);
    printf("wood:%d bricks:%d wool:%d wheat:%d metal:%d\n",
           gamePlayer[i].resource[WOOD], gamePlayer[i].resource[BRICKS],
           gamePlayer[i].resource[WOOL], gamePlayer[i].resource[WHEAT],
           gamePlayer[i].resource[METAL]);
    int red_flag = 0;
    while (give_amount != total_input || red_flag) {
      red_flag = 0;
      printf("%d amount of resource should be given, how would you give? input:wood, bricks, wool, wheat, metal\n",
             give_amount);
      scanf(" %d %d %d %d %d", &array_give[WOOD], &array_give[BRICKS],
            &array_give[WOOL], &array_give[WHEAT], &array_give[METAL]);
      total_input = 0;
      for (int k = 1; k < 6; k++) {
        total_input += array_give[k];
        if (gamePlayer[i].resource[k] < array_give[k] || array_give[k] < 0) {
          red_flag = 1;
          printf("\e[4;31m wrong input! please input legally\e[0m\n");
          break;
        }
      }
      if (red_flag == 0) {
        if (total_input != give_amount)
          printf("\e[4;31mwrong input! giveup amount should be %d \e[0m\n", give_amount);
        else {
          for (int k = 1; k < 6; k++)
            gamePlayer[i].resource[k] -= array_give[k];
        }
      }
    }
  }
  // move robber
  int robber_land, desert_land, input_land;
  for (int i = 0; i < 19; i++) {
    if (land[i].robber == 1)
      robber_land = i;
    if (land[i].type == DESERT)
      desert_land = i;
  }
  int bool_success = 0;
  while (!bool_success) {
    printf("which land would you move the robber to ? :");
    scanf(" %d", &input_land);
    if (input_land > 18 || input_land < 0) {
      continue;
    }
    if (input_land == robber_land || input_land == desert_land)
      printf("\e[4;31mwrong input! Desert is %d, original robber at %d\e[0m\n", desert_land,
             robber_land);
    else
      bool_success = 1;
  }
  land[input_land].robber = 1;
  land[robber_land].robber = 0;
  robber_land = input_land;
  //
  // steal resource
  int array_bool_player[4] = {0};
  int array_player_total[4] = {0};
  int bool_steal_able = 0;
  int steal_from; // 0 - 3
  for (int i = 0; i < 6; i++) {
    if (land[robber_land].linkedNode[i]->belong != PUBLIC)
      array_bool_player[land[robber_land].linkedNode[i]->belong - 1] = 1;
  }
  array_bool_player[current_player] = 0;
  for (int i = 0; i < 4; i++) {
    if (array_bool_player[i] == 1) {
      for (int k = 1; k < 6; k++) {
        array_player_total[i] += gamePlayer[i].resource[k];
      }
      if (array_player_total[i]) {
        printf("\e[38;5;%dmplayer %d \e[0mhas %d resources", TEAMCOLOR[gamePlayer[i].type], i + 1, array_player_total[i]);
        bool_steal_able = 1;
      } else
        array_bool_player[i] = 0; // 原本是候選人，但無資源所以移除
    }
  }
  int bool_select = 0;
  while (!bool_select && bool_steal_able) {
    printf(", choose one player to select their resources:");
    scanf(" %d", &steal_from); // input 1 - 4
    steal_from--;              // 0 - 3
    if (steal_from >= playerNumber || steal_from < 0) {
      printf("wrong input! Input illegal: out of player range\n");
      continue;
    }
    if (steal_from == current_player) {
      printf("wrong input! Input illegal: you chose Player %d yourself\n",
             current_player);
      continue;
    }
    if (array_bool_player[steal_from] == 0) {
      printf("wrong input! Input illegal: player not available\n");
    }
    bool_select = 1;
  }
  // get resource
  if (bool_select) {
    srand(time(NULL));
    int r = rand() % array_player_total[steal_from] + 1;
    int tmp = 0;
    while (r > 0) {
      tmp++;
      r -= gamePlayer[steal_from].resource[tmp];
    }
    gamePlayer[steal_from].resource[tmp] -= 1;
    gamePlayer[current_player].resource[tmp] += 1;
    printf("PLAYER %d steal %s from PLAYER %d\n", current_player + 1,
           resourceStr[tmp], steal_from + 1);
  } else {
    printf("no stealing\n");
  }
}
void giveResource(int dice_number, piece *land, player *gamePlayer) {
  if (dice_number == 2) {
    int the_land;
    for (int i = 0; i < 19; i++) {
      if (land[i].number == dice_number)
        the_land = i;
    }

    if (land[the_land].type == DESERT)
      return;
    for (int k = 0; k < 6; k++) {
      if (land[the_land].linkedNode[k]->belong == PUBLIC)
        continue;
      gamePlayer[land[the_land].linkedNode[k]->belong - 1]
          .resource[land[the_land].type] += 1;
    }
  } else {
    int the_land[2], idx = 0;
    for (int i = 0; i < 19; i++) {
      if (land[i].number == dice_number)
        the_land[idx++] = i;
    }
    for (int i = 0; i < 2; i++) {
      if (land[the_land[i]].type == DESERT)
        continue;
      for (int k = 0; k < 6; k++) {
        if (land[the_land[i]].linkedNode[k]->belong == PUBLIC)
          continue;
        gamePlayer[land[the_land[i]].linkedNode[k]->belong - 1]
            .resource[land[the_land[i]].type] += 1;
      }
    }
  }
  return;
}


void robberK(int current_player, player *gamePlayer, int playerNumber,
             piece *land) {
  // move robber
  int robber_land, desert_land, input_land;
  for (int i = 0; i < 19; i++) {
    if (land[i].robber == 1)
      robber_land = i;
    if (land[i].type == DESERT)
      desert_land = i;
  }
  int bool_success = 0;
  while (!bool_success) {
    printf("which land would you move the robber to ? :");
    scanf(" %d", &input_land);
    if (input_land > 18 || input_land < 0) {
      continue;
    }
    if (input_land == robber_land || input_land == desert_land)
      printf("wrong input! Desert is %d, original robber at %d\n", desert_land,
             robber_land);
    else
      bool_success = 1;
  }
  land[input_land].robber = 1;
  land[robber_land].robber = 0;
  robber_land = input_land;
  //
  // steal resource
  int array_bool_player[4] = {0};
  int array_player_total[4] = {0};
  int bool_steal_able = 0;
  int steal_from; // 0 - 3
  for (int i = 0; i < 6; i++) {
    if (land[robber_land].linkedNode[i]->belong != PUBLIC)
      array_bool_player[land[robber_land].linkedNode[i]->belong - 1] = 1;
  }
  array_bool_player[current_player] = 0;
  for (int i = 0; i < 4; i++) {
    if (array_bool_player[i] == 1) {
      for (int k = 1; k < 6; k++) {
        array_player_total[i] += gamePlayer[i].resource[k];
      }
      if (array_player_total[i]) {
        printf("PLAYER %d has %d resources", i + 1, array_player_total[i]);
        bool_steal_able = 1;
      } else
        array_bool_player[i] = 0; // 原本是候選人，但無資源所以移除
    }
  }
  int bool_select = 0;
  while (!bool_select && bool_steal_able) {
    printf(", choose one player to select their resources:");
    scanf(" %d", &steal_from); // input 1 - 4
    steal_from--;              // 0 - 3
    if (steal_from >= playerNumber || steal_from < 0) {
      printf("wrong input! Input illegal: out of player range\n");
      continue;
    }
    if (steal_from == current_player) {
      printf("wrong input! Input illegal: you chose Player %d yourself\n",
             current_player);
      continue;
    }
    if (array_bool_player[steal_from] == 0) {
      printf("wrong input! Input illegal: player not available\n");
    }
    bool_select = 1;
  }
  // get resource
  if (bool_select) {
    srand(time(NULL));
    int r = rand() % array_player_total[steal_from] + 1;
    int tmp = 0;
    while (r > 0) {
      tmp++;
      r -= gamePlayer[steal_from].resource[tmp];
    }
    gamePlayer[steal_from].resource[tmp] -= 1;
    gamePlayer[current_player].resource[tmp] += 1;
    printf("PLAYER %d steal %s from PLAYER %d\n", current_player + 1,
           resourceStr[tmp], steal_from + 1);
  } else {
    printf("no stealing\n");
  }
}
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
        printf("Please enter the type of resource you want to fetch：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType);
        if (resourceType < 1 || resourceType > 5)
        {
            printf("Input errors, please re-enter\n");
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
        printf("Please enter the first type of resource to get：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType1);
        if (resourceType1 < 1 || resourceType1 > 5)
        {
            printf("Input errors, please re-enter\n");
        }
        else
        {
            break;
        }
    }
    while(1)
    {
        printf("Please enter the second type of resource to get：\n");
        printf("1. WOOD\n");
        printf("2. BRICKS\n");
        printf("3. WOOL\n");
        printf("4. WHEAT\n");
        printf("5. METAL\n");
        scanf("%d", &resourceType2);
        if (resourceType2 < 1 || resourceType2 > 5)
        {
            printf("Input errors, please re-enter\n");
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
    printf("Please enter the first plate number and road number to build the road：\n");
    scanf("%d %d", &land_pos1, &linkedNode_pos1);
    //build_road_free(p, land_pos1, linkedNode_pos1, land);
    while(!(build_road_free(p, land_pos1, linkedNode_pos1, land)))
    {
        printf("Please enter the first plate number and road number to build the road：\n");
        scanf("%d %d", &land_pos1, &linkedNode_pos1);
    }
    printf("Please enter the second plate number and road number to build the road：\n");
    scanf("%d %d", &land_pos2, &linkedNode_pos2);
    //build_road_free(p, land_pos2, linkedNode_pos2, land);
    while(!(build_road_free(p, land_pos2, linkedNode_pos2, land)))
    {
        printf("Please enter the second plate number and road number to build the road：\n");
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
        printf("Please enter the plate number where you want to move the robber:\n");
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
        printf("Please enter the ID of the player that you wants to seize resources:\n");
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
void knight_king(player *gamePlayer, int current_player, int playerNumber, int *knight_owner){
  
  for (int j = 0; j < gamePlayer[current_player].card->size; ++j) {
    if (gamePlayer[current_player].card->get(gamePlayer[current_player].card, j) == KNIGHT) {
        gamePlayer[current_player].card->remove(gamePlayer[current_player].card, j);
        break;
    } 
  }
  ++gamePlayer[current_player].knight;
  int more_knight_out = 0;

  for(int j = 0; j < playerNumber; ++j){
    if(gamePlayer[current_player].knight > gamePlayer[j].knight){
      ++more_knight_out;
    }
  }

  //printf("player%d's outknight is more than %d player(s)\n", i+1, more_knight_out);
  if(more_knight_out == playerNumber - 1 && gamePlayer[current_player].knight >= 3){
    *knight_owner = current_player;
    printf("Knight owner is player %d.\n",*knight_owner+1);
    return ;
  }
  return;
}
int someone_win(player *gamePlayer, int playerNumber, int knight_owner) {
    for (int i = 0; i < playerNumber; i++) {
        if (gamePlayer[i].Score >= 10 || gamePlayer[i].Score >= 8 && knight_owner == i) {
            printf("Player %d wins!!!\n", i + 1);
            exit(0);
        }
    }
    return 1;
}
