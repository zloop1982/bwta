#include <BWTA.h>
#include "BWTA_Result.h"

namespace BWTA
{
  const std::set<Region*>& getRegions()
  {
    return BWTA_Result::regions;
  }
  const std::set<Chokepoint*>& getChokepoints()
  {
    return BWTA_Result::chokepoints;
  }
  const std::set<BaseLocation*>& getBaseLocations()
  {
    return BWTA_Result::baselocations;
  }
  const std::set<BaseLocation*>& getStartLocations()
  {
    return BWTA_Result::startlocations;
  }
  BaseLocation* getStartLocation(BWAPI::Player* player)
  {
    if (player==NULL) return NULL;
    return getNearestBaseLocation(player->getStartLocation());
  }
  BaseLocation* getNearestBaseLocation(BWAPI::TilePosition position)
  {
    if (position==BWAPI::TilePositions::Unknown) return NULL;
    BWAPI::Position pos(position.x()*32+64,position.y()*32+48);
    BaseLocation* baseLocation=NULL;
    double min_distance=-1;
    for(std::set<BaseLocation*>::iterator i=BWTA_Result::baselocations.begin();i!=BWTA_Result::baselocations.end();i++)
    {
      double distance=pos.getDistance((*i)->getPosition());
      if (distance<min_distance || baseLocation==NULL)
      {
        min_distance=distance;
        baseLocation=*i;
      }
    }
    return baseLocation;
  }
}