#pragma once;
#include "ConnectedComponent.h"
#include "functions.h"
void find_connected_components(const Util::RectangleArray<bool> &simplified_map
                              ,Util::RectangleArray<ConnectedComponent*> &get_component
                              ,std::list<ConnectedComponent> &components);
void extract_polygons(const Util::RectangleArray<bool> &walkability
                     ,const std::list<ConnectedComponent> &components
                     ,std::vector<PolygonD> &polygons);

void flood_fill_with_component(const Util::RectangleArray<bool> &read_map
                              ,const PointD start
                              ,ConnectedComponent* component
                              ,int fill_type
                              ,Util::RectangleArray<ConnectedComponent*> &write_map);