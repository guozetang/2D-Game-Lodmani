#ifndef _DRAWABLE__H
#define _DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "image.h"
#include "vector2f.h"

// Drawable is an Abstract Base Class (ABC) that specifies the methods
// that derived classes may or must have.
class Drawable {
 public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel)
      : name(n), position(pos), velocity(vel), scale(1.0) {}

  Drawable(const Drawable& s) : name(s.name), position(s.position), velocity(s.velocity), scale(s.scale) {}

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  virtual const SDL_Surface* getSurface() const = 0;
  virtual const Image* getImage() const = 0;
  float getScale() const { return scale; }
  void setScale(float s) { scale = s; }
  virtual int getScaledWidth() const = 0;
  virtual int getScaledHeight() const = 0;
  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n; }

  float getX() const { return position[0]; }
  float getY() const { return position[1]; }
  void setX(float x) { position[0] = x; }
  void setY(float y) { position[1] = y; }
  const Vector2f& getPosition() const { return position; }
  const Vector2f& getVelocity() const { return velocity; }
  float getVelocityX() const { return velocity[0]; }
  float getVelocityY() const { return velocity[1]; }
  float getPositionX() const { return position[0]; }
  float getPositionY() const { return position[1]; }
  void setVelocity(const Vector2f& vel) { velocity = vel; }
  void setPosition(const Vector2f& pos) { position = pos; }
  void setVelocityX(float vx) { velocity[0] = vx; }
  void setVelocityY(float vy) { velocity[1] = vy; }
  void setPositionY(float y) { position[1] = y; }

  virtual void explode() { throw std::string("I don't explode"); }
  virtual bool hasExplode() { return true; }

 private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  float scale;
};
#endif
