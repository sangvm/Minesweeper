#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "texture.h"

using namespace std;

Painter tTexture;

SDL_Texture* Painter::loadTexture(string path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Painter::getImage(string path, SDL_Rect dstrect)
{
    SDL_Texture* texture = tTexture.loadTexture(path.c_str());
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
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

