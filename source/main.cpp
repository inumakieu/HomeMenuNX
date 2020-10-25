#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include <unistd.h>

#include "homescreen.h"
#include "json.hpp"
#include "main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <switch.h>
#include <switch/services/applet.h>
#include <chrono>
#include <ctime>

using json = nlohmann::json;

SDL_Rect wallpaper_pos = {0, 0, 0, 0};

SDL_Rect icons_bg_pos = {20, 200, 0, 0};

SDL_Rect name_bg_pos = {1020, 260, 0, 0};
SDL_Rect news_text_pos = {120, 244, 0, 0};
SDL_Rect e_shop_text_pos = {120, 320, 0, 0};
SDL_Rect album_text_pos = {120, 404, 0, 0};
SDL_Rect controller_text_pos = {120, 475, 0, 0};
SDL_Rect settings_text_pos = {120, 556, 0, 0};
SDL_Rect power_text_pos = {120, 636, 0, 0};
SDL_Rect top_menu_bg_pos = {420, 50, 0, 0};
SDL_Rect time_text_pos = {606, 60, 0, 0};
SDL_Rect suspended_bg_pos = {550, 570, 0, 0};
SDL_Rect suspended_text_pos = {584, 580, 0, 0};
SDL_Rect game_title_text_pos = {0, 260, 0, 0};
SDL_Rect game_title_bg_pos = {0, 254, 0, 0};
SDL_Rect game_info_text_pos = {0, 306, 0, 0};
SDL_Rect game_info_bg_pos = {0, 284, 0, 0};

SDL_Color White = {255, 255, 255};
SDL_Color OffWhite = {230, 230, 230};
SDL_Color DarkGrey = {11, 11, 11};

SDL_Renderer *renderer;

std::unordered_map<std::string, std::pair<SDL_Texture *, SDL_Rect>> textureMap;
std::list<std::string> games = {};

int selected = 0;

std::vector<Title> titles;
std::unordered_map<u64, SDL_Texture *> icons;
NsApplicationControlData appControlData;

