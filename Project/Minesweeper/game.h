#ifndef _game_h
#define _game_h

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

void getDifficulty(int index);
void resetMineTable();
void randomMine();
void initializeMineTable();
void openTile(int i, int j);
void bfs(int i, int j);
void playGame();
int getEndGame();
void playAgain();
bool isPlayerQuitGame();

#endif // GAME__H

