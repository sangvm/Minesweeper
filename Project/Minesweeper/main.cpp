#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
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
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_VolumeMusic(40);
    setupMenuButton();
    setupButtonAndTextGame();
    playMainMenuMusic();
}

int main(int argc, char* argv[])
{
    initSDL(window, renderer);
    setupGame();
    createMenu();
    quitSDL(window, renderer);
    return 0;
}

