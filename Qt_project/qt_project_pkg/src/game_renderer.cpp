#include "../include/qt_project_pkg/game_renderer.hpp"

#include <QDebug>
#include <QColor>
#include <QBrush>


GameRenderer::GameRenderer()
{
    // 맵 크기
    mapRows_ =
        60;

    mapCols_ =
        78;


    // 타일 크기
    sourceTileSize_ =
        16;

    displayTileSize_ =
        16;


    // 플레이어 이미지 크기
    playerFrameWidth_ =
        11;

    playerFrameHeight_ =
        17;


    // 좀비 이미지 크기
    zombieFrameWidth_ =
        16;

    zombieFrameHeight_ =
        23;


    // 고양이 이미지 크기
    catWidth_ =
        24;

    catHeight_ =
        19;


    // 통조림 크기
    cannedFoodWidth_ =
        18;

    cannedFoodHeight_ =
        18;


    // 횃불 출력 크기
    torchWidth_ =
        18;

    torchHeight_ =
        26;


    // 충돌맵 출력 여부
    collisionDebug_ =
        false;
}


bool GameRenderer::loadImages(
    QString tileSheetPath,
    QString greenBuildingPath,
    QString redBuildingPath,
    QString playerSheetPath,
    QString zombieSheetPath,
    QString catImagePath,
    QString cannedFoodImagePath,
    QString torchImagePath)
{
    // 배경
    tileSheet_.
        load(
            tileSheetPath
        );


    // 초록 건물
    greenBuildingImage_.
        load(
            greenBuildingPath
        );


    // 빨간 벙커
    redBuildingImage_.
        load(
            redBuildingPath
        );


    // 플레이어
    playerSheet_.
        load(
            playerSheetPath
        );


    // 좀비
    zombieSheet_.
        load(
            zombieSheetPath
        );


    // 고양이
    catImage_.
        load(
            catImagePath
        );


    // 통조림
    cannedFoodImage_.
        load(
            cannedFoodImagePath
        );

    // 횃불 이미지
    torchImage_.
        load(
            torchImagePath
        );


    // 이미지 로딩 검사
    if (tileSheet_.isNull())
    {
        qDebug()
            << "배경 이미지 로딩 실패";

        return false;
    }


    if (greenBuildingImage_.isNull())
    {
        qDebug()
            << "초록 건물 이미지 로딩 실패";

        return false;
    }


    if (redBuildingImage_.isNull())
    {
        qDebug()
            << "빨간 건물 이미지 로딩 실패";

        return false;
    }


    if (playerSheet_.isNull())
    {
        qDebug()
            << "플레이어 이미지 로딩 실패";

        return false;
    }


    if (zombieSheet_.isNull())
    {
        qDebug()
            << "좀비 이미지 로딩 실패";

        return false;
    }


    if (catImage_.isNull())
    {
        qDebug()
            << "고양이 이미지 로딩 실패";

        return false;
    }


    if (cannedFoodImage_.isNull())
    {
        qDebug()
            << "통조림 이미지 로딩 실패";

        return false;
    }

    if (torchImage_.isNull())
    {
        qDebug()
            << "횃불 이미지 로딩 실패:"
            << torchImagePath;

        return false;
    }

    // 잔디 타일
    grassTile_ =
        tileSheet_.
        copy(
            5 *
            sourceTileSize_,

            0,

            sourceTileSize_,

            sourceTileSize_
        );


    grassTile_ =
        grassTile_.
        scaled(
            displayTileSize_,
            displayTileSize_,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation
        );


    // 플레이어 첫 프레임
    playerFrame_ =
        playerSheet_.
        copy(
            0,
            0,
            playerFrameWidth_,
            playerFrameHeight_
        );


    // 좀비 첫 프레임
    zombieFrame_ =
        zombieSheet_.
        copy(
            0,
            0,
            zombieFrameWidth_,
            zombieFrameHeight_
        );


    // 고양이 크기 변경
    catImage_ =
        catImage_.
        scaled(
            catWidth_,
            catHeight_,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation
        );


    // 통조림 크기 변경
    cannedFoodImage_ =
        cannedFoodImage_.
        scaled(
            cannedFoodWidth_,
            cannedFoodHeight_,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation
        );


        // 횃불 이미지 크기 변경
    torchImage_ =
        torchImage_.
        scaled(
            torchWidth_,
            torchHeight_,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation
        );


    qDebug()
        << "게임 이미지 로딩 성공";


    return true;
}


void GameRenderer::setCollisionDebug(
    bool enabled)
{
    collisionDebug_ =
        enabled;
}


