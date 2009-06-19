#include "functions.h"
#include "ConnectedComponent.h"
void rotate_ccw(int &x,int &y);
void simplify(PolygonD &polygon, double error_tol);

void find_connected_components(const Util::RectangleArray<bool> &simplified_map
                              ,Util::RectangleArray<ConnectedComponent*> &get_component
                              ,std::list<ConnectedComponent> &components);
void flood_fill_with_component(const Util::RectangleArray<bool> &read_map
                              ,const PointD start
                              ,ConnectedComponent* component
                              ,int fill_type
                              ,Util::RectangleArray<ConnectedComponent*> &write_map);


void extract_polygons(Util::RectangleArray<bool> &walkability,std::vector<PolygonD> &polygons)
{
  PointD pos;
  Util::RectangleArray<ConnectedComponent*> get_component(walkability.getWidth(),walkability.getHeight());
  std::list<ConnectedComponent> components;
  find_connected_components(walkability,get_component,components);
  for(std::list<ConnectedComponent>::iterator c=components.begin();c!=components.end();c++)
  {
    if (c->isWalkable()==false)
    {
      int fromx=1;
      int fromy=0;
      int cx=c->top_left().x();
      int cy=c->top_left().y();
      bool adjcol[3][3];
      PolygonD newpoly;
      newpoly.push_back(PointD(cx,cy));
      bool first=true;
      while(cx!=newpoly.vertices_begin()->x() || cy!=newpoly.vertices_begin()->y() || first) {
        first=false;
        for(int i=0;i<3;i++) {
          for(int j=0;j<3;j++) adjcol[i][j]=true;
        }
        if (cx>0 && cy>0)
          adjcol[0][0]=walkability[cx-1][cy-1];
        if (cy>0)
          adjcol[1][0]=walkability[cx][cy-1];
        if (cx<walkability.getWidth()-1 && cy>0)
          adjcol[2][0]=walkability[cx+1][cy-1];
        if (cx>0)
          adjcol[0][1]=walkability[cx-1][cy];
        if (cx<walkability.getWidth()-1)
          adjcol[2][1]=walkability[cx+1][cy];
        if (cx>0 && cy<walkability.getHeight()-1)
          adjcol[0][2]=walkability[cx-1][cy+1];
        if (cy<walkability.getHeight()-1)
          adjcol[1][2]=walkability[cx][cy+1];
        if (cx<walkability.getWidth()-1 && cy<walkability.getHeight()-1)
          adjcol[2][2]=walkability[cx+1][cy+1];
        int nextx=fromx;
        int nexty=fromy;
        rotate_ccw(nextx,nexty);
        while (adjcol[nextx][nexty]==false) {
          rotate_ccw(nextx,nexty);
        }
        while (adjcol[nextx][nexty]==true) {
          rotate_ccw(nextx,nexty);
        }
        cx=cx+nextx-1;
        cy=cy+nexty-1;
        fromx=2-nextx;
        fromy=2-nexty;
        newpoly.push_back(PointD(cx,cy));
      }
      simplify(newpoly,0.8);
      polygons.push_back(newpoly);
    }
  }
}

void rotate_ccw(int &x,int &y) {
  if (x==0 && y==0) {
    y=1;
    return;
  }
  if (x==0 && y==1) {
    y=2;
    return;
  }
  if (x==0 && y==2) {
    x=1;
    return;
  }
  if (x==1 && y==2) {
    x=2;
    return;
  }
  if (x==2 && y==2) {
    y=1;
    return;
  }
  if (x==2 && y==1) {
    y=0;
    return;
  }
  if (x==2 && y==0) {
    x=1;
    return;
  }
  if (x==1 && y==0) {
    x=0;
    return;
  }
}

