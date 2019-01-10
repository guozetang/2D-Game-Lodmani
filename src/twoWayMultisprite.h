#ifndef _TWOWAYMULTISPRITE__H
#define _TWOWAYMULTISPRITE__H
#include <cmath>
#include <string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
 public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return images[currentFrame]; }
  int getScaledWidth() const { return getScale() * images[currentFrame]->getWidth(); }
  int getScaledHeight() const { return getScale() * images[currentFrame]->getHeight(); }
  void randomizePosition();
  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  Vector2f makeVelocity(int vx, int vy) const;

  void explode();
  virtual bool hasExplode() { return explodeDone; }

  void setFlip();
  void clearFlip();
  bool getFlip() const { return flip; }

 protected:
  std::vector<Image*> images;
  bool explodeDone;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  bool flip;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int limitTop;
  int limitBotton;

  void advanceFrame(Uint32 ticks);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
