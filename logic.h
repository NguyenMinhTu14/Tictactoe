#ifndef LOGIC_H
#define LOGIC_H

#define BOARD_SIZE 10
#define EMPTY_CELL ' '
#define O_CELL 'o'
#define X_CELL 'x'

#define CELL_SIZE 45
#define CELL_PADDING 1
#define BOARD_X ((SCREEN_WIDTH - (BOARD_SIZE * (CELL_SIZE + CELL_PADDING))) / 2)
#define BOARD_Y ((SCREEN_HEIGHT - (BOARD_SIZE * (CELL_SIZE + CELL_PADDING))) / 2)

struct Tictactoe {
    char board[BOARD_SIZE][BOARD_SIZE];
    char nextMove = O_CELL;
    bool gameOver = false;

    void init() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                board[i][j] = EMPTY_CELL;

        gameOver = false;
        nextMove = O_CELL;
    }

    bool move(int row, int column) {
        if (!gameOver && row >= 0 && row < BOARD_SIZE && column >= 0 && column < BOARD_SIZE) {
            if (board[row][column] == EMPTY_CELL) {
                board[row][column] = nextMove;

                if (checkWin(nextMove) || isBoardFull()) {
                    gameOver = true;
                } else {
                    nextMove = (nextMove == O_CELL) ? X_CELL : O_CELL;
                }
                return true;
            }
        }
        return false;
    }

    bool checkWin(char player) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j <= BOARD_SIZE - 5; j++) {
                if (board[i][j] == player && board[i][j+1] == player &&
                    board[i][j+2] == player && board[i][j+3] == player &&
                    board[i][j+4] == player)
                    return true;

                if (board[j][i] == player && board[j+1][i] == player &&
                    board[j+2][i] == player && board[j+3][i] == player &&
                    board[j+4][i] == player)
                    return true;
            }
        }

        for (int i = 0; i <= BOARD_SIZE - 5; i++) {
            for (int j = 0; j <= BOARD_SIZE - 5; j++) {
                if (board[i][j] == player && board[i+1][j+1] == player &&
                    board[i+2][j+2] == player && board[i+3][j+3] == player &&
                    board[i+4][j+4] == player)
                    return true;
            }
        }

        for (int i = 0; i <= BOARD_SIZE - 5; i++) {
            for (int j = 4; j < BOARD_SIZE; j++) {
                if (board[i][j] == player && board[i+1][j-1] == player &&
                    board[i+2][j-2] == player && board[i+3][j-3] == player &&
                    board[i+4][j-4] == player)
                    return true;
            }
        }

        return false;
    }

    bool isBoardFull() {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                if (board[i][j] == EMPTY_CELL)
                    return false;
        return true;
    }
};

#endif // LOGIC_H
