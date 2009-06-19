#pragma once
#include <BWAPI.h>
#include <set>
#include <Polygon.h>
namespace BWTA
{
  class Chokepoint;
  class Region
  {
  public:
    Region(Polygon &poly);
    const Polygon& getPolygon() const;
    const BWAPI::Position& getCenter() const;
    const std::set<Chokepoint*>& getChokepoints() const;
    void setChokepoints(std::set<Chokepoint*> chokepoints);
  private:
    Polygon _polygon;
    BWAPI::Position _center;
    std::set<Chokepoint*> _chokepoints;
  };
}