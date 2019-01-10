#ifndef _SPRITE_HEALTHBAR__H
#define _SPRITE_HEALTHBAR__H
#include "SDL.h"
#include "clock.h"
#include "drawable.h"
#include "gameData.h"
#include "ioMod.h"

class SpriteHealthBar : public Drawable {
 public:
  SpriteHealthBar(const std::string&);
  void draw() const;
  void draw(int, int, int, int) const;
  void update(Uint32);
  void reset() { currentLength = barRect.w; }

  SpriteHealthBar(const SpriteHealthBar&) = delete;
  SpriteHealthBar& operator=(const SpriteHealthBar&) = delete;

  void setCurrentLength(float l) { currentLength = l; }
  float getCurrentLength() const { return currentLength; }
  virtual int getScaledWidth() const { return barRect.w; }
  virtual int getScaledHeight() const { return barRect.h; }
  virtual const Image* getImage() const { throw std::string("No image in HealthBar"); }
  virtual const SDL_Surface* getSurface() const {
    // return image->getSurface();
    return NULL;
  }
  void active() { decrease = true; }
  int getHp() { return currentLength; }

 private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  Clock& clock;
  Vector2f pos;
  int backBarWidth;
  int backBarHeight;
  float currentLength;
  int increment;
  float interval;  // elapsed time for each decrement (1000ms=1sec)
  bool decrease;
  SDL_Color barColor;
  SDL_Color backColor;
  IoMod& io;
};
#endif
