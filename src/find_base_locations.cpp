#include "functions.h"
#include <BWAPI.h>
#include <BWTA.h>
#include "BWTA_Result.h"
#include "BaseLocationImpl.h"
#include "RegionImpl.h"
#include "ConnectedComponent.h"
void find_mineral_clusters(const Util::RectangleArray<bool> &simplified_map
                         ,const std::set< BWAPI::Unit* > &minerals
                         ,const std::set< BWAPI::Unit* > &geysers
                         ,std::vector< std::vector< BWAPI::Unit* > > &resource_clusters)
{
  //minerals less than this distance will be grouped into the same cluster
  int mineral_cluster_distance=6*4;

  //a cluster with less than 4 mineral patches will be considered useless
  int mineral_group_discard_size=3;


  Util::RectangleArray<int> distance_map;
  std::vector<BWAPI::Unit*> resources;
  for(std::set<BWAPI::Unit*>::const_iterator i=minerals.begin();i!=minerals.end();i++)
  {
    resources.push_back(*i);
  }
  for(std::set<BWAPI::Unit*>::const_iterator i=geysers.begin();i!=geysers.end();i++)
  {
    resources.push_back(*i);
  }
  
  distance_map.resize(simplified_map.getWidth(),simplified_map.getHeight());

  //we will now group the minerals into clusters based on distance
  //we begin by putting each mineral in its own group
  std::vector<int> resource_set;
  for(unsigned int i=0;i<resources.size();i++) {
    resource_set.push_back(i);
  }
  //we will now join sets of minerals that are close together
  for(unsigned int index=0;index<resources.size();index++) {
    BWAPI::Position pos=BWAPI::Position(resources[index]->getTilePosition().x()*4+4,resources[index]->getTilePosition().y()*4+2);
    if (resources[index]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser) {
      pos=BWAPI::Position(resources[index]->getTilePosition().x()*4+8,resources[index]->getTilePosition().y()*4+4);
    }
    log("Walk tile position of resource %d: (%d,%d)",index,pos.x(),pos.y());
    //this will flood the nearby tiles with their distances to the current mineral
    calculate_walk_distances(simplified_map,pos,(mineral_cluster_distance+4*4)*10,distance_map);
    //lets look at some other minerals and see if they are close enough to
    //merge with this one
    for(unsigned int index2=index+1;index2<resources.size();index2++) {
      BWAPI::Position pos2=BWAPI::Position(resources[index2]->getTilePosition().x()*4+4,resources[index2]->getTilePosition().y()*4+2);
      if (resources[index2]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser) {
        pos2=BWAPI::Position(resources[index2]->getTilePosition().x()*4+8,resources[index2]->getTilePosition().y()*4+4);
      }
      //merge only if less than this distance
      int x2=(int)pos2.x();
      int y2=(int)pos2.y();
      int dist=distance_map[x2][y2];
      int augmented_cluster_distance=mineral_cluster_distance;
      log("distance between %d and %d: %d",index,index2,dist);

      if (resources[index]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser || resources[index2]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser) {
        //vespene geysers are often farther away from minerals than minerals 
        //are from each other. So we add some extra distance for vespene geyers
        augmented_cluster_distance+=3*4;
      }
      if (dist>=0 && dist<augmented_cluster_distance*10) {
        resource_set[get_set(resource_set,index2)]=get_set(resource_set,index);
      }
    }
  }

  //we will now map the cluster IDs to indices
  //first we will find the unique clusters
  std::map<int,int> unique_clusters;
  for(unsigned int index=0;index<resources.size();index++) {
    int set_index=get_set(resource_set,index);
    unique_clusters.insert(std::make_pair(set_index,0));
  }
  //now we will set their index values
  int index=0;
  for(std::map<int,int>::iterator iter=unique_clusters.begin();
    iter!=unique_clusters.end();iter++) {
    (*iter).second=index;
    index++;
  }
  //here we make the resource clusters vector of vectors large enough
  for(unsigned int i=0;i<unique_clusters.size();i++) {
    std::vector<BWAPI::Unit*> b;
    resource_clusters.push_back(b);
  }
  //fill the resource clusters array
  for(unsigned int index=0;index<resources.size();index++) {
    int set_index=(*unique_clusters.find(get_set(resource_set,index))).second;
    resource_clusters[set_index].push_back(resources[index]);
  }
  //remove resource clusters that are too small
  for(unsigned int index=0;index<resource_clusters.size();) {
    if ((int)resource_clusters[index].size()<=mineral_group_discard_size) {
      resource_clusters.erase(resource_clusters.begin()+index);
    } else {
      index++;
    }
  }
}

