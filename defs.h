#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "Hello World!";
#define CELL_SIZE 50
#define CELL_PADDING 1
#define BOARD_X ((SCREEN_WIDTH - (BOARD_SIZE * (CELL_SIZE + CELL_PADDING))) / 2)
#define BOARD_Y (((SCREEN_HEIGHT - (BOARD_SIZE * (CELL_SIZE + CELL_PADDING))) / 2) + 200) // Giảm xuống 1/4 chiều cao ảnh


#define START_BUTTON_X  140
#define START_BUTTON_Y  500
#define START_BUTTON_W  300
#define START_BUTTON_H   80

#define EXIT_BUTTON_X   140
#define EXIT_BUTTON_Y   600
#define EXIT_BUTTON_W   300
#define EXIT_BUTTON_H    70
#endif
