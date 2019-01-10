#ifndef _IOMOD__H
#define _IOMOD__H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

class IoMod {
 public:
  static IoMod& getInstance();
  ~IoMod();
  SDL_Texture* readTexture(const std::string& filename);
  SDL_Surface* readSurface(const std::string& filename);
  void writeText(const std::string&, int, int) const;
  void writeText(const std::string&, SDL_Color, int, int) const;
  SDL_Renderer* getRenderer() const { return renderer; }

 private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
  IoMod();
  IoMod(const IoMod&);
  IoMod& operator=(const IoMod&);
};
#endif
