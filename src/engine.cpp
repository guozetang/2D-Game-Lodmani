#include "engine.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include "collisionStrategy.h"
#include "frameGenerator.h"
#include "gameData.h"
#include "multisprite.h"
#include "player.h"
#include "smartSprite.h"
#include "sprite.h"
#include "twoWayMultisprite.h"

const std::string background[] = {"city", "lamp", "bg_tombwoof"};
const std::string smart_sprites_names[] = {"PinkBird", "RedBird", "monster_walk"};
// const std::string smart_sprites_names[] = {"PinkBird"};

Engine::~Engine() {
  delete player;
  delete boss;
  delete bossHealthBar;
  for (auto* w : world) delete w;
  for (auto* ss : smart_sprites) delete ss;
  for (CollisionStrategy* strategy : strategies) delete strategy;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine()
    : rc(RenderContext::getInstance()),
      renderer(rc.getRenderer()),
      io(IoMod::getInstance()),
      clock(Clock::getInstance()),
      menu(),
      hudPlayerInfo(HudPlayerInfo::getInstance()),
      hudBulletPool(HudBulletPool::getInstance()),
      hudTips(HudTips::getInstance()),
      hudGameOver(HudGameOver::getInstance()),
      healthBar(HealthBar::getInstance()),
      bossHealthBar(new SpriteHealthBar("bossHealthBar")),
      sound(),
      world(),
      viewport(Viewport::getInstance()),
      player(new Player("Viking")),
      boss(nullptr),
      // cute_sprites(),
      smart_sprites(),
      strategies(),
      currentStrategy(0),
      currentSprite(1),
      collision(false),
      final_flag(false),
      over(false),
      done(false),
      bossHp(Gamedata::getInstance().getXmlInt("bossHP")),
      score(0),
      makeVideo(false) {
  int background_number = sizeof(background) / sizeof(background[0]);
  world.reserve(background_number);
  for (int i = 0; i < background_number; ++i) {
    world.emplace_back(new World(background[i], Gamedata::getInstance().getXmlInt(background[i] + "/factor")));
  }

  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth(), h = player->getScaledHeight();
  int smart_sprites_number = sizeof(smart_sprites_names) / sizeof(smart_sprites_names[0]);
  smart_sprites.reserve(smart_sprites_number);
  for (int i = 0; i < smart_sprites_number; ++i) {
    int number = Gamedata::getInstance().getXmlInt(smart_sprites_names[i] + "/number");
    for (int j = 0; j < number; ++j) {
      smart_sprites.emplace_back(new SmartSprite(smart_sprites_names[i], pos, w, h));
    }
  }

  boss = new SmartSprite("boss", pos, w, h);

  for (auto* s_sprit : smart_sprites) {
    if (SmartSprite* temp = dynamic_cast<SmartSprite*>(s_sprit)) player->attach(temp);
  }

  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new MidPointCollisionStrategy);

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  for (auto* s : world) s->draw();
  for (auto* s : smart_sprites) s->draw();
  if (!over) player->draw();

  hudBulletPool.draw(player->getActiveBulletCount(), player->getFreeBulletCount());
  hudTips.draw();
  hudPlayerInfo.draw(player->getLivesLeft());
  viewport.draw(player->isGodMode(), clock.getFps(), smart_sprites.size(), final_flag);
  healthBar.draw();

  if (player->getLivesLeft() <= 0) {
    hudPlayerInfo.setVisible(false);
    hudBulletPool.setVisible(false);
    hudTips.setVisible(false);
    hudGameOver.setVisible(true);
    hudGameOver.draw(false);
  } else if (smart_sprites.size() <= 0) {
    if (done) {
      hudPlayerInfo.setVisible(false);
      hudBulletPool.setVisible(false);
      hudTips.setVisible(false);
      hudGameOver.setVisible(true);
      hudGameOver.draw(true);
    } else {
      boss->draw();
      bossHealthBar->draw();
    }
  }
  if (collision) collision = false;

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();

  for (auto* s : world) s->update();
  bossHealthBar->update(ticks);

  if (!smart_sprites.size()) setFinalFlag();
  if (!over) player->update(ticks);
  if (final_flag) boss->update(ticks);
  if (boss->hasExplode()) {
    done = true;
  }

  auto it = smart_sprites.begin();
  while (it != smart_sprites.end()) {
    (*it)->update(ticks);
    if ((*it)->hasExplode()) {
      SmartSprite* doa = dynamic_cast<SmartSprite*>(*it);
      player->detach(doa);
      delete doa;
      it = smart_sprites.erase(it);
    } else
      ++it;
  }

  viewport.update();
}