void calculate_base_build_map(const Util::RectangleArray<bool> &build_map
                              ,const std::vector< std::vector< BWAPI::Unit* > > &resource_clusters
                             ,Util::RectangleArray<bool> &base_build_map)
{
  base_build_map.resize(build_map.getWidth(),build_map.getHeight());
  for(int x=0;x<(int)build_map.getWidth();x++) {
    for(int y=0;y<(int)build_map.getHeight();y++) {
      int max_x=min(x+3,(int)build_map.getWidth()-1);
      int max_y=min(y+2,(int)build_map.getHeight()-1);
      base_build_map[x][y]=true;
      for(int ix=x;ix<=max_x;ix++) {
        for(int iy=y;iy<=max_y;iy++) {
          base_build_map[x][y]&=build_map[ix][iy];
        }
      }
      if (x+3>=(int)build_map.getWidth() || y+2>=(int)build_map.getHeight()) {
        base_build_map[x][y]=false;
      }
    }
  }
  for(int i=0;i<(int)resource_clusters.size();i++) {
    for(int j=0;j<(int)resource_clusters[i].size();j++) {
      int x=(int)resource_clusters[i][j]->getTilePosition().x();
      int y=(int)resource_clusters[i][j]->getTilePosition().y();
      if (resource_clusters[i][j]->getType()==BWAPI::UnitTypes::Resource_Mineral_Field) {
        int min_x=max(x-6,0);
        int max_x=min(x+4,(int)build_map.getWidth()-1);
        int min_y=max(y-5,0);
        int max_y=min(y+3,(int)build_map.getHeight()-1);
        for(int ix=min_x;ix<=max_x;ix++) {
          for(int iy=min_y;iy<=max_y;iy++) {
            base_build_map[ix][iy]=false;
          }
        }
      } else {
        int min_x=max(x-6,0);
        int max_x=min(x+6,build_map.getWidth()-1);
        int min_y=max(y-5,0);
        int max_y=min(y+4,build_map.getHeight()-1);
        for(int ix=min_x;ix<=max_x;ix++) {
          for(int iy=min_y;iy<=max_y;iy++) {
            base_build_map[ix][iy]=false;
          }
        }
      }
    }
  }
}


void calculate_base_locations(const Util::RectangleArray<bool> &simplified_map
                             ,const Util::RectangleArray<bool> &base_build_map
                             ,const std::vector< std::vector< BWAPI::Unit* > > &resource_clusters
                             ,std::set< BWTA::BaseLocation* > &base_locations)
{
  //Now that we have the map of the locations where we can place a command center
  //And have the clusters of minerals, we will try to find a base location for each cluster
  int max_influence_distance=12;
  Util::RectangleArray<double> tile_scores;
  Util::RectangleArray<int> distance_map;
  tile_scores.resize(base_build_map.getWidth(),base_build_map.getHeight());
  distance_map.resize(simplified_map.getWidth(),simplified_map.getHeight());

  for(unsigned int i=0;i<resource_clusters.size();i++) {
    for(int x=0;x<(int)tile_scores.getWidth();x++) {
      for(int y=0;y<(int)tile_scores.getHeight();y++) {
        tile_scores[x][y]=0;
      }
    }
    for(unsigned int j=0;j<resource_clusters[i].size();j++) {
      BWAPI::Position pos(resource_clusters[i][j]->getTilePosition().x()*4+4,resource_clusters[i][j]->getTilePosition().y()*4+2);
      if (resource_clusters[i][j]->getType()==BWAPI::UnitTypes::Resource_Vespene_Geyser) {
        pos=BWAPI::Position(resource_clusters[i][j]->getTilePosition().x()*4+8,resource_clusters[i][j]->getTilePosition().y()*4+4);
      }
      //this will flood the nearby tiles with their distances to the current mineral
      calculate_walk_distances(simplified_map,pos,max_influence_distance*4*10,distance_map);
      int x=(int)pos.x()/4;
      int y=(int)pos.y()/4;
      int min_x=max(x-max_influence_distance,0);
      int max_x=min(x+max_influence_distance,base_build_map.getWidth()-1);
      int min_y=max(y-max_influence_distance,0);
      int max_y=min(y+max_influence_distance,base_build_map.getHeight()-1);
      for(int ix=min_x;ix<=max_x;ix++) {
        for(int iy=min_y;iy<=max_y;iy++) {
          if (base_build_map[ix][iy]) {
            double distance=100000;
            for(int tx=ix*4;tx<(ix+4)*4;tx++) {
              for(int ty=iy*4;ty<(iy+3)*4;ty++) {
                if (distance_map[tx][ty]>=0) {
                  distance=min(distance,(double)distance_map[tx][ty]);
                }
              }
            }
            if (distance>=0) {
              double score=20.0*4*10.0-distance;
              tile_scores[ix][iy]+=score;
            }
          }
        }
      }
    }
    BWAPI::TilePosition maximum(-1,-1);
    double max_score=0;
    for(int x=0;x<(int)tile_scores.getWidth();x++) {
      for(int y=0;y<(int)tile_scores.getHeight();y++) {
        if (tile_scores[x][y]>max_score) {
          max_score=tile_scores[x][y];
          maximum=BWAPI::TilePosition(x,y);
        }
      }
    }
    log("max score %d",max_score);
    if (max_score>0) {
      base_locations.insert(new BWTA::BaseLocationImpl(maximum,resource_clusters[i]));
    }
  }
}

