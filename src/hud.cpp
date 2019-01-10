#include "hud.h"
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include "gameData.h"
#include "imageFactory.h"
#include "ioMod.h"
#include "sprite.h"

static const int boundary_x = 10, boundary_y = 5;
static const int interval_x = 100, interval_y = 20;
static const SDL_Color nameColor = {255, 255, 255, 0};

HudBulletPool& HudBulletPool::getInstance() {
  static HudBulletPool instance;
  return instance;
}

HudBulletPool::HudBulletPool()
    : width(Gamedata::getInstance().getXmlInt("hudBulletPool/width")),
      height(Gamedata::getInstance().getXmlInt("hudBulletPool/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudBulletPool/position/x"),
                   Gamedata::getInstance().getXmlInt("hudBulletPool/position/y"))),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/a"))}) {}

void HudBulletPool::draw(unsigned int activebuffernum, unsigned int freebuffernum) {
  if (isVisible()) {
    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

    SDL_Rect r;
    r.x = pos[0];
    r.y = pos[1];
    r.w = width;
    r.h = height;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderDrawRect(renderer, &r);

    std::stringstream strm_bullet;
    strm_bullet << "BULLET POOL";
    IoMod::getInstance().writeText(strm_bullet.str(), r.x + width / 2 - 65, r.y + boundary_y);
    strm_bullet.clear();
    strm_bullet.str("");
    strm_bullet << "Active bullets: " << activebuffernum;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 40);
    strm_bullet.clear();
    strm_bullet.str("");
    strm_bullet << "Bullet pool: " << freebuffernum;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 65);
  }
}

HudTips& HudTips::getInstance() {
  static HudTips instance;
  return instance;
}

HudTips::HudTips()
    : width(Gamedata::getInstance().getXmlInt("hudTips/width")),
      height(Gamedata::getInstance().getXmlInt("hudTips/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudTips/position/x"),
                   Gamedata::getInstance().getXmlInt("hudTips/position/y"))),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/a"))}) {}

void HudTips::draw() {
  if (isVisible()) {
    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

    SDL_Rect r;
    r.x = pos[0];
    r.y = pos[1];
    r.w = width;
    r.h = height;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderDrawRect(renderer, &r);

    IoMod& io = IoMod::getInstance();
    std::stringstream strm_bullet;
    strm_bullet << "TIPS";
    IoMod::getInstance().writeText(strm_bullet.str(), r.x + width / 2 - 30, r.y + boundary_y);
    io.writeText("WSAD: Move", textColor, r.x + boundary_x, r.y + boundary_y + interval_y);
    io.writeText("J: Shoot", textColor, r.x + boundary_x + 1.5 * interval_x, r.y + boundary_y + interval_y);
    io.writeText("SPACE: Jump", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 2);
    io.writeText("G: God Mode", textColor, r.x + boundary_x + 1.5 * interval_x, r.y + boundary_y + interval_y * 2);
    io.writeText("M: Open Menu", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 3);
    io.writeText("R: Restart", textColor, r.x + boundary_x + 1.5 * interval_x, r.y + boundary_y + interval_y * 3);
    io.writeText("P: Pause", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 4);
    io.writeText("F1: Hide", textColor, r.x + boundary_x + 1.5 * interval_x, r.y + boundary_y + interval_y * 4);
    io.writeText("C: Change Bullet Diretion", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 5);
  }
}

HudPlayerInfo& HudPlayerInfo::getInstance() {
  static HudPlayerInfo instance;
  return instance;
}

HudPlayerInfo::HudPlayerInfo()
    : playerPhoto(ImageFactory::getInstance().getImage("hudPlayerInfo/playerPhoto")),
      healthFull(ImageFactory::getInstance().getImage("hudPlayerInfo/healthFull")),
      healthEmpty(ImageFactory::getInstance().getImage("hudPlayerInfo/healthEmpty")),
      width(Gamedata::getInstance().getXmlInt("hudPlayerInfo/width")),
      height(Gamedata::getInstance().getXmlInt("hudPlayerInfo/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudPlayerInfo/position/x"),
                   Gamedata::getInstance().getXmlInt("hudPlayerInfo/position/y"))),
      healthPos(Vector2f(Gamedata::getInstance().getXmlInt("hudPlayerInfo/healthposition/x"),
                         Gamedata::getInstance().getXmlInt("hudPlayerInfo/healthposition/y"))),
      interval_health(Gamedata::getInstance().getXmlInt("hudPlayerInfo/healthposition/interval")),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudPlayerInfo/textColor/a"))}) {}

