#ifndef _HUD__H
#define _HUD__H

#include <SDL.h>
#include <iostream>
#include "drawable.h"
#include "vector2f.h"

class Sprite;
class IoMod;
class PlayerPhoto;

class Hud {
 public:
  virtual ~Hud() {}
  virtual bool isVisible() = 0;
  virtual void setVisible(const bool v) = 0;
};

class HudBulletPool : public Hud {
 public:
  static HudBulletPool& getInstance();
  virtual bool isVisible() { return visible; }
  virtual void setVisible(const bool v) { visible = v; }
  void draw(unsigned int, unsigned int);

 private:
  int width;
  int height;
  Vector2f pos;
  bool visible;
  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;
  HudBulletPool();
  HudBulletPool(const HudBulletPool&) = delete;
  HudBulletPool& operator=(const HudBulletPool&) = delete;
};

class HudTips : public Hud {
 public:
  static HudTips& getInstance();


  virtual bool isVisible() { return visible; }
  virtual void setVisible(const bool v) { visible = v; }
  void draw();

 private:
  int width;
  int height;
  Vector2f pos;
  bool visible;
  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;
  HudTips();
  HudTips(const HudTips&) = delete;
  HudTips& operator=(const HudTips&) = delete;
};

class HudPlayerInfo : public Hud {
 public:
  static HudPlayerInfo& getInstance();
  virtual bool isVisible() { return visible; }
  virtual void setVisible(const bool v) { visible = v; }
  void draw(unsigned int);

 private:
  Image* playerPhoto;
  Image* healthFull;
  Image* healthEmpty;
  int width;
  int height;
  Vector2f pos;
  Vector2f healthPos;
  int interval_health;
  bool visible;
  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;
  HudPlayerInfo();
  HudPlayerInfo(const HudPlayerInfo&) = delete;
  HudPlayerInfo& operator=(const HudPlayerInfo&) = delete;
};

class HudGameStar : public Hud {
 public:
  static HudGameStar& getInstance();

  virtual bool isVisible() { return visible; }
  virtual void setVisible(const bool v) { visible = v; }
  void draw();

 private:
  int width;
  int height;
  Vector2f pos;
  bool visible;
  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;
  HudGameStar();
  HudGameStar(const HudGameStar&) = delete;
  HudGameStar& operator=(const HudGameStar&) = delete;
};

class HudGameOver : public Hud {
 public:
  static HudGameOver& getInstance();
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  const Vector2f& getPosition() const { return pos; }
  virtual bool isVisible() { return visible; }
  virtual void setVisible(const bool v) { visible = v; }
  void draw(bool);

 private:
  Image* gamelose;
  Image* gamewin;
  int width;
  int height;
  Vector2f pos;
  bool visible;

  SDL_Color winBackgroundColor;
  SDL_Color winOutlineColor;
  SDL_Color winTextColor;

  SDL_Color loseBackgroundColor;
  SDL_Color loseOutlineColor;
  SDL_Color loseTextColor;

  HudGameOver();
  HudGameOver(const HudGameOver&) = delete;
  HudGameOver& operator=(const HudGameOver&) = delete;
};

#endif
