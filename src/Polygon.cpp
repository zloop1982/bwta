#include <BWTA/Polygon.h>
#include "functions.h"
namespace BWTA
{
  Polygon::Polygon()
  {
  }
  double Polygon::getArea() const
  {
    if (size()<3) return 0;
    double a=0;
    for(unsigned int i=0;i+1<size();i++)
    {
      a+=(double)(*this)[i].x()*(*this)[i+1].y()-(double)(*this)[i+1].x()*(*this)[i].y();
    }
    a+=back().x()*front().y()-front().x()*back().y();
    a/=2;
    a=fabs(a);
    return a;
  }
  double Polygon::getPerimeter() const
  {
    if (size()<2) return 0;
    double p=0;
    for(unsigned int i=0;i+1<size();i++)
    {
      p+=(*this)[i].getDistance((*this)[i+1]);
    }
    p+=back().getDistance(front());
    return p;
  }
  BWAPI::Position Polygon::getCenter() const
  {
    double a=getArea();
    double cx=0;
    double cy=0;
    double temp;
    for(unsigned int i=0,j=1;i<size();i++,j++)
    {
      if (j==size())
        j=0;
      temp=(double)(*this)[i].x()*(*this)[j].y()-(double)(*this)[j].x()*(*this)[i].y();
      cx+=((*this)[i].x()+(*this)[j].x())*temp;
      cy+=((*this)[i].y()+(*this)[j].y())*temp;
    }
    cx=cx/(6.0*a);
    cy=cy/(6.0*a);
    return BWAPI::Position((int)cx,(int)cy);
  }
  bool Polygon::isInside(BWAPI::Position p) const
  {
    PolygonD polyd;
    PointD query_pt(p.x(),p.y());
    for(unsigned int i=0;i<size();i++)
    {
      polyd.push_back(PointD((*this)[i].x(),(*this)[i].y()));
    }
    return (polyd.bounded_side(query_pt)==CGAL::ON_BOUNDED_SIDE);
  }
}