#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"
#include "game.h"
#include "button.h"
#include "texture.h"

using namespace std;

//painter variables
Painter gPainter;
//button variables
Button gTile;

const int gSCREEN_WIDTH = 1000;
const int gSCREEN_HEIGHT = 800;

const int MAX_TABLE_ROW = 20, MAX_TABLE_COLUMN = 20;
const int TILE_SIZE = 30;

int difficulty, mineNum; // 0 : easy, 1: normal, 2: hard, 3: custom
int TABLE_ROW;
int TABLE_COLUMN;
int gEndGame = 0; // = -1: lose, = -1: win
int tileLeft;

int mine[MAX_TABLE_ROW][MAX_TABLE_COLUMN];// -1: mine
int opened[MAX_TABLE_ROW][MAX_TABLE_COLUMN]; //0: not opened, 1: opened, -1: flagged, 2:is not opened and in queue
pair<int, int> mWindowPos[MAX_TABLE_ROW][MAX_TABLE_COLUMN];

int dX[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dY[8] = {0, 1, 1, 1, 0, -1, -1, -1};
/*
    manage mine table
    row x column y
    7 0 1
    6   2
    5 4 3
*/

void getDifficulty(int index)
{
    switch(index)
    {
    case 0:
        {
            difficulty = 0;
            TABLE_ROW = 8; TABLE_COLUMN = 8;
            mineNum = 10;
            break;
        }
    case 1:
        {
            difficulty = 1;
            TABLE_ROW = 14; TABLE_COLUMN = 14;
            mineNum = 36;
            break;
        }
    case 2:
        {
            difficulty = 2;
            TABLE_ROW = 20; TABLE_COLUMN = 20;
            mineNum = 80;
            break;
        }
    default:
        {
            difficulty = 3;
            TABLE_ROW = 20; TABLE_COLUMN = 20;
            mineNum = 80;
            break;
        }
    }
}

void resetMineTable()
{
    gEndGame = 0;
    memset(mine, 0, sizeof(mine));
    memset(opened, 0, sizeof(opened));
    memset(mWindowPos, 0, sizeof(mWindowPos));
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
    int mineLeft = mineNum;
    while(mineLeft > 0)
    {
        int cur = rand() % randSize;
        for(set<int>::iterator it = randTable.begin(); it != randTable.end(); it++)
        {
            if(cur == 0)
            {
                int id = *it;
                int idX = id / TABLE_COLUMN, idY = id % TABLE_COLUMN;
                mine[idX][idY] = -1;
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
    tileLeft = TABLE_ROW * TABLE_COLUMN - mineNum;
    //manage mine table
    for(int i = 0; i < TABLE_ROW; i++)
    {
        for(int j = 0; j < TABLE_COLUMN; j++)
        {
            // get the window position of tile
            int curX = mX + j * TILE_SIZE;
            int curY = mY + i * TILE_SIZE;
            mWindowPos[i][j] = {curX, curY};
            // if tile(i, j) is mine
            if(mine[i][j] == -1)continue;
            //check around tile(i, j)
            for(int id = 0; id < 8; id++)
            {
                int idX = i + dX[id], idY = j + dY[id];
                if(idX < 0 || idX >= TABLE_ROW || idY < 0 || idY >= TABLE_COLUMN)continue;
                if(mine[idX][idY] == -1)
                {
                    mine[i][j]++;
                }
            }
        }
    }
    for(int i = 0; i < TABLE_ROW; i++)
    {
        for(int j = 0; j < TABLE_COLUMN; j++)
        {
            if(mine[i][j] == -1)cout << "? ";
            else
                cout << mine[i][j] << " ";
        }
        cout << endl;
    }
    cout << "<<<<<<<<<" << endl;
    cout << endl;
}

void openTile(int i, int j)
{
    if(gEndGame != 0)return;
    // this tile is opened
    string path = "../Texture/Game/";
    // this tile is mine
    if(mine[i][j] == -1)
    {
        gEndGame = -1; // player lose
        path = path + "bomb.png";
        //show all mines left
        for(int idX = 0; idX < TABLE_ROW; idX++)
        {
            for(int idY = 0; idY < TABLE_COLUMN; idY++)
            {
                if(mine[idX][idY] == -1)
                {
                    gPainter.getImage(path, getRect(mWindowPos[idX][idY].first,
                                        mWindowPos[idX][idY].second, TILE_SIZE, TILE_SIZE));
                }
            }
        }
    }
    else
    {
        path = path + char(mine[i][j] + '0') + ".png";
        gPainter.getImage(path, getRect(mWindowPos[i][j].first, mWindowPos[i][j].second,
                                     TILE_SIZE, TILE_SIZE));
        tileLeft--;
        if(tileLeft == 0)
        {
            gEndGame = 1;
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
            if(opened[idX][idY] == 1 || opened[idX][idY] == -1 || mine[idX][idY] == -1)continue;
            opened[idX][idY] = 1;
            openTile(idX, idY);
        }
    }
}

void playGame()
{

    for(int i = 0; i < TABLE_ROW; i++)
        for(int j = 0; j < TABLE_COLUMN; j++)
        {
            int curX = mWindowPos[i][j].first;
            int curY = mWindowPos[i][j].second;
            gPainter.getImage("../Texture/Game/facingdown.png",
                               getRect(curX, curY, TILE_SIZE, TILE_SIZE));
        }
    SDL_RenderPresent(renderer);

    //main loop flag
    bool quitGame = false;
    bool quitWindow = false;

    //why the application is running
    SDL_Event gEvent;

    gTile.setSize(TILE_SIZE, TILE_SIZE);
    while(!quitGame)
    {
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
            // handle menu events
            for(int i = 0; i < TABLE_ROW; i++)
            {
                for(int j = 0; j < TABLE_COLUMN; j++)
                {
                    int curX = mWindowPos[i][j].first;
                    int curY = mWindowPos[i][j].second;
                    gTile.setPosition(curX, curY);
                    if(gTile.checkIfMouseIsInButton(&gEvent))
                    {
                        if(gEndGame != 0)break; // game is ended
                        if(gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT)
                        {
                            bfs(i, j); // open adjacent title
                        }
                        else if(gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_RIGHT)
                        {
                            if(opened[i][j] == 1)continue; // this tile is opened
                            if(opened[i][j] == 0)
                            {
                                opened[i][j] = -1; // flagged
                                gPainter.getImage("../Texture/Game/flagged.png",
                                            getRect(curX, curY, TILE_SIZE, TILE_SIZE));
                            }
                            else
                            {
                                opened[i][j] = 0; // take the flag
                                gPainter.getImage("../Texture/Game/facingDown.png",
                                            getRect(curX, curY, TILE_SIZE, TILE_SIZE));
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
}

int getEndGame()
{
    return gEndGame;
}

void playAgain()
{
    resetMineTable();
    randomMine();
    initializeMineTable();
    playGame();
}


