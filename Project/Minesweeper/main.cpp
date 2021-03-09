#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"
#include "texture.h"
#include "button.h"
#include "game.h"

using namespace std;

const int mSCREEN_WIDTH = 1000;
const int mSCREEN_HEIGHT = 800;

//painter variables
Painter mPainter;

//button variables
Button mPlayButton;
Button mHighScoreButton;

Button mDifficultyButton[4]; //0:easy, 1: medium, 2:hard, 3:custom

void setMenuButtonSize()
{
    //menu button size: 200x75
    mPlayButton.setSize(200, 75);
    mHighScoreButton.setSize(200, 75);

    //difficulty button 200x75
    for(int i = 0; i < 4; i++)
    {
        mDifficultyButton[i].setSize(200, 75);
    }
}

void setMenuButtonPosition()
{
    int idX, idY;
    // idX and idY in menu
    idX = 400; idY = 300;
    mPlayButton.setPosition(idX, idY);
    mHighScoreButton.setPosition(idX, idY + 100);
    // idX and idY in difficulty
    idX = 400; idY = 200;
    for(int i = 0; i < 4; i++)
    {
        mDifficultyButton[i].setPosition(idX, idY + i * 100);
    }
}

void gameLoop()
{
    bool loopQuit = false;
    SDL_Event loopEvent;
    while(!loopQuit)
    {
        while(SDL_PollEvent(&loopEvent))
        {
            if(loopEvent.type == SDL_QUIT)
            {
                loopQuit = true;
                break;
            }
            mPainter.getImage("../Texture/background.png", getRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
            playAgain();
//            SDL_Delay(2000);
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

void chooseDifficulty()
{
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
                string path = "../Texture/Menu/";
                if(i == 0)path = path + "easyButton.png";
                if(i == 1)path = path + "mediumButton.png";
                if(i == 2)path = path + "hardButton.png";
                if(i == 3)path = path + "customButton.png";
                mPainter.getImage(path, getRect(mDifficultyButton[i].mPosition.x, mDifficultyButton[i].mPosition.y,
                    mDifficultyButton[i].mBUTTON_WIDTH, mDifficultyButton[i].mBUTTON_HEIGHT));
                if(mDifficultyButton[i].checkIfMouseIsInButton(&chooseEvent))
                {
                    if(chooseEvent.type == SDL_MOUSEBUTTONDOWN && chooseEvent.button.button == SDL_BUTTON_LEFT)
                    {
                        getDifficulty(i);
                        gameLoop();
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
            mPainter.getImage("../Texture/background.png", getRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
            mPainter.getImage("../Texture/Menu/playButton.png",
                getRect(mPlayButton.mPosition.x, mPlayButton.mPosition.y, mPlayButton.mBUTTON_WIDTH, mPlayButton.mBUTTON_HEIGHT));
            mPainter.getImage("../Texture/Menu/highscoreButton.png",
                getRect(mHighScoreButton.mPosition.x, mHighScoreButton.mPosition.y, mHighScoreButton.mBUTTON_WIDTH, mHighScoreButton.mBUTTON_HEIGHT));

//            int x, y;
//            SDL_GetMouseState(&x, &y);
//            cout << x << " " << y << endl;

            if(mPlayButton.checkIfMouseIsInButton(&menuEvent))
            {
//                cout << "MOUSE INSIDE PLAY BUTTON" << endl;
                if(menuEvent.type == SDL_MOUSEBUTTONDOWN && menuEvent.button.button == SDL_BUTTON_LEFT)
                {
                    chooseDifficulty();
                }
            }
            if(mHighScoreButton.checkIfMouseIsInButton(&menuEvent))
            {
                if(menuEvent.type == SDL_MOUSEBUTTONDOWN && menuEvent.button.button == SDL_BUTTON_LEFT)
                {
                    showHighScore();
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
}

int main(int argc, char* argv[])
{

    initSDL(window, renderer);

    setMenuButtonSize();
    setMenuButtonPosition();
    createMenu();

    quitSDL(window, renderer);
    return 0;
}

