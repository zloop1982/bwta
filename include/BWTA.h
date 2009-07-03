#include <BWAPI.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/Polygon.h>
#include <BWTA/Region.h>
#include <BWTA/BaseLocation.h>
namespace BWTA
{
  void analyze();
  std::set<Region*>& getRegions();
  std::set<Chokepoint*>& getChokepoints();
  std::set<BaseLocation*>& getBaseLocations();
}