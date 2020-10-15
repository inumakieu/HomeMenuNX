#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include <unistd.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_power.h>
#include <SDL_ttf.h>
#include <switch.h>
#include <chrono>
#include <ctime>
#include "json.hpp"
#include "main.hpp"

using json = nlohmann::json;

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

std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> textureMap;
std::list<std::string> games = {};

int selected = 0;

std::vector<Title> titles;
std::unordered_map<u64, SDL_Texture*> icons;
NsApplicationControlData appControlData;

std::string getAppName(uint64_t Tid)
{
  memset(&appControlData, 0x0, sizeof(NsApplicationControlData));

  size_t appControlDataSize = 0;
  if (R_FAILED(nsGetApplicationControlData(NsApplicationControlSource::NsApplicationControlSource_Storage, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize))) {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  NacpLanguageEntry *languageEntry = nullptr;
  if (R_FAILED(nacpGetLanguageEntry(&appControlData.nacp, &languageEntry))) {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  return std::string(languageEntry->name);
}

std::vector<Title> getAllTitles()
{
  std::vector<Title> apps;
  NsApplicationRecord *appRecords = new NsApplicationRecord[1024]; // Nobody's going to have more than 1024 games hopefully...
  int32_t actualAppRecordCnt = 0;
  Result rc;
  rc = nsListApplicationRecord(appRecords, 1024, 0, &actualAppRecordCnt);
  if (R_FAILED(rc))
  {
    //while (brls::Application::mainLoop());
    exit(rc);
  }

  for (int32_t i = 0; i < actualAppRecordCnt; i++)
  {
    Title title;
    title.TitleID = appRecords[i].application_id;
    title.TitleName = getAppName(appRecords[i].application_id);
    memcpy(&title.icon, appControlData.icon, sizeof(title.icon));
    apps.push_back(title);
  }
  delete[] appRecords;
  return apps;
}


SDL_Texture* create_texture(std::string name, std::string filename)
{
  SDL_Texture *temp_tex = NULL;
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if (surface) {
    temp_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
  }
  return temp_tex;
}

SDL_Texture *create_masked_texture(SDL_Texture *game, int w)
{
  chdir("romfs:/assets/games/");
  SDL_Texture *mask_tex = create_texture("mask", "mask.png");

  SDL_Texture *maskedTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, w);

  SDL_SetTextureBlendMode(maskedTex, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(game, SDL_BLENDMODE_MOD);
  SDL_SetRenderTarget(renderer, maskedTex);
  SDL_Rect dst = {0, 0, w, w};

  SDL_RenderCopy(renderer, mask_tex, NULL, &dst);
  SDL_RenderCopy(renderer, game, NULL, &dst);
  SDL_SetRenderTarget(renderer, NULL);
  chdir("romfs:/assets/UI/");
  return maskedTex;
}

void init_ui()
{
  std::string name;
  SDL_Rect temp_rect = { 0, 0, 1280, 720 };
  int w, h = 0;
  //rectMap.insert(std::pair<std::string, SDL_Rect>("wallpaper", { 0, 0, 0, 0 }));
  //textureMap.insert(std::pair<std::string, std::pair<SDL_Texture*, SDL_Rect>>((create_texture("wallpaper", "wallpaper.png", 1280, 720, true, { 0, 0, 0, 0 }), { 0, 0, 0, 0 })));
  auto pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture("wallpaper", "wallpaper.png"), temp_rect);
  textureMap.insert(std::make_pair("wallpaper", pair));
  //textureMap.insert(std::make_pair("wallpaper", std::make_pair(create_texture("wallpaper", "wallpaper.png", 1280, 720, true, { 0, 0, 0, 0 }), { 0, 0, 0, 0 })));
  temp_rect = { 20, 200, 240, 500 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture("icons_bg", "icons_bg.png"), temp_rect);
  textureMap.insert(std::make_pair("icons_bg", pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>("icons_bg", { 20, 200, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>("icons_bg", create_texture("icons_bg", "icons_bg.png", 240, 500, true)));

  // icons
  for (auto i = 0; i < 6; i++) {
    switch(i) {
      case 0 :
      name = "news";
      temp_rect = { 60, 240, 35, 33 };
      break;
      case 1 :
      name = "e_shop";
      temp_rect = { 62, 316, 30, 30 };
      w = 30;
      h = 30;
      break;
      case 2 :
      name = "album";
      temp_rect = { 60, 400, 35, 23 };
      w = 35;
      h = 23;
      break;
      case 3 :
      name = "controller";
      temp_rect = { 60, 470, 34, 33 };
      w = 34;
      h = 33;
      break;
      case 4 :
      name = "settings";
      temp_rect = { 60, 550, 35, 36 };
      w = 35;
      h = 36;
      break;
      case 5 :
      name = "power";
      temp_rect = { 65, 630, 28, 33 };
      w = 28;
      h = 33;
      break;
    }
    pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
    textureMap.insert(std::make_pair(name, pair));
  }

  temp_rect = { 45, 290, 190, 5 };
  std::string f = "seperator";
  name = "seperator";
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  temp_rect = { 45, 370, 190, 5 };
  name = "seperator2";
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, f + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  temp_rect = { 45, 450, 190, 5 };
  name = "seperator3";
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, f + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  temp_rect = { 45, 530, 190, 5 };
  name = "seperator4";
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, f + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  temp_rect = { 45, 610, 190, 5 };
  name = "seperator5";
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, f + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  for (auto i = 0; i < 5; i++) {
    //rectMap.insert(std::pair<std::string, SDL_Rect>(name + std::to_string(i), temp_rect));
    //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name + std::to_string(i), name + ".png", 190, 5, true)));
  }

  // top menu
  // bg
  name = "top_bg";
  temp_rect = { 420, 50, 425, 50 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 420, 50, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 425, 50, true)));
  name = "handheld";
  temp_rect = { 445, 45, 58, 58 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 445, 45, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 58, 58, true)));
  name = "wifi";
  temp_rect = { 760, 63, 20, 20 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 760, 63, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 20, 20, true)));
  name = "battery";
  temp_rect = { 790, 68, 31, 15 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 790, 68, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 31, 15, true)));

  // avatar & hb menu
  name = "avatar";
  temp_rect = { 60, 40, 60, 60 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));

  name = "hb_menu";
  temp_rect = { 140, 40, 60, 60 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture("hb_menu", name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  // game info

  name = "company_bg";
  temp_rect = { 0, 284, 640, 50 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));

  name = "gamename_bg";
  temp_rect = { 0, 254, 640, 50 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));


  //suspended
  chdir("romfs:/assets/UI/");
  name = "suspended_bg";
  temp_rect = { 550, 570, 155, 40 };
  pair = std::pair<SDL_Texture*, SDL_Rect>(create_texture(name, name + ".png"), temp_rect);
  textureMap.insert(std::make_pair(name, pair));
  //rectMap.insert(std::pair<std::string, SDL_Rect>(name, { 550, 570, 0, 0 }));
  //textureMap.insert(std::pair<std::string, SDL_Texture*>(name, create_texture(name, name + ".png", 155, 40, true)));
}

