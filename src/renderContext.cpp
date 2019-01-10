#include "renderContext.h"
#include "gameData.h"

RenderContext::RenderContext() : window(nullptr), renderer(nullptr), factory(ImageFactory::getInstance()) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw(std::string("Could not init SDL: ") + SDL_GetError());
  }
  window = initWindow();
  renderer = initRenderer();
}

RenderContext::~RenderContext() {
  // delete ImageFactory::getInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

RenderContext& RenderContext::getInstance() {
  static RenderContext instance;
  return instance;
}

SDL_Window* RenderContext::initWindow() {
  std::string title = Gamedata::getInstance().getXmlStr("title");
  int width = Gamedata::getInstance().getXmlInt("view/width");
  int height = Gamedata::getInstance().getXmlInt("view/height");
  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    throw(std::string("Couldn't make a window: ") + SDL_GetError());
  }
  return window;
}

SDL_Renderer* RenderContext::initRenderer() {
  // To test the Clock class's ability to cap the frame rate, use:
  // SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) throw std::string("No Renderer");
  return renderer;
}
