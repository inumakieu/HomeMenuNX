#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <switch.h>
#include <vector>
#include <gifdec/gifdec.h>

#include "base_screen.h"
#include "main.hpp"

class homescreen : public base_screen
{
public:
  SDL_Rect wallpaper_pos = {0, 0, 1280, 720};
  SDL_Rect icons_bg_pos = {20, 200, 0, 0};
  SDL_Rect game_title_text_pos = {0, 260, 0, 0};
  SDL_Rect game_info_text_pos = {0, 306, 0, 0};
  int selected = 0;

  std::string name;
  std::pair<SDL_Texture *, SDL_Rect> pair;
  SDL_Rect temp_rect = {0, 0, 1280, 720};
  int w, h = 0;

  SDL_Texture *bg = NULL;
  SDL_Rect srcrect = {0, 0, 1280, 720};
  SDL_Rect dstrect = {0, 0, 1280, 720};
  Uint32 ticks = 0;
  Uint32 seconds = 0;
  Uint32 sprite = 0;
  int columns = 5;
  Uint32 x, y;

  homescreen(SDL_Renderer *r);

  void init();

  void update(std::vector<Title> titles, std::unordered_map<u64, SDL_Texture *> icons);

private:
  void init_wallpaper();

  void init_icons_menu();

  void init_icon_buttons();

  void init_seperator();

  void init_top_menu();

  void init_top_left();

  void init_game_info();

  void draw_wallpaper();

  void draw_top_menu();

  void draw_icons_menu();

  void draw_top_left();

  void draw_seperators();

  void draw_details();

  void draw_games(std::vector<Title> titles, std::unordered_map<u64, SDL_Texture *> icons);
};
