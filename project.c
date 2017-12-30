#include <stdio.h>
#include "myLib.h"

void init() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	sprintf(buffer, "Score: %d", score);
	drawString(150, 5, buffer, RED);

	// Fill screen with bgcolor using DBA
	DMA[3].src = &bgcolor;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;

	//Initialize mario
	mario.row = SCREENHEIGHT - 25;
	mario.col = SCREENWIDTH - 20;
	mario.width = 15;
	mario.height = 16;
	mario.is_left = 1;

	//Initialize donkey_kong
	donkey_kong.row = 5;
	donkey_kong.col = 5;
	donkey_kong.width = 32;
	donkey_kong.height = 43;

	//Initialize ladder_one
	ladder_one.row = 40;
	ladder_one.col = SCREENWIDTH - 10;
	ladder_one.width = 1;
	ladder_one.height = 50;

	// //initialize ladder_two
	ladder_two.row = 90;
	ladder_two.col = 30;
	ladder_two.width = 1;
	ladder_two.height = 50;

	// //initialize barrel_one
	barrel_level_one.row = 40 - 10;
	barrel_level_one.col = SCREENWIDTH - 190;
	barrel_level_one.width = 10;
	barrel_level_one.height = 12;
	barrel_level_one.orientation = 0;

	// //initialize barrel_two
	barrel_level_two.row = 90 - 10;
	barrel_level_two.col = 30;
	barrel_level_two.width = 10;
	barrel_level_two.height = 12;
	barrel_level_two.orientation = 0;

	// //initialize barrel_three
	barrel_level_three.row = 140 - 10;
	barrel_level_three.col = 30;
	barrel_level_three.width = 10;
	barrel_level_three.height = 12;
	barrel_level_three.orientation = 0;

	drawFullscreenImage(splash_screen);
	sprintf(buffer, "Press 'Enter'.");
	drawString(100, 80, buffer, WHITE);
	state = START;
	score = 0;
}

