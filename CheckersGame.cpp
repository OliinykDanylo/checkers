#include "CheckersGame.h"
#include <jni.h>
#include <vector>
#include <iostream>

Board::Board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (row < 3 && (row + col) % 2 == 1) {
                board[row][col] = 3;
            } else if (row > 4 && (row + col) % 2 == 1) {
                board[row][col] = 1;
            } else {
                board[row][col] = 0;
            }
        }
    }
}

std::vector<std::pair<int, int>> Board::getCapturingPieces(int currentPlayer) {
    std::vector<std::pair<int, int>> capturingPieces;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if ((currentPlayer == 1 && (board[row][col] == 1 || board[row][col] == 2)) ||
                (currentPlayer == 3 && (board[row][col] == 3 || board[row][col] == 4))) {
                if (canCapture(row, col, currentPlayer, true)) {
                    capturingPieces.push_back({row, col});
                }
            }
        }
    }
    return capturingPieces;
}

bool Board::canCapture(int fromRow, int fromCol, int currentPlayer, bool respectDirectionality = false) {
    int piece = board[fromRow][fromCol];
    bool isKing = (piece == 2 || piece == 4);

    int directions[2];

    if (isKing) {
        directions[0] = -1;
        directions[1] = 1;
    } else {
        if (currentPlayer == 1) {
            directions[0] = -1;
            directions[1] = 0;
        } else {
            directions[0] = 1;
            directions[1] = 0;
        }
    }

    for (int direction : directions) {
        if (direction == 0) continue;

        for (int dCol = -1; dCol <= 1; dCol += 2) {
            int toRow = fromRow + 2 * direction;
            int toCol = fromCol + 2 * dCol;
            if (toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8) {
                int midRow = fromRow + direction;
                int midCol = fromCol + dCol;
                int opponentPiece = currentPlayer == 1 ? 3 : 1;

                if ((board[midRow][midCol] == opponentPiece || board[midRow][midCol] == opponentPiece + 1) &&
                    board[toRow][toCol] == 0) {

                    if (respectDirectionality && !isKing) {
                        int validDirection = (currentPlayer == 1) ? -1 : 1;
                        if (toRow - fromRow != 2 * validDirection) {
                            continue;
                        }
                    }

                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol, int currentPlayer) {
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) return false;
    if (board[toRow][toCol] != 0) return false;

    int piece = board[fromRow][fromCol];
    if ((piece != 1 && piece != 2 && currentPlayer == 1) || (piece != 3 && piece != 4 && currentPlayer == 3)) {
        return false;
    }

    bool isKing = (piece == 2 || piece == 4);
    int direction = (currentPlayer == 1) ? -1 : 1;

    std::vector<std::pair<int, int>> capturingPieces = getCapturingPieces(currentPlayer);

    if (!capturingPieces.empty()) {
        return canCapture(fromRow, fromCol, currentPlayer, true) && abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 2;
    } else {
        if (isKing) {
            return abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 1;
        } else {
            return (toRow == fromRow + direction) && abs(toCol - fromCol) == 1;
        }
    }
}

void Board::printBoardState() {
    std::cout << "Current Board State:" << std::endl;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
} // used for debuging

bool Board::executeMove(int startX, int startY, int endX, int endY) {
    int piece = board[startX][startY];

    if (piece == 0) {
        std::cout << "Error: Attempted to move an empty piece from (" << startX << ", " << startY << ")" << std::endl;
        return false;
    }

    std::cout << "Executing move for piece " << piece << " from (" << startX << ", " << startY << ") to (" << endX << ", " << endY << ")" << std::endl;
    board[endX][endY] = piece;
    board[startX][startY] = 0;

    bool pieceCaptured = false;

    if (abs(endX - startX) == 2) {
        int midRow = (startX + endX) / 2;
        int midCol = (startY + endY) / 2;

        board[midRow][midCol] = 0;

        pieceCaptured = true;
        std::cout << "Piece captured at (" << midRow << ", " << midCol << ")" << std::endl;
    }

    if ((piece == 1 && endX == 0) || (piece == 3 && endX == 7)) {
        board[endX][endY] = piece + 1;
        std::cout << "Promoting piece at (" << endX << ", " << endY << ") to king." << std::endl;
    }

//    printBoardState(); // was used for debuging

    return pieceCaptured;
}

bool Board::hasValidMoves(int currentPlayer) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int piece = board[row][col];
            if ((currentPlayer == 1 && (piece == 1 || piece == 2)) ||
                (currentPlayer == 3 && (piece == 3 || piece == 4))) {
                for (int dRow = -2; dRow <= 2; ++dRow) {
                    for (int dCol = -2; dCol <= 2; ++dCol) {
                        if (isValidMove(row, col, row + dRow, col + dCol, currentPlayer)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

std::vector<std::vector<int>> Board::fetchBoardState() {
    std::vector<std::vector<int>> state(8, std::vector<int>(8));
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            state[i][j] = board[i][j];
        }
    }
    return state;
}

// CheckersGame class implementation

CheckersGame::CheckersGame() : currentPlayer(1), hasCapturedThisTurn(false), playerOneCaptured(0), playerTwoCaptured(0), winner(0) {
    board = new Board();
}

bool CheckersGame::isGameOver() {
    int playerOnePieces = 0, playerTwoPieces = 0;
    for (const auto& row : board->fetchBoardState()) {
        for (int piece : row) {
            if (piece == 1 || piece == 2) {
                playerOnePieces++;
            } else if (piece == 3 || piece == 4) {
                playerTwoPieces++;
            }
        }
    }

    std::cout << "Player One Pieces: " << playerOnePieces << ", Player Two Pieces: " << playerTwoPieces << std::endl;

    if (playerOnePieces == 0) {
        winner = 3;
        return true;
    }

    if (playerTwoPieces == 0) {
        winner = 1;
        return true;
    }

    if (!board->hasValidMoves(1)) {
        winner = 3;
        return true;
    }

    if (!board->hasValidMoves(3)) {
        winner = 1;
        return true;
    }

    return false;
}

int CheckersGame::getWinner() const {
    return winner;
}

const std::string PLAYER_ONE_NAME = "White";
const std::string PLAYER_TWO_NAME = "Black";

CheckersGame::~CheckersGame() {
    delete board;
}

bool CheckersGame::validateMove(int fromRow, int fromCol, int toRow, int toCol) {
    int piece = board->fetchBoardState()[fromRow][fromCol];

    // debug information
    std::cout << "Current Player: " << currentPlayer << std::endl;
    std::cout << "Piece at (" << fromRow << ", " << fromCol << "): " << piece << std::endl;

    if ((currentPlayer == 1 && (piece != 1 && piece != 2)) || (currentPlayer == 3 && (piece != 3 && piece != 4))) {
        std::cout << "It's not your turn to move that piece!" << std::endl;
        return false;
    }

    std::vector<std::pair<int, int>> capturingPieces = board->getCapturingPieces(currentPlayer);
    if (!capturingPieces.empty()) {
        if (!board->canCapture(fromRow, fromCol, currentPlayer) || abs(toRow - fromRow) != 2 || abs(toCol - fromCol) != 2) {
            return false;
        }
    }

    return board->isValidMove(fromRow, fromCol, toRow, toCol, currentPlayer);
}

void CheckersGame::executeMove(int startX, int startY, int endX, int endY) {
    bool pieceCaptured = board->executeMove(startX, startY, endX, endY);

    // ensure the captured piece is correctly removed
    if (pieceCaptured) {
        if (currentPlayer == 1) {
            playerOneCaptured++;
        } else if (currentPlayer == 3) {
            playerTwoCaptured++;
        }
    }

    if (pieceCaptured && board->canCapture(endX, endY, currentPlayer)) {
        hasCapturedThisTurn = true;
        return;
    }

    hasCapturedThisTurn = false;
    currentPlayer = (currentPlayer == 1) ? 3 : 1;

    if (isGameOver()) {
        std::cout << "Game Over! Player " << getWinner() << " wins!" << std::endl;
    }
}

std::vector<std::vector<int>> CheckersGame::fetchBoardState() {
    return board->fetchBoardState();
}

int CheckersGame::getCurrentPlayer() const {
    return currentPlayer;
}

// JNI methods
extern "C" {

JNIEXPORT jlong JNICALL Java_CheckersGame_initGame(JNIEnv* env, jobject obj) {
    CheckersGame* game = new CheckersGame();
    return reinterpret_cast<jlong>(game);
}

JNIEXPORT jboolean JNICALL Java_CheckersGame_validateMove(JNIEnv *env, jobject obj, jlong gamePtr, jint fromRow, jint fromCol, jint toRow, jint toCol) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->validateMove(fromRow, fromCol, toRow, toCol) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_CheckersGame_executeMove(JNIEnv *env, jobject obj, jlong gamePtr, jint startX, jint startY, jint endX, jint endY) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    game->executeMove(startX, startY, endX, endY);
}

JNIEXPORT jobjectArray JNICALL Java_CheckersGame_getBoardState(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    std::vector<std::vector<int>> boardState = game->fetchBoardState();

    jobjectArray jBoardArray = env->NewObjectArray(8, env->FindClass("[I"), nullptr);
    for (int i = 0; i < 8; ++i) {
        jintArray rowArray = env->NewIntArray(8);
        env->SetIntArrayRegion(rowArray, 0, 8, boardState[i].data());
        env->SetObjectArrayElement(jBoardArray, i, rowArray);
    }

    return jBoardArray;
}

JNIEXPORT jint JNICALL Java_CheckersGame_getPlayerOneCaptured(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->getPlayerOneCaptured();
}

JNIEXPORT jint JNICALL Java_CheckersGame_getPlayerTwoCaptured(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->getPlayerTwoCaptured();
}

JNIEXPORT jboolean JNICALL Java_CheckersGame_isGameOver(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->isGameOver() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jint JNICALL Java_CheckersGame_getWinner(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->getWinner();
}

JNIEXPORT jint JNICALL Java_CheckersGame_getCurrentPlayer(JNIEnv* env, jobject obj, jlong gamePtr) {
    CheckersGame* game = reinterpret_cast<CheckersGame*>(gamePtr);
    return game->getCurrentPlayer();
}
}