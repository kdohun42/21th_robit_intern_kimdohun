#ifndef TORCH_HPP_
#define TORCH_HPP_

#include "map_manager.hpp"


class Torch
{
public:
    Torch();

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