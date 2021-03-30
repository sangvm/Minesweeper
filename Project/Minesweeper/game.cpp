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
const int INF = 1e9;
const int mFrame = 8;

const int MAX_TABLE_ROW = 20, MAX_TABLE_COLUMN = 20;
const int TILE_SIZE = 30;

//mixer variables
Mix_Music *gGame;
Mix_Music *gWinGame;
Mix_Music *gLoseGame;
//color variables
SDL_Color gTextColor = {140, 140, 140};

//character variables
Button gCharButton[4];
Character gCharacter[4], gCurChar;
Text gChooseCharacter;
int charUsed;

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
int tileLeft, countMine;

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
    //choose character
    gChooseCharacter.loadText("CHOOSE CHARACTER", gTextColor);
    gChooseCharacter.setPosition(300, 100);
    gChooseCharacter.setSize(400, 75);
    for(int i = 0; i < 4; i ++)
    {
        gCharacter[i].setupChar();
        if(i == 1)
        {
            gCharacter[i].mineTime = 10;
            gCharacter[i].scoreTime = 60;
            gCharacter[i].extraSkill = 33;
        }
        if(i == 2)
        {
            gCharacter[i].mineTime = 11;
            gCharacter[i].extraSkill = 50;
        }
        if(i == 3)
        {
            gCharacter[i].mineTime = 8;
            gCharacter[i].scoreTime = -15;
            gCharacter[i].extraSkill = 40;
        }
        string path = "../Image/Character/character";
        path = path + char(i + '0') + ".png";
        gCharButton[i].loadImage(path);
        gCharButton[i].setPosition(300, 200 + i * 125);
        gCharButton[i].setSize(400, 100);
    }
    //custom input
    gPlayButton.loadImage("../Image/Menu/playButton.png");
    gPlayButton.setPosition(400, 550);
    gPlayButton.setSize(200, 75);
    gWrongInput.loadText("WRONG INPUT, TRY AGAIN", gTextColor);
    gWrongInput.setPosition(300, 650);
    gWrongInput.setSize(400, 50);
    gBackButton.loadImage("../Image/Menu/replayButton.png");
    gBackButton.setPosition(100, 200);
    gBackButton.setSize(50, 50);
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

