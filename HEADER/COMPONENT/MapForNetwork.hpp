#ifndef MAPFORNETWORK_HPP
#define MAPFORNETWORK_HPP

#include "Map.hpp"

class MapForNetwork: public Map {
public:
    MapForNetwork();
    ~MapForNetwork();
    bool addPosible(uint64_t nLines);
    void add(uint64_t nLines, int seed);
};

#endif