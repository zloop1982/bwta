#pragma once
#include "functions.h"
class Node
{
public:
  Node(Point point, double radius, bool is_region);
  Node(Point point, double radius, bool is_region,Arrangement_2::Vertex_handle handle);
  bool operator<(const Node& other) const;
  PolygonD get_polygon() const;
  Point point;
  double radius;
  std::set<Node*> neighbors;
  bool is_region;
  Arrangement_2::Vertex_handle handle;
};