int main() {
	init();

	while (1) {
		switch(state) {
			case START:
				if (KEY_DOWN_NOW(BUTTON_START)) {
					state = GAME;
				}
				break;
			case GAME:
				if (KEY_DOWN_NOW(BUTTON_B)) {
					state = PAUSE;
				}
				if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					if (mario.col > 0) {
						mario.col -= 2;
						mario.is_left = 1;
					}
				}
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
					if (mario.col < SCREENWIDTH - (mario.height + 1)) {
						mario.col += 2;
						mario.is_left = 0;
					}
				}
				if (KEY_DOWN_NOW(BUTTON_UP)) {
					if (mario.row > 0) {
						if (mario.col > ladder_two.col - 30
							&& mario.col < ladder_two.col && mario.row > ladder_two.height + mario.height + 8) {
							mario.row--;
							mario.is_left = 2;
						}  
						else if (mario.col > ladder_one.col - 30 
							&& mario.col < ladder_one.col && mario.row > mario.height + 8) {
							mario.row--;
							mario.is_left = 2;
						}
					}
				}
				if (KEY_DOWN_NOW(BUTTON_DOWN)) {
					if (mario.row > 0) {
						if (mario.col > ladder_two.col - 30
							&& mario.col < ladder_two.col && mario.row < 125) {
							mario.row++;
							mario.is_left = 2;
						}  
						else if (mario.col > ladder_one.col - 30 
							&& mario.col < ladder_one.col && mario.row < 75) {
							mario.row++;
							mario.is_left = 2;
						}
					}
				}
				if(!(mario.col > ladder_two.col - 30 && mario.col < ladder_two.col) && mario.row > ladder_two.height + mario.height + 8) {
					if ((mario.row + mario.height < 140) && (up == false)) {
						mario.row += 2;

						if(mario.row + mario.height > 140) {
							up = false;
							mario.row = 140;
						}
					} else if ((mario.row + mario.height == 140) || up == true) {
						if (KEY_DOWN_NOW(BUTTON_A)) {
							if (mario.row > 140 - 48) {
								up = true;
							}
						}
						if (mario.row < 140 - 40) {
							up = false;
						}
					}
					if (up == true) {
						mario.row -= 2;
					}
				} else if (!(mario.col > ladder_one.col - 35 && mario.col < ladder_one.col) && mario.row > mario.height + 8) {
					if ((mario.row + mario.height < 90) && (up == false)) {
						mario.row += 2;

						if(mario.row + mario.height > 90) {
							up = false;
							mario.row = 90;
						}
					} else if ((mario.row + mario.height == 90) || up == true) {
						if (KEY_DOWN_NOW(BUTTON_A)) {
							if (mario.row > 90 - 48) {
								up = true;
							}
						}
						if (mario.row < 90 - 40) {
							up = false;
						}
					}
					if (up == true) {
						mario.row -= 2;
					}
				}  else {
					if ((mario.row + mario.height < 40) && (up == false)) {
						mario.row += 2;

						if(mario.row + mario.height > 40) {
							up = false;
							mario.row = 40;
						}
					} else if ((mario.row + mario.height == 40) || up == true) {
						if (KEY_DOWN_NOW(BUTTON_A)) {
							if (mario.row > 20) {
								up = true;
							}
						}
						if (mario.row < 0) {
							up = false;
						}
					}
					if (up == true) {
						mario.row -= 2;
					}
				}

				//Draw background and floors
				background(160, 240, BLACK);
				drawHorizontal(40, 0, SCREENWIDTH, RED);
				drawHorizontal(90, 0, SCREENWIDTH, RED);
				drawHorizontal(140, 0, SCREENWIDTH, RED);

				//Update barrel_one
				if (barrel_level_one.col > SCREENWIDTH - 13) {
					barrel_level_one.col = SCREENWIDTH - 190;
				} else {
					barrel_level_one.col += 1;
				}
				if (barrel_level_one.orientation == 0) {
					drawImage3(barrel_level_one.row, barrel_level_one.col, barrel_level_one.width, barrel_level_one.height, barrel_one);
					barrel_level_one.orientation = 1;
				} else if (barrel_level_one.orientation == 1) {
					drawImage3(barrel_level_one.row, barrel_level_one.col, barrel_level_one.width, barrel_level_one.height, barrel_two);
					barrel_level_one.orientation = 2;
				} else if (barrel_level_one.orientation == 2) {
					drawImage3(barrel_level_one.row, barrel_level_one.col, barrel_level_one.width, barrel_level_one.height, barrel_three);
					barrel_level_one.orientation = 3;
				} else {
					drawImage3(barrel_level_one.row, barrel_level_one.col, barrel_level_one.width, barrel_level_one.height, barrel_four);
					barrel_level_one.orientation = 0;
				}

				//Update barrel_two
				if (barrel_level_two.col < 4) {
					barrel_level_two.col = SCREENWIDTH - 13;
				} else {
					barrel_level_two.col -= 1;
				}
				if (barrel_level_two.orientation == 0) {
					drawImage3(barrel_level_two.row, barrel_level_two.col, barrel_level_two.width, barrel_level_two.height, barrel_one);
					barrel_level_two.orientation = 1;
				} else if (barrel_level_two.orientation == 1) {
					drawImage3(barrel_level_two.row, barrel_level_two.col, barrel_level_two.width, barrel_level_two.height, barrel_two);
					barrel_level_two.orientation = 2;
				} else if (barrel_level_two.orientation == 2) {
					drawImage3(barrel_level_two.row, barrel_level_two.col, barrel_level_two.width, barrel_level_two.height, barrel_three);
					barrel_level_two.orientation = 3;
				} else {
					drawImage3(barrel_level_two.row, barrel_level_two.col, barrel_level_two.width, barrel_level_two.height, barrel_four);
					barrel_level_two.orientation = 0;
				}

				if (barrel_level_three.col > SCREENWIDTH - 13) {
					barrel_level_three.col = 1;
				} else {
					barrel_level_three.col += 1;
				}
				if (barrel_level_three.orientation == 0) {
					drawImage3(barrel_level_three.row, barrel_level_three.col, barrel_level_three.width, barrel_level_three.height, barrel_one);
					barrel_level_three.orientation = 1;
				} else if (barrel_level_three.orientation == 1) {
					drawImage3(barrel_level_three.row, barrel_level_three.col, barrel_level_three.width, barrel_level_three.height, barrel_two);
					barrel_level_three.orientation = 2;
				} else if (barrel_level_three.orientation == 2) {
					drawImage3(barrel_level_three.row, barrel_level_three.col, barrel_level_three.width, barrel_level_three.height, barrel_three);
					barrel_level_three.orientation = 3;
				} else {
					drawImage3(barrel_level_three.row, barrel_level_three.col, barrel_level_three.width, barrel_level_three.height, barrel_four);
					barrel_level_three.orientation = 0;
				}

				//Draw Donkey Kong
				drawImage3(donkey_kong.row, donkey_kong.col, donkey_kong.width, donkey_kong.height, kong_array);

				//Update Mario
				if (mario.is_left == 0) {
					drawImage3(mario.row, mario.col, mario.width, mario.height, mario_right);
				} else if (mario.is_left == 1) {
					drawImage3(mario.row, mario.col, mario.width, mario.height, mario_left);
				} else {
					drawImage3(mario.row, mario.col, mario.width, mario.height, mario_buttcheek);
				}
				
				//Update Ladder 
				drawLadder(40, SCREENWIDTH - 10, 50, 1, BLUE);
				drawLadder(90, 30, 50, 1, BLUE);
				
				//Update Score barrel_three
				if (mario.row < 140 && mario.row > 100 && (mario.col == barrel_level_three.col - 2)) {
					score++;
				} else if (mario.row < 140 && mario.row > 100 && (mario.col == barrel_level_three.col - 1)) {
					score++;
				} else if (mario.row < 140 && mario.row > 100 && (mario.col == barrel_level_three.col)) {
					score++;
				} else if (mario.row < 140 && mario.row > 100 && (mario.col == barrel_level_three.col + 1)) {
					score++;
				} else if (mario.row < 140 && mario.row > 100 && (mario.col == barrel_level_three.col + 2)) {
					score++;
				}	

				//Update Score barrel_two
				else if (mario.row < 90 && mario.row > 40 && (mario.col == barrel_level_two.col - 2)) {
					score++;
				} else if (mario.row < 90 && mario.row > 40 && (mario.col == barrel_level_two.col - 1)) {
					score++;
				} else if (mario.row < 90 && mario.row > 40 && (mario.col == barrel_level_two.col)) {
					score++;
				} else if (mario.row < 90 && mario.row > 40 && (mario.col == barrel_level_two.col + 1)) {
					score++;
				} else if (mario.row < 90 && mario.row > 40 && (mario.col == barrel_level_two.col + 2)) {
					score++;
				}

				//Update Score barrel_one
				else if (mario.row < 40 && (mario.col == barrel_level_one.col - 2)) {
					score++;
				} else if (mario.row < 40 && (mario.col == barrel_level_one.col - 1)) {
					score++;
				} else if (mario.row < 40 && (mario.col == barrel_level_one.col)) {
					score++;
				} else if (mario.row < 40 && (mario.col == barrel_level_one.col + 1)) {
					score++;
				} else if (mario.row < 40 && (mario.col == barrel_level_one.col + 2)) {
					score++;
				}
				sprintf(buffer, "Score: %d", score);
				drawString(150, 5, buffer, GREEN);

				if ((mario.col < donkey_kong.col + donkey_kong.width)
					&& mario.row < 40) {
					state = WIN;
				}
				if (hitBarrel(mario.row, mario.col, mario.height, mario.width, barrel_level_one.row, barrel_level_one.col, barrel_level_one.height, barrel_level_one.width)
					|| hitBarrel(mario.row, mario.col, mario.height, mario.width, barrel_level_two.row, barrel_level_two.col, barrel_level_two.height, barrel_level_two.width)
					|| hitBarrel(mario.row, mario.col, mario.height, mario.width, barrel_level_three.row, barrel_level_three.col, barrel_level_three.height, barrel_level_three.width)) {
					state = LOSE;
				}
				break;
			case WIN:
				drawFullscreenImage(win_screen);
				sprintf(buffer, "Score: %d", score);
				drawString(150, 175, buffer, GREEN);
				break;
			case LOSE:
				drawFullscreenImage(game_over);
				sprintf(buffer, "Score: %d", score);
				drawString(150, 175, buffer, GREEN);
				break;
			case PAUSE:
				//background(160, 240, BLACK);
				sprintf(buffer, "PAUSED");
				drawString(80, 105, buffer, WHITE);
				sprintf(buffer, "Press 'Enter'.");
				drawString(100, 80, buffer, WHITE);
				if (KEY_DOWN_NOW(BUTTON_START)) {
					state = GAME;
				}

		}
		waitForVblank();
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			init();
		}
	}
}

