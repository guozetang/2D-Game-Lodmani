#include "bulletPool.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include "gameData.h"
#include "ioMod.h"

CollisionStrategy* BulletPool::getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name + "/strategy");
  if (sName == "midpoint") return new MidPointCollisionStrategy;
  if (sName == "rectangular") return new RectangularCollisionStrategy;
  if (sName == "perpixel") return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}

BulletPool::~BulletPool() { delete strategy; }

BulletPool::BulletPool(const std::string& n)
    : name(n),
      change(false),
      strategy(getStrategy(name)),
      frameInterval(Gamedata::getInstance().getXmlInt(name + "/interval")),
      timeSinceLastFrame(0),
      bulletList(),
      freeList() {}

BulletPool::BulletPool(const BulletPool& b)
    : name(b.name),
      change(b.change),
      strategy(b.strategy),
      frameInterval(b.frameInterval),
      timeSinceLastFrame(b.timeSinceLastFrame),
      bulletList(b.bulletList),
      freeList(b.freeList) {}

BulletPool& BulletPool::operator=(const BulletPool& b) {
  name = b.name;
  change = b.change;
  strategy = b.strategy;
  frameInterval = b.frameInterval;
  timeSinceLastFrame = b.timeSinceLastFrame;
  bulletList = b.bulletList;
  freeList = b.freeList;
  return *this;
}

bool BulletPool::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if (strategy->execute(&(*ptr), obj)) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPool::changeBullet(std::string n) {
  name = n;
  change = true;
}

void BulletPool::shoot(const Vector2f& position, const Vector2f& velocity) {
  if (timeSinceLastFrame > frameInterval) {
    if (freeList.empty()) {
      Bullet b(name);
      b.setPosition(position);
      b.setVelocity(velocity);
      bulletList.push_back(b);
    } else {
      if (change) {
        freeList.clear();
        change = false;
      } else {
        Bullet b = freeList.front();
        freeList.pop_front();
        b.reset();
        b.setVelocity(velocity);
        b.setPosition(position);
        bulletList.push_back(b);
      }
    }
    timeSinceLastFrame = 0;
  }
}

void BulletPool::draw() const {
  for (Bullet bullet : bulletList) {
    bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    } else
      ++ptr;
  }
}
