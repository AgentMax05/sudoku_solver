#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include <thread>

#include "RenderWindow.hpp"
#include "Clock.hpp"

using namespace std;
typedef SDL_Texture Texture; // shorten typename

const int SCREENWIDTH = 451, SCREENHEIGHT = 450;
const int REFRESHRATE = 60;

float solveSpeed = REFRESHRATE * 5;
vector<Entity> numSprites = {};
vector<Entity> gridSquares = {};
Entity* visualizerSquare = NULL;
bool solving = false;

int mainloop(RenderWindow& window);

int main(int argc, char* args[]) {

    // initialize sdl video
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        cout << "SDL INIT FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
    }
    if (!IMG_Init(IMG_INIT_PNG)) {
        cout << "SDL IMAGE INIT FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
    }
    
    RenderWindow window("Sudoku Solver", SCREENWIDTH, SCREENHEIGHT);

    mainloop(window);

    window.cleanUp();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

Entity* getClickedSquare(float mouseX, float mouseY, vector<Entity>& squares) {
    for (int i = 0; i < squares.size(); i++) {
        if (squares[i].isClicked(mouseX, mouseY)) {
            return &squares[i];
        }
    }
    return NULL;
}

Entity numToEntity(float x, float y, Texture* numbers, int num) {
    if (num > 9) num = 9;
    Entity newEntity(x, y, 32, 32, 32, 32, numbers);
    newEntity.setCurrentFrame({(num - 1) * 32, 0, 32, 32});
    return newEntity;
}

int count(vector<int>& nums, int num) {
    int counter = 0;
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == num) {
            counter++;
        }
    }
    return counter;
}

vector<vector<int>> squareIndexes = {
    {0,  1,  2,  9,  10, 11, 18, 19, 20},
    {3,  4,  5,  12, 13, 14, 21, 22, 23},
    {6,  7,  8,  15, 16, 17, 24, 25, 26},
    {27, 28, 29, 36, 37, 38, 45, 46, 47},
    {30, 31, 32, 39, 40, 41, 48, 49, 50},
    {33, 34, 35, 42, 43, 44, 51, 52, 53},
    {54, 55, 56, 63, 64, 65, 72, 73, 74},
    {57, 58, 59, 66, 67, 68, 75, 76, 77},
    {60, 61, 62, 69, 70, 71, 78, 79, 80}
};

int findNextBlank(vector<int>& board, int currentIndex) {
    for (int i = currentIndex + 1; i < board.size(); i++) {
        if (board[i] == 0) {
            return i;
        }
    }
    return -1;
}