void GameRenderer::drawBuilding(
    QPainter& painter,
    Building& building,
    QPixmap& image)
{
    int width =
        building.
        getWidth() *
        displayTileSize_;


    int height =
        building.
        getHeight() *
        displayTileSize_;


    QPixmap scaledImage =
        image.
        scaled(
            width,
            height,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation
        );


    int x =
        building.
        getCol() *
        displayTileSize_;


    int y =
        building.
        getRow() *
        displayTileSize_;


    painter.
        drawPixmap(
            x,
            y,
            scaledImage
        );
}


void GameRenderer::drawCollisionMap(
    QPainter& painter,
    MapManager* mapManager)
{
    QColor collisionColor(
        255,
        0,
        0,
        100
    );


    for (
        int row = 0;
        row < mapManager->getMapRows();
        row++)
    {
        for (
            int col = 0;
            col < mapManager->getMapCols();
            col++)
        {
            if (
                mapManager->
                getCollisionValue(
                    row,
                    col
                ) == 1)
            {
                painter.
                    fillRect(
                        col *
                        displayTileSize_,

                        row *
                        displayTileSize_,

                        displayTileSize_,

                        displayTileSize_,

                        collisionColor
                    );
            }
        }
    }
}


void GameRenderer::drawPlayer(
    QPainter& painter,
    Player* player)
{
    if (player == nullptr)
    {
        return;
    }


    int tileX =
        player->
        getCol() *
        displayTileSize_;


    int tileY =
        player->
        getRow() *
        displayTileSize_;


    int playerX =
        tileX +
        (
            displayTileSize_ -
            playerFrameWidth_
        ) / 2;


    int playerY =
        tileY +
        displayTileSize_ -
        playerFrameHeight_;


    painter.
        drawPixmap(
            playerX,
            playerY,
            playerFrame_
        );
}


void GameRenderer::drawZombie(
    QPainter& painter,
    Zombie& zombie)
{
    if (!zombie.isPlaced())
    {
        return;
    }


    int tileX =
        zombie.
        getCol() *
        displayTileSize_;


    int tileY =
        zombie.
        getRow() *
        displayTileSize_;


    int zombieX =
        tileX +
        (
            displayTileSize_ -
            zombieFrameWidth_
        ) / 2;


    int zombieY =
        tileY +
        displayTileSize_ -
        zombieFrameHeight_;


    painter.
        drawPixmap(
            zombieX,
            zombieY,
            zombieFrame_
        );
}


void GameRenderer::drawCat(
    QPainter& painter,
    Cat& cat)
{
    if (!cat.isPlaced())
    {
        return;
    }


    int tileX =
        cat.
        getCol() *
        displayTileSize_;


    int tileY =
        cat.
        getRow() *
        displayTileSize_;


    int catX =
        tileX +
        (
            displayTileSize_ -
            catWidth_
        ) / 2;


    int catY =
        tileY +
        displayTileSize_ -
        catHeight_;


    painter.
        drawPixmap(
            catX,
            catY,
            catImage_
        );
}


void GameRenderer::drawCannedFood(
    QPainter& painter,
    CannedFood& cannedFood)
{
    // 이미 획득한 통조림
    if (
        cannedFood.
        isCollected())
    {
        return;
    }


    int tileX =
        cannedFood.
        getCol() *
        displayTileSize_;


    int tileY =
        cannedFood.
        getRow() *
        displayTileSize_;


    int canX =
        tileX +
        (
            displayTileSize_ -
            cannedFoodWidth_
        ) / 2;


    int canY =
        tileY +
        (
            displayTileSize_ -
            cannedFoodHeight_
        ) / 2;


    painter.
        drawPixmap(
            canX,
            canY,
            cannedFoodImage_
        );
}


