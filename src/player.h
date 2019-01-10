#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <cmath>
#include <list>
#include <string>
#include <vector>
#include "bulletPool.h"
#include "drawable.h"

class SmartSprite;
class ExplodingSprite;

class Player : public Drawable {
 public:
  Player(const std::string&);
  Player(const Player&);
  Player& operator=(const Player&);
  virtual ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return images[currentFrame]; }
  int getScaledWidth() const { return getScale() * images[currentFrame]->getWidth(); }
  int getScaledHeight() const { return getScale() * images[currentFrame]->getHeight(); }
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  Vector2f makeVelocity(int vx, int vy) const;

  void right();
  void left();
  void up();
  void down();
  void jump();
  void stop();
  void attack1();
  void attack2();
  void attack3();
  void jumpattack();
  void shoot();
  void explode();

  void setFlip();
  void clearFlip();

  void attach(SmartSprite* o) { observers.emplace_back(o); }
  void detach(SmartSprite* o);
  void notify();

  bool collided(const Drawable* obj) const { return bullets.collidedWith(obj); }
  virtual bool hasExplode() { return explodeDone; }
  virtual void seteExplodeDone(bool e) { explodeDone = e; }
  bool isColliding() const { return colliding; }

  void changeBullet();
  void resetBullet() { bullets.resetBullet(); }

  unsigned int getActiveBulletCount() { return bullets.bulletCount(); }
  unsigned int getFreeBulletCount() { return bullets.freeCount(); }

  void changeImages(const std::string name);

  bool getFlip() const { return flip; }
  unsigned int getLastExplosionTime() const { return explosionStartTime; }
  unsigned int getLivesLeft() const { return livesLeft; }
  void loseLife() { --livesLeft; }
  void resetHp() {  hp = initNumHp; }
  unsigned int getHpLeft() const { return hp; }
  unsigned int getInitNumHp() const { return initNumHp; }
  void loseHp() { --hp; }
  bool isGodMode() const { return godMode; }
  void toggleGodMode() { godMode = !godMode; }
  void setBulletDirection() { bullet_direction = !bullet_direction; }
  unsigned int getInitNumLives() const { return initNumLives; }
  void collide();

 private:
  std::map<std::string, std::vector<Image*>> multiImages;
  std::list<SmartSprite*> observers;
  unsigned currentFrame;
  std::vector<Image*> images;
  unsigned int initNumLives;
  unsigned int livesLeft;
  unsigned int hp;
  unsigned int initNumHp;
  bool godMode;
  bool collision;
  bool colliding;
  bool explodeDone;
  bool bullet_direction;
  unsigned int explosionStartTime;
  ExplodingSprite* explosion;
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;

  bool flip;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  Vector2f initialVelocity;
  int worldWidth;
  int worldHeight;
  int limitTop;
  int limitBotton;
  float slip;
  void advanceFrame(Uint32 ticks);
};
#endif
