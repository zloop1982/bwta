#include <BWAPI.h>
#include 
namespace BWTA
{
  void BWTA_init(Game* game);
  void analyze();
  std::set<Region*>& getRegions();
  std::set<Chokepoint*>& getChokepoints();
}