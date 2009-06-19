#pragma once
#include <BWAPI.h>
#include <vector>
namespace BWTA
{
  class Polygon : public std::vector <BWAPI::Position>
  {
    public:
    Polygon();
    double getArea();
    double getPerimeter();
    BWAPI::Position getCenter();
    bool isInside(BWAPI::Position p) const;
  };
}