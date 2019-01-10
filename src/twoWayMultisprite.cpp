// 9. twoWay multi-frame sprites
#include "twoWayMultisprite.h"
#include "explodingSprite.h"
#include "gameData.h"
#include "imageFactory.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

TwoWayMultiSprite::~TwoWayMultiSprite() {
  if (explosion) delete explosion;
}

void TwoWayMultiSprite::setFlip() { flip = true; }
void TwoWayMultiSprite::clearFlip() { flip = false; }

// TODO: Write the code to implement change the velocity by rand function.
Vector2f TwoWayMultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx - 50, vx + 50);
  float newvy = Gamedata::getInstance().getRandFloat(vy - 50, vy + 50);
  newvx *= []() {
    if (rand() % 2)
      return -1;
    else
      return 1;
  }();
  newvy *= []() {
    if (rand() % 2)
      return -1;
    else
      return 1;
  }();

  return Vector2f(newvx, newvy);
  // return Vector2f(vx, vy);
}

void TwoWayMultiSprite::randomizePosition() {
  float newpx = Gamedata::getInstance().getRandFloat(100, Gamedata::getInstance().getXmlInt("world/width"));
  float newpy = Gamedata::getInstance().getRandFloat(0, Gamedata::getInstance().getXmlInt("world/height"));
  setPosition(Vector2f(newpx, newpy));
  if(getVelocityX() < 0) {
    setFlip();
  }
}

TwoWayMultiSprite::TwoWayMultiSprite(const std::string& name)
    : Drawable(name,
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                                     Gamedata::getInstance().getXmlInt(name + "/startLoc/y"))),
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/speedX"),
                                     Gamedata::getInstance().getXmlInt(name + "/speedY")))),
      images(ImageFactory::getInstance().getImages(name)),
      explodeDone(false),
      explosion(nullptr),
      currentFrame(0),
      flip(false),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
      frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
      timeSinceLastFrame(0),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
      limitTop(Gamedata::getInstance().getXmlInt(name + "/limitloc/top")),
      limitBotton(Gamedata::getInstance().getXmlInt(name + "/limitloc/botton")) {
  randomizePosition();
}

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s)
    : Drawable(s),
      images(s.images),
      explodeDone(s.explodeDone),
      explosion(s.explosion),
      currentFrame(s.currentFrame),
      flip(s.flip),
      numberOfFrames(s.numberOfFrames),
      frameInterval(s.frameInterval),
      timeSinceLastFrame(s.timeSinceLastFrame),
      worldWidth(s.worldWidth),
      worldHeight(s.worldHeight),
      limitTop(s.limitTop),
      limitBotton(s.limitBotton) {}

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explodeDone = s.explodeDone;
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  flip = (s.flip);
  numberOfFrames = (s.numberOfFrames);
  frameInterval = (s.frameInterval);
  timeSinceLastFrame = (s.timeSinceLastFrame);
  worldWidth = (s.worldWidth);
  worldHeight = (s.worldHeight);
  limitTop = (s.limitTop);
  limitBotton = (s.limitBotton);
  return *this;
}

void TwoWayMultiSprite::explode() {
  if (!explosion) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void TwoWayMultiSprite::draw() const {
  if (explosion)
    explosion->draw();
  else {
    if (flip) {
      images[currentFrame]->draw(getX(), getY(), getScale(), true, 1);
    } else {
      images[currentFrame]->draw(getX(), getY(), getScale(), false, 1);
    }
  }
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  if (explodeDone) {
    explodeDone = false;
    return;
  }
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = nullptr;
      explodeDone = true;
    }
    return;
  }
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  // setPosition(getPosition() + incr);

  // if (getY() < limitTop) {
  //   setVelocityY(fabs(getVelocityY()));
  // }
  // if (getY() > limitBotton - getScaledHeight()) {
  //   setVelocityY(-fabs(getVelocityY()));
  // }

  // if (getX() < 0) {
  //   setVelocityX(fabs(getVelocityX()));
  //   flip = false;
  // }
  // if (getX() > worldWidth - getScaledWidth()) {
  //   setVelocityX(-fabs(getVelocityX()));
  //    flip = true;
  // }
  if (getY() < limitTop) {
    setVelocityY(fabs(getVelocityY()));
  }
  if (getY() > limitBotton - getScaledHeight()) {
    setVelocityY(-fabs(getVelocityY()));
  }

  if (getX() < 0) {
    setVelocityX(fabs(getVelocityX()));
    flip = false;
  }
  if (getX() > worldWidth - getScaledWidth()) {
    setVelocityX(-fabs(getVelocityX()));
    flip = true;
  }
}
