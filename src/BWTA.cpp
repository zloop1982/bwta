#include <BWTA.h>
#include "BWTA_Result.h"
#include "Heap.h"
#include "MapData.h"
#include "functions.h"
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
  BWAPI::Position getNearestUnwalkablePosition(BWAPI::Position position)
  {
    BWAPI::Position nearest=BWTA::getNearestUnwalkablePolygon(position.x()/32,position.y()/32)->getNearestPoint(position);
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
  BaseLocation* getNearestBaseLocation(int x, int y)
  {
    return getNearestBaseLocation(BWAPI::TilePosition(x,y));
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

  Region* getRegion(int x, int y)
  {
    return BWTA::BWTA_Result::getRegion[x][y];
  }
  Region* getRegion(BWAPI::TilePosition tileposition)
  {
    return BWTA::BWTA_Result::getRegion[tileposition.x()][tileposition.y()];
  }

  Polygon* getNearestUnwalkablePolygon(int x, int y)
  {
    return BWTA::BWTA_Result::getUnwalkablePolygon[x][y];
  }
  Polygon* getNearestUnwalkablePolygon(BWAPI::TilePosition tileposition)
  {
    return BWTA::BWTA_Result::getUnwalkablePolygon[tileposition.x()][tileposition.y()];
  }

  bool isConnected(int x1, int y1, int x2, int y2)
  {
    return getRegion(x1,y1)==getRegion(x2,y2);
  }
  bool isConnected(BWAPI::TilePosition a, BWAPI::TilePosition b)
  {
    return getRegion(a.x(),a.y())==getRegion(b.x(),b.y());
  }
  std::pair<BWAPI::TilePosition, double> getNearestTilePosition(BWAPI::TilePosition start,std::set<BWAPI::TilePosition>& targets)
  {
    std::set<BWAPI::TilePosition> valid_targets;
    for(std::set<BWAPI::TilePosition>::iterator i=targets.begin();i!=targets.end();i++)
    {
      if (isConnected(start,*i))
        valid_targets.insert(*i);
    }
    if (valid_targets.empty())
      return std::make_pair(BWAPI::TilePositions::None,-1);
    return AstarSearchDistance(start,valid_targets);
  }
  double getGroundDistance(BWAPI::TilePosition start, BWAPI::TilePosition end)
  {
    if (!isConnected(start,end))
      return -1;
    return AstarSearchDistance(start,end);
  }
  std::map<BWAPI::TilePosition, double> getGroundDistances(BWAPI::TilePosition start, std::set<BWAPI::TilePosition>& targets)
  {
    std::map<BWAPI::TilePosition, double> answer;
    std::set<BWAPI::TilePosition> valid_targets;
    for(std::set<BWAPI::TilePosition>::iterator i=targets.begin();i!=targets.end();i++)
    {
      if (isConnected(start,*i))
        valid_targets.insert(*i);
      else
        answer[*i]=-1;
    }
    if (valid_targets.empty())
      return answer;
    return AstarSearchDistanceAll(start,valid_targets);
  }
  std::vector<BWAPI::TilePosition> getShortestPath(BWAPI::TilePosition start, BWAPI::TilePosition end)
  {
    std::vector<BWAPI::TilePosition> path;
    if (!isConnected(start,end))
      return path;
    return AstarSearchPath(start,end);
  }
  std::vector<BWAPI::TilePosition> getShortestPath(BWAPI::TilePosition start, std::set<BWAPI::TilePosition>& targets)
  {
    std::vector<BWAPI::TilePosition> path;
    std::set<BWAPI::TilePosition> valid_targets;
    for(std::set<BWAPI::TilePosition>::iterator i=targets.begin();i!=targets.end();i++)
    {
      if (isConnected(start,*i))
        valid_targets.insert(*i);
    }
    if (valid_targets.empty())
      return path;
    return AstarSearchPath(start,valid_targets);
  }
}