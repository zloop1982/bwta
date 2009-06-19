#include "BWTA/Chokepoint.h"
namespace BWTA
{
  Chokepoint::Chokepoint(std::pair<Region*,Region*> regions, std::pair<BWAPI::Position,BWAPI::Position> sides)
  {
    this->_regions=regions;
    this->_sides=sides;
    this->_center=sides.first+sides.second;
    this->_center.x()/=2;
    this->_center.y()/=2;
  }
  std::pair<Region*,Region*>& Chokepoint::getRegions()
  {
    return this->_regions;
  }
  std::pair<BWAPI::Position,BWAPI::Position>& Chokepoint::getSides()
  {
    return this->_sides;
  }
  BWAPI::Position& Chokepoint::getCenter()
  {
    return this->_center;
  }
}