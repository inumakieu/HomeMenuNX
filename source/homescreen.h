#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <switch.h>
#include <vector>
#include <gifwrap/SDL_gifwrap.h>

#include "base_screen.h"
#include "main.hpp"

class homescreen : public base_screen
{
public:
  SDL_Rect wallpaper_pos = {0, 0, 0, 0};
  SDL_Rect icons_bg_pos = {20, 200, 0, 0};
  SDL_Rect game_title_text_pos = {0, 260, 0, 0};
  SDL_Rect game_info_text_pos = {0, 306, 0, 0};
  int selected = 0;

  GIF_Image *gif;

  homescreen(SDL_Renderer *r);

  void init();

  void update(std::vector<Title> titles, std::unordered_map<u64, SDL_Texture *> icons);

private:

  void update_wallpaper();

  void draw_top_menu();

  void draw_icons_menu();

  void draw_top_left();

  void draw_seperators();

  void draw_details();

  void draw_games(std::vector<Title> titles, std::unordered_map<u64, SDL_Texture *> icons);
};
