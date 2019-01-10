#ifndef _SPRITE__H
#define _SPRITE__H
#include <string>
#include "drawable.h"

class Sprite : public Drawable {
 public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel, const Image*);
  Sprite(const Sprite&);
  virtual ~Sprite() {}
  Sprite& operator=(const Sprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { return image->getSurface(); }

  int getScaledWidth() const { return getScale() * image->getWidth(); }
  int getScaledHeight() const { return getScale() * image->getHeight(); }

 private:
  const Image* image;

 protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Sprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
