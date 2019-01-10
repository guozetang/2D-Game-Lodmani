#include <cmath>

#include "bullet.h"

void Bullet::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  setVelocityY(getVelocityY());
  distance += (hypot(getX() - pos[0], getY() - pos[1]));
  if (distance > maxDistance) tooFar = true;
}
