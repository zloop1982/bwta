#include <BWTA.h>
#include "BWTA_Result.h"

namespace BWTA
{
  std::set<Region*>& getRegions()
  {
    return BWTA_Result::regions;
  }
  std::set<Chokepoint*>& getChokepoints()
  {
    return BWTA_Result::chokepoints;
  }
  std::set<BaseLocation*>& getBaseLocations()
  {
    return BWTA_Result::baselocations;
  }
  BaseLocation* getStartLocation(BWAPI::Player* player)
  {
    BWAPI::TilePosition tp=player->getStartLocation();
    BWAPI::Position pos(tp.x()*32+64,tp.y()*32+48);
    BaseLocation* startLocation=NULL;
    double min_distance=32*20;
    for(std::set<BaseLocation*>::iterator i=BWTA_Result::baselocations.begin();i!=BWTA_Result::baselocations.end();i++)
    {
      double distance=pos.getDistance((*i)->getPosition());
      if (distance<min_distance)
      {
        min_distance=distance;
        startLocation=*i;
      }
    }
    return startLocation;
  }
}