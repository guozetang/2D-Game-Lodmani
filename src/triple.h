#ifndef _TRIPLE_H_
#define _TRIPLE_H_

#include <string>
#include <vector>

class Triple {
 public:
  Triple(double xi = 0.0, double yi = 0.0, double zi = 0.0) : x(xi), y(yi), z(zi) {}
  Triple(const Triple& t) : x(t.x), y(t.y), z(t.z) {}

  Triple& operator=(const Triple& s) {
    if (this != &s) {
      x = s.x;
      y = s.y;
      z = s.z;
    }
    return *this;
  }

  double len() const;
  Triple scale(double) const;
  Triple normalized() const;

  Triple sum(const Triple&) const;
  Triple diff(const Triple&) const;

  double dot(const Triple&) const;
  Triple cross(const Triple&) const;

  std::string toString() const;

  Triple operator+(const Triple& s) const { return sum(s); }
  Triple operator-(const Triple& s) const { return diff(s); }
  Triple operator*(double s) const { return scale(s); }
  double operator*(const Triple& s) const { return dot(s); }

  inline double operator[](int index) const;
  inline double getX() const { return x; }
  inline double getY() const { return y; }
  inline double getZ() const { return z; }

  // Static operations on sets of Triple
  static void boundingRect(const std::vector<Triple>& pts, double& x, double& y, double& wid, double& hgt);

 private:
  double x, y, z;
};

#endif
