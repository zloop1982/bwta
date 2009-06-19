#include "Region.h"
namespace BWTA
{
  Region::Region(Polygon &poly)
  {
    this->_polygon=poly;
    this->_center=poly.getCenter();
  }
  const Polygon& Region::getPolygon() const
  {
    return this->_polygon;
  }
  const BWAPI::Position& Region::getCenter() const
  {
    return this->_center;
  }
  const std::set<Chokepoint*>& Region::getChokepoints() const
  {
    return this->_chokepoints;
  }
  void Region::setChokepoints(std::set<Chokepoint*> chokepoints)
  {
    this->_chokepoints=chokepoints;

  }
}