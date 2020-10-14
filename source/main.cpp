#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <map>

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

SDL_Rect icons_bg_pos = { 20, 200, 0, 0 };

SDL_Rect name_bg_pos = { 1020, 260, 0, 0 };
SDL_Rect news_text_pos = { 120, 244, 0, 0 };
SDL_Rect e_shop_text_pos = { 120, 320, 0, 0 };
SDL_Rect album_text_pos = { 120, 404, 0, 0 };
SDL_Rect controller_text_pos = { 120, 475, 0, 0 };
SDL_Rect settings_text_pos = { 120, 556, 0, 0 };
SDL_Rect power_text_pos = { 120, 636, 0, 0 };
SDL_Rect top_menu_bg_pos = { 420, 50, 0, 0 };
SDL_Rect time_text_pos = { 606, 60, 0, 0 };
SDL_Rect suspended_bg_pos = { 550, 570, 0, 0 };
SDL_Rect suspended_text_pos = { 584, 580, 0, 0 };
SDL_Rect game_title_text_pos = { 0, 260, 0, 0 };
SDL_Rect game_title_bg_pos = { 0, 254, 0, 0 };
SDL_Rect game_info_text_pos = { 0, 306, 0, 0 };
SDL_Rect game_info_bg_pos = { 0, 284, 0, 0 };


SDL_Color White = {255, 255, 255};
SDL_Color OffWhite = {230, 230, 230};
SDL_Color DarkGrey = {11, 11, 11};

SDL_Renderer* renderer;

std::map<std::string, SDL_Texture*> textureMap;
std::map<std::string, SDL_Rect> rectMap;



SDL_Texture *create_texture(std::string name, std::string filename, int w, int h, bool forceSize)
{
  SDL_Texture *temp_tex = NULL;
  SDL_Surface *surface = IMG_Load("wallpaper.png");
  if (surface) {
    if (forceSize) {
      for (auto i = rectMap.begin(); i != rectMap.end(); i++) {
        if(i->first == name) {
          i->second.w = w;
          i->second.h = h;
        }
      }
    }
    temp_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
  }
  return temp_tex;
}

SDL_Texture *create_masked_texture(std::string name, std::string filename, int w)
{
  // other titles
  SDL_Texture *game = create_texture(name, filename, w, w, true);
  SDL_Texture *mask_tex = create_texture("mask", "mask.png", w, w, true);

  SDL_Texture *maskedTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, w);

  SDL_SetTextureBlendMode(maskedTex, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(game, SDL_BLENDMODE_MOD);
  SDL_SetRenderTarget(renderer, maskedTex);
  SDL_Rect dst = {0, 0, w, w};

  SDL_RenderCopy(renderer, mask_tex, NULL, &dst);
  SDL_RenderCopy(renderer, game, NULL, &dst);
  SDL_SetRenderTarget(renderer, NULL);
  return maskedTex;
}

