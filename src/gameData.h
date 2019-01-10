#ifndef _GAMEDATA__H
#define _GAMEDATA__H

#include <map>
#include <string>
#include "parseXML.h"
#include "vector2f.h"

class Gamedata {
 public:
  static Gamedata& getInstance();
  ~Gamedata();
  void displayData() const;

  bool getXmlBool(const std::string&) const;
  const std::string& getXmlStr(const std::string&) const;
  float getXmlFloat(const std::string&) const;
  int getXmlInt(const std::string&) const;
  float getRandInRange(int min, int max) const;
  float getRandFloat(float min, float max) const;
  bool checkTag(const std::string&) const;

 private:
  ParseXML parser;
  const map<std::string, std::string> gameData;

  Gamedata(const std::string& fn = "xmlSpec/game.xml");
  Gamedata(const Gamedata&);
  Gamedata& operator=(const Gamedata&);
};

#endif
