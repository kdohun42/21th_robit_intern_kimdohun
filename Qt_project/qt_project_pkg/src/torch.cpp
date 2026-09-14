#include "../include/qt_project_pkg/torch.hpp"


Torch::Torch()
{
    // 초기 위치
    row_ = 0;

    col_ = 0;


    // 획득 여부
    collected_ = false;
}


bool Torch::setPosition(
    int row,
    int col,
    MapManager* mapManager)
{
    // 맵 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 장애물 위치에는 배치하지 않음
    if (
        mapManager->
        getCollisionValue(
            row,
            col
        ) == 1)
    {
        return false;
    }


    row_ = row;

    col_ = col;

    collected_ = false;


    return true;
}


int Torch::getRow()
{
    return row_;
}


int Torch::getCol()
{
    return col_;
}


bool Torch::isCollected()
{
    return collected_;
}


void Torch::collect()
{
    collected_ = true;
}