void init_ui()
{
  rectMap.insert(std::pair<std::string, SDL_Rect>("wallpaper", { 0, 0, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>("wallpaper", create_texture("wallpaper", "wallpaper.png", 1280, 720, true)));

  rectMap.insert(std::pair<std::string, SDL_Rect>("icons_bg", { 20, 200, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>("icons_bg", create_texture("icons_bg", "icons_bg.png", 240, 500, true)));

  // icons
  std::string name;
  SDL_Rect temp_rect = {};
  int w, h;
  for (auto i = 0; i < 6; i++) {
    switch(i) {
      case 0 :
      name = "news";
      temp_rect = { 60, 240, 0, 0 };
      w = 35;
      h = 33;
      break;
      case 1 :
      name = "e_shop";
      temp_rect = { 62, 316, 0, 0 };
      w = 30;
      h = 30;
      break;
      case 2 :
      name = "album";
      temp_rect = { 60, 400, 0, 0 };
      w = 35;
      h = 23;
      break;
      case 3 :
      name = "controller";
      temp_rect = { 60, 470, 0, 0 };
      w = 34;
      h = 33;
      break;
      case 4 :
      name = "settings";
      temp_rect = { 60, 550, 0, 0 };
      w = 35;
      h = 36;
      break;
      case 5 :
      name = "power";
      temp_rect = { 65, 630, 0, 0 };
      w = 28;
      h = 33;
      break;
    }
    rectMap.insert(std::pair<std::string, SDL_Rect>(name, temp_rect));
    textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", w, h, true)));
  }
  temp_rect = { 45, 290, 0, 0};
  name = "seperator";
  for (auto i = 0; i < 5; i++) {
    temp_rect.y += 80 * i;
    rectMap.insert(std::pair<std::string, SDL_Rect>(name + std::to_string(i), temp_rect));
    textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name + std::to_string(i), name + ".png", 190, 5, true)));
  }

  // top menu
  // bg
  name = "top_bg";
  rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 420, 50, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 425, 50, true)));
  name = "handheld";
  rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 445, 45, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 58, 58, true)));
  name = "wifi";
  rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 760, 63, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 20, 20, true)));
  name = "battery";
  rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 790, 68, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 31, 15, true)));

  // avatar & hb menu
  name = "avatar";
  temp_rect = { 60, 40, 0, 0 };
  for (auto i = 0; i < 2; i++) {
    rectMap.insert(std::pair<std::string, SDL_Rect>(name, temp_rect));
    textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 60, 60, true)));
    name = "hb_menu";
    temp_rect.x += 80;
  }
  // game info
  name = "gamename_bg";
  temp_rect = { 0, 254, 0, 0 };
  for (auto i = 0; i < 2; i++) {
    rectMap.insert(std::pair<std::string, SDL_Rect>(name, temp_rect));
    textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 640, 50, true)));
    name = "company_bg";
    temp_rect.y += 30;
  }

  // game icons
  chdir("romfs:/assets/games/");
  name = "genshin-impact";
  temp_rect = { 470, 340, 0, 0 };
  w = 320;
  for (auto i = 0; i < 3; i++) {
    rectMap.insert(std::pair<std::string, SDL_Rect>(name, temp_rect));
    textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_masked_texture(name, name + ".png", w)));
    w = 260;
    temp_rect.y = 400;
    if (i == 0) {
      name = "super-mario-odyssey";
      temp_rect.x = 830;
    } else if (i == 1) {
      name = "breath-of-the-wild";
      temp_rect.x = 1130;
    }
  }

  // suspended
  chdir("romfs:/assets/UI/");
  name = "suspended_bg";
  rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 550, 570, 0, 0 }));
  textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 155, 40, true)));
}

