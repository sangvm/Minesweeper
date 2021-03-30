#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"
#include "texture.h"

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

void Button::loadImage(string path)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void Button::getImage(SDL_Rect dstrect)
{
    SDL_RenderCopy(renderer, loadedTexture, NULL, &dstrect);
}

bool leftClicked(SDL_Event e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        return true;
    }
    return false;
}

void Text::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void Text::setSize(int x, int y)
{
    mTEXT_WIDTH = x;
    mTEXT_HEIGHT = y;
}

void Text::loadText(string path, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontText, path.c_str(), textColor);
    loadedText = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void Text::getText(SDL_Rect dstrect)
{
    SDL_RenderCopy(renderer, loadedText, NULL, &dstrect);
}

SDL_Rect getRect(int x, int y, int w, int h)
{
    SDL_Rect cRect;
    cRect.x = x;
    cRect.y = y;
    cRect.w = w;
    cRect.h = h;
    return cRect;
}

void Character::setupChar()
{
    mineTime = 10;
    scoreTime = 0;
    extraSkill = 0;
}
