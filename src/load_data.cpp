#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <BWAPI.h>
#include "BWTA_Result.h"
#include "BaseLocationImpl.h"
#include "ChokepointImpl.h"
#include "RegionImpl.h"
#include "find_base_locations.h"
#include "MapData.h"
namespace BWTA
{
  bool load_map()
  {
    int b_width=MapData::mapWidth;
    int b_height=MapData::mapHeight;
    int width=MapData::mapWidth*4;
    int height=MapData::mapHeight*4;
    MapData::buildability.resize(b_width,b_height);
    MapData::walkability.resize(width,height);
    MapData::rawWalkability.resize(width,height);

    //copy buildability data into buildability array
    for(int y=0;y<b_height;y++)
    {
      for(int x=0;x<b_width;x++)
      {
        MapData::buildability[x][y]=BWAPI::Broodwar->buildable(x,y);
      }
    }
    //copy and simplify walkability data as it is copies into walkability array
    for(int y=0;y<height;y++)
    {
      for(int x=0;x<width;x++)
      {
        MapData::rawWalkability[x][y]=BWAPI::Broodwar->walkable(x,y);
        MapData::walkability[x][y]=true;
      }
    }
    for(int y=0;y<height;y++)
    {
      for(int x=0;x<width;x++)
      {
        for(int x2=max(x-1,0);x2<=min(width-1,x+1);x2++)
        {
          for(int y2=max(y-1,0);y2<=min(height-1,y+1);y2++)
          {
            MapData::walkability[x2][y2]&=MapData::rawWalkability[x][y];
          }
        }
      }
    }
    return true;
  }

  bool load_resources()
  {
    MapData::rawMinerals=BWAPI::Broodwar->getStaticMinerals();
    //filter out all mineral patches under 200
    for(std::set<BWAPI::Unit*>::iterator m=MapData::rawMinerals.begin();m!=MapData::rawMinerals.end();m++)
    {
      if ((*m)->getInitialResources()>200)
      {
        MapData::minerals.insert(*m);
      }
    }
    MapData::geysers=BWAPI::Broodwar->getStaticGeysers();
    return true;
  }

