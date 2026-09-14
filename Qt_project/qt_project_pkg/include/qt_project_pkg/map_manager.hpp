#ifndef MAP_MANAGER_HPP_
#define MAP_MANAGER_HPP_

#include <vector>

class MapManager
{
public:
    MapManager();

    void resetMap();

    void applyMask(std::vector<std::vector<int>>& mask, int startRow, int startCol);

    bool canPlaceMask(std::vector<std::vector<int>>& mask, int startRow, int startCol);

    int getCollisionValue(int row, int col);

    int getMapRows();
    int getMapCols();

    void printCollisionMap();

private:
    int mapRows_;
    int mapCols_;

    int collisionMap_[60][78];
};

#endif