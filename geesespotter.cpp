// Andrew Stekar
// November 11, 2022

#include "geesespotter_lib.h"
#include <iostream>

// creates and populates a dynamically allocated char array
char * createBoard(std::size_t xdim, std::size_t ydim) {

    char *array{new char[xdim*ydim]};
    for (std::size_t i{0}; i < xdim*ydim; ++i) {
        array[i] = '\0';
    }
    return array;
}

// deallocates the current board
void cleanBoard(char * board) {
    delete[] board;
    return;
}

// prints the current board to the console
void printBoard(char * board, std::size_t xdim, std::size_t ydim) {

    for (std::size_t i{0}; i < ydim; ++i) {
        for (std::size_t k{0}; k < xdim; ++k) {
        
            if (board[i*xdim + k] <= 9) {
                std::cout << (int)board[i*xdim + k] << "";
            } else if ((board[i*xdim + k] & markedBit()) == markedBit()) {
                std::cout << "M";
            } else {
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    }
    return;
}

// populating the board with correct numbers (based on proximity to geese)
void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim) {
    int tally{0};

    for (std::size_t i{0}; i < ydim; ++i) {
        for (std::size_t k{0}; k < xdim; ++k) {
            tally = 0;
        // if not already a goose at the location
        if (board[i*xdim + k] != 9) {
            // to the left
            if (k != 0) {
                if (i != 0) {// above
                    if (board[(i - 1)*xdim + (k - 1)] == 9) {
                        ++tally;
                    }
                } 
                if ((i + 1) != ydim) {// below
                    if (board[(i + 1)*xdim + (k - 1)] == 9) {
                        ++tally;
                    }
                }
                if (board[i*xdim + (k - 1)] == 9) {// same level
                    ++tally;
                }
            }
            // to the right
            if ((k + 1) != xdim) {
                if (i != 0) {// above
                    if (board[(i - 1)*xdim + (k + 1)] == 9) {
                        ++tally;
                    }
                }
                if ((i + 1) != ydim) {// below
                    if (board[(i + 1)*xdim + (k + 1)] == 9) {
                        ++tally;
                    }
                }
                if (board[i*xdim + (k + 1)] == 9) {// same level
                        ++tally;
                }
            }
            // above/below
            if ((i != 0) && (board[(i - 1)*xdim + k] == 9)) {
                        ++tally;
            }
            if (((i + 1) != ydim) && (board[(i + 1)*xdim + k] == 9)) {
                        ++tally;
            }
            
            // assign new value
            board[i*xdim + k] = tally;
            }
        }
    }
    return;
}

// initially making every board tile hidden
void hideBoard(char * board, std::size_t xdim, std::size_t ydim) {

    for (std::size_t i{0}; i < xdim*ydim; ++i) {
       board[i] |= hiddenBit();
    }
    return;
}

// reveals the apropriate tiles when chosen to be shown
int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

    if (((board[(yloc)*xdim + (xloc)]) & markedBit()) == markedBit()) {// marked field
        return 1;
    } else if (((board[(yloc)*xdim + (xloc)]) & hiddenBit()) == 0) {// revealed field
        return 2;
    } else if (((board[(yloc)*xdim + (xloc)]) & valueMask()) == 9) {// goose
        board[(yloc)*xdim + (xloc)] ^= hiddenBit();
        return 9;
    } else if (((board[(yloc)*xdim + (xloc)]) & valueMask()) == 0) {//zero field
        // to the left
            if (xloc != 0) {
                if ((yloc != 0) && ((board[(yloc - 1)*xdim + (xloc - 1)] & hiddenBit()) == hiddenBit())) {// above
                    board[(yloc - 1)*xdim + (xloc - 1)] ^= hiddenBit();
                } 
                if (((yloc + 1) != ydim) && ((board[(yloc + 1)*xdim + (xloc - 1)] & hiddenBit()) == hiddenBit())) {// below
                    board[(yloc + 1)*xdim + (xloc - 1)] ^= hiddenBit();
                }
                if ((board[(yloc)*xdim + (xloc - 1)] & hiddenBit()) == hiddenBit()) {// same level
                    board[(yloc)*xdim + (xloc - 1)] ^= hiddenBit();
                }
            }
            // to the right
            if ((xloc + 1) != xdim) {
                if ((yloc != 0) && ((board[((yloc - 1)*xdim + xloc + 1)] & hiddenBit()) == hiddenBit())) {// above
                    board[(yloc - 1)*xdim + (xloc + 1)] ^= hiddenBit();
                }
                if (((yloc + 1) != ydim) && ((board[(yloc + 1)*xdim + (xloc + 1)] & hiddenBit()) == hiddenBit())) {// below
                    board[(yloc + 1)*xdim + (xloc + 1)] ^= hiddenBit();
                }
                if ((board[(yloc)*xdim + (xloc + 1)] & hiddenBit()) == hiddenBit()) {// same level
                    board[(yloc)*xdim + (xloc + 1)] ^= hiddenBit();
                }
            }
            // above/below
            if ((yloc != 0) && ((board[(yloc - 1)*xdim + (xloc)] & hiddenBit()) == hiddenBit())) {
                board[(yloc - 1)*xdim + (xloc)] ^= hiddenBit();
            }
            if (((yloc + 1) != ydim) && ((board[(yloc + 1)*xdim + (xloc)] & hiddenBit()) == hiddenBit())) {
                board[(yloc + 1)*xdim + (xloc)] ^= hiddenBit();
            }
        }
        if ((board[(yloc)*xdim + (xloc)] & hiddenBit()) == hiddenBit()) {
            board[(yloc)*xdim + (xloc)] ^= hiddenBit();
        }
        return 0;
    }

// marks/unmarks the tile when chosen to be marked
int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

    if ((board[yloc*xdim + xloc] & hiddenBit()) == 0) {
        return 2;
    } else {
        board[yloc*xdim + xloc] ^= markedBit();
        return 0;
    }
}

// returns whether the game has been won
bool isGameWon(char * board, std::size_t xdim, std::size_t ydim) {

    unsigned int goose_count{0};
    unsigned int reveal_count{0};
    
    for (std::size_t i{0}; i < xdim*ydim; ++i) {
        if ((board[i] & valueMask()) == 9) {
            ++goose_count;
        } else if ((board[i] & hiddenBit()) == 0) {
            ++reveal_count;
        }
    }
    if ((goose_count + reveal_count) == (xdim*ydim)) {
        return true;
    } else {
        return false;
    }
}

