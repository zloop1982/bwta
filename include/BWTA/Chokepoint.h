#pragma once
#include <utility>
#include <BWAPI.h>
namespace BWTA
{
  class Region;
  class Chokepoint
  {
  public:
    Chokepoint(std::pair<Region*,Region*> regions, std::pair<BWAPI::Position,BWAPI::Position> sides);
    std::pair<Region*,Region*>& getRegions();
    std::pair<BWAPI::Position,BWAPI::Position>& getSides();
    BWAPI::Position& getCenter();
  private:
    std::pair<Region*,Region*> _regions;
    std::pair<BWAPI::Position,BWAPI::Position> _sides;
    BWAPI::Position _center;
  };
}