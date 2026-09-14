#include "../include/qt_project_pkg/cat.hpp"


Cat::Cat()
{
    row_ = 0;

    col_ = 0;

    placed_ = false;
}


bool Cat::setStartPosition(
    int row,
    int col,
    MapManager* mapManager)
{
    if (mapManager == nullptr)
    {
        return false;
    }


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

    placed_ = true;


    return true;
}


bool Cat::moveTo(
    int row,
    int col,
    MapManager* mapManager)
{
    if (mapManager == nullptr)
    {
        return false;
    }


    // 건물에는 들어갈 수 없음
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


    return true;
}


int Cat::getRow()
{
    return row_;
}


int Cat::getCol()
{
    return col_;
}


bool Cat::isPlaced()
{
    return placed_;
}