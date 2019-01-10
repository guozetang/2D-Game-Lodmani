#include "imageFactory.h"
#include "ioMod.h"
#include "renderContext.h"
#include "vector2f.h"

ImageFactory& ImageFactory::getInstance() {
  static ImageFactory instance;
  return instance;
}

ImageFactory::~ImageFactory() {
  std::cout << "Deleting images in Factory" << std::endl;
  std::map<std::string, SDL_Surface*>::const_iterator si = surfaces.begin();
  std::map<std::string, SDL_Texture*>::const_iterator ti = textures.begin();
  std::map<std::string, Image*>::const_iterator fi = images.begin();

  while (si != surfaces.end()) {
    SDL_FreeSurface(si->second);
    ++si;
  }
  while (ti != textures.end()) {
    SDL_DestroyTexture(ti->second);
    ++ti;
  }
  while (fi != images.end()) {
    std::cout << "deleting " << fi->first << std::endl;
    delete fi->second;
    ++fi;
  }

  for (auto& surfaces : multiSurfaces) {
    for (unsigned int i = 0; i < surfaces.second.size(); ++i) {
      SDL_FreeSurface(surfaces.second[i]);
    }
  }
  for (auto& textures : multiTextures) {
    for (unsigned int i = 0; i < textures.second.size(); ++i) {
      SDL_DestroyTexture(textures.second[i]);
    }
  }

  for (auto& images : multiImages) {
    std::cout << "deleting " << images.first << std::endl;
    for (unsigned int i = 0; i < images.second.size(); ++i) {
      delete images.second[i];
    }
  }
}

Image* ImageFactory::getImage(const std::string& name) {
  std::map<std::string, Image*>::const_iterator it = images.find(name);
  if (it == images.end()) {
    SDL_Surface* const surface = IoMod::getInstance().readSurface(gdata.getXmlStr(name + "/file"));
    bool transparency = gdata.getXmlBool(name + "/transparency");
    if (transparency) {
      int keyColor = SDL_MapRGBA(surface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    surfaces[name] = surface;
    Image* const image = new Image(surface);
    images[name] = image;
    return image;
  } else {
    return it->second;
  }
}

std::vector<Image*> ImageFactory::getImages(const std::string& name) {
  std::map<std::string, std::vector<Image*> >::const_iterator pos = multiImages.find(name);
  if (pos != multiImages.end()) {
    return pos->second;
  }

  IoMod& iomod = IoMod::getInstance();
  RenderContext& renderContext = RenderContext::getInstance();
  std::string sheetPath = gdata.getXmlStr(name + "/file");
  SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
  bool transparency = gdata.getXmlBool(name + "/transparency");

  // It wasn't in the map, so we have to make the vector of Images:
  unsigned numberOfFrames = gdata.getXmlInt(name + "/frames");
  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(numberOfFrames);
  surfaces.reserve(numberOfFrames);
  textures.reserve(numberOfFrames);

  int width = spriteSurface->w / numberOfFrames;
  int height = spriteSurface->h;

  if (gdata.checkTag(name + "/imageWidth") && gdata.checkTag(name + "/imageHeight")) {
    width = gdata.getXmlInt(name + "/imageWidth");
    height = gdata.getXmlInt(name + "/imageHeight");
  }

  SpriteSheet sheet(spriteSurface, width, height);

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    SDL_Surface* surface = sheet[i];
    if (transparency) {
      int keyColor = SDL_MapRGBA(spriteSurface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderContext.getRenderer(), surface);
    surfaces.push_back(surface);
    textures.push_back(texture);
    images.push_back(new Image(surface));
  }
  multiSurfaces[name] = surfaces;
  multiTextures[name] = textures;
  multiImages[name] = images;
  return images;
}

std::vector<Image*> ImageFactory::getImages(const std::string& name, const std::string& suffix) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Image*> >::const_iterator pos = multiImages.find(name + suffix);
  if (pos != multiImages.end()) {
    return pos->second;
  }

  IoMod& iomod = IoMod::getInstance();
  RenderContext& renderContext = RenderContext::getInstance();
  std::string sheetPath = gdata.getXmlStr(name + "/file/" + suffix + "/file");
  SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
  bool transparency = gdata.getXmlBool(name + "/transparency");
  // It wasn't in the map, so we have to make the vector of Images:
  unsigned numberOfFrames = gdata.getXmlInt(name + "/file/" + suffix + "/frames");
  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(numberOfFrames);
  surfaces.reserve(numberOfFrames);
  textures.reserve(numberOfFrames);

  int width = spriteSurface->w / numberOfFrames;
  int height = spriteSurface->h;

  if (gdata.checkTag(name + "/imageWidth") && gdata.checkTag(name + "/imageHeight")) {
    width = gdata.getXmlInt(name + "/imageWidth");
    height = gdata.getXmlInt(name + "/imageHeight");
  }

  SpriteSheet sheet(spriteSurface, width, height);

  for (unsigned i = 0; i < numberOfFrames; ++i) {
    SDL_Surface* surface = sheet[i];
    if (transparency) {
      int keyColor = SDL_MapRGBA(spriteSurface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderContext.getRenderer(), surface);
    surfaces.push_back(surface);
    textures.push_back(texture);
    images.push_back(new Image(surface));
  }
  multiSurfaces[name + suffix] = surfaces;
  multiTextures[name + suffix] = textures;
  multiImages[name + suffix] = images;
  return images;
}
