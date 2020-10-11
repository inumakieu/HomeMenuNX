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
  "Super Mario Odyssey",
  "Genshin Impact",
  "The legend of Zelda Breath of the Wild",
};

SDL_Rect wallpaper_pos = { 0, 0, 0, 0 };

SDL_Rect gameicon1 = { 470, 340, 0, 0 }; // icon size 320x320
SDL_Rect gameicon2 = { 470 + 40 + 320 , 340 + 60, 0, 0 }; // icon size 260x260
SDL_Rect gameicon3 = { 470 + 40 + 320 + 260 + 20, 340 + 60, 0, 0 }; // icon size 260x260
SDL_Rect icons_bg_pos = { 20, 200, 0, 0 };
SDL_Rect news_pos = { 60, 240, 0, 0 };
SDL_Rect e_shop_pos = { 62, 316, 0, 0 };
SDL_Rect album_pos = { 60, 400, 0, 0 };
SDL_Rect controller_pos = { 60, 470, 0, 0 };
SDL_Rect settings_pos = { 60, 550, 0, 0 };
SDL_Rect power_pos = { 65, 630, 0, 0 };
SDL_Rect avatar_pos = { 60, 40, 0, 0 };
SDL_Rect hb_pos = { 160, 40, 0, 0 };
SDL_Rect name_bg_pos = { 1020, 260, 0, 0 };
SDL_Rect news_text_pos = { 120, 244, 0, 0 };
SDL_Rect e_shop_text_pos = { 120, 320, 0, 0 };
SDL_Rect album_text_pos = { 120, 404, 0, 0 };
SDL_Rect controller_text_pos = { 120, 474, 0, 0 };
SDL_Rect settings_text_pos = { 120, 554, 0, 0 };
SDL_Rect power_text_pos = { 120, 634, 0, 0 };
SDL_Rect seperator_pos = { 45, 290, 0, 0 };
SDL_Rect seperator2_pos = { 45, 370, 0, 0 };
SDL_Rect seperator3_pos = { 45, 450, 0, 0 };
SDL_Rect seperator4_pos = { 45, 530, 0, 0 };
SDL_Rect seperator5_pos = { 45, 610, 0, 0 };

SDL_Texture *wallpaper_tex = NULL;
SDL_Texture *game1_tex = NULL;
SDL_Texture *game2_tex = NULL;
SDL_Texture *game3_tex = NULL;
SDL_Texture *icons_bg_tex = NULL;
SDL_Texture *news_tex = NULL;
SDL_Texture *e_shop_tex = NULL;
SDL_Texture *album_tex = NULL;
SDL_Texture *controller_tex = NULL;
SDL_Texture *settings_tex = NULL;
SDL_Texture *power_tex = NULL;
SDL_Texture *avatar_tex = NULL;
SDL_Texture *hb_tex = NULL;
SDL_Texture *name_bg_tex = NULL;
SDL_Texture *seperator_tex = NULL;
SDL_Texture *seperator2_tex = NULL;
SDL_Texture *seperator3_tex = NULL;
SDL_Texture *seperator4_tex = NULL;
SDL_Texture *seperator5_tex = NULL;

SDL_Color White = {255, 255, 255};


