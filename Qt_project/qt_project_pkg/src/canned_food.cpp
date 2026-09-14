#include "../include/qt_project_pkg/canned_food.hpp"


CannedFood::CannedFood()
{
    row_ = 0;

    col_ = 0;

    collected_ = false;
}


bool CannedFood::setPosition(
    int row,
    int col,
    MapManager* mapManager)
{
    // 맵 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 장애물 확인
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


int CannedFood::getRow()
{
    return row_;
}


int CannedFood::getCol()
{
    return col_;
}


bool CannedFood::isCollected()
{
    return collected_;
}


void CannedFood::collect()
{
    collected_ = true;
}