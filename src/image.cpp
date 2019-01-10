#include "image.h"
#include "drawable.h"
#include "ioMod.h"
#include "renderContext.h"
#include "viewport.h"

Image::Image(SDL_Surface* surf)
    : renderer(RenderContext::getInstance().getRenderer()),
      surface(surf),
      texture(nullptr),
      view{0, 0, surf->w, surf->h} {
  regenerateTexture();
}

Image::Image(const Image& image)
    : renderer(image.renderer), surface(image.surface), texture(image.texture), view(image.view) {}

Image& Image::operator=(const Image& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  view = rhs.view;
  return *this;
}

void Image::regenerateTexture() {
  if (texture != nullptr) SDL_DestroyTexture(texture);
  RenderContext& renderContext = RenderContext::getInstance();
  texture = SDL_CreateTextureFromSurface(renderContext.getRenderer(), surface);
}

void Image::draw(int x, int y) const { draw(x, y, 1.0f); }

void Image::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale * view.h;
  int tempWidth = scale * view.w;
  SDL_Rect dest = {x, y, tempWidth, tempHeight};
  SDL_RenderCopy(renderer, texture, &view, &dest);
}


void Image::draw(int tx, int ty, float tscale, bool flip, int test) const {
  int t = test;
  t++;
  tx -= Viewport::getInstance().getX();
  ty -= Viewport::getInstance().getY();
  int tempHeight = tscale * view.h;
  int tempWidth = tscale * view.w;
  SDL_Rect dest = {tx, ty, tempWidth, tempHeight};
  if (flip) {
    SDL_RenderCopyEx(renderer, texture, &view, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
  } else {
    SDL_RenderCopyEx(renderer, texture, &view, &dest, 0, NULL, SDL_FLIP_NONE);
  }
}

void Image::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = {sx, sy, view.w, view.h};
  SDL_Rect dst = {dx, dy, getWidth(), getHeight()};
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Image* Image::crop(SDL_Rect sub) const {
  if (sub.x + sub.w > view.w || sub.y + sub.h > view.h) {
    std::cerr << "Attempted to crop image with invalid geometry." << std::endl
              << "(0,0 + " << view.w << "x" << view.h << ")"
              << " --> "
              << "(" << sub.x << "," << sub.y << " + " << sub.w << "x" << sub.h << ")" << std::endl;
    return nullptr;
  }

  Image* cloned = new Image(*this);
  cloned->view = sub;

  return cloned;
}
