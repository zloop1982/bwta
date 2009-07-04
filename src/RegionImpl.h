#pragma once
#include <BWAPI.h>
#include <set>
#include <BWTA/Polygon.h>
#include <BWTA/Region.h>
#include <BWTA/BaseLocation.h>
namespace BWTA
{
  class Chokepoint;
  class RegionImpl : public Region
  {
  public:
    RegionImpl(Polygon &poly);
    virtual const Polygon& getPolygon() const;
    virtual const BWAPI::Position& getCenter() const;
    virtual const std::set<Chokepoint*>& getChokepoints() const;
    virtual const std::set<BaseLocation*>& getBaseLocations() const;
    void setChokepoints(std::set<Chokepoint*> chokepoints);
    std::set<BaseLocation*> baseLocations;
  private:
    Polygon _polygon;
    BWAPI::Position _center;
    std::set<Chokepoint*> _chokepoints;
  };
}