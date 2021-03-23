#ifndef _game_h
#define _game_h

#include <bits/stdc++.h>
#include <SDL.h>

using namespace std;

void setupButtonAndTextGame();
//clock and mine count
string convertNum(int curNum);
void uploadTime(int curTime);
void uploadMineLeft(int curMine);

//manage table
void getDifficulty(int index);
void resetMineTable();
void randomMine();
void initializeMineTable();
//game play
void openTile(int i, int j);
void bfs(int i, int j);
bool clickReplay(SDL_Event gEvent);
bool clickBackHome(SDL_Event gEvent);
bool addMine();
int getWinScore(int index);

void playingGame();

void playOneGame();

#endif // GAME__H

