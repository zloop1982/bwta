#pragma once
#include "functions.h"
class ConnectedComponent
{
public:
  ConnectedComponent(int id, int walkable);
  bool isWalkable(void) const;
  void setWalkability(bool walkability);
  int getID(void) const;
  PointD& top_left();
  private:
  PointD top_left_tile;
  bool _walkability;
  int _id;
};
