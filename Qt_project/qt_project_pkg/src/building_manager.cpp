#include "../include/qt_project_pkg/building_manager.hpp"

#include <algorithm>
#include <QDebug>


BuildingManager::BuildingManager(
    MapManager* mapManager)
{
    mapManager_ = mapManager;

    bunkerCreated_ = false;

    std::random_device randomDevice;

    randomEngine_.seed(
        randomDevice()
    );
}


void BuildingManager::setupBuildings()
{
    mapManager_->resetMap();

    greenBuildings_.clear();

    createGreenBuildings();

    createRandomBunker();

    chooseFoodBuildings();
}


std::vector<std::vector<int>>
BuildingManager::createGreenBuildingMask()
{
    // 건물 모양을 직접 수정하는 부분

    std::vector<std::vector<int>> mask =
    {
        {0, 0, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 1, 1},
        {1, 1, 0, 0, 1, 1}
    };

    return mask;
}


std::vector<std::vector<int>>
BuildingManager::createBunkerMask()
{
    // 벙커 모양을 직접 수정하는 부분

    std::vector<std::vector<int>> mask =
    {
        {0, 0, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 1, 1},
        {1, 1, 0, 0, 1, 1}
    };

    return mask;
}


void BuildingManager::createGreenBuildings()
{
    int buildingRows[8] =
    {
        6,
        6,
        6,
        24,
        24,
        42,
        42,
        42
    };

    int buildingCols[8] =
    {
        6+8,
        26+8,
        46+8,
        10+8,
        40+8,
        6+8,
        26+8,
        46+8
    };


    for (int i = 0; i < 8; i++)
    {
        Building building(
            buildingRows[i],
            buildingCols[i],
            6,
            7,
            GREEN_BUILDING
        );

        std::vector<std::vector<int>> mask = createGreenBuildingMask();

        building.setCollisionMask(mask);

        greenBuildings_.push_back(building);

        mapManager_->applyMask(
            greenBuildings_.back().getCollisionMask(),
            buildingRows[i],
            buildingCols[i]
        );
    }
}


bool BuildingManager::overlapsWithGreenBuildings(
    int row,
    int col,
    int width,
    int height)
{
    int margin = 3;

    for (
        int i = 0;
        i < static_cast<int>(greenBuildings_.size());
        i++)
    {
        int otherRow =
            greenBuildings_[i].getRow();

        int otherCol =
            greenBuildings_[i].getCol();

        int otherWidth =
            greenBuildings_[i].getWidth();

        int otherHeight =
            greenBuildings_[i].getHeight();


        bool separated =
            col + width + margin <= otherCol ||
            otherCol + otherWidth + margin <= col ||
            row + height + margin <= otherRow ||
            otherRow + otherHeight + margin <= row;


        if (!separated)
        {
            return true;
        }
    }

    return false;
}


void BuildingManager::createRandomBunker()
{
    int bunkerWidth = 6;
    int bunkerHeight = 7;

    std::vector<std::vector<int>> bunkerMask =
        createBunkerMask();


    std::uniform_int_distribution<int> rowRandom(
        2,
        mapManager_->getMapRows() -
        bunkerHeight -
        2
    );


    std::uniform_int_distribution<int> colRandom(
        2,
        mapManager_->getMapCols() -
        bunkerWidth -
        2
    );


    for (int attempt = 0; attempt < 1000; attempt++)
    {
        int row =
            rowRandom(randomEngine_);

        int col =
            colRandom(randomEngine_);


        if (
            overlapsWithGreenBuildings(
                row,
                col,
                bunkerWidth,
                bunkerHeight))
        {
            continue;
        }


        if (
            !mapManager_->canPlaceMask(
                bunkerMask,
                row,
                col))
        {
            continue;
        }


        bunker_ = Building(
            row,
            col,
            bunkerWidth,
            bunkerHeight,
            RED_BUNKER
        );


        bunker_.setCollisionMask(
            bunkerMask
        );


        mapManager_->applyMask(
            bunker_.getCollisionMask(),
            row,
            col
        );


        bunkerCreated_ = true;

        return;
    }


    qDebug()
        << "벙커를 배치할 공간을 찾지 못했습니다.";
}


void BuildingManager::chooseFoodBuildings()
{
    std::vector<int> buildingIndexes;

    for (int i = 0; i < 8; i++)
    {
        buildingIndexes.push_back(i);

        greenBuildings_[i].setHasFood(
            false
        );
    }


    std::shuffle(
        buildingIndexes.begin(),
        buildingIndexes.end(),
        randomEngine_
    );


    for (int i = 0; i < 3; i++)
    {
        int buildingIndex =
            buildingIndexes[i];

        greenBuildings_[buildingIndex]
            .setHasFood(true);
    }
}


std::vector<Building>&
BuildingManager::getGreenBuildings()
{
    return greenBuildings_;
}


Building&
BuildingManager::getBunker()
{
    return bunker_;
}


void BuildingManager::printBuildingInfo()
{
    qDebug()
        << "========== Building ==========";


    for (
        int i = 0;
        i < static_cast<int>(greenBuildings_.size());
        i++)
    {
        qDebug()
            << "Green Building"
            << i
            << "row:"
            << greenBuildings_[i].getRow()
            << "col:"
            << greenBuildings_[i].getCol()
            << "food:"
            << greenBuildings_[i].getHasFood();
    }


    if (bunkerCreated_)
    {
        qDebug()
            << "Red Building"
            << "row:"
            << bunker_.getRow()
            << "col:"
            << bunker_.getCol();
    }


    qDebug()
        << "==============================";
}