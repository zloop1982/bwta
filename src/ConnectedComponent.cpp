#include "ConnectedComponent.h"

ConnectedComponent::ConnectedComponent(int id, int walkable=1) : _id(id),_walkability(walkable),top_left_tile(10000,10000)
{
}
bool ConnectedComponent::isWalkable(void) const
{
  return this->_walkability;
}
void ConnectedComponent::setWalkability(bool walkability)
{
  this->_walkability=walkability;
}
int ConnectedComponent::getID(void) const
{
  return this->_id;
}
PointD& ConnectedComponent::top_left()
{
  return this->top_left_tile;
}