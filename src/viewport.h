#ifndef _VIEWPORT__H
#define _VIEWPORT__H
#include "drawable.h"
#include "gameData.h"

class Viewport {
 public:
  static Viewport& getInstance();
  void draw(bool, int, int, bool) const;
  void update();

  Vector2f getPosition() const { return viewPos; }
  float getX() const { return viewPos[0]; }
  void setX(float x) { viewPos[0] = x; }
  float getY() const { return viewPos[1]; }
  void setY(float y) { viewPos[1] = y; }

  void setObjectToTrack(const Drawable* obj);
  const Drawable* getObjectToTrack() const { return objectToTrack; }

 private:
  const Gamedata& gdata;
  Vector2f viewPos;
  Vector2f msgPos;
  int worldWidth;
  int worldHeight;
  int viewWidth;
  int viewHeight;
  int objWidth;
  int objHeight;

  const Drawable* objectToTrack;

  Viewport();
  Viewport(const Viewport&);
  Viewport& operator=(const Viewport&);
};
#endif
