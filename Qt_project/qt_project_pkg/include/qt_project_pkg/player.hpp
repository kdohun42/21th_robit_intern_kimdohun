#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "map_manager.hpp"

class Player
{
public:
    Player();

    bool setStartPosition(
        int row,
        int col,
        MapManager* mapManager
    );

    bool move(
        int rowChange,
        int colChange,
        MapManager* mapManager
    );

    void addFood();

    int getFoodCount();

    int getRow();

    int getCol();

private:
    int row_;
    int col_;

    int foodCount_;
};

#endif