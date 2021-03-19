#ifndef _texture_h
#define _texture_h

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

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

    void loadImage(string path);
    void getImage(SDL_Rect dstrect);

private:
    SDL_Texture* loadedTexture;
};
bool leftClicked(SDL_Event e);
//initialize text
class Text
{
public:
    void setPosition(int x, int y);
    void loadText(string path, SDL_Color textColor);
    void getText(SDL_Rect dstrect);
    int textSize;
    SDL_Point mPosition;
    int mTEXT_WIDTH;
    int mTEXT_HEIGHT;
private:
    SDL_Texture* loadedText;
};
SDL_Rect getRect(int x, int y, int w, int h);

#endif // _texture_h

