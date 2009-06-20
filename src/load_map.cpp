#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <BWAPI.h>

bool load_map(Util::RectangleArray<bool> &walkability)
{
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
  return true;
}