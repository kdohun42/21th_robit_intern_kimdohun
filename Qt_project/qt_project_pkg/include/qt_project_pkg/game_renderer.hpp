#ifndef GAME_RENDERER_HPP_
#define GAME_RENDERER_HPP_

#include <QPixmap>
#include <QString>
#include <QPainter>

#include <vector>

#include "building_manager.hpp"
#include "map_manager.hpp"
#include "player.hpp"
#include "zombie.hpp"
#include "cat.hpp"
#include "canned_food.hpp"
#include "torch.hpp"


class GameRenderer
{
public:
    GameRenderer();

    bool loadImages(
        QString tileSheetPath,
        QString greenBuildingPath,
        QString redBuildingPath,
        QString playerSheetPath,
        QString zombieSheetPath,
        QString catImagePath,
        QString cannedFoodImagePath,
        QString torchImagePath
    );

    QPixmap renderMap(
        BuildingManager* buildingManager,
        MapManager* mapManager,
        Player* player,
        std::vector<Zombie>& zombies,
        std::vector<Cat>& cats,
        std::vector<CannedFood>& cannedFoods,
        std::vector<Torch>& torches,
        int visionRadius
    );

    void setCollisionDebug(
        bool enabled
    );

private:
    void drawBuilding(
        QPainter& painter,
        Building& building,
        QPixmap& image
    );

    void drawCollisionMap(
        QPainter& painter,
        MapManager* mapManager
    );

    void drawPlayer(
        QPainter& painter,
        Player* player
    );

    void drawZombie(
        QPainter& painter,
        Zombie& zombie
    );

    void drawCat(
        QPainter& painter,
        Cat& cat
    );

    void drawCannedFood(
        QPainter& painter,
        CannedFood& cannedFood
    );

    // 어둠 출력
    void drawDarkness(
        QPainter& painter,
        Player* player,
        int visionRadius
    );

    void drawTorch(
        QPainter& painter,
        Torch& torch
    );

    int mapRows_;

    int mapCols_;


    int sourceTileSize_;

    int displayTileSize_;


    int playerFrameWidth_;

    int playerFrameHeight_;


    int zombieFrameWidth_;

    int zombieFrameHeight_;


    int catWidth_;

    int catHeight_;


    int cannedFoodWidth_;

    int cannedFoodHeight_;

    int torchWidth_;

    int torchHeight_;

    QPixmap torchImage_;


    bool collisionDebug_;


    QPixmap tileSheet_;

    QPixmap grassTile_;

    QPixmap greenBuildingImage_;

    QPixmap redBuildingImage_;

    QPixmap playerSheet_;

    QPixmap playerFrame_;

    QPixmap zombieSheet_;

    QPixmap zombieFrame_;

    QPixmap catImage_;

    QPixmap cannedFoodImage_;
};

#endif