void init_ui(SDL_Renderer* renderer)
{
  // wallpaper
  SDL_Surface *wallpaper = IMG_Load("wallpaper.png");
  if (wallpaper) {
    wallpaper_pos.w = 1280;
    wallpaper_pos.h = 720;
    wallpaper_tex = SDL_CreateTextureFromSurface(renderer, wallpaper);
    SDL_FreeSurface(wallpaper);
  }

  // icons menu (left)
  // bg
  SDL_Surface *icons_bg = IMG_Load("icons_bg.png");
  if (icons_bg) {
    icons_bg_pos.w = 240;
    icons_bg_pos.h = 500;
    icons_bg_tex = SDL_CreateTextureFromSurface(renderer, icons_bg);
    SDL_FreeSurface(icons_bg);
  }
  // icons
  SDL_Surface *news = IMG_Load("news.png");
  if (news) {
    news_pos.w = 35;
    news_pos.h = 33;
    news_tex = SDL_CreateTextureFromSurface(renderer, news);
    SDL_FreeSurface(news);
  }
  SDL_Surface *e_shop = IMG_Load("e-shop.png");
  if (e_shop) {
    e_shop_pos.w = 30;
    e_shop_pos.h = 30;
    e_shop_tex = SDL_CreateTextureFromSurface(renderer, e_shop);
    SDL_FreeSurface(e_shop);
  }
  SDL_Surface *album = IMG_Load("album.png");
  if (album) {
    album_pos.w = 35;
    album_pos.h = 23;
    album_tex = SDL_CreateTextureFromSurface(renderer, album);
    SDL_FreeSurface(album);
  }
  SDL_Surface *controller = IMG_Load("controller.png");
  if (controller) {
    controller_pos.w = 34;
    controller_pos.h = 33;
    controller_tex = SDL_CreateTextureFromSurface(renderer, controller);
    SDL_FreeSurface(controller);
  }
  SDL_Surface *settings = IMG_Load("settings.png");
  if (settings) {
    settings_pos.w = 35;
    settings_pos.h = 36;
    settings_tex = SDL_CreateTextureFromSurface(renderer, settings);
    SDL_FreeSurface(settings);
  }
  SDL_Surface *power = IMG_Load("power.png");
  if (power) {
    power_pos.w = 28;
    power_pos.h = 33;
    power_tex = SDL_CreateTextureFromSurface(renderer, power);
    SDL_FreeSurface(power);
  }
  SDL_Surface *seperator = IMG_Load("seperator.png");
  if (seperator) {
    seperator_pos.w = 190;
    seperator_pos.h = 5;
    seperator_tex = SDL_CreateTextureFromSurface(renderer, seperator);
    SDL_FreeSurface(seperator);
  }
  SDL_Surface *seperator2 = IMG_Load("seperator.png");
  if (seperator2) {
    seperator2_pos.w = 190;
    seperator2_pos.h = 5;
    seperator2_tex = SDL_CreateTextureFromSurface(renderer, seperator2);
    SDL_FreeSurface(seperator2);
  }
  SDL_Surface *seperator3 = IMG_Load("seperator.png");
  if (seperator3) {
    seperator3_pos.w = 190;
    seperator3_pos.h = 5;
    seperator3_tex = SDL_CreateTextureFromSurface(renderer, seperator3);
    SDL_FreeSurface(seperator3);
  }
  SDL_Surface *seperator4 = IMG_Load("seperator.png");
  if (seperator4) {
    seperator4_pos.w = 190;
    seperator4_pos.h = 5;
    seperator4_tex = SDL_CreateTextureFromSurface(renderer, seperator4);
    SDL_FreeSurface(seperator4);
  }
  SDL_Surface *seperator5 = IMG_Load("seperator.png");
  if (seperator5) {
    seperator5_pos.w = 190;
    seperator5_pos.h = 5;
    seperator5_tex = SDL_CreateTextureFromSurface(renderer, seperator5);
    SDL_FreeSurface(seperator5);
  }


  // game icons
  chdir("romfs:/assets/games/");
  // suspended
  SDL_Surface *game1 = IMG_Load("genshin-impact.jpg");
  if (game1) {
    gameicon1.w = 320;
    gameicon1.h = 320;
    game1_tex = SDL_CreateTextureFromSurface(renderer, game1);
    SDL_FreeSurface(game1);
  }

  // other titles
  SDL_Surface *game2 = IMG_Load("super-mario-oddyssey.jpg");
  if (game2) {
    gameicon2.w = 260;
    gameicon2.h = 260;
    game2_tex = SDL_CreateTextureFromSurface(renderer, game2);
    SDL_FreeSurface(game2);
  }
  SDL_Surface *game3 = IMG_Load("breath-of-the-wild.png");
  if (game3) {
    gameicon3.w = 260;
    gameicon3.h = 260;
    game3_tex = SDL_CreateTextureFromSurface(renderer, game3);
    SDL_FreeSurface(game3);
  }

}