bool checkLegal(vector<int>& board) {
    // check rows
    for (int row = 0; row < 9; row++) {
        vector<int> used_nums = {};
        for (int column = 0; column < 9; column++) {
            int index = row * 9 + column;
            if (board[index] != 0) {
                used_nums.push_back(board[index]);
                if (count(used_nums, board[index]) > 1) {
                    return false;
                }
            }
        }
    }

    // check columns
    for (int column = 0; column < 9; column++) {
        vector<int> used_nums = {};
        for (int row = 0; row < 9; row++) {
            int index = row * 9 + column;
            if (board[index] != 0) {
                used_nums.push_back(board[index]);
                if (count(used_nums, board[index]) > 1) {
                    return false;
                }
            }
        }
    }

    // check squares
    for (int i = 0; i < squareIndexes.size(); i++) {
        vector<int>& currentSquare = squareIndexes[i];
        vector<int> used_nums = {};
        for (int j = 0; j < currentSquare.size(); j++) {
            if (board[currentSquare[j]] != 0) {
                used_nums.push_back(board[currentSquare[j]]);
                if (count(used_nums, board[currentSquare[j]]) > 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void setSquareNum(int num, vector<Entity>& numSprites, Texture* numbers, Entity* square) {
    for (int i = 0; i < numSprites.size(); i++) {
        if (numSprites[i].getX() - 9 == square->getX() && numSprites[i].getY() - 9 == square->getY()) {
            numSprites.erase(numSprites.begin() + i);
            break;
        }
    }
    if (num > 0) {
        Entity newNum = numToEntity(square->getX() + 9, square->getY() + 9, numbers, num);
        numSprites.push_back(newNum);
    }
}

int nextLegalNum(vector<int> board, int index, int num) {
    if (num > 9) {
        return -1;
    }
    board[index] = num;
    while (!checkLegal(board)) {
        if (num == 9) {
            return -1;
        }
        num++;
        board[index] = num;
    }
    return num;
}

void printBoard(vector<int>& board) {
    for (int i = 0; i < board.size(); i++) {
        cout << board[i] << ' ';
        if ((i + 1) % 9 == 0) {
            cout << '\n';
        }
    }
    cout << '\n';
}

bool solveBoard(int index, vector<int>& board, Texture* numbersWrong, Texture* numbersPossible, Clock* solveClock = NULL) {
    if (index == -1) {
        return true;
    }
    
    if (solveClock != NULL) {
        solveClock->tick();
    }
    visualizerSquare = &gridSquares[index];
    int num = nextLegalNum(board, index, 1);
    if (num == -1) {
        return false;
    } 
    setSquareNum(num, numSprites, numbersPossible, &gridSquares[index]);
    board[index] = num;

    if (index == board.size() - 1) {
        return true;
    }

    while (!solveBoard(findNextBlank(board, index), board, numbersWrong, numbersPossible, solveClock)) {
        num = nextLegalNum(board, index, num + 1);
        if (num == -1) {
            setSquareNum(0, numSprites, numbersPossible, &gridSquares[index]);
            board[index] = 0;
            return false;
        }
        setSquareNum(num, numSprites, numbersPossible, &gridSquares[index]);
        board[index] = num;
    }
    return true;
}

vector<int> startSolve(vector<int>& boardP, Texture* numbersWrong, Texture* numbersPossible) {
    vector<int> board = boardP;
    Clock* solveClock = NULL;
    if (solveSpeed != 0) {
        Clock clock(solveSpeed);
        solveClock = &clock;
    }
    solveBoard(findNextBlank(board, -1), board, numbersWrong, numbersPossible, solveClock);
    return board;
}

struct callSolveThreadArgs {
    vector<int>& board;
    Texture* numbersWrong;
    Texture* numbersPossible;
    Texture* numbersSolved;
};

// void callSolve(vector<int>& board, Texture* numbersWrong, Texture* numbersPossible, Texture* numbersSolved) {
void callSolve(callSolveThreadArgs& threadArgs) {
    solving = true;
    vector<int>& board = threadArgs.board;
    Texture* numbersWrong = threadArgs.numbersWrong;
    Texture* numbersPossible = threadArgs.numbersPossible;
    Texture* numbersSolved = threadArgs.numbersSolved;

    vector<int> solvedBoard = startSolve(board, numbersWrong, numbersPossible);
    printBoard(solvedBoard);
    for (int i = 0; i < board.size(); i++) {
        if (board[i] == 0) {
            setSquareNum(solvedBoard[i], numSprites, numbersSolved, &gridSquares[i]);
        }
    }
    visualizerSquare = NULL;
    solving = false;
}

int mainloop(RenderWindow& window) {
    bool mainloopRunning = true;
    SDL_Event event;
    Clock clock(REFRESHRATE);

    Texture* numbers = window.loadTexture("./res/numbers4.png");
    Texture* numbersSolved = window.loadTexture("./res/numbers4_green.png");
    Texture* numbersWrong = window.loadTexture("./res/numbers4_red.png");
    Texture* numbersPossible = window.loadTexture("./res/numbers4_blue.png");
    Texture* number0 = window.loadTexture("./res/number_0.png");

    Texture* speedText = window.loadTexture("./res/speed_text.png");

    SDL_Surface* windowIcon = window.loadSurface("./res/icon.png");
    window.setIcon(windowIcon);

    Entity speedEntity(0, 0, 140, 47, 140, 47, speedText);
    speedEntity.setX((SCREENWIDTH / 2) - (speedEntity.getCurrentFrame().w) / 2);
    speedEntity.setY((SCREENHEIGHT / 2) - (speedEntity.getCurrentFrame().h / 2) - 40);

    Entity speedNotif = numToEntity(0, 0, numbers, 5);
    speedNotif.setX((SCREENWIDTH / 2) - (speedNotif.getCurrentFrame().w) / 2);
    speedNotif.setY((SCREENHEIGHT / 2) - (speedNotif.getCurrentFrame().h / 2) + 30);

    Texture* black_outline = window.loadTexture("./res/black_outline3.png");

    vector<int> board = {
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            Entity gridSquare(x * 50, y * 50, 50, 50, 50, 50, black_outline);
            gridSquares.push_back(gridSquare);
        }
    }

    int mouseX, mouseY;
    bool shiftDown = false;

    Entity* clickedSquare = &gridSquares[0];
    int clickedSquareIndex = 0;

    int highlightX = 0;
    int highlightY = 0;
    bool highlight = true;

    while (mainloopRunning) {
        clock.tick();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&mouseX, &mouseY);
                    clickedSquare = getClickedSquare(mouseX, mouseY, gridSquares);
                    if (clickedSquare != NULL) {
                        highlight = true;
                        highlightX = clickedSquare->getX();
                        highlightY = clickedSquare->getY();

                        clickedSquareIndex = (clickedSquare->getY() / 50) * 9 + (clickedSquare->getX() / 50);
                    }
                }
            }

            if (clickedSquare != NULL && event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_9) {
                    if (shiftDown) {
                        solveSpeed = REFRESHRATE * (event.key.keysym.sym - SDLK_1 + 1);      
                        speedNotif = numToEntity(speedNotif.getX(), speedNotif.getY(), numbers, event.key.keysym.sym - SDLK_1 + 1);        
                    } else {
                        board[clickedSquareIndex] = event.key.keysym.sym - 48;
                        setSquareNum(event.key.keysym.sym - 48, numSprites, numbers, clickedSquare);
                    }
                }
                else if (event.key.keysym.sym == SDLK_0) {
                    solveSpeed = 0;
                    speedNotif.setTexture(number0);   
                    speedNotif.setCurrentFrame({0, 0, 32, 32});     
                }

                else if (!solving && event.key.keysym.sym == SDLK_RETURN) {
                    if (checkLegal(board)) {
                        // thread solveThread(callSolve, board, numbersWrong, numbersPossible, numbersSolved);
                        callSolveThreadArgs args = {board, numbersWrong, numbersPossible, numbersSolved};
                        thread solveThread(callSolve, ref(args));
                        solveThread.detach();
                        // solveThread.join();
                        // callSolve(board, numbersWrong, numbersPossible, numbersSolved);
                    }
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (clickedSquareIndex > 0) {
                        clickedSquareIndex--;
                        clickedSquare = &gridSquares[clickedSquareIndex];
                        highlightX = clickedSquare->getX();
                        highlightY = clickedSquare->getY();
                    }
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (clickedSquareIndex < 80) {
                        clickedSquareIndex++;
                        clickedSquare = &gridSquares[clickedSquareIndex];
                        highlightX = clickedSquare->getX();
                        highlightY = clickedSquare->getY();
                    }
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    if (clickedSquareIndex > 8) {
                        clickedSquareIndex -= 9;
                        clickedSquare = &gridSquares[clickedSquareIndex];
                        highlightX = clickedSquare->getX();
                        highlightY = clickedSquare->getY();
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (clickedSquareIndex < 72) {
                        clickedSquareIndex += 9;
                        clickedSquare = &gridSquares[clickedSquareIndex];
                        highlightX = clickedSquare->getX();
                        highlightY = clickedSquare->getY();
                    }
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    board = {
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0
                    };
                    numSprites = {};
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    board[clickedSquareIndex] = 0;
                    setSquareNum(0, numSprites, numbers, clickedSquare);
                }
                else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                    shiftDown = true;
                }
            }
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                    shiftDown = false;
                }
            }
        }

        window.clear({255, 255, 255});


        for (int i = 0; i < gridSquares.size(); i++) {
            window.render(gridSquares[i]);
        }

        if (visualizerSquare != NULL) {
            window.drawRect({(int)visualizerSquare->getX() + 1, (int)visualizerSquare->getY() + 1, 48, 48}, {0, 255, 255}, 128, true);
            window.drawRect({(int)visualizerSquare->getX(), (int)visualizerSquare->getY(), 50, 50}, {0, 0, 0}, 255, false);
        }

        if (highlight) {
            window.drawRect({highlightX + 1, highlightY + 1, 48, 48}, {255, 255, 0}, 128, true);
            // window.drawRect(({highlightX, highlightY, 50, 50}, {0, 0, 0}, 255, false));
            window.drawRect({highlightX, highlightY, 50, 50}, {0, 0, 0}, 255, false);
        }

        for (int i = 1; i < 3; i++) {
            window.drawLine({{0, i * 150 + 1}, {450, i * 150 + 1}}, {0, 0, 0});
            window.drawLine({{0, i * 150}, {450, i * 150}}, {0, 0, 0});
            window.drawLine({{0, i * 150 - 1}, {450, i * 150 - 1}}, {0, 0, 0});
            window.drawLine({{i * 150 + 1, 0}, {i * 150 + 1, 450}}, {0, 0, 0});
            window.drawLine({{i * 150, 0}, {i * 150, 450}}, {0, 0, 0});
            window.drawLine({{i * 150 - 1, 0}, {i * 150 - 1, 450}}, {0, 0, 0});
        }

    
        for (int i = 0; i < numSprites.size(); i++) {
            window.render(numSprites[i]);
        }

        if (shiftDown) {
            window.drawOpaqueRect({125, 125, 200, 200}, {230, 230, 230, 200}, true);
            window.render(speedEntity);
            window.render(speedNotif);
        }

        window.display();
    }
}