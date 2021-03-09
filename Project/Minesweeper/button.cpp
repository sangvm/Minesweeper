#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "button.h"

using namespace std;

void Button::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void Button::setSize(int x, int y)
{
    mBUTTON_WIDTH = x;
    mBUTTON_HEIGHT = y;
}

bool Button::checkIfMouseIsInButton(SDL_Event *e)
{
    if(e-> type == SDL_MOUSEMOTION || e -> type == SDL_MOUSEBUTTONDOWN)
    {
        // get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //cout << x << " " << y << endl;
        //check if mouse in the button
        bool mouseInside = true;

        if(x <= mPosition.x)
        {
            mouseInside = false;
        }
        else if(x >= mPosition.x + mBUTTON_WIDTH)
        {
            mouseInside = false;
        }
        else if(y <= mPosition.y)
        {
            mouseInside = false;
        }
        else if(y >= mPosition.y + mBUTTON_HEIGHT)
        {
            mouseInside = false;
        }
        return mouseInside;
    }
}
