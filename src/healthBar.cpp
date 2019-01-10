#include "healthBar.h"
#include <sstream>
#include "renderContext.h"

HealthBar& HealthBar::getInstance() {
  static HealthBar instance;
  return instance;
}

HealthBar::HealthBar()
    : renderer(RenderContext::getInstance().getRenderer()),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("healthBar/position/x"),
                   Gamedata::getInstance().getXmlInt("healthBar/position/y"))),
      backBarWidth(Gamedata::getInstance().getXmlInt("healthBar/width")),
      backBarHeight(Gamedata::getInstance().getXmlInt("healthBar/height")),
      barColor({(Uint8)Gamedata::getInstance().getXmlInt("healthBar/barColor/r"),
                (Uint8)Gamedata::getInstance().getXmlInt("healthBar/barColor/g"),
                (Uint8)Gamedata::getInstance().getXmlInt("healthBar/barColor/b"),
                (Uint8)Gamedata::getInstance().getXmlInt("healthBar/barColor/a")}),
      backColor({(Uint8)Gamedata::getInstance().getXmlInt("healthBar/backColor/r"),
                 (Uint8)Gamedata::getInstance().getXmlInt("healthBar/backColor/g"),
                 (Uint8)Gamedata::getInstance().getXmlInt("healthBar/backColor/b"),
                 (Uint8)Gamedata::getInstance().getXmlInt("healthBar/backColor/a")}),
      currentLength(1.0) {}

// void HealthBar::draw(int playerX, int playerY, int playerWidth, int playerHeight) const {
//   int viewWidth = Gamedata::getInstance().getXmlInt("view/width");
//   int bgWidth = Gamedata::getInstance().getXmlInt("world/width");

//   if ((playerX + (playerWidth / 2)) > viewWidth / 2) {
//     if (!(bgWidth - (playerX + (playerWidth / 2)) < viewWidth / 2)) {
//       playerX = (viewWidth / 2) - (playerWidth / 2);
//     } else {
//       playerX = (viewWidth / 2) + (playerX - (1600));
//     }
//   }

//   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

//   SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
//   SDL_Rect backBarRect = {playerX + 20, playerY - 16, backBarWidth, backBarHeight};
//   SDL_RenderFillRect(renderer, &backBarRect);

//   SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
//   SDL_Rect healthBarRect = {playerX + 22, playerY - 14, (int)(currentLength * backBarWidth) - 4, backBarHeight - 4};
//   SDL_RenderFillRect(renderer, &healthBarRect);
// }

void HealthBar::draw() const {
  SDL_Rect backBarRect;
  backBarRect.x = pos[0];
  backBarRect.y = pos[1];
  backBarRect.w = backBarWidth;
  backBarRect.h = backBarHeight;

  SDL_Rect healthBarRect;
  healthBarRect.x = pos[0] + 2;
  healthBarRect.y = pos[1] + 2;
  healthBarRect.w = (int)(currentLength * backBarWidth) - 4;
  healthBarRect.h = backBarHeight - 4;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  if (currentLength < 0.001) {
    SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
    SDL_RenderFillRect(renderer, &backBarRect);
  } else {
    SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
    SDL_RenderFillRect(renderer, &backBarRect);

    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_RenderFillRect(renderer, &healthBarRect);
  }
}
