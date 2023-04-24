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
const int TEAMCOLOR[5] = {255, 93, 75, 82, 196};
const int PIECECOLOR[6] = {11, 28, 202, 15, 94, 246};
const int PORTCOLOR[6] = {241, 136, 255, 9, 226, 246};
const char PORTTEXT[6] = {'?', 'l', 'w', 'b', 'h', 'm'};
const char NODETYPE[3] = {' ', 's', 'c'};
const char resourceStr[6][10] = {"None", "wood",  "bricks",
                                 "wool", "wheat", "metal"};

void initGame(piece *p, node *n, side *s) {
    // corner bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 3; ++j) {
            p[i].linkedNode[j] = &(n[i * 2 + (i >= 3) + (i >= 7) +
                                     (i >= 12) * 2 + (i >= 16) * 3 + j]);
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
static void printpart(const piece *p, int l, int r, int space, int t, int tl) {
    // printf("%d\n", p[l].linkedNode[0]->type);
    for (int i = 0; i < space - (p[l].linkedNode[0]->linkedSide[0] != NULL) * 2;
         ++i)
        printf("\e[48;5;17m \e[0m");

    if (p[l].linkedNode[0]->linkedSide[0] != NULL) {
        printf(
            "\e[38;5;%dm%c\e[0m",

            TEAMCOLOR[p[l].linkedNode[0]->linkedSide[0]->linkedNode[0]->belong],
            NODETYPE[p[l].linkedNode[0]->linkedSide[0]->linkedNode[0]->type]);
        printf("\e[38;5;%dm\\\e[0m",
               TEAMCOLOR[p[l].linkedNode[0]->linkedSide[0]->belong]);
    }
    for (int i = l; i <= r; ++i) {
        printf(
            "\e[38;5;%dm%c\e[0m"
            "\e[38;5;%dm/\e[0m"
            "\e[38;5;%dm%c\e[0m"
            "\e[38;5;%dm\\\e[0m",
            TEAMCOLOR[p[i].linkedNode[0]->belong],
            NODETYPE[p[i].linkedNode[0]->type],
            TEAMCOLOR[p[i].linkedSide[0]->belong],
            TEAMCOLOR[p[i].linkedNode[1]->belong],
            NODETYPE[p[i].linkedNode[1]->type],
            TEAMCOLOR[p[i].linkedSide[1]->belong]);
    }
    printf("\e[38;5;%dm%c\e[0m", TEAMCOLOR[p[r].linkedNode[2]->belong],
           NODETYPE[p[r].linkedNode[2]->type]);
    if (p[r].linkedNode[2]->linkedSide[1] != NULL) {
        printf("\e[38;5;%dm/\e[0m",
               TEAMCOLOR[p[r].linkedNode[2]->linkedSide[1]->belong]);
        printf(
            "\e[38;5;%dm%c\e[0m",
            TEAMCOLOR[p[r].linkedNode[2]->linkedSide[1]->linkedNode[0]->belong],
            NODETYPE[p[r].linkedNode[2]->linkedSide[1]->linkedNode[0]->type]);
    }
    for (int i = 0; i < space - (p[r].linkedNode[2]->linkedSide[1] != NULL) * 2;
         ++i)
        printf("\e[48;5;17m \e[0m");
    printf("\n");
    for (int i = 0; i < space - (!(tl & 1)); ++i) printf("\e[48;5;17m \e[0m");
    if (!(tl & 1)) {
        printf("\e[48;5;17m\e[38;5;%dm%c\e[0m", PORTCOLOR[t], PORTTEXT[t]);
    }
    for (int i = l; i <= r; ++i) {
        printf("\e[38;5;%dm|\e[0m\e[48;5;%dm%c\e[m",
               TEAMCOLOR[p[i].linkedSide[2]->belong], PIECECOLOR[p[i].type],
               (p[i].robber ? '*' : ' '));
        if (p[i].number == 7)
            printf("\e[48;5;%dm  \e[m", PIECECOLOR[p[i].type]);
        else
            printf("\e[48;5;%dm%2d\e[m", PIECECOLOR[p[i].type], p[i].number);
    }
    printf("\e[38;5;%dm|\e[0m", TEAMCOLOR[p[r].linkedSide[3]->belong]);
    if ((tl & 1)) {
        printf("\e[48;5;17m\e[38;5;%dm%c\e[0m", PORTCOLOR[t], PORTTEXT[t]);
    }
    for (int i = 0; i < space - (tl & 1); ++i) printf("\e[48;5;17m \e[0m");
    printf("\n");
}
void printMap(const piece *p, int n, const port *t) {
    // int cnt = 0;
    // for (int i = -2; i <= 2; ++i) {
    //     for (int j = 0; j < abs(i); ++j) printf(" ");
    //     for (int j = 0; j < 5 - abs(i); ++j) {
    //         printf("%d ", p[cnt].type);
    //         cnt++;
    //     }
    //     printf("\n");
    // }
    printf("\e[48;5;17m      \e[38;5;%dm%c     \e[38;5;%dm%c          \e[0m\n",
           PORTCOLOR[t[0].type], PORTTEXT[t[0].type], PORTCOLOR[t[1].type],
           PORTTEXT[t[1].type]);
    printpart(p, 0, 2, 5, t[3].type, 3);
    printpart(p, 3, 6, 3, t[2].type, 2);
    printpart(p, 7, 11, 1, t[5].type, 5);
    printpart(p, 12, 15, 3, t[4].type, 4);
    printpart(p, 16, 18, 5, t[7].type, 7);
    printf("\e[48;5;17m     \e[0m");
    for (int i = 16; i <= 18; ++i) {
        printf(
            "\e[38;5;%dm%c\e[0m"
            "\e[38;5;%dm\\\e[0m"
            "\e[38;5;%dm%c\e[0m"
            "\e[38;5;%dm/\e[0m",
            TEAMCOLOR[p[i].linkedNode[3]->belong],
            NODETYPE[p[i].linkedNode[3]->type],
            TEAMCOLOR[p[i].linkedSide[4]->belong],
            TEAMCOLOR[p[i].linkedNode[4]->belong],
            NODETYPE[p[i].linkedNode[4]->type],
            TEAMCOLOR[p[i].linkedSide[5]->belong]);
    }
    printf("\e[38;5;%dm%c\e[0m\e[48;5;17m     \e[0m\n",
           TEAMCOLOR[p[18].linkedNode[5]->belong],
           NODETYPE[p[18].linkedNode[5]->type]);
    printf("\e[48;5;17m      \e[38;5;%dm%c     \e[38;5;%dm%c          \e[0m\n",
           PORTCOLOR[t[6].type], PORTTEXT[t[6].type], PORTCOLOR[t[8].type],
           PORTTEXT[t[8].type]);
}