void GameRenderer::drawDarkness(
    QPainter& painter,
    Player* player,
    int visionRadius)
{
    // 플레이어 확인
    if (player == nullptr)
    {
        return;
    }


    // 시야 값 확인
    if (visionRadius <= 0)
    {
        return;
    }


    int playerRow =
        player->
        getRow();


    int playerCol =
        player->
        getCol();


    // 완전히 밝은 영역과 경계 계산
    int radiusSquared =
        visionRadius *
        visionRadius;


    int middleRadius =
        visionRadius - 1;


    int innerRadius =
        visionRadius - 2;


    if (middleRadius < 0)
    {
        middleRadius =
            0;
    }


    if (innerRadius < 0)
    {
        innerRadius =
            0;
    }


    int middleRadiusSquared =
        middleRadius *
        middleRadius;


    int innerRadiusSquared =
        innerRadius *
        innerRadius;


    // 맵 전체를 검사
    for (
        int row = 0;
        row < mapRows_;
        row++)
    {
        for (
            int col = 0;
            col < mapCols_;
            col++)
        {
            int rowDistance =
                row -
                playerRow;


            int colDistance =
                col -
                playerCol;


            // 원형 거리 계산
            int distanceSquared =
                rowDistance *
                rowDistance
                +
                colDistance *
                colDistance;


            int alpha =
                0;


            // 시야 완전히 밖
            if (
                distanceSquared >
                radiusSquared)
            {
                alpha =
                    235;
            }

            // 가장 바깥쪽 경계
            else if (
                distanceSquared >
                middleRadiusSquared)
            {
                alpha =
                    170;
            }

            // 안쪽 경계
            else if (
                distanceSquared >
                innerRadiusSquared)
            {
                alpha =
                    80;
            }


            // 어두운 영역 출력
            if (alpha > 0)
            {
                QColor darknessColor(
                    0,
                    0,
                    0,
                    alpha
                );


                painter.
                    fillRect(
                        col *
                        displayTileSize_,

                        row *
                        displayTileSize_,

                        displayTileSize_,

                        displayTileSize_,

                        darknessColor
                    );
            }
        }
    }
}


QPixmap GameRenderer::renderMap(
    BuildingManager* buildingManager,
    MapManager* mapManager,
    Player* player,
    std::vector<Zombie>& zombies,
    std::vector<Cat>& cats,
    std::vector<CannedFood>& cannedFoods,
    std::vector<Torch>& torches,
    int visionRadius)
{
    // 전체 맵 크기
    QPixmap mapImage(
        mapCols_ *
        displayTileSize_,

        mapRows_ *
        displayTileSize_
    );


    QPainter painter(
        &mapImage
    );


    // 잔디 출력
    for (
        int row = 0;
        row < mapRows_;
        row++)
    {
        for (
            int col = 0;
            col < mapCols_;
            col++)
        {
            painter.
                drawPixmap(
                    col *
                    displayTileSize_,

                    row *
                    displayTileSize_,

                    grassTile_
                );
        }
    }


    // 초록 건물
    std::vector<Building>& buildings =
        buildingManager->
        getGreenBuildings();


    for (
        int i = 0;
        i <
        static_cast<int>(
            buildings.size()
        );
        i++)
    {
        drawBuilding(
            painter,
            buildings[i],
            greenBuildingImage_
        );
    }


    // 빨간 벙커
    Building& bunker =
        buildingManager->
        getBunker();


    drawBuilding(
        painter,
        bunker,
        redBuildingImage_
    );


    // 통조림
    for (
        int i = 0;
        i <
        static_cast<int>(
            cannedFoods.size()
        );
        i++)
    {
        drawCannedFood(
            painter,
            cannedFoods[i]
        );
    }

    // 횃불 출력
    for (
        int i = 0;
        i <
        static_cast<int>(
            torches.size()
        );
        i++)
    {
        drawTorch(
            painter,
            torches[i]
        );  
    }


    // 고양이
    for (
        int i = 0;
        i <
        static_cast<int>(
            cats.size()
        );
        i++)
    {
        drawCat(
            painter,
            cats[i]
        );
    }


    // 좀비
    for (
        int i = 0;
        i <
        static_cast<int>(
            zombies.size()
        );
        i++)
    {
        drawZombie(
            painter,
            zombies[i]
        );
    }


    // 플레이어
    drawPlayer(
        painter,
        player
    );


    // 충돌맵 디버그
    if (collisionDebug_)
    {
        drawCollisionMap(
            painter,
            mapManager
        );
    }


    // 모든 게임 요소를 그린 후
    // 마지막에 어둠을 덮음
    drawDarkness(
        painter,
        player,
        visionRadius
    );


    painter.
        end();


    return mapImage;
}

void GameRenderer::drawTorch(
    QPainter& painter,
    Torch& torch)
{
    // 이미 획득한 횃불은 출력하지 않음
    if (torch.isCollected())
    {
        return;
    }


    // 횃불의 논리적 타일 위치
    int tileX =
        torch.
        getCol() *
        displayTileSize_;


    int tileY =
        torch.
        getRow() *
        displayTileSize_;


    // 타일 가로 중앙
    int torchX =
        tileX +
        (
            displayTileSize_ -
            torchWidth_
        ) / 2;


    // 횃불 아래쪽을 해당 타일에 맞춤
    int torchY =
        tileY +
        displayTileSize_ -
        torchHeight_;


    painter.
        drawPixmap(
            torchX,
            torchY,
            torchImage_
        );
}