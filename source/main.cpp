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
SDL_Rect hb_menu_pos = { 160, 40, 0, 0 };
SDL_Rect wallpaper_pos = { 0, 0, 0, 0 };
SDL_Rect icons_bg_pos = { 20, 200, 0, 0 };
SDL_Rect settings_pos = { 50, 720 - 60 - 50, 0, 0 };



SDL_Texture *wallpaper_tex = NULL;
SDL_Texture *tex = NULL;
SDL_Texture *tex2 = NULL;
SDL_Texture *hb_menu_tex = NULL;
SDL_Texture *icons_bg_tex = NULL;
SDL_Texture *settings_tex = NULL;


SDL_Color White = {255, 255, 255};
SDL_Rect Message_rect = { 0, 0, 0, 0 };



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

  SDL_Surface *hb_menu = IMG_Load("hb_menu.png");
  if (hb_menu) {
    hb_menu_pos.w = 65;
    hb_menu_pos.h = 65;
    hb_menu_tex = SDL_CreateTextureFromSurface(renderer, hb_menu);
    SDL_FreeSurface(hb_menu);
  }

  SDL_Surface *icons_bg = IMG_Load("icons_bg.png");
  if (icons_bg) {
    icons_bg_pos.w = icons_bg->w;
    icons_bg_pos.h = icons_bg->h;
    icons_bg_tex = SDL_CreateTextureFromSurface(renderer, icons_bg);
    SDL_FreeSurface(icons_bg);
  }

  SDL_Surface *settings = IMG_Load("settings.png");
  if (settings) {
    settings_pos.w = 50;
    settings_pos.h = 50;
    settings_tex = SDL_CreateTextureFromSurface(renderer, settings);
    SDL_FreeSurface(settings);
  }

}

void draw_ui()
{

}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  romfsInit();
  chdir("romfs:/");

  TTF_Init();

  /*gfxInitDefault();
  consoleInit(nullptr);*/

  //Switch screen size: 720p. Must set to full screen.
  SDL_Window* window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
  if (!window) SDL_Quit();
  SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
  if (!renderer) SDL_Quit();
  SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

  init_ui(renderer);
  TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "Settings", White);
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);


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
    if (hb_menu_tex){
      SDL_RenderCopy(renderer, hb_menu_tex, NULL, &hb_menu_pos);
    }
    if (icons_bg_tex){
      SDL_RenderCopy(renderer, icons_bg_tex, NULL, &icons_bg_pos);
    }
    if (settings_tex){
      SDL_RenderCopy(renderer, settings_tex, NULL, &settings_pos);
    }
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_RenderPresent(renderer);

    SDL_Delay(1);
  }
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  SDL_FreeSurface(windowSurface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  //gfxExit();
  return 0;
}
