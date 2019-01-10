#include "ioMod.h"
#include <SDL_image.h>
#include "gameData.h"
#include "renderContext.h"

IoMod& IoMod::getInstance() {
  static IoMod instance;
  return instance;
}

IoMod::~IoMod() {
  TTF_CloseFont(font);
  TTF_Quit();
}

IoMod::IoMod()
    : init(TTF_Init()),
      renderer(RenderContext::getInstance().getRenderer()),
      font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                        Gamedata::getInstance().getXmlInt("font/size"))),
      textColor({0xff, 0, 0, 0}) {
  if (init == -1) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("font/color/r");
  textColor.g = Gamedata::getInstance().getXmlInt("font/color/g");
  textColor.b = Gamedata::getInstance().getXmlInt("font/color/b");
  textColor.a = Gamedata::getInstance().getXmlInt("font/color/a");
}

SDL_Texture* IoMod::readTexture(const std::string& filename) {
  SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());
  if (texture == NULL) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IoMod::readSurface(const std::string& filename) {
  SDL_Surface* surface = IMG_Load(filename.c_str());
  if (!surface) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IoMod::writeText(const std::string& msg, int x, int y) const {
  SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IoMod::writeText(const std::string& msg, SDL_Color addColor, int x, int y) const {
  SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), addColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
