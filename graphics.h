#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>  // Thêm thư viện âm thanh
#include "defs.h"
#include "logic.h"

struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Texture *cellEmpty, *cellX, *cellO;
    SDL_Texture *youWin, *youLose;
    SDL_Texture *background;
    SDL_Texture *menuTexture;

    Mix_Music *backgroundMusic; // Biến lưu nhạc nền
    Mix_Chunk *moveSound;       // Âm thanh khi đánh X hoặc O

    void init() {
        initSDL();

        cellEmpty = loadTexture("cell_empty.png");
        cellX = loadTexture("cell_x.png");
        cellO = loadTexture("cell_o.png");
        background = loadTexture("background.jpg");
        menuTexture = loadTexture("menu.png");

        // Khởi tạo SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }

        // Load nhạc nền
        backgroundMusic = Mix_LoadMUS("music.mp3");
        if (!backgroundMusic) {
            SDL_Log("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        } else {
            Mix_PlayMusic(backgroundMusic, -1); // Phát nhạc lặp vô hạn
        }

        // Tải âm thanh khi đánh X hoặc O
        moveSound = Mix_LoadWAV("move1.mp3");
        if (!moveSound) {
            SDL_Log("Failed to load move sound: %s", Mix_GetError());
        }
    }

    void playMoveSound() {
        if (moveSound) {
            Mix_PlayChannel(-1, moveSound, 0);
        }
    }

    bool isInside(int x, int y, int btnX, int btnY, int btnW, int btnH) {
        return (x >= btnX && x <= btnX + btnW && y >= btnY && y <= btnY + btnH);
    }

    bool showMenu() {
        SDL_Event event;

        while (true) {
            renderMenu();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (isInside(x, y, START_BUTTON_X, START_BUTTON_Y, START_BUTTON_W, START_BUTTON_H)) {
                        return true;
                    } else if (isInside(x, y, EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_W, EXIT_BUTTON_H)) {
                        return false;
                    }
                }
            }
        }
    }

    void renderMenu() {
        SDL_RenderClear(renderer);

        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, menuTexture, NULL, &dest);

        SDL_RenderPresent(renderer);
    }

    void renderResult(int gameState) {
        SDL_Texture* resultImage = nullptr;

        if (gameState == 1) resultImage = loadTexture("X_win.png");
        else if (gameState == -1) resultImage = loadTexture("O_win.png");
        else resultImage = loadTexture("draw.png");

        SDL_RenderClear(renderer);

        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, resultImage, NULL, &dest);

        SDL_Rect returnRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - SCREEN_HEIGHT / 3, 100, 50};

        SDL_RenderPresent(renderer);

        SDL_Event event;
        bool waiting = true;

        while (waiting) {
            SDL_WaitEvent(&event);

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= returnRect.x && x <= returnRect.x + returnRect.w &&
                    y >= returnRect.y && y <= returnRect.y + returnRect.h) {
                    waiting = false;
                }
            }
        }

        SDL_DestroyTexture(resultImage);
    }

    void render(const Tictactoe& game) {
        SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, background, NULL, &bgRect);

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                int x = BOARD_X + j * (CELL_SIZE + CELL_PADDING);
                int y = BOARD_Y + i * (CELL_SIZE + CELL_PADDING);

                SDL_Rect destRect = {x, y, CELL_SIZE, CELL_SIZE};

                switch (game.board[i][j]) {
                    case EMPTY_CELL:
                        SDL_RenderCopy(renderer, cellEmpty, NULL, &destRect);
                        break;
                    case X_CELL:
                        SDL_RenderCopy(renderer, cellX, NULL, &destRect);
                        break;
                    case O_CELL:
                        SDL_RenderCopy(renderer, cellO, NULL, &destRect);
                        break;
                }
            }
        }

        presentScene();
    }

    void logErrorAndExit(const char* msg, const char* error) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void initSDL() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void prepareScene(SDL_Texture *background) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void prepareScene() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void presentScene() {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;

        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void quit() {
        SDL_DestroyTexture(cellEmpty);
        SDL_DestroyTexture(cellX);
        SDL_DestroyTexture(cellO);
        SDL_DestroyTexture(youWin);
        SDL_DestroyTexture(youLose);
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(menuTexture);

        if (backgroundMusic) {
            Mix_HaltMusic();
            Mix_FreeMusic(backgroundMusic);
        }

        Mix_FreeChunk(moveSound);
        Mix_CloseAudio();

        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