void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color; //pixel location = col + row * 240
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}

void background(int height, int width, unsigned short color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (width * height) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
}


void drawFullscreenImage(const unsigned short *image) {
	DMA[3].src = image;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
}

void drawImage3(int row, int col, int height, int width, const unsigned short *image) {
	for(int r = 0; r < height; r++) {
		DMA[3].src = &image[OFFSET(r, 0, width)];
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCREENWIDTH)];
		DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	}
}

void drawHorizontal(int row, int col, int width, unsigned int color) {
	DMA[3].src = &color;
	DMA[3].dst = &videoBuffer[OFFSET(row, col, SCREENWIDTH)];
	DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
}

void drawVertical(int row, int col, int height, int width, unsigned int color) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCREENWIDTH)];
		DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}

void drawLadder(int row, int col, int height, int width, unsigned int color) {
	//Draw ladder sides
	drawVertical(row, col, height, width, color);
	drawVertical(row, col - 20, height, width, color);

	//Draw rungs
	drawHorizontal(row, col - 20, 20, color);
	drawHorizontal(row + 5, col - 20, 20, color);
	drawHorizontal(row + 10, col - 20, 20, color);
	drawHorizontal(row + 15, col - 20, 20, color);
	drawHorizontal(row + 20, col - 20, 20, color);
	drawHorizontal(row + 25, col - 20, 20, color);
	drawHorizontal(row + 30, col - 20, 20, color);
	drawHorizontal(row + 35, col - 20, 20, color);
	drawHorizontal(row + 40, col - 20, 20, color);
}

int hitBarrel(int row_one, int col_one, int height_one, int width_one, int row_two, int col_two, int height_two, int width_two) {

    return row_one < row_two + height_two - 1 && row_two < row_one + height_one - 1
        && col_one < col_two + width_two - 1 && col_two < col_one + width_one - 1;
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawChar(int row, int col, char ch, unsigned short color) {
	for(int r = 0; r < 8; r++) {
		for(int c = 0; c < 6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1) {
				setPixel(r+row, c+col, color);
			}
		}
	}
}

void drawString(int row, int col, char str[], unsigned short color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}
