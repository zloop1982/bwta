#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <BWAPI.h>
#include "BWTA_Result.h"

bool load_map(Util::RectangleArray<bool> &walkability,Util::RectangleArray<bool> &buildability)
{
  int b_width=BWAPI::Broodwar->mapWidth();
  int b_height=BWAPI::Broodwar->mapHeight();
  int width=BWAPI::Broodwar->mapWidth()*4;
  int height=BWAPI::Broodwar->mapHeight()*4;
  walkability.resize(width,height);
  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      walkability[x][y]=true;
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
          walkability[x2][y2]&=BWAPI::Broodwar->walkable(x,y);
        }
      }
    }
  }
  buildability.resize(b_width,b_height);
  for(int y=0;y<b_height;y++)
  {
    for(int x=0;x<b_width;x++)
    {
      buildability[x][y]=BWAPI::Broodwar->buildable(x,y);
    }
  }
  return true;
}

bool load_resources(std::set< BWAPI::Unit* > &minerals,std::set< BWAPI::Unit* > &geysers)
{
  std::set<BWAPI::Unit*> bwminerals=BWAPI::Broodwar->getMinerals();
  for(std::set<BWAPI::Unit*>::iterator m=bwminerals.begin();m!=bwminerals.end();m++)
  {
    if ((*m)->resources()>200)
    {
      minerals.insert(*m);
    }
  }
  geysers=BWAPI::Broodwar->getGeysers();
  return true;
}
namespace BWTA
{
  void load_data(std::string filename)
  {
    std::ifstream file_in;
    file_in.open(filename.c_str());
    file_in.close();

  }
  void save_data(std::string filename)
  {
    std::map<BaseLocation*,int> bid;
    std::map<Chokepoint*,int> cid;
    std::map<Region*,int> rid;
    int b_id=0;
    int c_id=0;
    int r_id=0;
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
    int file_version=1;
    file_out << file_version << "\n";
    file_out << BWTA_Result::baselocations.size() << "\n";
    file_out << BWTA_Result::chokepoints.size() << "\n";
    file_out << BWTA_Result::regions.size() << "\n";
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