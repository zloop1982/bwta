#include "Color.h"
#include "VertexData.h"
#include "functions.h"
#include "Graph.h"
#include "Node.h"
#include <BWAPI.h>
#include "BWTA.h"
#include "Globals.h"
#include <BWTA/BaseLocation.h>
using namespace std;
namespace BWTA
{
  namespace BWTA_Result
  {
    std::set<Region*> regions;
    std::set<Chokepoint*> chokepoints;
    std::set<BaseLocation*> baselocations;
  };
  int render();

  class My_observer : public CGAL::Arr_observer<Arrangement_2>
  {
    public:
    My_observer (Arrangement_2& arr) :
      CGAL::Arr_observer<Arrangement_2> (arr)
    {}

    virtual void after_split_edge ( Halfedge_handle e1, Halfedge_handle e2)
    {
      if (e1->data()==BLACK || e1->data()==BLUE || e1->data()==RED)
      {
        e1->twin()->set_data(e1->data());
        e2->set_data(e1->data());
        e2->twin()->set_data(e1->data());
      }
      else if (e2->data()==BLACK || e2->data()==BLUE || e2->data()==RED)
      {
        e2->twin()->set_data(e2->data());
        e1->set_data(e2->data());
        e1->twin()->set_data(e2->data());
      }
      else if (e1->twin()->data()==BLACK || e1->twin()->data()==BLUE || e1->twin()->data()==RED)
      {
        e1->set_data(e1->twin()->data());
        e2->set_data(e1->twin()->data());
        e2->twin()->set_data(e1->twin()->data());
      }
      else if (e2->twin()->data()==BLACK || e2->twin()->data()==BLUE || e2->twin()->data()==RED)
      {
        e2->set_data(e2->twin()->data());
        e1->set_data(e2->twin()->data());
        e1->twin()->set_data(e2->twin()->data());
      }
    }
  };
  void analyze()
  {
    log("Starting to analyze...");
    Util::RectangleArray<bool> walkability;
    Util::RectangleArray<bool> buildability;
    load_map(walkability,buildability);
    log("Loaded map.");
    /*
    std::vector< BWAPI::TilePosition > minerals;
    std::vector< BWAPI::TilePosition > geysers;
    load_resources(minerals,geysers);
    log("Loaded resources.");
    for(int i=0;i<minerals.size();i++)
    {
      log("m");
    }
    std::vector< std::vector< Resource > > clusters;
    find_mineral_clusters(walkability,minerals,geysers,clusters);
    log("Found mineral clusters.");
    for(int i=0;i<clusters.size();i++)
    {
      log("c");
      for(int j=0;j<clusters[i].size();j++)
      {
        if (clusters[i][j].type==1)
        {
          log("r1");
        }
        else
        {
          log("r2");
        }
      }
    }
    Util::RectangleArray<bool> base_build_map;
    calculate_base_build_map(buildability,clusters,base_build_map);
    log("Calculated base build map.");
    calculate_base_locations(walkability,base_build_map,clusters,BWTA_Result::baselocations);
    log("Calculated base locations.");
    */
    vector<PolygonD> polygons;
    extract_polygons(walkability,polygons);
    log("Extracted polygons.");
    for(unsigned int p=0;p<polygons.size();)
    {
      if (abs(polygons[p].area())<=256 && distance_to_border(polygons[p],walkability.getWidth(),walkability.getHeight())>4)
      {
        polygons.erase(polygons.begin()+p);
      }
      else
      {
        p++;
      }
    }
    vector<SDGS2> sites;
    sites.push_back(SDGS2::construct_site_2(PointD(0,0),PointD(0,walkability.getHeight()-1)));
    sites.push_back(SDGS2::construct_site_2(PointD(0,walkability.getHeight()-1),PointD(walkability.getWidth()-1,walkability.getHeight()-1)));
    sites.push_back(SDGS2::construct_site_2(PointD(walkability.getWidth()-1,walkability.getHeight()-1),PointD(walkability.getWidth()-1,0)));
    sites.push_back(SDGS2::construct_site_2(PointD(walkability.getWidth()-1,0),PointD(0,0)));
    SDG2 sdg;
    for(unsigned int i=0;i<sites.size();i++)
    {
      sdg.insert(sites[i]);
    }
    for(unsigned int p=0;p<polygons.size();p++)
    {
      SDG2::Vertex_handle h;
      for(int i=0;i<polygons[p].size();i++)
      {
        int j=(i+1)%polygons[p].size();
        PointD a(polygons[p].vertex(i).x(),polygons[p].vertex(i).y());
        PointD b(polygons[p].vertex(j).x(),polygons[p].vertex(j).y());
        sites.push_back(SDGS2::construct_site_2(b,a));
        if (i==0)
        {
          h=sdg.insert(sites[sites.size()-1]);
        }
        else
        {
          h=sdg.insert(sites[sites.size()-1],h);
        }
      }
    }
    log("Created voronoi diagram.");
    assert( sdg.is_valid(true, 1) );
    cout << endl << endl;
    log("Verified voronoi diagram.");

    vector< Segment > voronoi_diagram_edges;
    std::map<Point, std::set< Point >, ptcmp > nearest;
    std::map<Point, double, ptcmp> distance;
    get_voronoi_edges(sdg,voronoi_diagram_edges,nearest,distance);
    Arrangement_2 arr;
    My_observer obs(arr);
    Graph g(&arr);
    for(unsigned int i=0;i<sites.size();i++)
    {
      CGAL::insert(arr,Segment_2(Point_2(sites[i].segment().vertex(0).x(),sites[i].segment().vertex(0).y()),Point_2(sites[i].segment().vertex(1).x(),sites[i].segment().vertex(1).y())));
    }
    for (Arrangement_2::Edge_iterator eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
    {
      eit->data()=BLACK;
    }
    for (Arrangement_2::Vertex_iterator vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit)
    {
      vit->data().c=BLACK;
    }
    for(unsigned int i=0;i<voronoi_diagram_edges.size();i++)
    {
      NumberType x0(voronoi_diagram_edges[i].vertex(0).x());
      NumberType y0(voronoi_diagram_edges[i].vertex(0).y());
      NumberType x1(voronoi_diagram_edges[i].vertex(1).x());
      NumberType y1(voronoi_diagram_edges[i].vertex(1).y());
      if (x0!=x1 || y0!=y1)
      {
        if (is_real(x0)!=0 && is_real(y0)!=0
         && is_real(x1)!=0 && is_real(y1)!=0)
        {
          bool add=true;
          for(unsigned int p=0;p<polygons.size();p++)
          {
            if (polygons[p].bounded_side(PointD(cast_to_double(voronoi_diagram_edges[i].vertex(0).x()),cast_to_double(voronoi_diagram_edges[i].vertex(0).y())))==CGAL::ON_BOUNDED_SIDE)
            {
              add=false;
              break;
            }
            if (polygons[p].bounded_side(PointD(cast_to_double(voronoi_diagram_edges[i].vertex(1).x()),cast_to_double(voronoi_diagram_edges[i].vertex(1).y())))==CGAL::ON_BOUNDED_SIDE)
            {
              add=false;
              break;
            }
          }
          if (add)
          {
            CGAL::insert(arr,Segment_2(Point_2(x0,y0),Point_2(x1,y1)));
          }
        }
      }
    }
    log("Added voronoi edges.");
    for (Arrangement_2::Edge_iterator eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
    {
      if (eit->data()!=BLACK)
      {
        eit->data()=BLUE;
        eit->twin()->data()=BLUE;
      }
    }
    for (Arrangement_2::Vertex_iterator vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit)
    {
      if (vit->data().c!=BLACK)
      {
        vit->data().c=BLUE;
      }
    }
    bool redo=true;
    while (redo)
    {
      redo=false;
      for (Arrangement_2::Edge_iterator eit = arr.edges_begin(); eit != arr.edges_end();++eit)
      {
        if (eit->data()==BLUE)
        {
          if (eit->source()->data().c==BLACK)
          {
            arr.remove_edge(eit,true,false);
            redo=true;
            break;
          } else if (eit->target()->data().c==BLACK)
          {
            arr.remove_edge(eit,false,true);
            redo=true;
            break;
          }
        }
      }
    }
    log("Removed some useless edges.");
    for (Arrangement_2::Vertex_iterator vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit)
    {
      if (vit->data().c==BLUE)
      {
        Point p(vit->point().x(),vit->point().y());
        double dist_node=distance.find(p)->second;
        vit->data().radius=dist_node;
      }
    }
    redo=true;
    std::set<Arrangement_2::Vertex_handle,vhradiuscmp> vertices;
    while (redo)
    {
      redo=false;
      for (Arrangement_2::Vertex_iterator vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit)
      {
        vertices.insert(vit);
      }
      for(std::set<Arrangement_2::Vertex_handle,vhradiuscmp>::iterator vh=vertices.begin();vh!=vertices.end();vh++)
      {
        if ((*vh)->data().c==BLUE)
        {
          double dist_node=(*vh)->data().radius;
          if ((*vh)->degree()==0)
          {
            if (dist_node<10)
            {
              arr.remove_isolated_vertex(*vh);
            }
          }
          else if ((*vh)->degree()==1)
          {
            Arrangement_2::Halfedge_handle h((*vh)->incident_halfedges());
            double dist_parent;
            if (h->source()==*vh)
            {
              dist_parent=h->target()->data().radius;
            }
            else
            {
              dist_parent=h->source()->data().radius;
            }
            if (dist_node<=dist_parent || dist_node<10)
            {
              arr.remove_edge(h,false,true);
              redo=true;
              break;
            }
          }
        }
      }
      vertices.clear();
    }
    for (Arrangement_2::Vertex_iterator vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit)
    {
      if (vit->data().c==BLUE)
      {
        if (vit->degree()!=2)
        {
          g.add_region(Point(vit->point().x(),vit->point().y()),vit->data().radius,vit);
          vit->data().is_region=true;
        }
        else
        {
          double dist_node=vit->data().radius;
          Arrangement_2::Halfedge_around_vertex_circulator c=vit->incident_halfedges();
          Arrangement_2::Halfedge_handle e1(c);
          c++;
          Arrangement_2::Halfedge_handle e2(c);
          bool region=true;
          if (dist_node<e1->source()->data().radius || (dist_node==e1->source()->data().radius && vit->point()<e1->source()->point()))
          {
            region=false;
          }
          if (dist_node<e2->source()->data().radius || (dist_node==e2->source()->data().radius && vit->point()<e2->source()->point()))
          {
            region=false;
          }
          if (vit->data().radius<16)
          {
            region=false;
          }
          if (region)
          {
            g.add_region(Point(vit->point().x(),vit->point().y()),vit->data().radius,vit);
            vit->data().is_region=true;
          }
        }
      }
    }
    log("Removed useless parts of voronoi diagram.");
    std::list< Segment_2 > new_segments;
    std::set<Node*> chokepoints_to_merge;
    for(std::set<Node*>::iterator r=g.regions_begin();r!=g.regions_end();r++)
    {
      if ((*r)->handle->is_isolated()) continue;
      Arrangement_2::Halfedge_around_vertex_circulator e=(*r)->handle->incident_halfedges();
      Arrangement_2::Halfedge_around_vertex_circulator firste=e;
      do
      {
        Arrangement_2::Vertex_handle node=(*r)->handle;
        Arrangement_2::Vertex_handle chokepoint_node=(*r)->handle;
        bool first=true;
        double min_radius;
        Arrangement_2::Halfedge_around_vertex_circulator mye=e;
        Arrangement_2::Vertex_handle nextnode;
        if (mye->source()==node)
        {
          nextnode=e->target();
        }
        else
        {
          nextnode=e->source();
        }
        Point v(node->point().x(),node->point().y());
        Point w(nextnode->point().x(),nextnode->point().y());
        double distance_before=0;
        double distance_after=0;
        double distance_travelled=0;

        while(node==(*r)->handle || node->data().is_region==false)
        {
          w=v;
          v=Point(node->point().x(),node->point().y());
          double dist=node->data().radius;
          if (first || dist<min_radius || (dist==min_radius && v<w))
          {
            first=false;
            min_radius=dist;
            chokepoint_node=node;
            distance_after=0;
          }

          if (mye->source()==node)
          {
            node=mye->target();
          }
          else
          {
            node=mye->source();
          }
          distance_travelled+=get_distance(mye->target()->point(),mye->source()->point());
          distance_after+=get_distance(mye->target()->point(),mye->source()->point());
          Arrangement_2::Halfedge_around_vertex_circulator newe=node->incident_halfedges();
          if (Arrangement_2::Halfedge_handle(newe)==Arrangement_2::Halfedge_handle(mye->twin()))
          {
            newe++;
          }
          mye=newe;
        }
        distance_before=distance_travelled-distance_after;
        bool is_last=false;
        w=v;
        v=Point(node->point().x(),node->point().y());
        double dist=distance[v];
        if (first || dist<min_radius || (dist==min_radius && v<w))
        {
          first=false;
          min_radius=dist;
          chokepoint_node=node;
          is_last=true;
        }
        Node* new_chokepoint =g.add_chokepoint(Point(chokepoint_node->point().x(),chokepoint_node->point().y()),chokepoint_node->data().radius,chokepoint_node,*r,g.get_region(Point(node->point().x(),node->point().y())));
        bool adding_this=true;
        if (is_last)
        {
          chokepoints_to_merge.insert(new_chokepoint);
          adding_this=false;
        }
        if (chokepoint_node!=(*r)->handle && adding_this)
        {
          NumberType x0=chokepoint_node->point().x();
          NumberType y0=chokepoint_node->point().y();
          std::set<Point> npoints=nearest[Point(x0,y0)];
          if (npoints.size()==2)
          {
            for(std::set<Point>::iterator p=npoints.begin();p!=npoints.end();p++)
            {
              NumberType x1=p->x();
              NumberType y1=p->y();
              new_segments.push_back(Segment_2(Point_2(x0,y0),Point_2(x1,y1)));
            }
            std::set<Point>::iterator p=npoints.begin();
            new_chokepoint->side1=*p;
            p++;
            new_chokepoint->side2=*p;

          }
          else
          {
            double edge_angle=atan2(cast_to_double(w.y()-v.y()),cast_to_double(w.x()-v.x()));
            while (edge_angle>PI) {edge_angle-=2*PI;}
            while (edge_angle<-PI) {edge_angle+=2*PI;}
            double min_pos_angle=100;
            Point min_pos_point;
            bool found_min_point=false;
            double max_neg_angle=-100;
            Point max_neg_point;
            bool found_max_point=false;

            for(std::set<Point>::iterator p=npoints.begin();p!=npoints.end();p++)
            {
              double angle=atan2(cast_to_double(p->y()-v.y()),cast_to_double(p->x()-v.x()));
              angle-=edge_angle;
              while (angle>PI) {angle-=2*PI;}
              while (angle<-PI) {angle+=2*PI;}
              if (angle>0 && angle<min_pos_angle)
              {
                min_pos_angle=angle;
                min_pos_point=*p;
                found_min_point=true;
              }
              if (angle<0 && angle>max_neg_angle)
              {
                max_neg_angle=angle;
                max_neg_point=*p;
                found_max_point=true;
              }
            }
            int added=0;
            if (found_min_point)
            {
              NumberType x1=min_pos_point.x();
              NumberType y1=min_pos_point.y();
              new_segments.push_back(Segment_2(Point_2(x0,y0),Point_2(x1,y1)));
              new_chokepoint->side1=Point(min_pos_point.x(),min_pos_point.y());
              added++;
            }
            if (found_max_point)
            {
              NumberType x1=max_neg_point.x();
              NumberType y1=max_neg_point.y();
              new_segments.push_back(Segment_2(Point_2(x0,y0),Point_2(x1,y1)));
              new_chokepoint->side2=Point(min_pos_point.x(),min_pos_point.y());
              added++;
            }
          }
        }
        e++;
      } while (e!=firste);
    }
    log("Identified chokepoints.");
    for(std::list<Segment_2>::iterator s=new_segments.begin();s!=new_segments.end();s++)
    {
      CGAL::insert(arr,*s);
    }
    for (Arrangement_2::Edge_iterator eit = arr.edges_begin(); eit != arr.edges_end(); ++eit)
    {
      if (eit->data()!=BLACK && eit->data()!=BLUE)
      {
        eit->data()=RED;
        eit->twin()->data()=RED;
      }
    }
    for(std::set<Node*>::iterator i=chokepoints_to_merge.begin();i!=chokepoints_to_merge.end();i++)
    {
      g.merge_chokepoint(*i);
    }
    chokepoints_to_merge.clear();
    bool finished;
    bool all_finished;
    all_finished=false;
    while(!all_finished)
    {
      finished=false;
      while (!finished)
      {
        finished=true;
        Node* chokepoint_to_merge=NULL;
        for(std::set<Node*>::iterator r=g.regions_begin();r!=g.regions_end();r++)
        {
          if ((*r)->neighbors.size()==2)
          {
            std::set<Node*>::iterator j=(*r)->neighbors.begin();
            Node* e1=*j;
            j++;
            Node* e2=*j;
            Node* smaller=e1;
            Node* larger=e2;
            if (larger->radius<smaller->radius)
            {
              smaller=e2;
              larger=e1;
            }
            double larger_radius=larger->radius;
            double smaller_radius=smaller->radius;
            if (smaller_radius > (*r)->radius*0.7 || larger_radius > (*r)->radius*0.8)
            {
              chokepoint_to_merge=larger;
              finished=false;
              break;
            }
          }
        }
        if (chokepoint_to_merge)
        {
          g.merge_chokepoint(chokepoint_to_merge);
        }
      }
      finished=false;
      while (!finished)
      {
        finished=true;
        Node* chokepoint_to_merge=NULL;
        for(std::set<Node*>::iterator c=g.chokepoints_begin();c!=g.chokepoints_end();c++)
        {
          std::set<Node*>::iterator r=(*c)->neighbors.begin();
          Node* r1=*r;
          r++;
          Node* r2=*r;
          Node* smaller=r1;
          Node* larger=r2;
          if (larger->radius<smaller->radius)
          {
            smaller=r2;
            larger=r1;
          }
          double larger_radius=larger->radius+get_distance(larger->point,(*c)->point)*0.1;
          double smaller_radius=smaller->radius+get_distance(smaller->point,(*c)->point)*0.1;
          if ((*c)->radius>larger_radius*0.7 || (*c)->radius>smaller_radius*0.9)
          {
            chokepoint_to_merge=*c;
            finished=false;
            break;
          }
        }
        if (chokepoint_to_merge)
        {
          g.merge_chokepoint(chokepoint_to_merge);
        }
      }
      all_finished=finished;
    }
    redo=true;
    while(redo)
    {
      redo=false;
      for(std::set<Node*>::iterator r=g.regions_begin();r!=g.regions_end();r++)
      {
        if ((*r)->radius<8)
        {
          g.remove_region(*r);
          redo=true;
          break;
        }
      }
    }
    log("Merged regions.");

    redo=true;
    while (redo)
    {
      redo=false;
      for (Arrangement_2::Edge_iterator eit = arr.edges_begin(); eit != arr.edges_end();++eit)
      {
        if (eit->data()==BLUE)
        {
          arr.remove_edge(eit);
          redo=true;
          break;
        }
      }
    }
    log("Removed voronoi edges.");
    for(std::set<BWTA::Region*>::iterator i=BWTA_Result::regions.begin();i!=BWTA_Result::regions.end();i++)
    {
      delete *i;
    }
    BWTA_Result::regions.clear();
    for(std::set<BWTA::Chokepoint*>::iterator i=BWTA_Result::chokepoints.begin();i!=BWTA_Result::chokepoints.end();i++)
    {
      delete *i;
    }
    log("Finding regions.");
    BWTA_Result::chokepoints.clear();
    std::map<Node*,Region*> node2region;
    for(std::set<Node*>::iterator r=g.regions_begin();r!=g.regions_end();r++)
    {
      Polygon poly;
      PolygonD pd=(*r)->get_polygon();
      for(int i=0;i<pd.size();i++)
      {
        poly.push_back(BWAPI::Position((int)(pd[i].x()*8),(int)(pd[i].y()*8)));
      }
      Region* new_region= new Region(poly);
      BWTA_Result::regions.insert(new_region);
      node2region.insert(std::make_pair(*r,new_region));
    }
    log("Finding chokepoints and linking them to regions.");
    std::map<Node*,Chokepoint*> node2chokepoint;
    for(std::set<Node*>::iterator c=g.chokepoints_begin();c!=g.chokepoints_end();c++)
    {
      std::set<Node*>::iterator i=(*c)->neighbors.begin();
      Region* r1=node2region[*i];
      i++;
      Region* r2=node2region[*i];
      BWAPI::Position side1((int)cast_to_double((*c)->side1.x())*8,(int)cast_to_double((*c)->side1.y())*8);
      BWAPI::Position side2((int)cast_to_double((*c)->side2.x())*8,(int)cast_to_double((*c)->side2.y())*8);
      Chokepoint* new_chokepoint= new Chokepoint(std::make_pair(r1,r2),std::make_pair(side1,side2));
      BWTA_Result::chokepoints.insert(new_chokepoint);
      node2chokepoint.insert(std::make_pair(*c,new_chokepoint));
    }
    log("Linking regions to chokepoints.");
    for(std::set<Node*>::iterator r=g.regions_begin();r!=g.regions_end();r++)
    {
      Region* region=node2region[*r];
      std::set<Chokepoint*> chokepoints;
      for(std::set<Node*>::iterator i=(*r)->neighbors.begin();i!=(*r)->neighbors.end();i++)
      {
        chokepoints.insert(node2chokepoint[*i]);
      }
      region->setChokepoints(chokepoints);
    }
    log("Created result sets.");
  }
  std::set<Region*>& getRegions()
  {
    return BWTA_Result::regions;
  }
  std::set<Chokepoint*>& getChokepoints()
  {
    return BWTA_Result::chokepoints;
  }
  std::set<BaseLocation*>& getBaseLocations()
  {
    return BWTA_Result::baselocations;

  }
}