void Engine::checkForCollisions() {
  auto it = smart_sprites.begin();
  while (it != smart_sprites.end()) {
    if (strategies[currentStrategy]->execute(player, *it) && !player->isColliding()) {
      collision = true;
      if (!player->isGodMode() && (Clock::getInstance().getSeconds() - player->getLastExplosionTime()) >= 4) {
        static_cast<Drawable*>(player)->explode();
        sound[5];
        if (player->getLivesLeft() > 0) player->loseLife();
        healthBar.setCurrentLength(healthBar.getCurrentLength() - (1.0 / player->getInitNumLives()));
        player->resetBullet();
        SmartSprite* doa = dynamic_cast<SmartSprite*>(*it);
        player->detach(doa);
        delete doa;
        it = smart_sprites.erase(it);
      } else
        ++it;
    } else
      ++it;
  }

  it = smart_sprites.begin();
  while (it != smart_sprites.end()) {
    if (player->collided(*it)) {
      collision = true;
      (*it)->explode();
      sound[0];
      (*it)->setPosition(Vector2f(0, 0));
    }
    ++it;
  }

  if (final_flag) {
    if (strategies[currentStrategy]->execute(player, boss) && !player->isColliding()) {
      collision = true;
      if (!player->isGodMode() && (Clock::getInstance().getSeconds() - player->getLastExplosionTime()) >= 6) {
        static_cast<Drawable*>(player)->explode();
        if (player->getLivesLeft() > 0) player->loseLife();
        healthBar.setCurrentLength(healthBar.getCurrentLength() - (1.0 / player->getInitNumLives()));
        player->resetBullet();
        sound[5];
      }
    }
    if (player->collided(boss)) {
      bossHealthBar->active();
      sound[0];
    }
    if (!bossHealthBar->getHp()) {
      boss->explode();
      // sound[5];
      boss->setPosition(Vector2f(2000, 800));
    }
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();

  while (!done) {
    while (SDL_PollEvent(&event)) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type == SDL_QUIT) {
        done = true;
        break;
      }
      if (event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_P]) {
          if (clock.isPaused())
            clock.unpause();
          else
            clock.pause();
          sound.toggleMusic();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          hudTips.setVisible(!hudTips.isVisible());
          hudBulletPool.setVisible(!hudBulletPool.isVisible());
        }
        if (keystate[SDL_SCANCODE_T]) {
          player->changeBullet();
        }
        if (keystate[SDL_SCANCODE_G]) {
          player->toggleGodMode();
        }

        if (keystate[SDL_SCANCODE_J]) {
          static_cast<Player*>(player)->shoot();
        }

        if (keystate[SDL_SCANCODE_C]) {
          static_cast<Player*>(player)->setBulletDirection();
        }

        if (keystate[SDL_SCANCODE_R]) {
          healthBar.setCurrentLength(1.0);
          clock.unpause();
          return true;
        }
        // if (keystate[SDL_SCANCODE_C]) {
        //   currentStrategy = (1 + currentStrategy) % strategies.size();
        // }
        if (keystate[SDL_SCANCODE_M]) {
          clock.pause();
          menu.play();
          clock.unpause();
        }
      }
    }

    ticks = clock.getElapsedTicks();
    if (ticks > 0) {
      clock.incrFrame();
      if (!keystate[SDL_SCANCODE_D] && !keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_W] &&
          !keystate[SDL_SCANCODE_S] && !keystate[SDL_SCANCODE_SPACE] && !keystate[SDL_SCANCODE_K] &&
          !keystate[SDL_SCANCODE_M] && !keystate[SDL_SCANCODE_N] && !keystate[SDL_SCANCODE_I]) {
        player->changeImages("idle");
      }
      if (keystate[SDL_SCANCODE_A] && !keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D] && !keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S] && !keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->down();
      }

      if (keystate[SDL_SCANCODE_SPACE]) {
        static_cast<Player*>(player)->jump();
      }
      if (keystate[SDL_SCANCODE_K]) {
        static_cast<Player*>(player)->attack1();
      }
      if (keystate[SDL_SCANCODE_M]) {
        static_cast<Player*>(player)->attack2();
      }
      if (keystate[SDL_SCANCODE_N]) {
        static_cast<Player*>(player)->attack3();
      }
      if (keystate[SDL_SCANCODE_I]) {
        static_cast<Player*>(player)->jumpattack();
      }
      draw();
      update(ticks);
    }
  }
  return false;
}
