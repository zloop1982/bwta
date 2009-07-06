#pragma once
#include <BWAPI.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/Polygon.h>
#include <BWTA/Region.h>
#include <BWTA/BaseLocation.h>
namespace BWTA
{
  void analyze();
  const std::set<Region*>& getRegions();
  const std::set<Chokepoint*>& getChokepoints();
  const std::set<BaseLocation*>& getBaseLocations();
  BaseLocation* getStartLocation(BWAPI::Player* player);
}