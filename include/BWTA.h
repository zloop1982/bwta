#include <BWAPI.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/Polygon.h>
#include <BWTA/Region.h>
namespace BWTA
{
  void analyze();
  std::set<Region*>& getRegions();
  std::set<Chokepoint*>& getChokepoints();
}