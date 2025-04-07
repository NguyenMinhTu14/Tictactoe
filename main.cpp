#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "graphics.h"
#include "defs.h"
#include "logic.h"

using namespace std;

void waitUntilKeyPressed() {
    SDL_Event e;

    while (true) {
        if (SDL_WaitEvent(&e) && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) {
            return;
        }
    }
}

void processClick(int x, int y, Tictactoe& game, Graphics& graphics) {
    int clickedCol = (x - BOARD_X) / (CELL_SIZE + CELL_PADDING);
    int clickedRow = (y - BOARD_Y) / (CELL_SIZE + CELL_PADDING);

    if (clickedRow >= 0 && clickedRow < BOARD_SIZE && clickedCol >= 0 && clickedCol < BOARD_SIZE) {
        if (game.move(clickedRow, clickedCol)) {
            graphics.playMoveSound();  // Phát âm thanh nếu di chuyển thành công
        }
    }
}

int main(int argc, char* argv[]) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }

    Graphics graphics;
    graphics.init();

    if (!graphics.showMenu()) {
        graphics.quit();
        Mix_CloseAudio();
        return 0;
    }

    Tictactoe game;
    game.init();
    graphics.render(game);

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN: {
                int x, y;
                SDL_GetMouseState(&x, &y);

                processClick(x, y, game, graphics);
                graphics.render(game);

                if (game.gameOver) {
                    int gameState = 0;

                    if (game.checkWin(X_CELL)) {
                        gameState = 1;
                    } else if (game.checkWin(O_CELL)) {
                        gameState = -1;
                    } else if (game.isBoardFull()) {
                        gameState = 0;
                    }

                    graphics.renderResult(gameState);

                    if (!graphics.showMenu()) {
                        quit = true;
                    } else {
                        game.init();
                        graphics.render(game);
                    }
                }
                break;
            }
        }
    }

    graphics.quit();
    Mix_CloseAudio();
    return 0;
}
