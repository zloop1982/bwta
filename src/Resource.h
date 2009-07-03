#pragma once
#include <BWAPI.h>
class Resource {
  public:
  Resource(BWAPI::TilePosition _p, int _t) {position=_p;type=_t;}
  BWAPI::TilePosition position;
  int type;  
};
