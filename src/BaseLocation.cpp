#include "BaseLocation.h"
namespace BWTA
{
  BaseLocation::BaseLocation(const BWAPI::TilePosition &tp) {
    tilePosition=tp;
    position=BWAPI::Position(tp.x()*32+64,tp.y()*32+48);
    mineral_only=false;
    island=false;
    region=NULL;
  }

  BWAPI::Position BaseLocation::getPosition() const
  {
    return this->position;
  }
  BWAPI::TilePosition BaseLocation::getTilePosition() const
  {
    return this->tilePosition;
  }
  Region* BaseLocation::getRegion() const
  {
    return this->region;
  }
  const std::set<BWAPI::Unit*> &BaseLocation::getGeysers() const
  {
    return this->geysers;
  }

  bool BaseLocation::isMineralOnly() const
  {
    return this->mineral_only;
  }
  bool BaseLocation::isIsland() const
  {
    return this->island;
  }
}