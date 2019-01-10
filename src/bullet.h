#ifndef _BULLET__H
#define _BULLET__H
#include "gameData.h"
#include "sprite.h"

class Bullet : public Sprite {
 public:
  explicit Bullet(const string& name)
      : Sprite(name), distance(0), maxDistance(Gamedata::getInstance().getXmlInt(name + "/distance")), tooFar(false) {}
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

 private:
  float distance;
  float maxDistance;
  bool tooFar;
};

#endif
