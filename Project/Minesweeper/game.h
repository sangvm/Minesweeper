#ifndef _game_h
#define _game_h

#include <bits/stdc++.h>

using namespace std;

void setupButtonAndText();
void setupReplayAndHome();
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

void playingGame();

void playOneGame();

#endif // GAME__H

