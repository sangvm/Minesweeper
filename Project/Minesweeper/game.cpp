#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "initialize.h"
#include "texture.h"
#include "menu.h"
#include "game.h"

using namespace std;

const int gSCREEN_WIDTH = 1000;
const int gSCREEN_HEIGHT = 800;

const int MAX_TABLE_ROW = 20, MAX_TABLE_COLUMN = 20;
const int TILE_SIZE = 30;

//mixer variables
Mix_Music *gGame;
Mix_Music *gWinGame;
Mix_Music *gLoseGame;
//color variables
SDL_Color gTextColor = {140, 140, 140};

//game button variables
Button gImageTile[12]; // 9: mine, 10: not opened, 11: flag
Button gReplayGame, gBackHome, gTheme;
Text gClock[1000], gMine[405];

Button gTableTile[MAX_TABLE_ROW][MAX_TABLE_COLUMN];

//custom variables
Text gCustom[3], gNum[10];
Text gWrongInput;
Button gInputButton[3];
Button gPlayButton, gBackButton;

const int maxNumber[3] = {20, 20, 400};
int inputNum[3] = {0, 0, 1};

Text gYOUWIN, gYOULOSE, gPLAYAGAIN;

//game variables
int difficulty, MINE_NUM; // 0 : easy, 1: normal, 2: hard, 3: custom
int TABLE_ROW, TABLE_COLUMN;
int gEndGame = 0; // = -1: lose, = -1: win
int tileLeft;

int mine[MAX_TABLE_ROW][MAX_TABLE_COLUMN];// 9: mine
int opened[MAX_TABLE_ROW][MAX_TABLE_COLUMN]; //0: not opened, 1: opened, -1: flagged, 2:is not opened and in queue

