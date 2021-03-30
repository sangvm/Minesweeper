#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "initialize.h"
#include "texture.h"
#include "menu.h"
#include "game.h"

using namespace std;

const int mSCREEN_WIDTH = 1000;
const int mSCREEN_HEIGHT = 800;
const int INF = 1e9;
int TEXT_SIZE = 35;
bool mPlayerClickedHome = false;

//mixer variables
Mix_Music *mMainMenu;

//menu button variables
Button mTheme;
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
    //setup background
    mTheme.loadImage("../Image/minesweeper.png");
    mBackGround.loadImage("../Image/background.png");
    //setup main menu music
    mMainMenu = Mix_LoadMUS("../Music/mainmenu.mp3");
    //load menu button
    mPlayButton.loadImage("../Image/Menu/playButton.png");
    mPlayButton.setPosition(400, 325);
    mPlayButton.setSize(200, 75);

    mHighScoreButton.loadImage("../Image/Menu/highscoreButton.png");
    mHighScoreButton.setPosition(400, 425);
    mHighScoreButton.setSize(200, 75);

    mBackButton.loadImage("../Image/Menu/replayButton.png");
    mBackButton.setPosition(100, 200);
    mBackButton.setSize(50, 50);

    //difficulty button 200x75
    for(int i = 0; i < 4; i++)
    {
        mDifficulty[i].setPosition(400, 275 + i * 100);
        mDifficulty[i].setSize(200, 75);

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
        Mix_PauseMusic();
        mBackGround.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
        playOneGame();
        playerPlayAgain = playAgain();

    }while(playerPlayAgain);

    if(mPlayerClickedHome)
    {
        mPlayerClickedHome = false;
        playMainMenuMusic();
        createMenu();
    }
}

void chooseDifficulty()
{
    mTheme.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    mBackButton.getImage(getRect(mBackButton.mPosition.x, mBackButton.mPosition.y,
        mBackButton.mBUTTON_WIDTH, mBackButton.mBUTTON_HEIGHT));
    for(int i = 0; i < 4; i++)
    {
        mDifficulty[i].getImage(getRect(mDifficulty[i].mPosition.x, mDifficulty[i].mPosition.y,
            mDifficulty[i].mBUTTON_WIDTH, mDifficulty[i].mBUTTON_HEIGHT));
    }
    int quit = 0;
    SDL_Event chooseEvent;
    while(!quit)
    {
        while(SDL_PollEvent(&chooseEvent))
        {
            if(chooseEvent.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(mBackButton.checkIfMouseIsInButton(&chooseEvent) && leftClicked(chooseEvent))
            {
                quit = -1;
                break;
            }
            for(int i = 0; i < 4; i++)
            {
                if(mDifficulty[i].checkIfMouseIsInButton(&chooseEvent) && leftClicked(chooseEvent))
                {
                    quit = 1 + i;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == INF)quitSDL(window, renderer);
    if(quit == -1)createMenu();
    if(quit >= 1)getDifficulty(quit - 1);
}

void showHighScore()
{
    mTheme.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    mBackButton.getImage(getRect(mBackButton.mPosition.x, mBackButton.mPosition.y,
        mBackButton.mBUTTON_WIDTH, mBackButton.mBUTTON_HEIGHT));
    for(int i = 0; i < 4; i++)
    {
        mHighscore[i] = min(mHighscore[i], getWinScore(i));
        mDiffText[i].getText(getRect(250, 300 + 50 * i,
            mDiffText[i].mTEXT_WIDTH, mDiffText[i].mTEXT_HEIGHT));
        int mTextWidth = (mHighscore[i] == 1000 ? 7 * TEXT_SIZE : 3 * TEXT_SIZE);
        mNumText[mHighscore[i]].getText(getRect(250 + mDiffText[i].mTEXT_WIDTH + 15,
            300 + 50 * i, mTextWidth, mDiffText[i].mTEXT_HEIGHT));
    }
    SDL_Event showEvent;
    int quit = 0;
    while(!quit)
    {
        while(SDL_PollEvent(&showEvent))
        {
            if(showEvent.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(mBackButton.checkIfMouseIsInButton(&showEvent) && leftClicked(showEvent))
            {
                quit = -1;
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == INF)quitSDL(window, renderer);
    if(quit == -1)createMenu();
}

void createMenu()
{
    mTheme.getImage(getRect(0, 0, mSCREEN_WIDTH, mSCREEN_HEIGHT));
    mPlayButton.getImage(getRect(mPlayButton.mPosition.x, mPlayButton.mPosition.y,
        mPlayButton.mBUTTON_WIDTH, mPlayButton.mBUTTON_HEIGHT));
    mHighScoreButton.getImage(getRect(mHighScoreButton.mPosition.x, mHighScoreButton.mPosition.y,
        mHighScoreButton.mBUTTON_WIDTH, mHighScoreButton.mBUTTON_HEIGHT));
    int quit = 0;
    SDL_Event menuEvent;
    while(!quit)
    {
        while(SDL_PollEvent(&menuEvent))
        {
            if(menuEvent.type == SDL_QUIT)
            {
                quit = INF;
                break;
            }
            if(mPlayButton.checkIfMouseIsInButton(&menuEvent) && leftClicked(menuEvent))
            {
                quit = 1;
                break;
            }
            if(mHighScoreButton.checkIfMouseIsInButton(&menuEvent) && leftClicked(menuEvent))
            {
                quit = 2;
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    if(quit == INF)quitSDL(window, renderer);
    if(quit == 1)chooseDifficulty();
    if(quit == 2)showHighScore();
}

void playMainMenuMusic()
{
    if(Mix_PausedMusic() == 0)
    {
        Mix_PauseMusic();
    }
    Mix_PlayMusic(mMainMenu, -1);
}

