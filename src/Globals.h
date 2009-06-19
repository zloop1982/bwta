#pragma once
#include <set>
namespace BWTA
{
  class Region;
  class Chokepoint;

  class BWTA_Result
  {
  public:
    static std::set<Region*> regions;
    static std::set<Chokepoint*> chokepoints;
  };
}