int dX[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dY[8] = {0, 1, 1, 1, 0, -1, -1, -1};
/*
    manage mine table
    row x column y
    7 0 1
    6   2
    5 4 3
*/
//score variable
int mWinScore[4];
// setup game play
string convertNum(int curNum)
{
    string tmp = "";
    while(curNum > 0)
    {
        tmp = char(curNum % 10 + '0') + tmp;
        curNum /= 10;
    }
    while(tmp.size() < 3)tmp = "0" + tmp;
    return tmp;
}

void setupButtonAndTextGame()
{
    for(int i = 0; i < MAX_TABLE_ROW; i++)
    {
        for(int j = 0; j < MAX_TABLE_COLUMN; j++)
        {
            gTableTile[i][j].setSize(TILE_SIZE, TILE_SIZE);
        }
    }
    //initial tile
    for(int i = 0; i < 12; i++)
    {
        string path = "../Image/Game/";
        if(i < 9)path = path + char(i + '0') + ".png";
        if(i == 9)path = path + "bomb.png";
        if(i == 10)path = path + "facingDown.png";
        if(i == 11)path = path + "flagged.png";
        gImageTile[i].loadImage(path);
    }
    //custom input
    gPlayButton.loadImage("../Image/Menu/playButton.png");
    gPlayButton.setPosition(400, 550);
    gPlayButton.setSize(200, 75);
    gWrongInput.loadText("WRONG INPUT, TRY AGAIN", gTextColor);
    gWrongInput.setPosition(300, 650);
    gWrongInput.setSize(400, 50);
    gBackButton.loadImage("../Image/Menu/replayButton.png");
    gBackButton.setSize(50, 50);
    gBackButton.setPosition(50, 50);
    for(int i = 0; i < 10; i++)
    {
        string path = "";
        path = path + char(i + '0');
        gNum[i].loadText(path, gTextColor);
        gNum[i].setSize(40, 50);
    }
    for(int i = 0; i < 3; i ++)
    {
        string path;
        if(i == 0)path = "ROW(MINIMUM 1 - MAXIMUM 20)";
        if(i == 1)path = "COLUMN(MINIMUM 1 - MAXIMUM 20)";
        if(i == 2)path = "MINE(LESS THAN THE NUMBER OF TILES)";
        gCustom[i].setPosition(200, 200 + 100 * i);
        gCustom[i].setSize((int)path.size() * 20, 25);
        gCustom[i].loadText(path, gTextColor);
        gInputButton[i].setPosition(200, 235 + 100 * i);
        gInputButton[i].setSize(20 * 20, 50);
        gInputButton[i].loadImage("../Image/Game/input.png");
    }
    //initial clock and time
    gTheme.loadImage("../Image/background.png");
    for(int i = 0; i < 1000; i++)
    {
        gClock[i].loadText("Time:" + convertNum(i), gTextColor);
    }
    for(int i = 0; i <= 400; i++)
    {
        gMine[i].loadText("Mine:" + convertNum(i),  gTextColor);
    }
    //initial replay and home
    gReplayGame.loadImage("../Image/Menu/replayButton.png");
    gReplayGame.setSize(50, 50);
    gReplayGame.setPosition(850, 25);

    gBackHome.loadImage("../Image/Menu/home.png");
    gBackHome.setSize(50, 50);
    gBackHome.setPosition(910, 25);
    //initial win score
    for(int i = 0; i < 4; i++)
    {
        mWinScore[i] = 1000;
    }
    //endgame
    gYOUWIN.loadText("YOU WIN", gTextColor);
    gYOUWIN.setPosition(400, 50);
    gYOUWIN.setSize(200, 50);

    gYOULOSE.loadText("YOU LOSE", gTextColor);
    gYOULOSE.setPosition(400, 50);
    gYOULOSE.setSize(200, 50);

    gPLAYAGAIN.loadText("Press 'SPACE' to play again", gTextColor);
    gPLAYAGAIN.setPosition(250, 700);
    gPLAYAGAIN.setSize(500, 50);
    //setup game music
    gGame = Mix_LoadMUS("../Music/gameplay.mp3");
    gWinGame = Mix_LoadMUS("../Music/winner.mp3");
    gLoseGame = Mix_LoadMUS("../Music/loser.mp3");
}

bool getNum(int index)
{
    int i = index, maxNum = maxNumber[i];

    gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
        gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));

    int curX = gInputButton[i].mPosition.x, curY = gInputButton[i].mPosition.y;
    int ans = 0;

    int quit = 0;
    SDL_Event e;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = -1;
                break;
            }
            if(gBackButton.checkIfMouseIsInButton(&e) && leftClicked(e))
            {
                chooseDifficulty();
                quit = -1;
                break;
            }
            for(int id = 0; id < 3; id++)
            {
                if(gInputButton[id].checkIfMouseIsInButton(&e) && leftClicked(e))
                {
                    //click in custom input
                    inputNum[i] = ans;
                    //reset data
                    i = id;
                    maxNum = maxNumber[i];
                    gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
                        gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));
                    curX = gInputButton[i].mPosition.x, curY = gInputButton[i].mPosition.y;
                    ans = 0;
                    break;
                }
            }
            //start game
            if(gPlayButton.checkIfMouseIsInButton(&e) && leftClicked(e))
            {
                inputNum[i] = ans;
                if(inputNum[2] >= inputNum[0] * inputNum[1] || inputNum[0] == 0 || inputNum[1] == 0)
                {
                    gWrongInput.getText(getRect(gWrongInput.mPosition.x, gWrongInput.mPosition.y,
                        gWrongInput.mTEXT_WIDTH, gWrongInput.mTEXT_HEIGHT));
                }
                else
                {
                    TABLE_ROW = inputNum[0];
                    TABLE_COLUMN = inputNum[1];
                    MINE_NUM = inputNum[2];
                    quit = 1;
                    break;
                }
            }
            if(e.key.keysym.sym == SDLK_0 && e.type == SDL_KEYDOWN && ans > 0)
            {
                ans = ans * 10 + 0;
                gNum[0].getText(getRect(curX, curY, gNum[0].mTEXT_WIDTH, gNum[0].mTEXT_HEIGHT));
                curX += gNum[0].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_1 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 1;
                gNum[1].getText(getRect(curX, curY, gNum[1].mTEXT_WIDTH, gNum[1].mTEXT_HEIGHT));
                curX += gNum[1].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_2 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 2;
                gNum[2].getText(getRect(curX, curY, gNum[2].mTEXT_WIDTH, gNum[2].mTEXT_HEIGHT));
                curX += gNum[2].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_3 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 3;
                gNum[3].getText(getRect(curX, curY, gNum[3].mTEXT_WIDTH, gNum[3].mTEXT_HEIGHT));
                curX += gNum[3].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_4 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 4;
                gNum[4].getText(getRect(curX, curY, gNum[4].mTEXT_WIDTH, gNum[4].mTEXT_HEIGHT));
                curX += gNum[4].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_5 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 5;
                gNum[5].getText(getRect(curX, curY, gNum[5].mTEXT_WIDTH, gNum[5].mTEXT_HEIGHT));
                curX += gNum[5].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_6 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 6;
                gNum[6].getText(getRect(curX, curY, gNum[6].mTEXT_WIDTH, gNum[6].mTEXT_HEIGHT));
                curX += gNum[6].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_7 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 7;
                gNum[7].getText(getRect(curX, curY, gNum[7].mTEXT_WIDTH, gNum[7].mTEXT_HEIGHT));
                curX += gNum[7].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_8 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 8;
                gNum[8].getText(getRect(curX, curY, gNum[8].mTEXT_WIDTH, gNum[8].mTEXT_HEIGHT));
                curX += gNum[8].mTEXT_WIDTH;
            }
            if(e.key.keysym.sym == SDLK_9 && e.type == SDL_KEYDOWN)
            {
                ans = ans * 10 + 9;
                gNum[9].getText(getRect(curX, curY, gNum[9].mTEXT_WIDTH, gNum[9].mTEXT_HEIGHT));
                curX += gNum[9].mTEXT_WIDTH;
            }
