#ifndef _initialize_h
#define _initialize_h

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

// screen size
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

// initialize SDL
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* fontText;

void logSDLError(const string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);


void setupMenuButton();

#endif // _initialize_h

