#include "BaseLocationImpl.h"
namespace BWTA
{
  BaseLocationImpl::BaseLocationImpl(){}
  BaseLocationImpl::BaseLocationImpl(const BWAPI::TilePosition &tp,std::vector<BWAPI::Unit*> resource_cluster)
  {
    tilePosition=tp;
    position=BWAPI::Position(tp.x()*32+64,tp.y()*32+48);
    for(unsigned int i=0;i<resource_cluster.size();i++)
    {
      if (resource_cluster[i]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser)
      {
        geysers.insert(resource_cluster[i]);
      }
      else
      {
        minerals.insert(resource_cluster[i]);
      }
    }
    island=false;
    region=NULL;
  }

  BWAPI::Position BaseLocationImpl::getPosition() const
  {
    return this->position;
  }
  BWAPI::TilePosition BaseLocationImpl::getTilePosition() const
  {
    return this->tilePosition;
  }
  Region* BaseLocationImpl::getRegion() const
  {
    return this->region;
  }
  const std::set<BWAPI::Unit*>& BaseLocationImpl::getMinerals() const
  {
    return this->minerals;
  }
  const std::set<BWAPI::Unit*>& BaseLocationImpl::getGeysers() const
  {
    return this->geysers;
  }

  double BaseLocationImpl::getGroundDistance(BaseLocation* other) const
  {
    std::map<BaseLocation*,double>::const_iterator i=ground_distances.find(other);
    if (i==ground_distances.end())
    {
      return -1;
    }
    return (*i).second;
  }
  double BaseLocationImpl::getAirDistance(BaseLocation* other) const
  {
    std::map<BaseLocation*,double>::const_iterator i=air_distances.find(other);
    if (i==air_distances.end())
    {
      return -1;
    }
    return (*i).second;
  }
  bool BaseLocationImpl::isMineralOnly() const
  {
    return this->geysers.empty();
  }
  bool BaseLocationImpl::isIsland() const
  {
    return this->island;
  }
}