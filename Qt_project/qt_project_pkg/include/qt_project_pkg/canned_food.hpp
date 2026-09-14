#ifndef CANNED_FOOD_HPP_
#define CANNED_FOOD_HPP_

#include "map_manager.hpp"


class CannedFood
{
public:
    CannedFood();

    bool setPosition(
        int row,
        int col,
        MapManager* mapManager
    );

    int getRow();

    int getCol();

    bool isCollected();

    void collect();

private:
    int row_;

    int col_;

    bool collected_;
};

#endif