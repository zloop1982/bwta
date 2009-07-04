#include "RectangleArray.h"
#include <list>
#include <vector>
#include <set>
#include <BWAPI.h>
#include <BWTA.h>
#include "ConnectedComponent.h"
int find_mineral_clusters(const Util::RectangleArray<bool> &simplified_map
                          ,const std::set< BWAPI::Unit* > &minerals
                         ,const std::set< BWAPI::Unit* > &geysers
                         ,std::vector< std::vector< BWAPI::Unit* > > &resource_clusters);

void calculate_base_build_map(const Util::RectangleArray<bool> &build_map
                             ,const std::vector< std::vector< BWAPI::Unit* > > &resource_clusters
                             ,Util::RectangleArray<bool> &base_build_map);

void calculate_base_locations(const Util::RectangleArray<bool> &simplified_map
                             ,const Util::RectangleArray<bool> &base_build_map
                             ,const std::vector< std::vector< BWAPI::Unit* > > &resource_clusters
                             ,std::set< BWTA::BaseLocation* > &base_locations);

void calculate_base_location_properties(const Util::RectangleArray<bool> &walk_map
                                       ,const Util::RectangleArray<ConnectedComponent*> &get_component
                                       ,const std::list<ConnectedComponent> &components
                                       ,const std::set<BWAPI::Unit*> &minerals
                                       ,const std::set<BWAPI::Unit*> &geysers
                                       ,std::set< BWTA::BaseLocation* > &base_locations);