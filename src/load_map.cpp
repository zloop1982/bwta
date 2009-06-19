#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>

bool load_map(std::string filename, std::string &mapname,Util::RectangleArray<bool> &walkability)
{
  std::ifstream filedata;
  filedata.open(filename.c_str());
  getline(filedata,mapname);
  std::string line;
  getline(filedata,line);
  getline(filedata,line);
  int c1=line.find_first_of(",");
  int height=str2int(line.substr(0,c1));
  int width=str2int(line.substr(c1+1));
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
    getline(filedata,line);
    for(int x=0;x<width;x++)
    {
      for(int x2=max(x-1,0);x2<=min(width-1,x+1);x2++)
      {
        for(int y2=max(y-1,0);y2<=min(height-1,y+1);y2++)
        {
          walkability[x2][y2]&=line[x]-'0';
        }
      }
    }
  }
  return true;
}