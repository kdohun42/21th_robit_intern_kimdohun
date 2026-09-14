#include "../include/qt_project_pkg/map_manager.hpp"

#include <QDebug>


MapManager::MapManager()
{
    mapRows_ = 60;
    mapCols_ = 78;

    resetMap();
}


void MapManager::resetMap()
{
    for (int row = 0; row < mapRows_; row++)
    {
        for (int col = 0; col < mapCols_; col++)
        {
            collisionMap_[row][col] = 0;
        }
    }
}


bool MapManager::canPlaceMask(
    std::vector<std::vector<int>>& mask,
    int startRow,
    int startCol)
{
    for (int row = 0; row < static_cast<int>(mask.size()); row++)
    {
        for (
            int col = 0;
            col < static_cast<int>(mask[row].size());
            col++)
        {
            if (mask[row][col] == 0)
            {
                continue;
            }

            int mapRow = startRow + row;
            int mapCol = startCol + col;

            if (
                mapRow < 0 ||
                mapRow >= mapRows_ ||
                mapCol < 0 ||
                mapCol >= mapCols_)
            {
                return false;
            }

            if (collisionMap_[mapRow][mapCol] == 1)
            {
                return false;
            }
        }
    }

    return true;
}


void MapManager::applyMask(
    std::vector<std::vector<int>>& mask,
    int startRow,
    int startCol)
{
    for (int row = 0; row < static_cast<int>(mask.size()); row++)
    {
        for (
            int col = 0;
            col < static_cast<int>(mask[row].size());
            col++)
        {
            if (mask[row][col] == 0)
            {
                continue;
            }

            int mapRow = startRow + row;
            int mapCol = startCol + col;

            if (
                mapRow >= 0 &&
                mapRow < mapRows_ &&
                mapCol >= 0 &&
                mapCol < mapCols_)
            {
                collisionMap_[mapRow][mapCol] = 1;
            }
        }
    }
}


int MapManager::getCollisionValue(
    int row,
    int col)
{
    if (
        row < 0 ||
        row >= mapRows_ ||
        col < 0 ||
        col >= mapCols_)
    {
        return 1;
    }

    return collisionMap_[row][col];
}


int MapManager::getMapRows()
{
    return mapRows_;
}


int MapManager::getMapCols()
{
    return mapCols_;
}


void MapManager::printCollisionMap()
{
    qDebug() << "Collision Map";

    for (int row = 0; row < mapRows_; row++)
    {
        QString line;

        for (int col = 0; col < mapCols_; col++)
        {
            line +=
                QString::number(
                    collisionMap_[row][col]
                );

            line += " ";
        }

        qDebug().noquote() << line;
    }
}
