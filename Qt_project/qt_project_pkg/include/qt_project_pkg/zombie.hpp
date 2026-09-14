#ifndef ZOMBIE_HPP_
#define ZOMBIE_HPP_

#include "map_manager.hpp"


class Zombie
{
public:
    Zombie();

    bool setStartPosition(
        int row,
        int col,
        MapManager* mapManager
    );

    bool moveTo(
        int row,
        int col,
        MapManager* mapManager
    );

    int getRow();

    int getCol();

    bool isPlaced();

private:
    int row_;

    int col_;

    bool placed_;
};

#endif