#include "spriteHealthBar.h"
#include <sstream>
#include "renderContext.h"

SpriteHealthBar::SpriteHealthBar(const std::string& name)
    : Drawable(name,
               Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                        Gamedata::getInstance().getXmlInt(name + "/startLoc/y")),
               Vector2f(Gamedata::getInstance().getXmlInt(name + "/speedX"),
                        Gamedata::getInstance().getXmlInt(name + "/speedY"))),
      renderer(RenderContext::getInstance().getRenderer()),
      barRect({Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
               Gamedata::getInstance().getXmlInt(name + "/startLoc/y"),
               Gamedata::getInstance().getXmlInt(name + "/width"),
               Gamedata::getInstance().getXmlInt(name + "/height")}),
      clock(Clock::getInstance()),
      pos(Vector2f(Gamedata::getInstance().getXmlInt(name + "/position/x"),
                   Gamedata::getInstance().getXmlInt(name + "/position/y"))),
      backBarWidth(Gamedata::getInstance().getXmlInt(name + "/width")),
      backBarHeight(Gamedata::getInstance().getXmlInt(name + "/height")),
      currentLength(barRect.w),
      increment(barRect.w / Gamedata::getInstance().getXmlInt(name + "/increment")),
      interval(Gamedata::getInstance().getXmlInt(name + "/interval")),
      decrease(false),
      barColor({(Uint8)Gamedata::getInstance().getXmlInt(name + "/barColor/r"),
                (Uint8)Gamedata::getInstance().getXmlInt(name + "/barColor/g"),
                (Uint8)Gamedata::getInstance().getXmlInt(name + "/barColor/b"),
                (Uint8)Gamedata::getInstance().getXmlInt(name + "/barColor/a")}),
      backColor({(Uint8)Gamedata::getInstance().getXmlInt(name + "/backColor/r"),
                 (Uint8)Gamedata::getInstance().getXmlInt(name + "/backColor/g"),
                 (Uint8)Gamedata::getInstance().getXmlInt(name + "/backColor/b"),
                 (Uint8)Gamedata::getInstance().getXmlInt(name + "/backColor/a")}),
      io(IoMod::getInstance()) {}

/*void HealthBar::update(Uint32 ticks) {
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= increment;
  }
}*/
void SpriteHealthBar::update(Uint32 ticks) {
  ++ticks;
  if (decrease) {
    if (currentLength > 0) {
      currentLength -= increment;
      decrease = false;
    }
  }
}

void SpriteHealthBar::draw(int playerX, int playerY, int playerWidth, int playerHeight) const {
  int viewWidth = Gamedata::getInstance().getXmlInt("view/width");
  int bgWidth = Gamedata::getInstance().getXmlInt("world/width");

  if ((playerX + (playerWidth / 2)) > viewWidth / 2) {
    if (!(bgWidth - (playerX + (playerWidth / 2)) < viewWidth / 2)) {
      playerX = (viewWidth / 2) - (playerWidth / 2);
    } else {
      playerX = (viewWidth / 2) + (playerX - (1600));
    }
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
  SDL_Rect backBarRect = {playerX + 20, playerY + playerHeight - 10 + 15, backBarWidth, backBarHeight};
  // SDL_Rect backBarRect = {playerX + 20, playerY - 16, backBarWidth, backBarHeight};
  SDL_RenderFillRect(renderer, &backBarRect);

  SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
  SDL_Rect healthBarRect = {playerX + 22, playerY + playerHeight - 8 + 15, (int)(currentLength * backBarWidth) - 4,
                            backBarHeight - 4};
  // SDL_Rect healthBarRect = {playerX + 22, playerY - 14, (int)(currentLength * backBarWidth) - 4, backBarHeight - 4};
  SDL_RenderFillRect(renderer, &healthBarRect);
}

void SpriteHealthBar::draw() const {
  SDL_Rect healthBarRect;
  healthBarRect.x = static_cast<int>(getX());
  healthBarRect.y = static_cast<int>(getY());
  healthBarRect.w = static_cast<int>(currentLength);
  healthBarRect.h = static_cast<int>(barRect.h);
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the health bar background color:
  SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

  // Draw the  background
  SDL_RenderFillRect(renderer, &barRect);
  SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
  // int x = static_cast<int>(getX());
  // int y = static_cast<int>(getY());
  // SDL_Rect healthRect = {x, y, currentLength, static_cast<int>(barRect.h)};
  SDL_RenderFillRect(renderer, &healthBarRect);

  // Set the  color for the health bar outline:
  SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
  SDL_RenderDrawRect(renderer, &barRect);
}