void getNum(int index)
{
    int i = index, maxNum = maxNumber[i];
    gInputButton[i].getImage(getRect(gInputButton[i].mPosition.x, gInputButton[i].mPosition.y,
        gInputButton[i].mBUTTON_WIDTH, gInputButton[i].mBUTTON_HEIGHT));

    int curX = gInputButton[i].mPosition.x, curY = gInputButton[i].mPosition.y;
    int ans = 0, quit = 0;
    SDL_Event e;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(gBackButton.checkIfMouseIsInButton(&e) && leftClicked(e))
            {
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
    if(quit == INF)quitSDL(window, renderer);
    if(quit == -1)chooseDifficulty();
}
void getDifficulty(int index)
{
    if(index == 0)
    {
        difficulty = 0;
        TABLE_ROW = 8; TABLE_COLUMN = 8;
        MINE_NUM = 8;
        chooseCharacter();
    }
    if(index == 1)
    {
        difficulty = 1;
        TABLE_ROW = 14; TABLE_COLUMN = 14;
        MINE_NUM = 28;
        chooseCharacter();
    }
    if(index == 2)
    {
        difficulty = 2;
        TABLE_ROW = 20; TABLE_COLUMN = 20;
        MINE_NUM = 69;
        chooseCharacter();
    }
    if(index == 3)
    {
        //reset input number
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
                    quit = INF;
                    break;
                }
                if(gBackButton.checkIfMouseIsInButton(&customEvent) && leftClicked(customEvent))
                {
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
        if(quit == INF)quitSDL(window, renderer);
        if(quit == -1)chooseDifficulty();
        if(quit == 1)chooseCharacter();
    }
}

void chooseCharacter()
{
    gTheme.getImage(getRect(0, 0, gSCREEN_WIDTH, gSCREEN_HEIGHT));
    gChooseCharacter.getText(getRect(gChooseCharacter.mPosition.x, gChooseCharacter.mPosition.y,
        gChooseCharacter.mTEXT_WIDTH, gChooseCharacter.mTEXT_HEIGHT));
    gBackButton.getImage(getRect(gBackButton.mPosition.x, gBackButton.mPosition.y,
        gBackButton.mBUTTON_WIDTH, gBackButton.mBUTTON_HEIGHT));
    for(int i = 0; i < 4; i++)
    {
        gCharButton[i].getImage(getRect(gCharButton[i].mPosition.x, gCharButton[i].mPosition.y,
            gCharButton[i].mBUTTON_WIDTH, gCharButton[i].mBUTTON_HEIGHT));
    }
    int quit = 0;
    SDL_Event gChooseChar;
    while(!quit)
    {
        while(SDL_PollEvent(&gChooseChar))
        {
            if(gChooseChar.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(gBackButton.checkIfMouseIsInButton(&gChooseChar) && leftClicked(gChooseChar))
            {
                quit = -1;
                break;
            }
            for(int i = 0; i < 4; i++)
            {
                if(gCharButton[i].checkIfMouseIsInButton(&gChooseChar) && leftClicked(gChooseChar))
                {
                    charUsed = i;
                    gCurChar = gCharacter[i];
                    quit = 1;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == INF)quitSDL(window, renderer);
    if(quit == -1)chooseDifficulty();
    if(quit == 1)loopGame();
}

void resetMineTable()
{
    countMine = MINE_NUM;
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

bool randomExtraSkill(int index)
{
    int cur = rand() % 100;
    return cur < index;
}

int randExtraTime(int index)
{
    int cur = rand() % 100;
    if(index > 0)
    {
        return cur < index;
    }
    if(cur < abs(index))return -1;
    return 0;
}
void openTile(int i, int j)
{
    // this tile is opened
    if(gEndGame != 0)return;
    // this tile is mine
    if(mine[i][j] == 9)
    {
        int curLife = 1;
        //skill char 1
        if(charUsed == 1 && randomExtraSkill(gCurChar.extraSkill))
        {
            curLife++;
            if(gCurChar.extraSkill > 0)gCurChar.extraSkill  -= 11;
        }
        if(curLife == 2)
        {
            gImageTile[mine[i][j]].getImage(getRect(gTableTile[i][j].mPosition.x,
                gTableTile[i][j].mPosition.y, TILE_SIZE, TILE_SIZE));
        }
        else
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
    if(gReplayGame.checkIfMouseIsInButton(&gEvent) && leftClicked(gEvent))
        return true;
    return false;
}

bool clickBackHome(SDL_Event gEvent)
{
    if(gBackHome.checkIfMouseIsInButton(&gEvent) && leftClicked(gEvent))
        return true;
    return false;
}

bool addMine()
{
    int tableSize = TABLE_ROW * TABLE_COLUMN, randSize = 0;
    set<int>randTable;
    for(int i = 0; i < tableSize; i++)
    {
        int idX = i / TABLE_COLUMN, idY = i % TABLE_COLUMN;
        //open[idX][idY] = -1 because player can use flag at wrong tiles
        if(mine[idX][idY] == 9 || opened[idX][idY] == 1)continue;
        randTable.insert(i);
        randSize++;
    }
    int cur = rand() % randSize;
    for(set<int>::iterator it = randTable.begin(); it != randTable.end(); it++)
    {
        if(cur == 0)
        {
            int id = *it;
            int i = id / TABLE_COLUMN, j = id % TABLE_COLUMN;
            mine[i][j] = 9;
            tileLeft--;
            countMine++;
            uploadMineLeft(countMine);
            if(tileLeft == 0)return true;
            for(int id = 0; id < 8; id++)
            {
                int idX = i + dX[id], idY = j + dY[id];
                if(idX < 0 || idX >= TABLE_ROW || idY < 0 || idY >= TABLE_COLUMN)continue;
                //avoid 10 and 11 tiles
                if(mine[idX][idY] < 8)mine[idX][idY]++;
                if(opened[idX][idY] == 1)
                {
                    tileLeft++;
                    openTile(idX, idY);
                }
            }
            return false;
        }
        else
        {
            cur--;
        }
    }
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
    int prevTime = 0, addTime = 0;
    uploadTime(0);
    uploadMineLeft(countMine);
    //initial replay and home
    gReplayGame.getImage(getRect(gReplayGame.mPosition.x, gReplayGame.mPosition.y,
        gReplayGame.mBUTTON_WIDTH, gReplayGame.mBUTTON_HEIGHT));
    gBackHome.getImage(getRect(gBackHome.mPosition.x, gBackHome.mPosition.y,
        gBackHome.mBUTTON_WIDTH, gBackHome.mBUTTON_HEIGHT));
    //start music
    Mix_PlayMusic(gGame, -1);
    //main loop flag
    int quit = 0; //1:endgame, 2: replay, 3: back home
    bool terroristAddMine = false;
    //why the application is running
    SDL_Event gEvent;
    while(!quit)
    {
        //check clock
        if(firstClicked)
        {
            endTime = clock();
            int timeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
            if(timeUsed != prevTime)
            {
                if(timeUsed % gCurChar.mineTime != 0)
                {
                   terroristAddMine = false;
                }
                addTime += randExtraTime(gCurChar.scoreTime);
                prevTime = timeUsed;
                uploadTime(timeUsed + addTime);
            }
            if(charUsed != 0 && timeUsed != 0 && timeUsed % gCurChar.mineTime == 0 && !terroristAddMine)
            {
                terroristAddMine = true;
                if(addMine())
                {
                    gEndGame = -1;
                    quit = 1;
                    break;
                }
                //skill char 3
                if(charUsed == 3 && randomExtraSkill(gCurChar.extraSkill))
                {
                    if(addMine())
                    {
                        gEndGame = -1;
                        quit = 1;
                        break;
                    }
                }
            }
        }
        // handle events on queue
        while(SDL_PollEvent(&gEvent) != 0)
        {
            if(gEvent.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(gEndGame != 0)
            {
                quit = 1;
                break;
            }
            //if replay or back home
            if(clickReplay(gEvent))
            {
                quit = -1;
                break;
            }
            if(clickBackHome(gEvent))
            {
                quit = -2;
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
                                    //skill char 2
                                    if(charUsed == 2 && randomExtraSkill(gCurChar.extraSkill))
                                    {
                                        if(addMine())
                                        {
                                            gEndGame = -1;
                                            quit = 1;
                                            break;
                                        }
                                    }
                                    countMine--;
                                    uploadMineLeft(countMine);
                                    opened[i][j] = -1; // flagged
                                    gImageTile[11].getImage(getRect(curX, curY,
                                        TILE_SIZE, TILE_SIZE));
                                }
                                else
                                {
                                    countMine++;
                                    uploadMineLeft(countMine);
                                    opened[i][j] = 0; // take the flag
                                    gImageTile[10].getImage(getRect(curX, curY,
                                        TILE_SIZE, TILE_SIZE));
                                }
                            }
                        }
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == INF)quitSDL(window, renderer);
    if(quit == 1)
    {
        Mix_PauseMusic();
        if(gEndGame == 1)
        {
            //get new score
            Mix_PlayMusic(gWinGame, 0);
            mWinScore[difficulty] = min(mWinScore[difficulty], prevTime + addTime);
            gYOUWIN.getText(getRect(gYOUWIN.mPosition.x, gYOUWIN.mPosition.y,
                gYOUWIN.mTEXT_WIDTH, gYOUWIN.mTEXT_HEIGHT));
        }
        else
        {
            Mix_PlayMusic(gLoseGame, 0);
            gYOULOSE.getText(getRect(gYOULOSE.mPosition.x, gYOULOSE.mPosition.y,
                gYOULOSE.mTEXT_WIDTH, gYOULOSE.mTEXT_HEIGHT));
        }
        gPLAYAGAIN.getText(getRect(gPLAYAGAIN.mPosition.x, gPLAYAGAIN.mPosition.y,
                gPLAYAGAIN.mTEXT_WIDTH, gPLAYAGAIN.mTEXT_HEIGHT));
        SDL_RenderPresent(renderer);
    }
    if(quit == -1)loopGame();
    if(quit == -2)
    {
        //back menu
        playMainMenuMusic();
        createMenu();
    }
}

int getWinScore(int index)
{
    return mWinScore[index];
}
void playOneGame()
{
    gCurChar = gCharacter[charUsed];
    resetMineTable();
    randomMine();
    initializeMineTable();
    playingGame();
}


