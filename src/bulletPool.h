#ifndef _BULLETPOOL__H
#define _BULLETPOOL__H

#include <list>
#include "bullet.h"
#include "collisionStrategy.h"

class BulletPool {
 public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  BulletPool& operator=(const BulletPool&);
  ~BulletPool();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& vel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool shooting() const { return bulletList.empty(); }
  bool collidedWith(const Drawable* obj) const;
  CollisionStrategy* getStrategy(const string& name);
  void changeBullet(std::string n);
  void resetBullet() {
    name = "bullet1";
    change = true;
  };

 private:
  std::string name;
  bool change;
  CollisionStrategy* strategy;
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<Bullet> bulletList;
  mutable std::list<Bullet> freeList;
};

#endif
