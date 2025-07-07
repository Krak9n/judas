#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> //This is an sample library not included with stock SDL2. https://www.libsdl.org/projects/SDL_ttf/release-1.2.html

const char* WINDOW_TITLE = "Hello World SDL2 + TTF";
const char* FONT_NAME = "include/font/basic.ttf";
const int FONT_SIZE = 128;

SDL_Window* window; // window created by SDL.
SDL_Renderer* Renderer; // The renderer that shows our textures.
SDL_Event windowEvent; // Event capturer from SDL window.
SDL_Color TextColor = { 0, 0, 0, 0}; // white.
TTF_Font* Font; // The font to be loaded from the ttf file.
SDL_Surface* TextSurface; // The surface necessary to create the font texture.
SDL_Texture* TextTexture; // The font texture prepared for render.
SDL_Rect TextRect; // Text rectangle area with the position for the texture text.

void CreateRenderer() 
{
    Renderer = SDL_CreateRenderer(window, -1, 0);
    if (!Renderer)
    {
      printf("There was a problem creating the renderer.\n");
    }
}

void CreateText(const char* Message) 
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
    if (!font)
    {
      printf("Couldn't find/init open ttf font.\n");
    }

    TextSurface = TTF_RenderText_Solid(font, Message, TextColor);
    TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
    TextRect.x = WINDOW_WIDTH - TextSurface->w * 0.5; // Center horizontaly
    TextRect.y = WINDOW_HEIGHT - TextSurface->h * 0.5; // Center verticaly
    TextRect.w = TextSurface->w;
    TextRect.h = TextSurface->h;
    // After you create the texture you can release the surface memory allocation because we actually render the texture not the surface.
    SDL_FreeSurface(TextSurface);
    TTF_Quit();
}

bool IsPollingEvent() 
{
    while(SDL_PollEvent(&windowEvent)) 
    {
        switch (windowEvent.type) 
        {
            case SDL_QUIT: return false;
        }
    }
    return true;
}

void RenderText() 
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer); // Paint screen black.
    SDL_RenderCopy(Renderer, TextTexture, NULL, &TextRect); // Add text to render queue.
    SDL_RenderPresent(Renderer); // Render everything that's on the queue.
    SDL_Delay(10); // Delay to prevent CPU overhead as suggested by the user `not2qubit`
}

void ClearMemory() {
    SDL_DestroyTexture(TextTexture);
    SDL_DestroyRenderer(Renderer);
    SDL_Quit();
    printf("Clear proccess done.\n");
}

int main() 
{
    CreateRenderer();
    CreateText("Hello SDL_Ttf");
    while (IsPollingEvent()) 
    {
        RenderText();
    }
    ClearMemory();
    return 0;
}