void draw_top_menu()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("top_bg")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("handheld")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("wifi")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("battery")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
}

void draw_icons_menu()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("icons_bg")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("news")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("e_shop")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("album")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("controller")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("settings")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("power")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
}

void draw_top_left()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("avatar")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("hb_menu")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
}

void draw_seperators()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("seperator")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("seperator2")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("seperator3")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("seperator4")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("seperator5")->second;
  t = p.first;
  r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
}

void draw_details()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("company_bg")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = { 0, 284, 640, 50 };
  r.x = game_info_text_pos.x -20;
  SDL_RenderCopy(renderer, t, NULL, &r);
  p = textureMap.find("gamename_bg")->second;
  t = p.first;
  r = { 0, 254, 640, 50 };
  r.x = game_title_text_pos.x - 20;
  SDL_RenderCopy(renderer, t, NULL, &r);
}

void draw_games()
{

  // draw selected game
  //auto it = std::next(titles.begin(), selected);
  SDL_Texture *t = icons.find(titles[selected].TitleID)->second;
  SDL_Rect r = { 470, 340, 320, 320 };
  SDL_RenderCopy(renderer, t, NULL, &r);

  // loop through all games to the left of it and draw them to correct positions
  SDL_Rect temp_rect = { 470 - 260 - 40, 400, 260, 260 };
  if (selected != 0) {
    for (auto i = selected - 1; i >= 0; i--) {
      SDL_Texture *t = icons.find(titles[i].TitleID)->second;
      SDL_RenderCopy(renderer, t, NULL, &temp_rect);
      temp_rect.x -= 260 + 20; // icon size plus margin
    }
  }

  // loop through all games to the right of it and draw them to correct positions

  temp_rect = { 470 + 320 + 20, 400, 260, 260 };
  if (selected != titles.size() - 1) {
    for (auto i = selected + 1; i < titles.size(); i++) {
      SDL_Texture *t = icons.find(titles[i].TitleID)->second;
      SDL_RenderCopy(renderer, t, NULL, &temp_rect);
      temp_rect.x += 260 + 20; // icon size plus margin
    }
  }


}

