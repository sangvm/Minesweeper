#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"

using namespace std;

// menu button size
const int MENU_BUTTON_WIDTH = 200;
const int MENU_BUTTON_HEIGHT = 100;

//initialize button
class Button
{
public:
    void setPosition(int x, int y);
    void setSize(int x, int y);
    bool checkIfMouseIsInButton(SDL_Event *e);
    SDL_Point mPosition;
    int mBUTTON_WIDTH;
    int mBUTTON_HEIGHT;
};


