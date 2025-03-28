#ifndef MAPFORNETWORK_HPP
#define MAPFORNETWORK_HPP

#include "Map.hpp"

class MapForNetwork: public Map {
    
public:
    void add(uint64_t nLines, int seed);
};

#endif