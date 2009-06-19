#pragma once
#include "functions.h"
class ConnectedComponent
{
public:
  ConnectedComponent(int id, int walkable);
  int isWalkable(void) const;
  void setWalkability(int walkability);
  int getID(void) const;
  PointD& top_left();
  private:
  PointD top_left_tile;
  int _walkability;
  int _id;
};
