#ifndef PLAY_H
#define PLAY_H

#define DOWN    0
#define UP      1
#define LEFT    2
#define RIGHT   3

typedef int color;

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define L_GRAY 0x7
#define GRAY 0x8
#define L_BLUE 0x9
#define L_GREEN 0xA
#define L_CYAN 0xB
#define L_RED 0xC
#define L_MAGENTA 0xD
#define YELLOW 0xE
#define WHITE 0xF

#define WIDTH 80
#define HEIGHT 25

#define AREA WIDTH*HEIGHT

void play(unsigned int fd);
int drawMovement(char c, int player);
int checkCollision(int player);
#endif