  void load_data(std::string filename)
  {
    int version;
    int unwalkablePolygon_amount;
    int baselocation_amount;
    int chokepoint_amount;
    int region_amount;
    std::vector<Polygon*> unwalkablePolygons;
    std::vector<BaseLocation*> baselocations;
    std::vector<Chokepoint*> chokepoints;
    std::vector<Region*> regions;
    std::ifstream file_in;
    file_in.open(filename.c_str());
    file_in >> version;
    if (version!=3)
    {
      file_in.close();
      return;
    }
    file_in >> unwalkablePolygon_amount;
    file_in >> baselocation_amount;
    file_in >> chokepoint_amount;
    file_in >> region_amount;
    for(int i=0;i<unwalkablePolygon_amount;i++)
    {
      Polygon* p=new Polygon();
      unwalkablePolygons.push_back(p);
      BWTA_Result::unwalkablePolygons.insert(p);
    }
    for(int i=0;i<baselocation_amount;i++)
    {
      BaseLocation* b=new BaseLocationImpl();
      baselocations.push_back(b);
      BWTA_Result::baselocations.insert(b);
    }
    for(int i=0;i<chokepoint_amount;i++)
    {
      Chokepoint* c=new ChokepointImpl();
      chokepoints.push_back(c);
      BWTA_Result::chokepoints.insert(c);
    }
    for(int i=0;i<region_amount;i++)
    {
      Region* r=new RegionImpl();
      regions.push_back(r);
      BWTA_Result::regions.insert(r);
    }
    for(int i=0;i<unwalkablePolygon_amount;i++)
    {
      int id, polygon_size;
      file_in >> id >> polygon_size;
      for(int j=0;j<polygon_size;j++)
      {
        int x,y;
        file_in >> x >> y;
        unwalkablePolygons[i]->push_back(BWAPI::Position(x,y));
      }
    }
    for(int i=0;i<baselocation_amount;i++)
    {
      int id,px,py,tpx,tpy;
      file_in >> id >> px >> py >> tpx >> tpy;
      ((BaseLocationImpl*)baselocations[i])->position=BWAPI::Position(px,py);
      ((BaseLocationImpl*)baselocations[i])->tilePosition=BWAPI::TilePosition(tpx,tpy);
      int rid;
      file_in >> rid;
      ((BaseLocationImpl*)baselocations[i])->region=regions[rid];
      for(int j=0;j<baselocation_amount;j++)
      {
        double g_dist, a_dist;
        file_in >> g_dist >> a_dist;
        ((BaseLocationImpl*)baselocations[i])->ground_distances[baselocations[j]]=g_dist;
        ((BaseLocationImpl*)baselocations[i])->air_distances[baselocations[j]]=a_dist;
      }
      file_in >> ((BaseLocationImpl*)baselocations[i])->island;
      file_in >> ((BaseLocationImpl*)baselocations[i])->start;
      if (((BaseLocationImpl*)baselocations[i])->start)
        BWTA::BWTA_Result::startlocations.insert(baselocations[i]);
    }
    for(int i=0;i<chokepoint_amount;i++)
    {
      int id,rid1,rid2,s1x,s1y,s2x,s2y,cx,cy;
      double width;
      file_in >> id >> rid1 >> rid2 >> s1x >> s1y >> s2x >> s2y >> cx >> cy >> width;
      ((ChokepointImpl*)chokepoints[i])->_regions.first=regions[rid1];
      ((ChokepointImpl*)chokepoints[i])->_regions.second=regions[rid2];
      ((ChokepointImpl*)chokepoints[i])->_sides.first=BWAPI::Position(s1x,s1y);
      ((ChokepointImpl*)chokepoints[i])->_sides.second=BWAPI::Position(s2x,s2y);
      ((ChokepointImpl*)chokepoints[i])->_center=BWAPI::Position(cx,cy);
      ((ChokepointImpl*)chokepoints[i])->_width=width;
    }
    for(int i=0;i<region_amount;i++)
    {
      int id, polygon_size;
      file_in >> id >> polygon_size;
      for(int j=0;j<polygon_size;j++)
      {
        int x,y;
        file_in >> x >> y;
        ((RegionImpl*)regions[i])->_polygon.push_back(BWAPI::Position(x,y));
      }
      int cx,cy,chokepoints_size;
      file_in >> cx >> cy >> chokepoints_size;
      ((RegionImpl*)regions[i])->_center=BWAPI::Position(cx,cy);
      for(int j=0;j<chokepoints_size;j++)
      {
        int cid;
        file_in >> cid;
        ((RegionImpl*)regions[i])->_chokepoints.insert(chokepoints[cid]);
      }
      int baselocations_size;
      file_in >> baselocations_size;
      for(int j=0;j<baselocations_size;j++)
      {
        int bid;
        file_in >> bid;
        ((RegionImpl*)regions[i])->baseLocations.insert(baselocations[bid]);
      }
    }
    file_in.close();
    attach_resources_to_base_locations(BWTA_Result::baselocations);
  }

