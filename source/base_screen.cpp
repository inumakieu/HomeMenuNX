#include "base_screen.h"


base_screen::base_screen(SDL_Renderer *r)
{
  renderer = r;
}

void base_screen::init() {}

void base_screen::update() {}

SDL_Texture* base_screen::create_texture(std::string name, std::string filename)
{
  SDL_Texture *temp_tex = NULL;
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if (surface) {
    temp_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
  }
  return temp_tex;
}

SDL_Texture* base_screen::create_masked_texture(SDL_Texture *game, int w)
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
