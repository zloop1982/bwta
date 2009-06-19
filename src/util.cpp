#include "functions.h"
#include <string>
#include <sstream>

double cast_to_double( double q)
{
  return q;
}
double cast_to_double( CGAL::MP_Float q)
{
  return CGAL::to_double(q);
}
double cast_to_double( CGAL::Quotient<CGAL::MP_Float> q)
{
  return CGAL::INTERN_MP_FLOAT::to_double(q.numerator())/CGAL::INTERN_MP_FLOAT::to_double(q.denominator());
}

double cast_to_double( CGAL::Gmpq q)
{
  return q.to_double();
}
double cast_to_double( CGAL::Lazy_exact_nt<CGAL::Gmpq > q)
{
  return CGAL::to_double(q.approx());
}
bool is_real( double q)
{
  return q+1>q;
}
bool is_real( CGAL::MP_Float q)
{
  return q+1>q;
}
bool is_real( CGAL::Quotient<CGAL::MP_Float> q)
{
  return q+1>q;
}
bool is_real( CGAL::Gmpq q)
{
  return q+1>q;
}
bool is_real( CGAL::Lazy_exact_nt<CGAL::Gmpq > q)
{
  return q+1>q;
}
#ifdef USE_EXACT
double get_distance(Point a, Point b)
{
  return sqrt(cast_to_double((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())));
}
#endif
double get_distance(PointD a, PointD b)
{
  return sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
}
int str2int(std::string str) {
  std::stringstream t(str);
  int x;
  t>>x;
  return x;
}

std::string int2str(int number) {
  std::stringstream t;
  t<<number;
  std::string str=t.str();
  return str;
}

int max(int a, int b) {return (a>b) ? a : b;}
int min(int a, int b) {return (a<b) ? a : b;}



double distance_to_border(PolygonD& polygon,int width, int height)
{
  double distance=polygon.left_vertex()->x();
  if (polygon.top_vertex()->y()<distance)
  {
    distance=polygon.top_vertex()->y();
  }
  if (width-polygon.right_vertex()->x()<distance)
  {
    distance=width-polygon.right_vertex()->x();
  }
  if (height-polygon.bottom_vertex()->y()<distance)
  {
    distance=height-polygon.bottom_vertex()->y();
  }
  if (distance<0)
  {
    return 0;
  }
  return distance;
}