#ifndef BUILDING_HPP_
#define BUILDING_HPP_

#include <vector>

enum BuildingType
{
    GREEN_BUILDING,
    RED_BUNKER
};

class Building
{
public:
    Building();

    Building(
        int row,
        int col,
        int width,
        int height,
        BuildingType type
    );

    void setRow(int row);

    void setCol(int col);

    int getRow();

    int getCol();

    int getWidth();

    int getHeight();

    void setHasFood(bool hasFood);

    bool getHasFood();

    void setSearched(bool searched);

    bool getSearched();

    BuildingType getType();

    void setCollisionMask(
        std::vector<std::vector<int>> mask
    );

    std::vector<std::vector<int>>& getCollisionMask();

    bool isDoorPosition(
        int playerRow,
        int playerCol
    );

private:
    int row_;
    int col_;

    int width_;
    int height_;

    bool hasFood_;

    bool searched_;

    BuildingType type_;

    std::vector<std::vector<int>> collisionMask_;
};

#endif