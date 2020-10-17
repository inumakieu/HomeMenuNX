#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <switch.h>
#include <unordered_map>
#include <string>

class base_screen {
public:
  std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> textureMap;
  SDL_Renderer *renderer;

  base_screen(SDL_Renderer *r);

  void init();

  void update();


protected:

  SDL_Texture *create_texture(std::string name, std::string filename);

  SDL_Texture *create_masked_texture(SDL_Texture *game, int w);

};
