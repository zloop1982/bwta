#pragma once
#include <BWAPI.h>
#include <BWTA/Region.h>
namespace BWTA
{
  class BaseLocation
  {
  public:
    BaseLocation(const BWAPI::TilePosition &tp);
    BWAPI::Position getPosition() const;
    BWAPI::TilePosition getTilePosition() const;
    Region* getRegion() const;
    const std::set<BWAPI::Unit*> &getGeysers() const;

    bool isMineralOnly() const;
    bool isIsland() const;
  private:
    BWAPI::TilePosition tilePosition;
    BWAPI::Position position;
    std::set<BWAPI::Unit*> geysers;
    Region* region;
    bool mineral_only;
    bool island;
  };
}