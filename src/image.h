#ifndef _IMAGE__H
#define _IMAGE__H
#include <SDL.h>
#include <iostream>
#include <string>

class Image {
 public:
  Image(SDL_Surface*);
  Image(const Image&);
  Image& operator=(const Image&);

  void regenerateTexture();
  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  // Two way sprites
  void draw(int tx, int ty, float tscalet, bool flip, int test) const;

  void draw(int sx, int sy, int dx, int dy) const;

  inline int getWidth() const { return surface->w; }
  inline int getHeight() const { return surface->h; }
  SDL_Surface* getSurface() const { return surface; }
  Image* crop(SDL_Rect) const;

 private:
  SDL_Renderer* renderer;
  SDL_Surface* surface;
  SDL_Texture* texture;
  SDL_Rect view;
  Image();
};

#endif
