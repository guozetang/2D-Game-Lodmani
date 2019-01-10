// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

int main(int, char* []) {
  bool keepPlaying = true;
  try {
    while (keepPlaying) {
      Engine* engine = new Engine;
      keepPlaying = engine->play();
      delete engine;
      HudPlayerInfo::getInstance().setVisible(true);
      HudBulletPool::getInstance().setVisible(true);
      HudTips::getInstance().setVisible(true);
    }
  } catch (const string& msg) {
    std::cout << msg << std::endl;
  } catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }
  return 0;
}