//            if(e.key.keysym.sym == SDLK_BACKSPACE && e.type == SDL_KEYDOWN && ans > 0)
//            {
//                ans = ans / 10;
//                curX -= gNum[0].mTEXT_WIDTH;
//                gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
//                    gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));
//            }
            //get new maximum for MINE_NUM
            if(i == 2 && inputNum[0] != 0 && inputNum[1] != 0)maxNum = inputNum[0] * inputNum[1] - 1;
            if(ans > maxNum)
            {
                gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
                    gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));
                ans = maxNum;
                int curNum = maxNum;
                inputNum[i] = ans;
                stack<int> numStack;
                while(curNum > 0)
                {
                    numStack.push(curNum % 10);
                    curNum /= 10;
                }
                //reset position
                curX = gInputButton[i].mPosition.x, curY = gInputButton[i].mPosition.y;
                while(!numStack.empty())
                {
                    int curIndex = numStack.top();
                    numStack.pop();
                    gNum[curIndex].getText(getRect(curX, curY,
                        gNum[curIndex].mTEXT_WIDTH, gNum[curIndex].mTEXT_HEIGHT));
                    curX += gNum[curIndex].mTEXT_WIDTH;
                }
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == -1)
    {
        quitSDL(window, renderer);
    }
}
void getDifficulty(int index)
{
    if(index == 0)
    {
        difficulty = 0;
        TABLE_ROW = 8; TABLE_COLUMN = 8;
        MINE_NUM = 10;
    }
    if(index == 1)
    {
        difficulty = 1;
        TABLE_ROW = 14; TABLE_COLUMN = 14;
        MINE_NUM = 36;
    }
    if(index == 2)
    {
        difficulty = 2;
        TABLE_ROW = 20; TABLE_COLUMN = 20;
        MINE_NUM = 80;
    }
    if(index == 3)
    {
        //reset inputNum
        inputNum[0] = inputNum[1] = 0;
        inputNum[2] = 1;
        difficulty = 3;
        gTheme.getImage(getRect(0, 0, gSCREEN_WIDTH, gSCREEN_HEIGHT));
        gPlayButton.getImage(getRect(gPlayButton.mPosition.x, gPlayButton.mPosition.y,
            gPlayButton.mBUTTON_WIDTH, gPlayButton.mBUTTON_HEIGHT));
        gBackButton.getImage(getRect(gBackButton.mPosition.x, gBackButton.mPosition.y,
            gBackButton.mBUTTON_WIDTH, gBackButton.mBUTTON_HEIGHT));
        for(int i = 0; i < 3; i++)
        {
            gCustom[i].getText(getRect(gCustom[i].mPosition.x, gCustom[i].mPosition.y,
                gCustom[i].mTEXT_WIDTH, gCustom[i].mTEXT_HEIGHT));
            gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
                gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));
        }
        SDL_Event customEvent;
        int quit = 0;
        while(!quit)
        {
            while(SDL_PollEvent(&customEvent))
            {
                if(customEvent.type == SDL_QUIT)
                {
                    quit = -1;
                    break;
                }
                if(gBackButton.checkIfMouseIsInButton(&customEvent) && leftClicked(customEvent))
                {
                    chooseDifficulty();
                    quit = -1;
                    break;
                }
                for(int i = 0; i < 3; i ++)
                {
                    if(gInputButton[i].checkIfMouseIsInButton(&customEvent))
                    {
                        if(leftClicked(customEvent))
                        {
                            getNum(i);
                            quit = 1;
                            break;
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
        if(quit == -1)quitSDL(window, renderer);
    }
}

void resetMineTable()
{
    gEndGame = 0;
    memset(mine, 0, sizeof(mine));
    memset(opened, 0, sizeof(opened));
}

void randomMine()
{
    srand(time(NULL));
    //initialize table
    int randSize = TABLE_ROW * TABLE_COLUMN;
    set<int>randTable;
    for(int i = 0; i < randSize; i++)
    {
        randTable.insert(i);
    }
    //random mine
    int mineLeft = MINE_NUM;
    while(mineLeft > 0)
    {
        int cur = rand() % randSize;
        for(set<int>::iterator it = randTable.begin(); it != randTable.end(); it++)
        {
            if(cur == 0)
            {
                int id = *it;
                int idX = id / TABLE_COLUMN, idY = id % TABLE_COLUMN;
                mine[idX][idY] = 9;
                mineLeft--;
                randSize--;
                randTable.erase(it);
                break;
            }
            else
            {
                cur--;
            }
        }
    }
}
void initializeMineTable()
{
    // initialize mine table start
    int mX = (gSCREEN_WIDTH - TILE_SIZE * TABLE_COLUMN) / 2;
    int mY = (gSCREEN_HEIGHT - TILE_SIZE * TABLE_ROW) / 2;
    tileLeft = TABLE_ROW * TABLE_COLUMN - MINE_NUM;
    //manage mine table
    for(int i = 0; i < TABLE_ROW; i++)
    {
        for(int j = 0; j < TABLE_COLUMN; j++)
        {
            // get the window position of tile
            int curX = mX + j * TILE_SIZE;
            int curY = mY + i * TILE_SIZE;
            gTableTile[i][j].setPosition(curX, curY);
            // if tile(i, j) is mine
            if(mine[i][j] == 9)continue;
            //check around tile(i, j)
            for(int id = 0; id < 8; id++)
            {
                int idX = i + dX[id], idY = j + dY[id];
                if(idX < 0 || idX >= TABLE_ROW || idY < 0 || idY >= TABLE_COLUMN)continue;
                if(mine[idX][idY] == 9)
                {
                    mine[i][j]++;
                }
            }
        }
    }
}

void openTile(int i, int j)
{
    // this tile is opened
    if(gEndGame != 0)return;
    // this tile is mine
    if(mine[i][j] == 9)
    {
        gEndGame = -1; // player lose
        //show all mines left
        for(int idX = 0; idX < TABLE_ROW; idX++)
        {
            for(int idY = 0; idY < TABLE_COLUMN; idY++)
            {
                if(mine[idX][idY] == 9)
                {
                    gImageTile[mine[idX][idY]].getImage(getRect(gTableTile[idX][idY].mPosition.x,
                        gTableTile[idX][idY].mPosition.y, TILE_SIZE, TILE_SIZE));
                }
            }
        }
    }
    else
    {
        gImageTile[mine[i][j]].getImage(getRect(gTableTile[i][j].mPosition.x,
            gTableTile[i][j].mPosition.y, TILE_SIZE, TILE_SIZE));
        tileLeft--;
        if(tileLeft == 0)
        {
            gEndGame = 1; // player win
        }
    }
}

void bfs(int i, int j)
{
    if(opened[i][j] != 0)return;
    opened[i][j] = 1;
    openTile(i, j);
    queue< pair<int, int> >gQueue;
    queue< pair<int, int> >gOpen;
    gQueue.push({i, j});
    if(mine[i][j] == 0)gOpen.push({i, j});
    //find 0 tile
    while(!gQueue.empty())
    {
        int x = gQueue.front().first, y = gQueue.front().second;
        gQueue.pop();
        for(int id = 0; id < 8; id++)
        {
            int idX = x + dX[id], idY = y + dY[id];
            if(idX < 0 || idX >= TABLE_ROW || idY < 0 || idY >= TABLE_COLUMN)continue; // outside the table
            //if this tile is 0, not opened -> push it in gOpen queue
            if(mine[idX][idY] == 0 && opened[idX][idY] == 0)
            {
                opened[idX][idY] = 2;
                gQueue.push({idX, idY});
                gOpen.push({idX, idY});
            }
        }
    }
    //open tile
    while(!gOpen.empty())
    {
        int x = gOpen.front().first, y = gOpen.front().second;
        gOpen.pop();
        //if this tile is in gOpen queue and is not opened
        if(opened[x][y] == 2)
        {
            opened[x][y] = 1;
            openTile(x, y);
        }
        for(int id = 0; id < 8; id++)
        {
            int idX = x + dX[id], idY = y + dY[id];
            if(idX < 0 || idX >= TABLE_ROW || idY < 0 || idY >= TABLE_COLUMN)continue;
            //if tile(idX, idY) is opened or flagged or is mine
            if(opened[idX][idY] == 1 || opened[idX][idY] == -1 || mine[idX][idY] == 9)continue;
            opened[idX][idY] = 1;
            openTile(idX, idY);
        }
    }
}

void uploadTime(int curTime)
{
    if(curTime > 999)curTime = 999;
    gTheme.getImage(getRect(0, 0, 150, 40));
    gClock[curTime].getText(getRect(0, 0, 150, 40));
    SDL_RenderPresent(renderer);
}
void uploadMineLeft(int curMine)
{
    if(curMine < 0)curMine = 0;
    gTheme.getImage(getRect(0, 40, 150, 40));
    gMine[curMine].getText(getRect(0, 40, 150, 40));
    SDL_RenderPresent(renderer);
}

bool clickReplay(SDL_Event gEvent)
{
    if(gReplayGame.checkIfMouseIsInButton(&gEvent))
    {
        if(leftClicked(gEvent))
        {
            return true;
        }
    }
    return false;
}

bool clickBackHome(SDL_Event gEvent)
{
    if(gBackHome.checkIfMouseIsInButton(&gEvent))
    {
        if(leftClicked(gEvent))
        {
            return true;
        }
    }
    return false;
}

void playingGame()
{

    for(int i = 0; i < TABLE_ROW; i++)
        for(int j = 0; j < TABLE_COLUMN; j++)
        {
            int curX = gTableTile[i][j].mPosition.x;
            int curY = gTableTile[i][j].mPosition.y;
            gImageTile[10].getImage(getRect(curX, curY, TILE_SIZE, TILE_SIZE));
        }
    //initial clock
    bool firstClicked = false;
    clock_t startTime, endTime;
    int prevTime = 0;
    //initial count mine
    int countMine = MINE_NUM;
    uploadTime(0);
    uploadMineLeft(countMine);
    //initial replay and home
    gReplayGame.getImage(getRect(gReplayGame.mPosition.x, gReplayGame.mPosition.y,
        gReplayGame.mBUTTON_WIDTH, gReplayGame.mBUTTON_HEIGHT));
    gBackHome.getImage(getRect(gBackHome.mPosition.x, gBackHome.mPosition.y,
        gBackHome.mBUTTON_WIDTH, gBackHome.mBUTTON_HEIGHT));
    //main loop flag
    int quitGame = 0; //1:endgame, 2: replay, 3: back home
    bool quitWindow = false;
    //why the application is running
    SDL_Event gEvent;
    while(!quitGame)
    {
        //check clock
        if(firstClicked)
        {
            endTime = clock();
            int timeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
            if(timeUsed != prevTime)
            {
                prevTime = timeUsed;
                uploadTime(timeUsed);
            }
        }
        // handle events on queue
        while(SDL_PollEvent(&gEvent) != 0)
        {
            if(gEvent.type == SDL_QUIT)
            {
                quitWindow = true;
                quitGame = 1;
                break;
            }
            if(gEndGame != 0)
            {
                quitGame = 1;
                break;
            }
            //if replay or back home
            if(clickReplay(gEvent))
            {
                quitGame = 2;
                break;
            }
            if(clickBackHome(gEvent))
            {
                quitGame = 3;
                break;
            }
            // handle menu events
            for(int i = 0; i < TABLE_ROW; i++)
            {
                for(int j = 0; j < TABLE_COLUMN; j++)
                {
                    int curX = gTableTile[i][j].mPosition.x;
                    int curY = gTableTile[i][j].mPosition.y;
                    if(gTableTile[i][j].checkIfMouseIsInButton(&gEvent))
                    {
                        if(gEndGame != 0)break; // game is ended
                        if(gEvent.type == SDL_MOUSEBUTTONDOWN)
                        {
                            if(!firstClicked)
                            {
                                firstClicked = true;
                                //start music
                                Mix_PlayMusic(gGame, -1);
                                startTime = clock();
                            }
                            if(gEvent.button.button == SDL_BUTTON_LEFT)
                            {
                                bfs(i, j); // open adjacent title
                            }
                            else if(gEvent.button.button == SDL_BUTTON_RIGHT)
                            {
                                if(opened[i][j] == 1)continue; // this tile is opened
                                if(opened[i][j] == 0)
                                {
                                    if(countMine == 0)continue; // used all flags
                                    countMine--;
                                    uploadMineLeft(countMine);
                                    opened[i][j] = -1; // flagged
                                    gImageTile[11].getImage(getRect(curX, curY, TILE_SIZE, TILE_SIZE));
                                }
                                else
                                {
                                    countMine++;
                                    uploadMineLeft(countMine);
                                    opened[i][j] = 0; // take the flag
                                    gImageTile[10].getImage(getRect(curX, curY, TILE_SIZE, TILE_SIZE));
                                }
                            }
                        }
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quitWindow)
    {
        quitSDL(window, renderer);
    }
    switch(quitGame)
    {
    case 2:
        {
            //replay
            loopGame();
            break;
        }
    case 3:
        {
            //back menu
            playMainMenuMusic();
            createMenu();
            break;
        }
    default:
        {
            if(gEndGame == 1)
            {
                //get new score
                Mix_PauseMusic();
                Mix_PlayMusic(gWinGame, 0);
                mWinScore[difficulty] = min(mWinScore[difficulty], prevTime);
                gYOUWIN.getText(getRect(gYOUWIN.mPosition.x, gYOUWIN.mPosition.y,
                    gYOUWIN.mTEXT_WIDTH, gYOUWIN.mTEXT_HEIGHT));
            }
            else
            {
                Mix_PauseMusic();
                Mix_PlayMusic(gLoseGame, 0);
                gYOULOSE.getText(getRect(gYOULOSE.mPosition.x, gYOULOSE.mPosition.y,
                    gYOULOSE.mTEXT_WIDTH, gYOULOSE.mTEXT_HEIGHT));
            }
            gPLAYAGAIN.getText(getRect(gPLAYAGAIN.mPosition.x, gPLAYAGAIN.mPosition.y,
                    gPLAYAGAIN.mTEXT_WIDTH, gPLAYAGAIN.mTEXT_HEIGHT));
            SDL_RenderPresent(renderer);
            break;
        }
    }
}

int getWinScore(int index)
{
    return mWinScore[index];
}
void playOneGame()
{
    resetMineTable();
    randomMine();
    initializeMineTable();
    playingGame();
}