void draw_ui()
{

}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  romfsInit();
  chdir("romfs:/assets/UI/");

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

  chdir("romfs:/assets/UI/");
  TTF_Font* font = TTF_OpenFont("font.ttf", 18);

  SDL_Surface *news_surface = TTF_RenderText_Solid(font, "News", White);
  SDL_Texture *news_text = SDL_CreateTextureFromSurface(renderer, news_surface);
  SDL_Surface *e_shop_surface = TTF_RenderText_Solid(font, "E-Shop", White);
  SDL_Texture *e_shop_text = SDL_CreateTextureFromSurface(renderer, e_shop_surface);
  SDL_Surface *album_surface = TTF_RenderText_Solid(font, "Album", White);
  SDL_Texture *album_text = SDL_CreateTextureFromSurface(renderer, album_surface);
  SDL_Surface *controller_surface = TTF_RenderText_Solid(font, "Controller", White);
  SDL_Texture *controller_text = SDL_CreateTextureFromSurface(renderer, controller_surface);
  SDL_Surface *settings_surface = TTF_RenderText_Solid(font, "Settings", White);
  SDL_Texture *settings_text = SDL_CreateTextureFromSurface(renderer, settings_surface);
  SDL_Surface *power_surface = TTF_RenderText_Solid(font, "Power", White);
  SDL_Texture *power_text = SDL_CreateTextureFromSurface(renderer, power_surface);

  int w, h;
  SDL_QueryTexture(news_text, NULL, NULL, &w, &h);
  news_text_pos.w = w;
  news_text_pos.h = h;
  SDL_QueryTexture(e_shop_text, NULL, NULL, &w, &h);
  e_shop_text_pos.w = w;
  e_shop_text_pos.h = h;
  SDL_QueryTexture(album_text, NULL, NULL, &w, &h);
  album_text_pos.w = w;
  album_text_pos.h = h;
  SDL_QueryTexture(controller_text, NULL, NULL, &w, &h);
  controller_text_pos.w = w;
  controller_text_pos.h = h;
  SDL_QueryTexture(settings_text, NULL, NULL, &w, &h);
  settings_text_pos.w = w;
  settings_text_pos.h = h;
  SDL_QueryTexture(power_text, NULL, NULL, &w, &h);
  power_text_pos.w = w;
  power_text_pos.h = h;



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
    if (game1_tex){
      SDL_RenderCopy(renderer, game1_tex, NULL, &gameicon1);
    }
    if (game2_tex){
      SDL_RenderCopy(renderer, game2_tex, NULL, &gameicon2);
    }
    if (game3_tex){
      SDL_RenderCopy(renderer, game3_tex, NULL, &gameicon3);
    }
    if (icons_bg_tex){
      SDL_RenderCopy(renderer, icons_bg_tex, NULL, &icons_bg_pos);
    }
    if (news_tex){
      SDL_RenderCopy(renderer, news_tex, NULL, &news_pos);
    }
    if (e_shop_tex){
      SDL_RenderCopy(renderer, e_shop_tex, NULL, &e_shop_pos);
    }
    if (album_tex){
      SDL_RenderCopy(renderer, album_tex, NULL, &album_pos);
    }
    if (controller_tex){
      SDL_RenderCopy(renderer, controller_tex, NULL, &controller_pos);
    }
    if (settings_tex){
      SDL_RenderCopy(renderer, settings_tex, NULL, &settings_pos);
    }
    if (power_tex){
      SDL_RenderCopy(renderer, power_tex, NULL, &power_pos);
    }

    SDL_RenderCopy(renderer, seperator_tex, NULL, &seperator_pos);
    SDL_RenderCopy(renderer, seperator2_tex, NULL, &seperator2_pos);
    SDL_RenderCopy(renderer, seperator3_tex, NULL, &seperator3_pos);
    SDL_RenderCopy(renderer, seperator4_tex, NULL, &seperator4_pos);
    SDL_RenderCopy(renderer, seperator5_tex, NULL, &seperator5_pos);


    SDL_RenderCopy(renderer, news_text, NULL, &news_text_pos);
    SDL_RenderCopy(renderer, e_shop_text, NULL, &e_shop_text_pos);
    SDL_RenderCopy(renderer, album_text, NULL, &album_text_pos);
    SDL_RenderCopy(renderer, controller_text, NULL, &controller_text_pos);
    SDL_RenderCopy(renderer, settings_text, NULL, &settings_text_pos);
    SDL_RenderCopy(renderer, power_text, NULL, &power_text_pos);

    SDL_RenderPresent(renderer);

    SDL_Delay(1);
  }
  SDL_FreeSurface(news_surface);
  SDL_FreeSurface(e_shop_surface);
  SDL_FreeSurface(album_surface);
  SDL_FreeSurface(controller_surface);
  SDL_FreeSurface(settings_surface);
  SDL_FreeSurface(power_surface);
  SDL_DestroyTexture(news_text);
  SDL_DestroyTexture(e_shop_text);
  SDL_DestroyTexture(album_text);
  SDL_DestroyTexture(controller_text);
  SDL_DestroyTexture(settings_text);
  SDL_DestroyTexture(power_text);
  SDL_FreeSurface(windowSurface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  //gfxExit();
  return 0;
}
