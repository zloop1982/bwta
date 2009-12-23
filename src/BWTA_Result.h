#pragma once
#include <BWTA.h>
#include "RectangleArray.h"
namespace BWTA
{
  namespace BWTA_Result
  {
    extern std::set<Region*> regions;
    extern std::set<Chokepoint*> chokepoints;
    extern std::set<BaseLocation*> baselocations;
    extern std::set<BaseLocation*> startlocations;
    extern std::set<Polygon*> unwalkablePolygons;
    extern RectangleArray<Region*> getRegion;
    extern RectangleArray<Polygon*> getUnwalkablePolygon;
  };
}