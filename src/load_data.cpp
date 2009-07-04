#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <BWAPI.h>

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