  void save_data(std::string filename)
  {
    std::map<Polygon*,int> pid;
    std::map<BaseLocation*,int> bid;
    std::map<Chokepoint*,int> cid;
    std::map<Region*,int> rid;
    int p_id=0;
    int b_id=0;
    int c_id=0;
    int r_id=0;
    for(std::set<Polygon*>::const_iterator p=BWTA_Result::unwalkablePolygons.begin();p!=BWTA_Result::unwalkablePolygons.end();p++)
    {
      pid[*p]=p_id++;
    }
    for(std::set<BaseLocation*>::const_iterator b=BWTA_Result::baselocations.begin();b!=BWTA_Result::baselocations.end();b++)
    {
      bid[*b]=b_id++;
    }
    for(std::set<Chokepoint*>::const_iterator c=BWTA_Result::chokepoints.begin();c!=BWTA_Result::chokepoints.end();c++)
    {
      cid[*c]=c_id++;
    }
    for(std::set<Region*>::const_iterator r=BWTA_Result::regions.begin();r!=BWTA_Result::regions.end();r++)
    {
      rid[*r]=r_id++;
    }
    std::ofstream file_out;
    file_out.open(filename.c_str());
    int file_version=3;
    file_out << file_version << "\n";
    file_out << BWTA_Result::unwalkablePolygons.size() << "\n";
    file_out << BWTA_Result::baselocations.size() << "\n";
    file_out << BWTA_Result::chokepoints.size() << "\n";
    file_out << BWTA_Result::regions.size() << "\n";
    for(std::set<Polygon*>::const_iterator p=BWTA_Result::unwalkablePolygons.begin();p!=BWTA_Result::unwalkablePolygons.end();p++)
    {
      file_out << pid[*p] << "\n";
      file_out << (*p)->size() << "\n";
      for(unsigned int i=0;i<(*p)->size();i++)
      {
        file_out << (**p)[i].x() << "\n";
        file_out << (**p)[i].y() << "\n";
      }
    }
    for(std::set<BaseLocation*>::const_iterator b=BWTA_Result::baselocations.begin();b!=BWTA_Result::baselocations.end();b++)
    {
      file_out << bid[*b] << "\n";
      file_out << (*b)->getPosition().x() << "\n";
      file_out << (*b)->getPosition().y() << "\n";
      file_out << (*b)->getTilePosition().x() << "\n";
      file_out << (*b)->getTilePosition().y() << "\n";
      file_out << rid[(*b)->getRegion()] << "\n";
      for(std::set<BaseLocation*>::const_iterator b2=BWTA_Result::baselocations.begin();b2!=BWTA_Result::baselocations.end();b2++)
      {
        file_out << (*b)->getGroundDistance(*b2) << "\n";
        file_out << (*b)->getAirDistance(*b2) << "\n";
      }
      file_out << (*b)->isIsland() << "\n";
      file_out << (*b)->isStartLocation() << "\n";
    }
    for(std::set<Chokepoint*>::const_iterator c=BWTA_Result::chokepoints.begin();c!=BWTA_Result::chokepoints.end();c++)
    {
      file_out << cid[*c] << "\n";
      file_out << rid[(*c)->getRegions().first] << "\n";
      file_out << rid[(*c)->getRegions().second] << "\n";
      file_out << (*c)->getSides().first.x() << "\n";
      file_out << (*c)->getSides().first.y() << "\n";
      file_out << (*c)->getSides().second.x() << "\n";
      file_out << (*c)->getSides().second.y() << "\n";
      file_out << (*c)->getCenter().x() << "\n";
      file_out << (*c)->getCenter().y() << "\n";
      file_out << (*c)->getWidth() << "\n";
    }
    for(std::set<Region*>::const_iterator r=BWTA_Result::regions.begin();r!=BWTA_Result::regions.end();r++)
    {
      file_out << rid[*r] << "\n";
      Polygon poly=(*r)->getPolygon();
      file_out << poly.size() << "\n";
      for(unsigned int i=0;i<poly.size();i++)
      {
        file_out << poly[i].x() << "\n";
        file_out << poly[i].y() << "\n";
      }
      file_out << (*r)->getCenter().x() << "\n";
      file_out << (*r)->getCenter().y() << "\n";
      file_out << (*r)->getChokepoints().size() << "\n";
      for(std::set<Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
      {
        file_out << cid[*c] << "\n";
      }
      file_out << (*r)->getBaseLocations().size() << "\n";
      for(std::set<BaseLocation*>::const_iterator b=(*r)->getBaseLocations().begin();b!=(*r)->getBaseLocations().end();b++)
      {
        file_out << bid[*b] << "\n";
      }
    }
    file_out.close();
  }
}