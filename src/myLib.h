#include <stdio.h>
#include "splash_screen.h"
#include "win_screen.h"
#include "game_over.h"
#include "mario_left.h"
#include "mario_right.h"
#include "mario_buttcheek.h"
#include "kong_array.h"
#include "barrel_one.h"
#include "barrel_two.h"
#include "barrel_three.h"
#include "barrel_four.h"

typedef unsigned int u32;

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3

#define SCANLINECOUNTER *(volatile unsigned short *) 0x4000006

#define SCREENHEIGHT 149
#define SCREENWIDTH 240

#define BG2_ENABLE (1 << 10)
#define COLOR(r, g, b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define WHITE COLOR(31, 31, 31)
#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))
#define BLACK COLOR(0, 0, 0)
#define BROWN COLOR(165, 42, 42)
#define GRAY COLOR (26, 26, 26)

extern const unsigned char fontdata_6x8[12288];

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */
#define REG_DMA0SAD *(volatile u32*)0x40000B0 		// source address
#define REG_DMA0DAD *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD *(volatile u32*)0x40000BC 		// source address
#define REG_DMA1DAD *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD *(volatile u32*)0x40000C8 		// source address
#define REG_DMA2DAD *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD *(volatile u32*)0x40000D4 		// source address
#define REG_DMA3DAD *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT *(volatile u32*)0x40000DC       // control register


typedef struct {
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

struct MARIO {
	int row;
	int col;
	int prev_row;
	int prev_col;
	int width;
	int height;
	int is_left;
};

struct BARREL {
	int row;
	int col;
	int width;
	int height;
	int orientation;
};

struct LADDER {
	int row;
	int col;
	int width;
	int height;
};

struct DONKEY_KONG {
	int row;
	int col;
	int width;
	int height;
};

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)


#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

unsigned short *videoBuffer = (unsigned short *) 0x6000000;

// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char str[], unsigned short color);
void drawHorizontal(int col, int width, int row, unsigned int color);
void drawRect2(int row, int col, int height, int width, unsigned short color);
void drawFullscreenImage(const unsigned short *image);
void drawImage3(int row, int col, int height, int width, const unsigned short *image);
void drawLadder(int row, int col, int height, int width, unsigned int color);
void background(int height, int width, unsigned short color);
void drawVertical(int row, int col, int height, int width, unsigned int color);
int hitBarrel(int row_one, int col_one, int height_one, int width_one, int row_two, int col_two, int height_two, int width_two);

enum {
	START,
	GAME,
	LOSE,
	WIN,
	PAUSE
};

typedef enum {
	false, 
	true
} bool;

int state;
int score = 0;
char buffer[41];

struct MARIO mario;
struct DONKEY_KONG donkey_kong;
struct LADDER ladder_one;
struct LADDER ladder_two;
struct BARREL barrel_level_one;
struct BARREL barrel_level_two;
struct BARREL barrel_level_three;

int bgcolor = BLACK;
bool up = false;
bool paused = false;