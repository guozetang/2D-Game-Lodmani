#include "viewport.h"
#include <sstream>
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport()
    : gdata(Gamedata::getInstance()),
      viewPos(0, 0),
      msgPos(Vector2f(gdata.getXmlInt("view/loc/x"), gdata.getXmlInt("view/loc/y"))),
      worldWidth(gdata.getXmlInt("world/width")),
      worldHeight(gdata.getXmlInt("world/height")),
      viewWidth(gdata.getXmlInt("view/width")),
      viewHeight(gdata.getXmlInt("view/height")),
      objWidth(0),
      objHeight(0),
      objectToTrack(NULL) {}

void Viewport::setObjectToTrack(const Drawable* obj) {
  objectToTrack = obj;
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw(bool godMode, int fps, int enemy_num, bool final_flag) const {
  // IoMod::getInstance().writeText("Tracking: " + objectToTrack->getName(), msgPos[0], msgPos[1]);
  std::stringstream strm_fps;
  SDL_Color fpsColor = {2, 113, 192, 0};
  strm_fps << "fps: " << fps;
  IoMod::getInstance().writeText(strm_fps.str(), fpsColor, gdata.getXmlInt("text/fps/x"),
                                 gdata.getXmlInt("text/fps/y"));

  IoMod::getInstance().writeText((godMode ? "GOD MODE: ENABLED" : "GOD MODE: DISBALED"),
                                 gdata.getXmlInt("text/godMode/x"), gdata.getXmlInt("text/godMode/y"));

  SDL_Color nameColor = {255, 255, 255, 0};
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("authorname"), nameColor,
                                 gdata.getXmlInt("text/authorname/x"), gdata.getXmlInt("text/authorname/y"));
  if (final_flag) {
    IoMod::getInstance().writeText("BOSS REMAINING: " + std::to_string(1), nameColor,
                                   gdata.getXmlInt("text/enemyRemaining/x")+20, gdata.getXmlInt("text/enemyRemaining/y"));
  } else {
    IoMod::getInstance().writeText("ENEMY REMAINING: " + std::to_string(enemy_num), nameColor,
                                   gdata.getXmlInt("text/enemyRemaining/x"), gdata.getXmlInt("text/enemyRemaining/y"));
  }
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  viewPos[0] = (x + objWidth / 2) - viewWidth / 2;
  viewPos[1] = (y + objHeight / 2) - viewHeight / 2;
  if (viewPos[0] < 0) viewPos[0] = 0;
  if (viewPos[1] < 0) viewPos[1] = 0;
  if (viewPos[0] > (worldWidth - viewWidth)) {
    viewPos[0] = worldWidth - viewWidth;
  }
  if (viewPos[1] > (worldHeight - viewHeight)) {
    viewPos[1] = worldHeight - viewHeight;
  }
}
