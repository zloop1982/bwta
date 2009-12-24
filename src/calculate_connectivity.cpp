#include "functions.h"
#include <BWAPI.h>
#include <BWTA.h>
#include "BWTA_Result.h"
#include "BaseLocationImpl.h"
#include "RegionImpl.h"
#include "ChokepointImpl.h"
#include "ConnectedComponent.h"
#include "MapData.h"
#include "Heap.h"
namespace BWTA
{
  void calculate_connectivity()
  {
    std::map<BWTA::RegionImpl*,BWTA::RegionImpl*> regionGroup;
    for(std::set<BWTA::Region*>::iterator r=BWTA::BWTA_Result::regions.begin();r!=BWTA::BWTA_Result::regions.end();r++)
    {
      BWTA::RegionImpl* region1=(BWTA::RegionImpl*)(*r);
      for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
      {
        BWTA::ChokepointImpl* cp=(BWTA::ChokepointImpl*)(*c);
        BWTA::RegionImpl* region2=(BWTA::RegionImpl*)(cp->_regions.first);
        if (region1==region2)
          region2=(BWTA::RegionImpl*)(cp->_regions.second);
        regionGroup[get_set2(regionGroup,region2)]=get_set2(regionGroup,region1);
      }
    }
    for(std::set<BWTA::Region*>::iterator r=BWTA::BWTA_Result::regions.begin();r!=BWTA::BWTA_Result::regions.end();r++)
    {
      BWTA::RegionImpl* region1=(BWTA::RegionImpl*)(*r);
      region1->reachableRegions.insert(region1);
      for(std::set<BWTA::Region*>::iterator r2=BWTA::BWTA_Result::regions.begin();r2!=BWTA::BWTA_Result::regions.end();r2++)
      {
        BWTA::RegionImpl* region2=(BWTA::RegionImpl*)(*r2);
        if (region1==region2)
          continue;
        if (get_set2(regionGroup,region1)==get_set2(regionGroup,region2))
        {
          region1->reachableRegions.insert(region2);
          region2->reachableRegions.insert(region1);
        }
      }
    }
    for(int x=0;x<(int)BWTA::BWTA_Result::getRegion.getWidth();x++)
    {
      for(int y=0;y<(int)BWTA::BWTA_Result::getRegion.getHeight();y++)
      {
        BWTA::Region* closestRegion = NULL;
        bool inRegion = false;
        BWTA::Polygon* closestUnwalkablePolygon = NULL;
        bool inPolygon = false;
        double minPolygonDist = -1;
        BWAPI::Position point=BWAPI::Position(x*32+16,y*32+16);
        for(std::set<BWTA::Region*>::iterator r=BWTA::BWTA_Result::regions.begin();r!=BWTA::BWTA_Result::regions.end();r++)
        {
          if ((*r)->getPolygon().isInside(point))
          {
            closestRegion=*r;
            inRegion=true;
            break;
          }
        }
        for(std::set<BWTA::Polygon*>::iterator p=BWTA::BWTA_Result::unwalkablePolygons.begin();p!=BWTA::BWTA_Result::unwalkablePolygons.end();p++)
        {
          
          if ((*p)->isInside(point))
          {
            closestUnwalkablePolygon=*p;
            inPolygon=true;
            minPolygonDist=0;
            break;
          }
          else
          {
            double dist=(*p)->getNearestPoint(point).getDistance(point);
            if (minPolygonDist == -1 || dist<minPolygonDist)
            {
              minPolygonDist=dist;
              closestUnwalkablePolygon=*p;
            }
          }
        }
        if (inRegion)
          BWTA::BWTA_Result::getRegion[x][y]=closestRegion;
        BWTA::BWTA_Result::getUnwalkablePolygon[x][y]=closestUnwalkablePolygon;
      }
    }
  }
}