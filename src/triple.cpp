#include <cmath>
#include <sstream>
#include <string>

#include "triple.h"

double Triple::len() const { return sqrt(x * x + y * y + z * z); }

Triple Triple::scale(double s) const { return Triple(x * s, y * s, z * s); }

Triple Triple::normalized() const { return scale(1.0 / len()); }

Triple Triple::sum(const Triple& a) const { return Triple(x + a.x, y + a.y, z + a.z); }

Triple Triple::diff(const Triple& a) const { return Triple(x - a.x, y - a.y, z - a.z); }

double Triple::dot(const Triple& a) const { return x * a.x + y * a.y + z * a.z; }

Triple Triple::cross(const Triple& a) const {
  double nx = y * a.z - a.y * z;
  double ny = -(x * a.z - a.x * z);
  double nz = x * a.y - a.x * y;
  return Triple(nx, ny, nz);
}

double Triple::operator[](int index) const {
  double ret;
  switch (index) {
    case 0:
      ret = x;
      break;
    case 1:
      ret = y;
      break;
    default:
      ret = z;
      break;
  }
  return ret;
}

std::string Triple::toString() const {
  std::stringstream strm;
  strm << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  return strm.str();
}

void Triple::boundingRect(const std::vector<Triple>& pts, double& x, double& y, double& wid, double& hgt) {
  // try and protect us from segfaults at least
  if (pts.size() == 0) return;

  Triple fPoint = pts[0];
  double minX = fPoint[0], maxX = fPoint[0];
  double minY = fPoint[1], maxY = fPoint[1];

  for (unsigned i = 1; i < pts.size(); i++) {
    fPoint = pts[i];
    double x = fPoint[0], y = fPoint[1];
    minX = x < minX ? x : minX;
    maxX = x > maxX ? x : maxX;

    minY = y < minY ? y : minY;
    maxY = y > maxY ? y : maxY;
  }

  x = minX;
  y = minY;

  wid = maxX - minX;
  hgt = maxY - minY;
}
