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
//menu button variables
Button mBackGround;
Button mPlayButton, mHighScoreButton;
Button mDifficulty[4]; //0:easy, 1: medium, 2:hard, 3:custom

//setup menu
void setupMenuButton()
{
    mBackGround.loadImage("../Image/background.png");
    //menu button size: 200x75
    mPlayButton.setSize(200, 75);
    mHighScoreButton.setSize(200, 75);

    int idX, idY;
    // idX and idY in menu
    idX = 400; idY = 300;
    mPlayButton.setPosition(idX, idY);
    mHighScoreButton.setPosition(idX, idY + 100);

    //load button
    mPlayButton.loadImage("../Image/Menu/playButton.png");
    mHighScoreButton.loadImage("../Image/Menu/highscoreButton.png");

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
}

bool playAgain()
{
    bool mAgain = false;
    bool mQuit = false;
    SDL_Event mAgainEvent;
    while(!mQuit)
    {
        while(SDL_PollEvent(&mAgainEvent))
        {
            if(mAgainEvent.type == SDL_QUIT)
            {
                mQuit = true;
                break;
            }
            if(mAgainEvent.key.keysym.sym == SDLK_SPACE)
            {
                mQuit = true;
                mAgain = true;
                break;
            }
        }
    }
    return mAgain;
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
}


void chooseDifficulty()
{
    mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    bool chooseQuit = false;
    SDL_Event chooseEvent;
    while(!chooseQuit)
    {
        while(SDL_PollEvent(&chooseEvent))
        {
            if(chooseEvent.type == SDL_QUIT)
            {
                chooseQuit = true;
                break;
            }
            for(int i = 0; i < 4; i++)
            {

                mDifficulty[i].getImage(getRect(mDifficulty[i].mPosition.x, mDifficulty[i].mPosition.y,
                    mDifficulty[i].mBUTTON_WIDTH, mDifficulty[i].mBUTTON_HEIGHT));
                if(mDifficulty[i].checkIfMouseIsInButton(&chooseEvent))
                {
                    if(chooseEvent.type == SDL_MOUSEBUTTONDOWN && chooseEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        chooseQuit = true;
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

}

void createMenu()
{

    mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    bool menuQuit = false;
    SDL_Event menuEvent;
    while(!menuQuit)
    {
        while(SDL_PollEvent(&menuEvent))
        {
            if(menuEvent.type == SDL_QUIT)
            {
                menuQuit = true;
                break;
            }
            mPlayButton.getImage(getRect(mPlayButton.mPosition.x, mPlayButton.mPosition.y,
                mPlayButton.mBUTTON_WIDTH, mPlayButton.mBUTTON_HEIGHT));
            mHighScoreButton.getImage(getRect(mHighScoreButton.mPosition.x, mHighScoreButton.mPosition.y,
                mHighScoreButton.mBUTTON_WIDTH, mHighScoreButton.mBUTTON_HEIGHT));

            if(mPlayButton.checkIfMouseIsInButton(&menuEvent))
            {
                if(menuEvent.type == SDL_MOUSEBUTTONDOWN && menuEvent.button.button == SDL_BUTTON_LEFT)
                {
                    chooseDifficulty();
                    menuQuit = true;
                    break;
                }
            }
            if(mHighScoreButton.checkIfMouseIsInButton(&menuEvent))
            {
                if(menuEvent.type == SDL_MOUSEBUTTONDOWN && menuEvent.button.button == SDL_BUTTON_LEFT)
                {
                    showHighScore();
                    menuQuit = true;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

