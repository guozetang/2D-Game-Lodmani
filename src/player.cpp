#include "player.h"
#include "clock.h"
#include "explodingSprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "smartSprite.h"

static const std::string player_actions[] = {"idle",       "run",     "walk",    "jump",
                                             "jumpattack", "attack1", "attack2", "attack3"};

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

Player::~Player() {
  if (explosion) delete explosion;
}

void Player::setFlip() { flip = true; }
void Player::clearFlip() { flip = false; }

// TODO: Write the code to implement change the velocity by rand function.
Vector2f Player::makeVelocity(int vx, int vy) const { return Vector2f(vx, vy); }

Player::Player(const std::string& name)
    : Drawable(name,
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                                     Gamedata::getInstance().getXmlInt(name + "/startLoc/y"))),
               Vector2f(makeVelocity(Gamedata::getInstance().getXmlInt(name + "/speed/x"),
                                     Gamedata::getInstance().getXmlInt(name + "/speed/y")))),
      multiImages(),
      observers(),
      currentFrame(0),
      images(ImageFactory::getInstance().getImages(name, "idle")),
      initNumLives(Gamedata::getInstance().getXmlInt("numOfLife")),
      livesLeft(Gamedata::getInstance().getXmlInt("numOfLife")),
      hp(Gamedata::getInstance().getXmlInt("playerHp")),
      initNumHp(Gamedata::getInstance().getXmlInt("playerHp")),
      godMode(true),
      collision(false),
      colliding(false),
      explodeDone(false),
      bullet_direction(false),
      explosionStartTime(-1),
      explosion(nullptr),
      bulletName(Gamedata::getInstance().getXmlStr(name + "/bullet1")),
      bullets(bulletName),
      minSpeed(Gamedata::getInstance().getXmlInt(bulletName + "/speedX")),
      flip(false),
      numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/file/run/frames")),
      frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
      timeSinceLastFrame(0),
      initialVelocity(getVelocity()),
      worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
      worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
      limitTop(Gamedata::getInstance().getXmlInt(name + "/limitloc/top")),
      limitBotton(Gamedata::getInstance().getXmlInt(name + "/limitloc/botton")),
      slip(Gamedata::getInstance().getXmlFloat("slip")) {
  int action_number = sizeof(player_actions) / sizeof(player_actions[0]);
  for (int i = 0; i < action_number; ++i) {
    std::vector<Image*> temp_images = ImageFactory::getInstance().getImages(name, player_actions[i]);
    multiImages[player_actions[i]] = temp_images;
  }
  setScale(0.5);
}

Player::Player(const Player& s)
    : Drawable(s),
      multiImages(s.multiImages),
      observers(s.observers),
      currentFrame(s.currentFrame),
      images(s.images),
      initNumLives(s.initNumLives),
      livesLeft(s.livesLeft),
      hp(s.hp),
      initNumHp(s.initNumHp),
      godMode(s.godMode),
      collision(s.collision),
      colliding(s.colliding),
      explodeDone(s.explodeDone),
      bullet_direction(s.bullet_direction),
      explosionStartTime(s.explosionStartTime),
      explosion(s.explosion),
      bulletName(s.bulletName),
      bullets(s.bulletName),
      minSpeed(s.minSpeed),
      flip(s.flip),
      numberOfFrames(s.numberOfFrames),
      frameInterval(s.frameInterval),
      timeSinceLastFrame(s.timeSinceLastFrame),
      initialVelocity(s.initialVelocity),
      worldWidth(s.worldWidth),
      worldHeight(s.worldHeight),
      limitTop(s.limitTop),
      limitBotton(s.limitBotton),
      slip(s.slip) {}

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  multiImages = s.multiImages;
  observers = s.observers;
  currentFrame = s.currentFrame;
  images = s.images;
  initNumLives = s.initNumLives;
  livesLeft = s.livesLeft;
  hp = s.hp;
  initNumHp = s.initNumHp;
  godMode = s.godMode;
  collision = s.collision;
  colliding = s.colliding;
  explodeDone = s.explodeDone;
  bullet_direction = s.bullet_direction, explosionStartTime = s.explosionStartTime;
  explosion = s.explosion;
  bulletName = s.bulletName;
  bullets = s.bullets;
  minSpeed = s.minSpeed;
  flip = s.flip;
  numberOfFrames = s.numberOfFrames;
  frameInterval = s.frameInterval;
  timeSinceLastFrame = s.timeSinceLastFrame;
  initialVelocity = s.initialVelocity;
  worldWidth = s.worldWidth;
  worldHeight = s.worldHeight;
  limitTop = s.limitTop;
  limitBotton = s.limitBotton;
  slip = s.slip;
  return *this;
}

