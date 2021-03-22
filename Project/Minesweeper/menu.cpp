#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"
#include "texture.h"
#include "menu.h"
#include "game.h"

using namespace std;

const int mSCREEN_WIDTH = 1000;
const int mSCREEN_HEIGHT = 800;
int TEXT_SIZE = 35;
bool mPlayerClickedHome = false;

//menu button variables
Button mBackGround;
Button mPlayButton, mHighScoreButton, mBackButton;
Button mDifficulty[4]; //0:easy, 1: medium, 2:hard, 3:custom
//high score variables
Text mDiffText[4], mNumText[1005];
SDL_Color mTextColor = {140, 140, 140};
int mHighscore[4], mCharWidth[4];

//setup menu
void setupMenuButton()
{
    mBackGround.loadImage("../Image/background.png");

    //load menu button
    int idX, idY;
    idX = 400; idY = 300;
    mPlayButton.loadImage("../Image/Menu/playButton.png");
    mPlayButton.setSize(200, 75);
    mPlayButton.setPosition(idX, idY);

    mHighScoreButton.loadImage("../Image/Menu/highscoreButton.png");
    mHighScoreButton.setSize(200, 75);
    mHighScoreButton.setPosition(idX, idY + 100);

    mBackButton.loadImage("../Image/Menu/replayButton.png");
    mBackButton.setSize(50, 50);
    mBackButton.setPosition(50, 50);

    idX = 400; idY = 200;
    //difficulty button 200x75
    for(int i = 0; i < 4; i++)
    {
        mDifficulty[i].setSize(200, 75);
        mDifficulty[i].setPosition(idX, idY + i * 100);

        string path = "../Image/Menu/";
        if(i == 0)path = path + "easyButton.png";
        if(i == 1)path = path + "mediumButton.png";
        if(i == 2)path = path + "hardButton.png";
        if(i == 3)path = path + "customButton.png";
        mDifficulty[i].loadImage(path);
    }
    //get high score
    for(int i = 0; i < 4; i++)
    {
        string path = "";
        if(i == 0)path = "EASY:";
        if(i == 1)path = "MEDIUM:";
        if(i == 2)path = "HARD:";
        if(i == 3)path = "CUSTOM:";
        mDiffText[i].loadText(path, mTextColor);
        mDiffText[i].mTEXT_WIDTH = (int)path.size() * TEXT_SIZE;
        mDiffText[i].mTEXT_HEIGHT = TEXT_SIZE;
    }
    for(int i = 0; i < 1000; i++)
    {
        mNumText[i].loadText(convertNum(i), mTextColor);
    }
    mNumText[1000].loadText("NO DATA", mTextColor);
    //read high score
    ifstream infile;
    infile.open("highscore.txt", ios::in);
    for(int i = 0; i < 4; i++)
    {
        mHighscore[i] = 1000;
    }
    int x = 0, y = 1000;
    while(!infile.eof())
    {
        infile >> x >> y;
        mHighscore[x] = min(mHighscore[x], y);
    }
}

void addHighScore()
{
    ofstream outfile;
    outfile.open("highscore.txt", ios::app);
    for(int i = 0; i < 4; i++)
    {
        outfile << i << " " << mHighscore[i] << endl;
    }
}

bool playAgain()
{
    SDL_Event mAgainEvent;
    while(true)
    {
        while(SDL_PollEvent(&mAgainEvent))
        {
            if(mAgainEvent.type == SDL_QUIT)
            {
                return false;
            }
            if(mAgainEvent.type == SDL_KEYDOWN && mAgainEvent.key.keysym.sym == SDLK_SPACE)
            {
                return true;
            }
            if(clickReplay(mAgainEvent))
            {
                return true;
            }
            if(clickBackHome(mAgainEvent))
            {
                mPlayerClickedHome = true;
                return false;
            }
        }
    }
    return false;
}

void loopGame()
{
    bool playerPlayAgain = false;
    do
    {
        mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
        playOneGame();
        playerPlayAgain = playAgain();

    }while(playerPlayAgain);

    if(mPlayerClickedHome)
    {
        mPlayerClickedHome = false;
        createMenu();
    }
}

void chooseDifficulty()
{
    mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    bool quit = false;
    SDL_Event chooseEvent;
    while(!quit)
    {
        while(SDL_PollEvent(&chooseEvent))
        {
            if(chooseEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            mBackButton.getImage(getRect(mBackButton.mPosition.x, mBackButton.mPosition.y,
                mBackButton.mBUTTON_WIDTH, mBackButton.mBUTTON_HEIGHT));
            if(mBackButton.checkIfMouseIsInButton(&chooseEvent))
            {
                if(leftClicked(chooseEvent))
                {
                    createMenu();
                    quit = true;
                    break;
                }
            }
            for(int i = 0; i < 4; i++)
            {
                mDifficulty[i].getImage(getRect(mDifficulty[i].mPosition.x, mDifficulty[i].mPosition.y,
                    mDifficulty[i].mBUTTON_WIDTH, mDifficulty[i].mBUTTON_HEIGHT));
                if(mDifficulty[i].checkIfMouseIsInButton(&chooseEvent))
                {
                    if(leftClicked(chooseEvent))
                    {
                        quit = true;
                        getDifficulty(i);
                        loopGame();
                        break;
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

void showHighScore()
{
    mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    for(int i = 0; i < 4; i++)
    {
        mHighscore[i] = min(mHighscore[i], getWinScore(i));
    }
    SDL_Event showEvent;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&showEvent))
        {
            if(showEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            mBackButton.getImage(getRect(mBackButton.mPosition.x, mBackButton.mPosition.y,
                mBackButton.mBUTTON_WIDTH, mBackButton.mBUTTON_HEIGHT));
            for(int i = 0; i < 4; i++)
            {
                mDiffText[i].getText(getRect(250, 300 + 50 * i,
                    mDiffText[i].mTEXT_WIDTH, mDiffText[i].mTEXT_HEIGHT));
                int mTextWidth = (mHighscore[i] == 1000 ? 7 * TEXT_SIZE : 3 * TEXT_SIZE);
                mNumText[mHighscore[i]].getText(getRect(250 + mDiffText[i].mTEXT_WIDTH + 15,
                    300 + 50 * i, mTextWidth, mDiffText[i].mTEXT_HEIGHT));
            }
            if(mBackButton.checkIfMouseIsInButton(&showEvent))
            {
                if(leftClicked(showEvent))
                {
                    createMenu();
                    quit = true;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

void createMenu()
{
    mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    bool quit = false;
    SDL_Event menuEvent;
    while(!quit)
    {
        while(SDL_PollEvent(&menuEvent))
        {
            if(menuEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            mPlayButton.getImage(getRect(mPlayButton.mPosition.x, mPlayButton.mPosition.y,
                mPlayButton.mBUTTON_WIDTH, mPlayButton.mBUTTON_HEIGHT));
            mHighScoreButton.getImage(getRect(mHighScoreButton.mPosition.x, mHighScoreButton.mPosition.y,
                mHighScoreButton.mBUTTON_WIDTH, mHighScoreButton.mBUTTON_HEIGHT));
            if(mPlayButton.checkIfMouseIsInButton(&menuEvent))
            {
                if(leftClicked(menuEvent))
                {
                    chooseDifficulty();
                    quit = true;
                    break;
                }
            }
            if(mHighScoreButton.checkIfMouseIsInButton(&menuEvent))
            {
                if(leftClicked(menuEvent))
                {
                    showHighScore();
                    quit = true;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

