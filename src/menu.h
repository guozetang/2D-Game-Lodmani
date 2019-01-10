#ifndef _MENU__H
#define _MENU__H

#include <string>
#include <vector>
#include "SDL.h"
#include "clock.h"
#include "gameData.h"
#include "ioMod.h"
#include "sprite.h"

class Menu {
 public:
  Menu(SDL_Renderer*);
  void draw() const;
  void update();
  void incrIcon();
  void decrIcon();
  void lightOn() { currentClick = 1; }
  void lightOff() { currentClick = 0; }

  int getOptionNo() const { return currentOption; }
  int getNumStars() const;

  Menu(const Menu&) = delete;
  Menu& operator=(const Menu&) = delete;

 private:
  SDL_Renderer* renderer;
  Gamedata& gdata;
  SDL_Rect hudFrame;
  SDL_Color backColor;
  SDL_Color menuColor;
  Clock& clock;
  IoMod& io;
  std::vector<std::string> options;
  std::vector<int> optionLoc;
  std::vector<Sprite> clicks;
  int currentClick;
  int currentOption;
  int spaces;
  int startClickX;
  int startClickY;
  int clickX;
  int clickY;
  void drawBackground() const;
  int getInputEventLoop() const;
};
#endif