std::string getAppName(uint64_t Tid)
{
  memset(&appControlData, 0x0, sizeof(NsApplicationControlData));

  size_t appControlDataSize = 0;
  if (R_FAILED(nsGetApplicationControlData(NsApplicationControlSource::NsApplicationControlSource_Storage, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize)))
  {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  NacpLanguageEntry *languageEntry = nullptr;
  if (R_FAILED(nacpGetLanguageEntry(&appControlData.nacp, &languageEntry)))
  {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  return std::string(languageEntry->name);
}

std::string getAppAuthor(uint64_t Tid)
{
  memset(&appControlData, 0x0, sizeof(NsApplicationControlData));

  size_t appControlDataSize = 0;
  if (R_FAILED(nsGetApplicationControlData(NsApplicationControlSource::NsApplicationControlSource_Storage, Tid, &appControlData, sizeof(NsApplicationControlData), &appControlDataSize)))
  {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  NacpLanguageEntry *languageEntry = nullptr;
  if (R_FAILED(nacpGetLanguageEntry(&appControlData.nacp, &languageEntry)))
  {
    char returnTID[17];
    sprintf(returnTID, "%016" PRIx64, Tid);
    return (std::string)returnTID;
  }

  return std::string(languageEntry->author);
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
  /**
  if (actualAppRecordCnt > 10)
    actualAppRecordCnt = 10;
  **/

  for (int32_t i = 0; i < actualAppRecordCnt; i++)
  {
    Title title;
    title.TitleID = appRecords[i].application_id;
    title.TitleName = getAppName(appRecords[i].application_id);
    title.TitleAuthor = getAppAuthor(appRecords[i].application_id);
    memcpy(&title.icon, appControlData.icon, sizeof(title.icon));
    apps.push_back(title);
  }
  delete[] appRecords;
  return apps;
}

SDL_Texture *create_texture(std::string name, std::string filename)
{
  SDL_Texture *temp_tex = NULL;
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if (surface)
  {
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

SDL_Texture *saveIcon(uint8_t *icon)
{
  SDL_RWops *temp = SDL_RWFromMem(icon, 0x20000);
  SDL_Surface *test_s = IMG_LoadJPG_RW(temp);
  SDL_Texture *test = SDL_CreateTextureFromSurface(renderer, test_s);
  SDL_Texture *masked = create_masked_texture(test, 320);
  SDL_FreeSurface(test_s);
  return masked;
}

// credit to SwitchIdent
u32 GetBatteryPercent(void)
{
  Result ret = 0;
  u32 out = 0;

  if (R_FAILED(ret = psmGetBatteryChargePercentage(&out)))
    return -1;

  return out;
}

AppletApplication appletApplication;

Result LaunchApplication(AppletApplication *a, u64 *app_id)
{

  return 0;
}

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);

  romfsInit();
  psmInitialize();
  //appletInitialize();
  chdir("romfs:/assets/UI/");

  TTF_Init();

  //Switchpush_back screen size: 720p. Must set to full screen.
  SDL_Window *window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN);
  if (!window)
    SDL_Quit();
  renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    SDL_Quit();
  SDL_Surface *windowSurface = SDL_GetWindowSurface(window);

  socketInitializeDefault(); // Initialize sockets
  nxlinkStdio();             // Redirect stdout and stderr over the network to nxlink

  Result nsError = nsInitialize();

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  homescreen home(renderer);
  home.init();

  chdir("romfs:/assets/UI/");
  TTF_Font *font14 = TTF_OpenFont("font.ttf", 14);
  TTF_Font *font = TTF_OpenFont("font.ttf", 18);
  TTF_Font *font26 = TTF_OpenFont("font.ttf", 26);
  TTF_Font *font30 = TTF_OpenFont("font.ttf", 30);

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

  std::string battery_s = std::to_string(GetBatteryPercent()) + "%";
  SDL_Rect battery_pos = {748, 64, 0, 0};
  SDL_Surface *power_pct_surface = TTF_RenderText_Blended(font, battery_s.c_str(), White);
  SDL_Texture *power_pct_text = SDL_CreateTextureFromSurface(renderer, power_pct_surface);

  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, "%H:%M", timeinfo);

  SDL_Surface *time_surface = TTF_RenderText_Blended(font26, buffer, White);
  SDL_Texture *time_text = SDL_CreateTextureFromSurface(renderer, time_surface);

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
  SDL_QueryTexture(power_pct_text, NULL, NULL, &w, &h);
  battery_pos.w = w;
  battery_pos.h = h;

  std::ifstream ifs("homescreen.json");
  json j;
  j << ifs;

  std::cout << GetBatteryPercent() << '\n';

  titles = getAllTitles();
  for (Title n : titles)
  {
    SDL_Texture *t = saveIcon(n.icon);
    icons.insert(std::make_pair(n.TitleID, t));
    std::cout << n.TitleName << '\n';
  }

  SDL_Surface *game_title_text_surface = TTF_RenderText_Blended(font30, titles[home.selected].TitleName.c_str(), OffWhite);
  SDL_Texture *game_title_text = SDL_CreateTextureFromSurface(renderer, game_title_text_surface);

  SDL_Surface *game_info_text_surface = TTF_RenderText_Blended(font, titles[home.selected].TitleAuthor.c_str(), DarkGrey);
  SDL_Texture *game_info_text = SDL_CreateTextureFromSurface(renderer, game_info_text_surface);
  SDL_QueryTexture(game_title_text, NULL, NULL, &w, &h);
  home.game_title_text_pos.x = 1280 - w - 20;
  home.game_title_text_pos.w = w;
  home.game_title_text_pos.h = h;
  SDL_QueryTexture(game_info_text, NULL, NULL, &w, &h);
  home.game_info_text_pos.x = 1280 - w - 10;
  home.game_info_text_pos.w = w;
  home.game_info_text_pos.h = h;

  home.textureMap.find("company_bg")->second.second.x = home.game_info_text_pos.x - 20;

  while (appletMainLoop())
  {
    hidScanInput();

    u32 keyDown = hidKeysDown(CONTROLLER_P1_AUTO);
    if (keyDown & KEY_PLUS)
      break;

    if (keyDown & KEY_LEFT)
    {
      if (home.selected != 0)
      {
        home.selected--;
      }
    }
    else if (keyDown & KEY_RIGHT)
    {
      if (home.selected != titles.size() - 1)
      {
        home.selected++;
      }
    }
    else if (keyDown & KEY_A)
    {
    }

    home.update(titles, icons);

    SDL_FreeSurface(time_surface);
    SDL_DestroyTexture(time_text);

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M", timeinfo);

    time_surface = TTF_RenderText_Blended(font26, buffer, White);
    time_text = SDL_CreateTextureFromSurface(renderer, time_surface);

    SDL_FreeSurface(game_title_text_surface);
    SDL_DestroyTexture(game_title_text);

    game_title_text_surface = TTF_RenderText_Blended(font30, titles[home.selected].TitleName.c_str(), OffWhite);
    game_title_text = SDL_CreateTextureFromSurface(renderer, game_title_text_surface);

    SDL_FreeSurface(game_info_text_surface);
    SDL_DestroyTexture(game_info_text);

    game_info_text_surface = TTF_RenderText_Blended(font, titles[home.selected].TitleAuthor.c_str(), DarkGrey);
    game_info_text = SDL_CreateTextureFromSurface(renderer, game_info_text_surface);

    SDL_FreeSurface(power_pct_surface);
    SDL_DestroyTexture(power_pct_text);

    battery_s = std::to_string(GetBatteryPercent()) + "%";

    power_pct_surface = TTF_RenderText_Blended(font, battery_s.c_str(), White);
    power_pct_text = SDL_CreateTextureFromSurface(renderer, power_pct_surface);

    SDL_QueryTexture(game_title_text, NULL, NULL, &w, &h);
    home.game_title_text_pos.x = 1280 - w - 20;
    home.game_title_text_pos.w = w;
    home.game_title_text_pos.h = h;
    SDL_QueryTexture(game_info_text, NULL, NULL, &w, &h);
    home.game_info_text_pos.x = 1280 - w - 10;
    home.game_info_text_pos.w = w;
    home.game_info_text_pos.h = h;
    SDL_QueryTexture(power_pct_text, NULL, NULL, &w, &h);
    battery_pos.w = w;
    battery_pos.h = h;

    SDL_RenderCopy(renderer, news_text, NULL, &news_text_pos);
    SDL_RenderCopy(renderer, e_shop_text, NULL, &e_shop_text_pos);
    SDL_RenderCopy(renderer, album_text, NULL, &album_text_pos);
    SDL_RenderCopy(renderer, controller_text, NULL, &controller_text_pos);
    SDL_RenderCopy(renderer, settings_text, NULL, &settings_text_pos);
    SDL_RenderCopy(renderer, power_text, NULL, &power_text_pos);
    SDL_RenderCopy(renderer, power_pct_text, NULL, &battery_pos);
    SDL_RenderCopy(renderer, time_text, NULL, &time_text_pos);
    SDL_RenderCopy(renderer, game_title_text, NULL, &home.game_title_text_pos);
    SDL_RenderCopy(renderer, game_info_text, NULL, &home.game_info_text_pos);

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
  psmExit();
  //appletExit();

  socketExit(); // Cleanup

  //gfxExit();
  return 0;
}
