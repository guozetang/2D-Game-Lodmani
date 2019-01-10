#include "sprite.h"
#include <cmath>
#include <functional>
#include <random>
#include "gameData.h"
#include "imageFactory.h"

Vector2f Sprite::makeVelocity(int vx, int vy) const {
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
}

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel, const Image* img)
    : Drawable(n, pos, vel),
      image(img),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")) {}

Sprite::Sprite(const std::string& name)
    : Drawable(name,
               // 8. seven stars
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                                     Gamedata::getInstance().getXmlInt(name + "/startLoc/y"))),
               /*Vector2f( Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                        Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),*/
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/speedX"),
                                     Gamedata::getInstance().getXmlInt(name + "/speedY")))),
      // Change the IMageFactory For GoF singletons to Meyers singletons.
      image(ImageFactory::getInstance().getImage(name)),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")) {}

Sprite::Sprite(const Sprite& s)
    : Drawable(s),
      image(s.image),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")) {}

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=(rhs);
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace { constexpr float SCALE_EPSILON = 2e-7; }

void Sprite::draw() const {
  if (getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale());
}

void Sprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  // setPosition(getPosition());

  if (getY() < 0) {
    setVelocityY(std::abs(getVelocityY()));
  }
  if (getY() > worldHeight - getScaledHeight()) {
    setVelocityY(-std::abs(getVelocityY()));
  }

  if (getX() < 0) {
    setVelocityX(std::abs(getVelocityX()));
  }
  if (getX() > worldWidth - getScaledWidth()) {
    setVelocityX(-std::abs(getVelocityX()));
  }
}
