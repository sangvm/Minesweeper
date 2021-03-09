#ifndef _texture_h
#define _texture_h

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "initialize.h"

using namespace std;

// initialize painter
class Painter
{
public:
    SDL_Texture* loadTexture(string path);
    void getImage(string path, SDL_Rect dstrect);
};

SDL_Rect getRect(int x, int y, int w, int h);

#endif // _texture_h
