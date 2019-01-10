#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

MultiSprite::MultiSprite(const std::string& name)
    : Drawable(name,
               Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                        Gamedata::getInstance().getXmlInt(name + "/startLoc/y")),
               Vector2f(Gamedata::getInstance().getXmlInt(name + "/speedX"),
                        Gamedata::getInstance().getXmlInt(name + "/speedY"))),
      images(ImageFactory::getInstance().getImages(name)),

      currentFrame(0),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
      frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
      timeSinceLastFrame(0),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")) {}

MultiSprite::MultiSprite(const MultiSprite& s)
    : Drawable(s),
      images(s.images),
      currentFrame(s.currentFrame),
      numberOfFrames(s.numberOfFrames),
      frameInterval(s.frameInterval),
      timeSinceLastFrame(s.timeSinceLastFrame),
      worldWidth(s.worldWidth),
      worldHeight(s.worldHeight) {}

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = (s.numberOfFrames);
  frameInterval = (s.frameInterval);
  timeSinceLastFrame = (s.timeSinceLastFrame);
  worldWidth = (s.worldWidth);
  worldHeight = (s.worldHeight);
  return *this;
}

void MultiSprite::draw() const { images[currentFrame]->draw(getX(), getY(), getScale()); }

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (getY() < 0) {
    setVelocityY(fabs(getVelocityY()));
  }
  if (getY() > worldHeight - getScaledHeight()) {
    setVelocityY(-fabs(getVelocityY()));
  }

  if (getX() < 0) {
    setVelocityX(fabs(getVelocityX()));
  }
  if (getX() > worldWidth - getScaledWidth()) {
    setVelocityX(-fabs(getVelocityX()));
  }
}
