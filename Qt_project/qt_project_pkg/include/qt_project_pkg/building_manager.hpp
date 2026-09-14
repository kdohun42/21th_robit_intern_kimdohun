#ifndef BUILDING_MANAGER_HPP_
#define BUILDING_MANAGER_HPP_

#include <vector>
#include <random>

#include "building.hpp"
#include "map_manager.hpp"


class BuildingManager
{
public:
    BuildingManager(MapManager* mapManager);

    void setupBuildings();

    std::vector<Building>& getGreenBuildings();

    Building& getBunker();

    void printBuildingInfo();

private:
    void createGreenBuildings();

    void createRandomBunker();

    void chooseFoodBuildings();

    bool overlapsWithGreenBuildings(
        int row,
        int col,
        int width,
        int height
    );

    std::vector<std::vector<int>>
    createGreenBuildingMask();

    std::vector<std::vector<int>>
    createBunkerMask();

    MapManager* mapManager_;

    std::vector<Building> greenBuildings_;

    Building bunker_;

    bool bunkerCreated_;

    std::mt19937 randomEngine_;
};

#endif