// void Player::shoot() {
//   if (!getFlip()) {
//     float x = getX() + getScaledWidth();
//     float y = getY() + getScaledHeight() / 3;
//     bullets.shoot(Vector2f(x, y), Vector2f(minSpeed + getVelocityX(), 0));
//   } else {
//     float x = getX() - getScaledWidth() / 5;
//     float y = getY() + getScaledHeight() / 3;
//     bullets.shoot(Vector2f(x, y), -Vector2f(minSpeed - getVelocityX(), 0));
//   }
// }

void Player::shoot() {
  if (!getFlip()) {
    float x = getX() + getScaledWidth();
    float y = getY() + getScaledHeight() / 3;
    if (bullet_direction) {
      bullets.shoot(Vector2f(x, y), Vector2f(minSpeed + getVelocityX(), -minSpeed - getVelocityY()));
    } else {
      bullets.shoot(Vector2f(x, y), Vector2f(minSpeed + getVelocityX(), 0));
    }

  } else {
    float x = getX() - getScaledWidth() / 5;
    float y = getY() + getScaledHeight() / 3;
    // bullets.shoot(Vector2f(x, y), -Vector2f(minSpeed - getVelocityX(), 0));
    if (bullet_direction) {
      bullets.shoot(Vector2f(x, y), -Vector2f(minSpeed - getVelocityX(), minSpeed - getVelocityY()));
    } else {
      bullets.shoot(Vector2f(x, y), -Vector2f(minSpeed - getVelocityX(), 0));
    }
  }
}

void Player::changeBullet() {
  std::string bName = Gamedata::getInstance().getXmlStr(getName() + "/bullet2");
  bullets.changeBullet(bName);
}

void Player::explode() {
  if (!godMode) {
    if (!explosion) {
      Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
      sprite.setScale(getScale());
      explosion = new ExplodingSprite(sprite);
      colliding = true;
      explosionStartTime = Clock::getInstance().getSeconds();
    }
  }
}

void Player::draw() const {
  if (explosion)
    explosion->draw();
  else {
    if (flip) {
      images[currentFrame]->draw(getX(), getY(), getScale(), true, 1);
    } else {
      images[currentFrame]->draw(getX(), getY(), getScale(), false, 1);
    }
    bullets.draw();
  }
}

void Player::changeImages(const std::string name) {
  std::map<std::string, std::vector<Image*>>::const_iterator pos = multiImages.find(name);
  if (pos != multiImages.end()) {
    images = pos->second;
  }
}

void Player::stop() {
  setVelocityX(getVelocityX() * slip);
  setVelocityY(0);
}

void Player::right() {
  changeImages("run");
  if (getX() < worldWidth - getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    clearFlip();
  }
}

void Player::left() {
  changeImages("run");
  if (getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    setFlip();
  }
}

void Player::up() {
  changeImages("run");
  if (getY() > limitTop) {
    setVelocityY(-initialVelocity[1]);
  }
}

void Player::jumpattack() { changeImages("jumpattack"); }
void Player::attack1() { changeImages("attack1"); }
void Player::attack2() { changeImages("attack2"); }
void Player::attack3() { changeImages("attack3"); }

void Player::jump() { changeImages("jump"); }

void Player::down() {
  changeImages("run");
  if (getY() < limitBotton - getScaledHeight()) {
    setVelocityY(initialVelocity[1]);
  }
}

void Player::detach(SmartSprite* o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    if (*ptr == o) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::update(Uint32 ticks) {
  if (explodeDone) {
    explodeDone = false;
    return;
  }
  if (explosion && colliding) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      if ((Clock::getInstance().getSeconds() - explosionStartTime) >= 1) {
          delete explosion;
          colliding = false;
          explodeDone = true;
          explosion = nullptr;
        }
    }
    return;

  } else {
    if (livesLeft <= 0) {
      setPositionY(-500);
      return;
    }

    advanceFrame(ticks);
    bullets.update(ticks);

    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

    if (getY() < limitTop) {
      setVelocityY(fabs(getVelocityY()));
    }
    if (getY() > limitBotton - getScaledHeight()) {
      setVelocityY(-fabs(getVelocityY()));
    }

    if (getX() < 0) {
      setVelocityX(fabs(getVelocityX()));
    }
    if (getX() > worldWidth - getScaledWidth()) {
      setVelocityX(-fabs(getVelocityX()));
    }
  }
  notify();
  stop();
}

void Player::notify() {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    (*ptr)->setPlayerPos(getPosition());
    ++ptr;
  }
  stop();
}
