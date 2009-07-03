#include "functions.h"
#include <string>
#include <sstream>
#include "Heap.h"

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

void log(const char* text)
{
  FILE * pFile;
  pFile = fopen ("C:\\BWTA.txt","a");
  if (pFile!=NULL)
  {
    fputs (text,pFile);
    fputs ("\n",pFile);
    fclose (pFile);
  }
}


int get_set(std::vector<int> &a,int i)
{
  if (i==a[i]) return i;
  a[i]=get_set(a,a[i]);
  return a[i];
}

void calculate_walk_distances(const Util::RectangleArray<bool> &read_map
                             ,const BWAPI::Position &start
                             ,int max_distance
                             ,Util::RectangleArray<int> &distance_map)
{
  Heap< BWAPI::Position , int > heap;
  for(unsigned int x=0;x<distance_map.getWidth();x++) {
    for(unsigned int y=0;y<distance_map.getHeight();y++) {
      distance_map[x][y]=-1;
    }
  }
  heap.push(std::make_pair(start,0));
  int sx=(int)start.x();
  int sy=(int)start.y();
  distance_map[sx][sy]=0;
  while (!heap.empty()) {
    BWAPI::Position pos=heap.top().first;
    int distance=heap.top().second;
    heap.pop();
    int x=(int)pos.x();
    int y=(int)pos.y();
    if (distance>max_distance && max_distance>0) break;
    int min_x=max(x-1,0);
    int max_x=min(x+1,read_map.getWidth()-1);
    int min_y=max(y-1,0);
    int max_y=min(y+1,read_map.getHeight()-1);
    for(int ix=min_x;ix<=max_x;ix++) {
      for(int iy=min_y;iy<=max_y;iy++) {
        int f=abs(ix-x)*10+abs(iy-y)*10;
        if (f>10) {f=14;}
        int v=distance+f;
        if (distance_map[ix][iy]>v) {
          heap.set(BWAPI::Position(x,y),v);
          distance_map[ix][iy]=v;
        } else {
          if (distance_map[ix][iy]==-1 && read_map[ix][iy]==true) {
            distance_map[ix][iy]=v;
            heap.push(std::make_pair(BWAPI::Position(ix,iy),v));
          }
        }
      }
    }
  }
}


float max(float a, float b) {return (a>b) ? a : b;}
float min(float a, float b) {return (a<b) ? a : b;}

double max(double a, double b) {return (a>b) ? a : b;}
double min(double a, double b) {return (a<b) ? a : b;}
