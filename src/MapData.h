#pragma once
#include <BWAPI.h>
namespace BWTA
{
  namespace MapData
  {
    extern std::set<BWAPI::Unit*> minerals;
    extern std::set<BWAPI::Unit*> rawMinerals;
    extern std::set<BWAPI::Unit*> geysers;
    extern RectangleArray<bool> walkability;
    extern RectangleArray<bool> rawWalkability;
    extern RectangleArray<bool> buildability;
    extern std::set<BWAPI::TilePosition> startLocations;
    extern int hash;
    extern int mapWidth;
    extern int mapHeight;
  }
}