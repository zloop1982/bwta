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
  const std::set<Polygon*>& getUnwalkablePolygons()
  {
    return BWTA_Result::unwalkablePolygons;
  }
  BWAPI::Position nearestUnwalkablePosition(BWAPI::Position position)
  {
    BWAPI::Position nearest=BWAPI::Positions::Unknown;
    for(std::set<Polygon*>::const_iterator i=BWTA_Result::unwalkablePolygons.begin();i!=BWTA_Result::unwalkablePolygons.end();i++)
    {
      BWAPI::Position current=(*i)->getNearestPoint(position);
      if (position.getDistance(current) < position.getDistance(nearest))
        nearest=current;
    }
    if (position.x()<position.getDistance(nearest))
      nearest=BWAPI::Position(0,position.y());
    if (position.y()<position.getDistance(nearest))
      nearest=BWAPI::Position(position.x(),0);
    if (BWAPI::Broodwar->mapWidth()*32-position.x()<position.getDistance(nearest))
      nearest=BWAPI::Position(BWAPI::Broodwar->mapWidth()*32,position.y());
    if (BWAPI::Broodwar->mapHeight()*32-position.y()<position.getDistance(nearest))
      nearest=BWAPI::Position(position.x(),BWAPI::Broodwar->mapHeight()*32);
    return nearest;
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