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
void robber(piece *land, int *robberLoc) {
    printf("Robber!\n");
    int locate;
    while (1) {
        printf("choice locate:");
        if (locate == *robberLoc)
            printf("this locate is already robber\n");
        else if (locate < 0 || locate >= 19)
            printf("you should input in [0,18]\n");
        else
            break;
    }
    land[*robberLoc].robber = 0;
    land[locate].robber = 1;
    *robberLoc = locate;
}
void giveResource(piece *land, int index, player *p, int playerNum) {
    for (int i = 0; i < 6; ++i) {
        if (land[index].linkedNode[i]->belong) {
            for (int j = 0; j < playerNum; ++j) {
                if (p[j].type == land[index].linkedNode[i]->belong) {
                    p[j].resource[land[index].type]++;
                    break;
                }
            }
        }
    }
}