// HudPlayerInfo::~HudPlayerInfo() {
//   if (playerphoto) delete playerphoto;
// }

void HudPlayerInfo::draw(unsigned int liveleft) {
  SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
  SDL_Rect r;
  r.x = pos[0];
  r.y = pos[1];
  r.w = width;
  r.h = height;
  if (isVisible()) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderDrawRect(renderer, &r);

    std::stringstream strm2;
    strm2.str("");
    strm2 << "Press F1 to display/hide HUD";
    IoMod::getInstance().writeText(strm2.str(), nameColor, r.x + boundary_x - 10, r.y + boundary_y + 70);
    strm2.clear();
    strm2.str("");
    strm2 << liveleft << " Life Remianing";
    IoMod::getInstance().writeText(strm2.str(), nameColor, r.x + boundary_x - 10, r.y + boundary_y + 90);
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
  SDL_RenderFillRect(renderer, &r);
  SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
  SDL_RenderDrawRect(renderer, &r);

  playerPhoto->draw(0, 0, r.x, r.y);
  for (unsigned int i = 0; i < liveleft; i++) {
    healthFull->draw(0, 0, healthPos[0] + healthFull->getWidth() * i + interval_health * i, healthPos[1]);
  }
}

HudGameOver& HudGameOver::getInstance() {
  static HudGameOver instance;
  return instance;
}

HudGameOver::HudGameOver()
    : gamelose(ImageFactory::getInstance().getImage("gameOver/gamelose")),
      gamewin(ImageFactory::getInstance().getImage("gameOver/gamewin")),
      width(Gamedata::getInstance().getXmlInt("gameOver/width")),
      height(Gamedata::getInstance().getXmlInt("gameOver/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("gameOver/position/x"),
                   Gamedata::getInstance().getXmlInt("gameOver/position/y"))),
      visible(false),
      winBackgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/r")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/g")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/b")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/a"))}),
      winOutlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/outlineColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/outlineColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/outlineColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/outlineColor/a"))}),
      winTextColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/a"))}),
      loseBackgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/r")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/g")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/b")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/a"))}),
      loseOutlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/outlineColor/r")),
                        static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/outlineColor/g")),
                        static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/outlineColor/b")),
                        static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/outlineColor/a"))}),
      loseTextColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/r")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/g")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/b")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/a"))}) {}

void HudGameOver::draw(bool won) {
  if (visible) {
    // SDL_Rect outer;
    // outer.x = pos[0];
    // outer.y = pos[1];
    // outer.w = width;
    // outer.h = height;
    // SDL_Rect r;
    // r.x = pos[0];
    // r.y = pos[1];
    // r.w = width;
    // r.h = height;

    // SDL_Rect inner;
    // inner.x = pos[0] + 5;
    // inner.y = pos[1] + 5;
    // inner.w = width - 10;
    // inner.h = height - 10;

    // SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // std::string textToWrite;
    // bool display_picture = t:

    if (won) {
      gamewin->draw(0, 0, 0, 0);
      // SDL_SetRenderDrawColor(renderer, winOutlineColor.r, winOutlineColor.g, winOutlineColor.b, winOutlineColor.a);
      // SDL_RenderFillRect(renderer, &outer);
      // SDL_SetRenderDrawColor(renderer, winBackgroundColor.r, winBackgroundColor.g, winBackgroundColor.b,
      //                        winBackgroundColor.a);
      // SDL_RenderFillRect(renderer, &inner);
      // std::stringstream strm2;
      // strm2.str("");
      // strm2 << "YOU WIN!  Press R to play again!";
      // IoMod::getInstance().writeText(strm2.str(), winTextColor, r.x + boundary_x, r.y + 4 * boundary_y);
    } else {
      gamelose->draw(0, 0, 0, 0);
      // SDL_SetRenderDrawColor(renderer, loseOutlineColor.r, loseOutlineColor.g, loseOutlineColor.b, loseOutlineColor.a);
      // SDL_RenderFillRect(renderer, &outer);
      // SDL_SetRenderDrawColor(renderer, loseBackgroundColor.r, loseBackgroundColor.g, loseBackgroundColor.b,
      //                        loseBackgroundColor.a);
      // SDL_RenderFillRect(renderer, &inner);
      // std::stringstream strm2;
      // strm2.str("");
      // strm2 << "YOU Loses! Press R to play again!";
      // IoMod::getInstance().writeText(strm2.str(), loseTextColor, r.x + boundary_x, r.y + 4 * boundary_y);
    }
  }
}