void simplify(PolygonD &polygon, double error_tol)
{
  polygon.push_back(polygon.vertex(0));
  for(PolygonD::Vertex_iterator i=polygon.vertices_begin();i!=polygon.vertices_end();i++ )
  {
    PolygonD::Vertex_iterator ip1=i;
    ip1++;
    PolygonD::Vertex_iterator j=ip1;
    PolygonD::Vertex_iterator last_good_point=j;
    bool within_tol=true;
    while (within_tol && j!=polygon.vertices_end())
    {
      j++;
      if (j==polygon.vertices_end())
      {
        break;
      }
      LineD line;
      line=LineD(*i,*j);
      for(PolygonD::Vertex_iterator k=ip1;k!=j;k++)
      {
        PointD projection(line.projection(*k));
        if (get_distance(projection,*k)>=error_tol)
        {
          within_tol=false;
          break;
        }
      }
      if (within_tol)
      {
        last_good_point=j;
      }
    }
    if (ip1!=last_good_point)
      polygon.erase(ip1,last_good_point);
  }
  if (polygon.vertex(0)==polygon.vertex(polygon.size()-1))
  {
    PolygonD::Vertex_iterator last=polygon.vertices_begin();
    PolygonD::Vertex_iterator next=last;
    next++;
    while (next!=polygon.vertices_end())
    {
      last=next;
      next++;
    }
    polygon.erase(last);
  }
}

void find_connected_components(const Util::RectangleArray<bool> &simplified_map
                              ,Util::RectangleArray<ConnectedComponent*> &get_component
                              ,std::list<ConnectedComponent> &components)
{
  int currentID=1;
  components.clear();
  get_component.resize(simplified_map.getWidth(),simplified_map.getHeight());
  for (int x = 0; x < simplified_map.getWidth(); x++) {
    for (int y = 0; y < simplified_map.getHeight(); y++) {
      get_component[x][y]=NULL;
    }
  }
  for (int x = 0; x < simplified_map.getWidth(); x++) {
    for (int y = 0; y < simplified_map.getHeight(); y++) {
      if (get_component[x][y] == NULL) {
        components.push_back(ConnectedComponent(currentID++,simplified_map[x][y]));
        ConnectedComponent *current_component=&(components.back());
        int fill_type=0;
        if (simplified_map[x][y]==0) fill_type=2;
        current_component->top_left()=PointD(x,y);
        flood_fill_with_component(simplified_map,PointD(x,y),current_component,fill_type,get_component);
      }
    }
  }
}

void flood_fill_with_component(const Util::RectangleArray<bool> &read_map
                              ,const PointD start
                              ,ConnectedComponent* component
                              ,int fill_type
                              ,Util::RectangleArray<ConnectedComponent*> &write_map)
{
  if (component==NULL)
    return;
  int fill_count=0;
  std::list< PointD > q;
  q.push_back(start);
  while (!q.empty())
  {
    PointD p=q.front();
    if (p.x()<component->top_left().x() || (p.x()==component->top_left().x() && p.y()<component->top_left().y()))
    {
      component->top_left()=p;
    }
    int x=(int)p.x();
    int y=(int)p.y();
    q.pop_front();
    if (write_map[x][y]==NULL)
    {
      write_map[x][y]=component;
      int min_x=max(x-1,0);
      int max_x=min(x+1,read_map.getWidth()-1);
      int min_y=max(y-1,0);
      int max_y=min(y+1,read_map.getHeight()-1);
      for(int ix=min_x;ix<=max_x;ix++)
      {
        for(int iy=min_y;iy<=max_y;iy++)
        {
          int fill=0;
          if (fill_type==0) {//4-directional movement
            if (x==ix||y==iy) {
              if (read_map[ix][iy]==component->isWalkable() && write_map[ix][iy]==NULL)
              {
                fill=1;
              }
            }
          } else if (fill_type==1) {//limited 8-directional movement
            if (x==ix||y==iy || read_map[x][iy]==component->isWalkable()
                             || read_map[ix][y]==component->isWalkable()) {
              if (read_map[ix][iy]==component->isWalkable() && write_map[ix][iy]==NULL)
              {
                fill=1;
              }
            }
            
          } else if (fill_type==2) {//full 8-directional movement
            if (read_map[ix][iy]==component->isWalkable() && write_map[ix][iy]==NULL)
            {
              fill=1;
            }
          }
          if (fill) {
            fill_count++;
            q.push_back(PointD(ix,iy));
          }
        }
      }
    }
  }
}