void draw_ui()
{
  for (auto i = textureMap.begin(); i != textureMap.end(); i++) {
    SDL_RenderCopy(renderer, i->second, NULL, &rectMap.find(i->first)->second);
  }

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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
  if (!renderer) SDL_Quit();
  SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

  init_ui();

  chdir("romfs:/assets/UI/");
  TTF_Font* font14 = TTF_OpenFont("font.ttf", 14);
  TTF_Font* font = TTF_OpenFont("font.ttf", 18);
  TTF_Font* font26 = TTF_OpenFont("font.ttf", 26);
  TTF_Font* font30 = TTF_OpenFont("font.ttf", 30);


  SDL_Surface *news_surface = TTF_RenderText_Blended(font, "News", White);
  SDL_Texture *news_text = SDL_CreateTextureFromSurface(renderer, news_surface);
  SDL_Surface *e_shop_surface = TTF_RenderText_Blended(font, "E-Shop", White);
  SDL_Texture *e_shop_text = SDL_CreateTextureFromSurface(renderer, e_shop_surface);
  SDL_Surface *album_surface = TTF_RenderText_Blended(font, "Album", White);
  SDL_Texture *album_text = SDL_CreateTextureFromSurface(renderer, album_surface);
  SDL_Surface *controller_surface = TTF_RenderText_Blended(font, "Controller", White);
  SDL_Texture *controller_text = SDL_CreateTextureFromSurface(renderer, controller_surface);
  SDL_Surface *settings_surface = TTF_RenderText_Blended(font, "Settings", White);
  SDL_Texture *settings_text = SDL_CreateTextureFromSurface(renderer, settings_surface);
  SDL_Surface *power_surface = TTF_RenderText_Blended(font, "Power", White);
  SDL_Texture *power_text = SDL_CreateTextureFromSurface(renderer, power_surface);
  SDL_Surface *time_surface = TTF_RenderText_Blended(font26, "20:21", White);
  SDL_Texture *time_text = SDL_CreateTextureFromSurface(renderer, time_surface);
  SDL_Surface *suspended_text_surface = TTF_RenderText_Blended(font14, "SUSPENDED", White);
  SDL_Texture *suspended_text = SDL_CreateTextureFromSurface(renderer, suspended_text_surface);
  SDL_Surface *game_title_text_surface = TTF_RenderText_Blended(font30, "Genshin Impact", OffWhite);
  SDL_Texture *game_title_text = SDL_CreateTextureFromSurface(renderer, game_title_text_surface);
  SDL_Surface *game_info_text_surface = TTF_RenderText_Blended(font, "miHoYo, 1.0.0", DarkGrey);
  SDL_Texture *game_info_text = SDL_CreateTextureFromSurface(renderer, game_info_text_surface);

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
  SDL_QueryTexture(time_text, NULL, NULL, &w, &h);
  time_text_pos.w = w;
  time_text_pos.h = h;
  SDL_QueryTexture(suspended_text, NULL, NULL, &w, &h);
  suspended_text_pos.w = w;
  suspended_text_pos.h = h;
  SDL_QueryTexture(game_title_text, NULL, NULL, &w, &h);
  game_title_text_pos.x = 1280 - w - 20;
  game_title_text_pos.w = w;
  game_title_text_pos.h = h;
  game_title_bg_pos.x = game_title_text_pos.x - 20;
  SDL_QueryTexture(game_info_text, NULL, NULL, &w, &h);
  game_info_text_pos.x = 1280 - w - 10;
  game_info_text_pos.w = w;
  game_info_text_pos.h = h;
  game_info_bg_pos.x = game_info_text_pos.x - 20;



  while (appletMainLoop()) {
    hidScanInput();

    u32 keyDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (keyDown & KEY_PLUS)
    break;

    SDL_RenderClear(renderer);


    draw_ui();
    SDL_RenderCopy(renderer, news_text, NULL, &news_text_pos);
    SDL_RenderCopy(renderer, e_shop_text, NULL, &e_shop_text_pos);
    SDL_RenderCopy(renderer, album_text, NULL, &album_text_pos);
    SDL_RenderCopy(renderer, controller_text, NULL, &controller_text_pos);
    SDL_RenderCopy(renderer, settings_text, NULL, &settings_text_pos);
    SDL_RenderCopy(renderer, power_text, NULL, &power_text_pos);
    SDL_RenderCopy(renderer, time_text, NULL, &time_text_pos);
    SDL_RenderCopy(renderer, suspended_text, NULL, &suspended_text_pos);
    SDL_RenderCopy(renderer, game_title_text, NULL, &game_title_text_pos);
    SDL_RenderCopy(renderer, game_info_text, NULL, &game_info_text_pos);

    SDL_RenderPresent(renderer);

    SDL_Delay(1);
  }
  SDL_FreeSurface(news_surface);
  SDL_FreeSurface(e_shop_surface);
  SDL_FreeSurface(album_surface);
  SDL_FreeSurface(controller_surface);
  SDL_FreeSurface(settings_surface);
  SDL_FreeSurface(power_surface);
  SDL_FreeSurface(time_surface);
  SDL_FreeSurface(game_title_text_surface);
  SDL_FreeSurface(game_info_text_surface);
  SDL_DestroyTexture(news_text);
  SDL_DestroyTexture(e_shop_text);
  SDL_DestroyTexture(album_text);
  SDL_DestroyTexture(controller_text);
  SDL_DestroyTexture(settings_text);
  SDL_DestroyTexture(power_text);
  SDL_DestroyTexture(time_text);
  SDL_DestroyTexture(game_title_text);
  SDL_DestroyTexture(game_info_text);
  SDL_FreeSurface(windowSurface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  //gfxExit();
  return 0;
}
