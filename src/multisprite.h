#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <cmath>
#include <string>
#include <vector>
#include "drawable.h"

class MultiSprite : public Drawable {
 public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return images[currentFrame]; }
  int getScaledWidth() const { return getScale() * images[currentFrame]->getWidth(); }
  int getScaledHeight() const { return getScale() * images[currentFrame]->getHeight(); }

  virtual const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }

 protected:
  std::vector<Image*> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
};
#endif
