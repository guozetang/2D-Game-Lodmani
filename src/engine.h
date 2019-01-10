#ifndef _ENGINE__H
#define _ENGINE__H
#include <SDL.h>
#include <vector>
#include "clock.h"
#include "healthBar.h"
#include "hud.h"
#include "ioMod.h"
#include "menuEngine.h"
#include "renderContext.h"
#include "sound.h"
#include "spriteHealthBar.h"
#include "viewport.h"
#include "world.h"

class CollisionStrategy;
class Player;
class SmartSprite;

class Engine {
 public:
  Engine();
  ~Engine();
  bool play();
  void setFinalFlag() { final_flag = true; }

 private:
  const RenderContext& rc;
  SDL_Renderer* const renderer;
  const IoMod& io;
  Clock& clock;
  MenuEngine menu;
  HudPlayerInfo& hudPlayerInfo;
  HudBulletPool& hudBulletPool;
  HudTips& hudTips;
  HudGameOver& hudGameOver;
  HealthBar& healthBar;
  SpriteHealthBar* bossHealthBar;
  SDLSound sound;
  std::vector<World*> world;
  Viewport& viewport;
  Player* player;
  Drawable* boss;

  // std::vector<Drawable*> cute_sprites;
  std::vector<Drawable*> smart_sprites;
  std::vector<CollisionStrategy*> strategies;

  int currentStrategy;
  int currentSprite;
  mutable bool collision;
  bool final_flag;
  bool over;
  bool done;
  int bossHp;
  int score;
  bool makeVideo;

  void draw() const;
  void update(Uint32);
  void checkForCollisions();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

#endif