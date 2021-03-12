#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "initialize.h"
#include "texture.h"
#include "menu.h"
#include "game.h"

using namespace std;

const int gSCREEN_WIDTH = 1000;
const int gSCREEN_HEIGHT = 800;

const int MAX_TABLE_ROW = 20, MAX_TABLE_COLUMN = 20;
const int TILE_SIZE = 30;

//color variables
SDL_Color textColor = {140, 140, 140};

//game button variables
Button gImageTile[12]; // 9: mine, 10: not opened, 11: flag
Button gReplayGame, gBackHome, gTheme;
Text gClock[1000], gMine[405];

Button gTableTile[MAX_TABLE_ROW][MAX_TABLE_COLUMN];

Text gYOUWIN, gYOULOSE, gPLAYAGAIN;

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

void setupButtonAndText()
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
    //initial clock and time
    for(int i = 0; i < 1000; i++)
    {
        gClock[i].loadText("Time:" + convertNum(i), textColor);
    }
    for(int i = 0; i <= 400; i++)
    {
        gMine[i].loadText("Mine:" + convertNum(i),  textColor);
    }
    gTheme.loadImage("../Image/background.png");
    //initial replay and home
    gReplayGame.loadImage("../Image/Menu/replayButton.png");
    gBackHome.loadImage("../Image/Menu/home.png");

    //endgame
    gYOUWIN.loadText("YOU WIN", textColor);
    gYOULOSE.loadText("YOU LOSE", textColor);
    gPLAYAGAIN.loadText("Press 'SPACE' to play again", textColor);
}

void setupReplayAndHome()
{
    //size 50x50
    gReplayGame.setSize(50, 50);
    gBackHome.setSize(50, 50);

    gReplayGame.setPosition(850, 25);
    gBackHome.setPosition(910, 25);

}

void getDifficulty(int index)
{
    switch(index)
    {
    case 0:
        {
            difficulty = 0;
            TABLE_ROW = 8; TABLE_COLUMN = 8;
            MINE_NUM = 10;
            break;
        }
    case 1:
        {
            difficulty = 1;
            TABLE_ROW = 14; TABLE_COLUMN = 14;
            MINE_NUM = 36;
            break;
        }
    case 2:
        {
            difficulty = 2;
            TABLE_ROW = 20; TABLE_COLUMN = 20;
            MINE_NUM = 80;
            break;
        }
    default:
        {
            difficulty = 3;
            int inRow, inColumn, inMine;
            while(true)
            {
                cout << "ROW(MAXIMUM 20)" << endl;
                cin >> inRow;
                cout << "COLUMN(MAXIMUM 20)" << endl;
                cin >> inColumn;
                cout << "MINE(LESS THAN THE NUMBER OF TILES)" << endl;
                cin >> inMine;
                if(inMine > inRow * inColumn || inRow > 20 || inColumn > 20)
                {
                    cout << "WRONG INPUT, TRY AGAIN" << endl;
                    cout << endl;
                }
                else
                {
                    TABLE_ROW = inRow;
                    TABLE_COLUMN = inColumn;
                    MINE_NUM = inMine;
                    cout << endl;
                    break;
                }
            }
            break;
        }
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
    bool quitGame = false;
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
                quitGame = true;
                break;
            }
            if(gEndGame != 0)
            {
                quitGame = true;
                break;
            }
            //if replay or back home
            if(gReplayGame.checkIfMouseIsInButton(&gEvent))
            {
                if(gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT)
                    loopGame();
            }
            if(gBackHome.checkIfMouseIsInButton(&gEvent))
            {
                if(gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT)
                    createMenu();
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
    if(gEndGame == 1)
    {
        gYOUWIN.getText(getRect(400, 50, 200, 50));
    }
    else
    {
        gYOULOSE.getText(getRect(400, 50, 200, 50));
    }
    gPLAYAGAIN.getText(getRect(250, 700, 500, 50));
    SDL_RenderPresent(renderer);
}


void playOneGame()
{
    resetMineTable();
    randomMine();
    initializeMineTable();
    playingGame();
}


