#include "Node.h"
#include "Graph.h"

Node::Node(Point point, double radius, bool is_region):
point(point),radius(radius),is_region(is_region)
{
}
Node::Node(Point point, double radius, bool is_region,Arrangement_2::Vertex_handle handle):
point(point),radius(radius),is_region(is_region),handle(handle)
{
}
bool Node::operator<(const Node& other) const
{
  return ptcmp()(point,other.point);
}
PolygonD Node::get_polygon() const
{
  Arrangement_2::Face_const_handle f;
  PolygonD poly;
  if (CGAL::assign(f,spl.locate(Point_2(point.x(),point.y()))))
  {
    Arrangement_2::Ccb_halfedge_const_circulator c=f->outer_ccb();
    Arrangement_2::Ccb_halfedge_const_circulator start=c;
    do
    {
      
      poly.push_back(PointD(cast_to_double(c->source()->point().x()),cast_to_double(c->source()->point().y())));
      c++;
    } while (c!=start);
  }
  return poly;
}