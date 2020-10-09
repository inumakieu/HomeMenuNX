#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>

#include <unistd.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_power.h>
#include <SDL_ttf.h>
#include <switch.h>

std::list<std::string> games {
  "super-mario-odyssey",
  "links-awakening",
};

SDL_Rect pos = { (1280 / 2) - (316 / 2), 720 - 316 - 60, 0, 0 };
SDL_Rect pos2 = { (1280 / 2) + (316 / 2) + 20, 720 - 250 - 60, 0, 0 };

SDL_Rect wallpaper_pos = { 0, 0, 0, 0 };

SDL_Texture *wallpaper_tex = NULL;
SDL_Texture *tex = NULL;
SDL_Texture *tex2 = NULL;

int pct;

void init_ui(SDL_Renderer* renderer)
{
  // wallpaper
  SDL_Surface *wallpaper = IMG_Load("wallpaper.png");
  if (wallpaper) {
    wallpaper_pos.w = wallpaper->w;
    wallpaper_pos.h = wallpaper->h;
    wallpaper_tex = SDL_CreateTextureFromSurface(renderer, wallpaper);
    SDL_FreeSurface(wallpaper);
  }

  // game icons
  SDL_Surface *image = IMG_Load("links-awakening.jpg");
  if (image) {
    pos.w = image->w;
    pos.h = image->h;
    tex = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
  }

  for (int i = 0; i < games.size(); i++) {
    SDL_Surface *image2 = IMG_Load("super-mario-oddyssey.jpg");
    if (image2) {
      pos2.w = 250;
      pos2.h = 250;
      tex2 = SDL_CreateTextureFromSurface(renderer, image2);
      SDL_FreeSurface(image2);
    }
  }

}

void draw_ui()
{

}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  romfsInit();
  chdir("romfs:/");

  /*gfxInitDefault();
  consoleInit(nullptr);*/

  //Switch screen size: 720p. Must set to full screen.
  SDL_Window* window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
  if (!window) SDL_Quit();
  SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
  if (!renderer) SDL_Quit();
  SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

  consoleInit(NULL);

  init_ui(renderer);

  while (appletMainLoop()) {
    hidScanInput();

    u32 keyDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (keyDown & KEY_PLUS)
    break;

    SDL_RenderClear(renderer);



    //draw_ui(renderer, 1280, 720);
    if (wallpaper_tex){
      SDL_RenderCopy(renderer, wallpaper_tex, NULL, &wallpaper_pos);
    }
    if (tex){
      SDL_RenderCopy(renderer, tex, NULL, &pos);
    }
    if (tex2){
      for (int i = 0; i < games.size(); i++) {
        pos2.x = (1280 / 2) + (316 / 2) + 20 + (250 + 20) * i;
        SDL_RenderCopy(renderer, tex2, NULL, &pos2);
      }
    }
    
    SDL_RenderPresent(renderer);

    if (SDL_GetPowerInfo(NULL, &pct) == SDL_POWERSTATE_ON_BATTERY) {
      printf("test");
    }


    /*gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();*/

    SDL_Delay(1);
  }
  SDL_FreeSurface(windowSurface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  //gfxExit();
  return 0;
}