void draw_ui()
{
  std::pair<SDL_Texture*, SDL_Rect> p = textureMap.find("wallpaper")->second;
  SDL_Texture *t = p.first;
  SDL_Rect r = p.second;
  SDL_RenderCopy(renderer, t, NULL, &r);
  draw_top_menu();
  draw_games();
  draw_icons_menu();
  draw_seperators();
  draw_top_left();
  draw_details();

}

void change_selected()
{

  // test
  //auto it1 = std::next(games.begin(), selected);
  //std::cout << *it1 << '\n'; // gives back gesnhin-impact, so it works as expected
}

SDL_Texture* saveIcon(uint8_t* icon)
{
  SDL_RWops *temp = SDL_RWFromMem(icon, 0x20000);
  SDL_Surface *test_s = IMG_LoadJPG_RW(temp);
  SDL_Texture *test = SDL_CreateTextureFromSurface(renderer, test_s);
  SDL_Texture *masked = create_masked_texture(test, 320);
  SDL_FreeSurface(test_s);
  return masked;
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  romfsInit();
  chdir("romfs:/assets/UI/");

  TTF_Init();

  /*gfxInitDefault();
  consoleInit(nullptr);*/

  //Switchpush_back screen size: 720p. Must set to full screen.
  SDL_Window* window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
  if (!window) SDL_Quit();
  renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
  if (!renderer) SDL_Quit();
  SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

  socketInitializeDefault();              // Initialize sockets
  nxlinkStdio();                          // Redirect stdout and stderr over the network to nxlink

  Result nsError = nsInitialize();

  init_ui();

  //titles = getAllTitles();

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


  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%H:%M",timeinfo);

  SDL_Surface *time_surface = TTF_RenderText_Blended(font26, buffer, White);
  SDL_Texture *time_text = SDL_CreateTextureFromSurface(renderer, time_surface);

  SDL_Surface *suspended_text_surface = TTF_RenderText_Blended(font14, "SUSPENDED", White);
  SDL_Texture *suspended_text = SDL_CreateTextureFromSurface(renderer, suspended_text_surface);



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

  textureMap.find("company_bg")->second.second.x = game_info_text_pos.x - 20;


  SDL_QueryTexture(game_info_text, NULL, NULL, &w, &h);
  game_info_text_pos.x = 1280 - w - 10;
  game_info_text_pos.w = w;
  game_info_text_pos.h = h;

  //draw_ui();
  titles = getAllTitles();
  for(Title n : titles)
  {
    std::cout << n.TitleName << '\n';
    SDL_Texture *t = saveIcon(n.icon);
    icons.insert(std::make_pair(n.TitleID, t));
  }
  std::cout << titles[3].TitleName << '\n';
  //loadIcon(titles.front().TitleID, &appControlData, 200);

  SDL_Surface *game_title_text_surface = TTF_RenderText_Blended(font30, titles[selected].TitleName.c_str(), OffWhite);
  SDL_Texture *game_title_text = SDL_CreateTextureFromSurface(renderer, game_title_text_surface);
  SDL_QueryTexture(game_title_text, NULL, NULL, &w, &h);
  game_title_text_pos.x = 1280 - w - 20;
  game_title_text_pos.w = w;
  game_title_text_pos.h = h;


  while (appletMainLoop()) {
    hidScanInput();

    u32 keyDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (keyDown & KEY_PLUS)
    break;

    if (keyDown & KEY_LEFT) {
      if (selected != 0) {
        selected--;
      }
    } else if (keyDown & KEY_RIGHT) {
      if (selected != titles.size() - 1) {
        selected++;
      }
    }


    draw_ui();
    SDL_FreeSurface(time_surface);

    SDL_DestroyTexture(time_text);

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%H:%M",timeinfo);

    time_surface = TTF_RenderText_Blended(font26, buffer, White);
    time_text = SDL_CreateTextureFromSurface(renderer, time_surface);
    change_selected();

    SDL_FreeSurface(game_title_text_surface);

    SDL_DestroyTexture(game_title_text);

    game_title_text_surface = TTF_RenderText_Blended(font30, titles[selected].TitleName.c_str(), OffWhite);
    game_title_text = SDL_CreateTextureFromSurface(renderer, game_title_text_surface);
    SDL_QueryTexture(game_title_text, NULL, NULL, &w, &h);
    game_title_text_pos.x = 1280 - w - 20;
    game_title_text_pos.w = w;
    game_title_text_pos.h = h;

    //SDL_RenderCopy(renderer, test, NULL, &test_pos);


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
    SDL_RenderClear(renderer);
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
  nsExit();
  romfsExit();

  socketExit();                           // Cleanup

  //gfxExit();
  return 0;
}
