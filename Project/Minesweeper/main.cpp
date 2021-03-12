#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "initialize.h"
#include "texture.h"
#include "game.h"
#include "menu.h"

using namespace std;

void setupGame()
{
    TTF_Init();
	fontText = TTF_OpenFont("DTM-Mono.ttf", 40);
    SDL_Color textColor = {140, 140, 140};
    setupMenuButton();
    setupButtonAndText();
    setupReplayAndHome();
}

int main(int argc, char* argv[])
{
    initSDL(window, renderer);
    setupGame();
    createMenu();
    quitSDL(window, renderer);
    return 0;
}