void calculate_base_location_properties(const Util::RectangleArray<bool> &walk_map
                                       ,const Util::RectangleArray<ConnectedComponent*> &get_component
                                       ,const std::list<ConnectedComponent> &components
                                       ,const std::set<BWAPI::Unit*> &minerals
                                       ,const std::set<BWAPI::Unit*> &geysers
                                       ,std::set< BWTA::BaseLocation* > &base_locations)
{
  Util::RectangleArray<int> distance_map(walk_map.getWidth(),walk_map.getHeight());
  for(std::set<BWTA::BaseLocation*>::iterator i=base_locations.begin();i!=base_locations.end();i++) {
    BWAPI::Position p((*i)->getTilePosition().x()*4,(*i)->getTilePosition().y()*4);
    calculate_walk_distances_area(walk_map,p,16,12,0,distance_map);
    BWTA::BaseLocationImpl* ii=(BWTA::BaseLocationImpl*)(*i);
    for(std::set<BWAPI::Unit*>::const_iterator j=geysers.begin();j!=geysers.end();j++) {
      int x=(int)(*j)->getTilePosition().x()*4+8;
      int y=(int)(*j)->getTilePosition().y()*4+4;
      if (distance_map[x][y]>=0 && distance_map[x][y]<=4*10*10) {
        ii->geysers.insert(*j);
      }
    }
    for(std::set<BWAPI::Unit*>::const_iterator j=minerals.begin();j!=minerals.end();j++) {
      int x=(int)(*j)->getTilePosition().x()*4+4;
      int y=(int)(*j)->getTilePosition().y()*4+2;
      if (distance_map[x][y]>=0 && distance_map[x][y]<=4*10*10) {
        ii->minerals.insert(*j);
      }
    }
    ii->island=true;
    for(std::set<BWTA::BaseLocation*>::iterator j=base_locations.begin();j!=base_locations.end();j++) {
      if (*j!=*i) {
        int x=(int)(*j)->getTilePosition().x()*4+8;
        int y=(int)(*j)->getTilePosition().y()*4+6;
        if (distance_map[x][y]>0) {
          ii->island=false;
        }
        ii->ground_distances[*j]=distance_map[x][y]*0.8;
        ii->air_distances[*j]=(*i)->getPosition().getDistance((*j)->getPosition());
      }
    }

    for(std::set<BWTA::Region*>::iterator r=BWTA::BWTA_Result::regions.begin();r!=BWTA::BWTA_Result::regions.end();r++)
    {
      if ((*r)->getPolygon().isInside(ii->getPosition()))
      {
        ii->region=*r;
        ((BWTA::RegionImpl*)(*r))->baseLocations.insert(*i);
        break;
      }
    }

  }
}
