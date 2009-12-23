#pragma once
#include <BWAPI.h>
#include <BWTA/Chokepoint.h>
#include <BWTA/Polygon.h>
#include <BWTA/Region.h>
#include <BWTA/BaseLocation.h>
namespace BWTA
{
  void readMap();
  void analyze();
  const std::set<Region*>& getRegions();
  const std::set<Chokepoint*>& getChokepoints();
  const std::set<BaseLocation*>& getBaseLocations();
  const std::set<BaseLocation*>& getStartLocations();
  const std::set<Polygon*>& getUnwalkablePolygons();
  BWAPI::Position getNearestUnwalkablePosition(BWAPI::Position position);
  BaseLocation* getStartLocation(BWAPI::Player* player);

  BaseLocation* getNearestBaseLocation(int x, int y);
  BaseLocation* getNearestBaseLocation(BWAPI::TilePosition position);

  Region* getRegion(int x, int y);
  Region* getRegion(BWAPI::TilePosition tileposition);

  Polygon* getNearestUnwalkablePolygon(int x, int y);
  Polygon* getNearestUnwalkablePolygon(BWAPI::TilePosition tileposition);

  bool isConnected(int x1, int y1, int x2, int y2);
  bool isConnected(BWAPI::TilePosition a, BWAPI::TilePosition b);

  std::pair<BWAPI::TilePosition, double> getNearestTilePosition(BWAPI::TilePosition start, std::set<BWAPI::TilePosition>& targets);
  double getGroundDistance(BWAPI::TilePosition start, BWAPI::TilePosition end);
  std::map<BWAPI::TilePosition, double> getGroundDistances(BWAPI::TilePosition start, std::set<BWAPI::TilePosition>& targets);
  std::vector<BWAPI::TilePosition> getShortestPath(BWAPI::TilePosition start, BWAPI::TilePosition end);
  std::vector<BWAPI::TilePosition> getShortestPath(BWAPI::TilePosition start, std::set<BWAPI::TilePosition>& targets);
}