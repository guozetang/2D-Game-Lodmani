#ifndef _HEALTHBAR__H
#define _HEALTHBAR__H

#include "SDL.h"
#include "gameData.h"
#include "vector2f.h"

class HealthBar {
 public:
  static HealthBar& getInstance();

  // void draw(int, int, int, int) const;
  void draw() const;
  void reset() { currentLength = 1.0; }
  void setCurrentLength(float l) { currentLength = l; }
  float getCurrentLength() const { return currentLength; }

 private:
  SDL_Renderer* renderer;
  Vector2f pos;
  int backBarWidth;
  int backBarHeight;
  SDL_Color barColor;
  SDL_Color backColor;
  float currentLength;

  HealthBar();
  HealthBar(const HealthBar&);
  HealthBar& operator=(const HealthBar&) = delete;
};

#endif