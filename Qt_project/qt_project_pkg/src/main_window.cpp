#include "../include/qt_project_pkg/main_window.hpp"

#include "ui_mainwindow.h"

#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QString>

#include <cstdlib>
#include <ctime>
#include <utility>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindowDesign),
      qnode(nullptr),
      zombieAiNode_(nullptr),
      mapManager_(nullptr),
      buildingManager_(nullptr),
      gameRenderer_(nullptr),
      player_(nullptr),
      visionManager_(nullptr),
      zombieSpawnRequestTimer_(nullptr),
      catFollowTimer_(nullptr),
      catFollowEndTimer_(nullptr),
      darknessTimer_(nullptr)
{
    // UI 생성
    ui->setupUi(this);


    // 키 입력 설정
    this->setFocusPolicy(
        Qt::StrongFocus
    );

    this->setFocus();


    // 게임 상태
    gameCleared_ =
        false;

    gameOver_ =
        false;


    // 통조림 개수
    cannedFoodCount_ =
        0;


    // 따라오는 고양이 없음
    activeFollowCatIndex_ =
        -1;


    // 게임 메시지 설정
    ui->label_game_message->
        setAlignment(
            Qt::AlignCenter
        );


    // label_map과 같은 위치와 크기 사용
    ui->label_game_message->
        setGeometry(
            ui->label_map->
            geometry()
        );


    // 게임 메시지를 맨 앞으로
    ui->label_game_message->
        raise();


    // 처음에는 숨김
    ui->label_game_message->
        hide();


    // 랜덤 초기화
    std::srand(
        static_cast<unsigned int>(
            std::time(
                nullptr
            )
        )
    );


    // 맵 생성
    mapManager_ =
        new MapManager();


    // 건물 관리자
    buildingManager_ =
        new BuildingManager(
            mapManager_
        );


    // 건물 생성
    buildingManager_->
        setupBuildings();


    // 플레이어 생성
    player_ =
        new Player();


    // 플레이어 시작 위치
    bool playerPlaced =
        player_->
        setStartPosition(
            50,
            10,
            mapManager_
        );


    if (!playerPlaced)
    {
        qDebug()
            << "Player Start Error";
    }


    // 고양이 생성
    setupCats();


    // 통조림 생성
    setupCannedFoods();


    // 횃불 생성
    setupTorches();


    // 시야 관리자
    visionManager_ =
        new VisionManager();


    // Renderer
    gameRenderer_ =
        new GameRenderer();


    gameRenderer_->
        setCollisionDebug(
            false
        );


    // 배경 이미지
    QString tileSheetPath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/"
        "Background_Green_TileSet.png";


    // 초록 건물
    QString greenBuildingPath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/buildings/"
        "building_green.png";


    // 빨간 벙커
    QString redBuildingPath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/buildings/"
        "building_red.png";


    // 플레이어
    QString playerSheetPath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/player/"
        "player.png";


    // 좀비
    QString zombieSheetPath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/zombie/"
        "zombie.png";


    // 고양이
    QString catImagePath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/cat/"
        "cat.png";


    // 통조림
    QString cannedFoodImagePath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/items/"
        "can.png";


    // 횃불
    QString torchImagePath =
        "/home/kdh/Desktop/colcon_ws/src/"
        "21th_robit_intern_kimdohun/Qt_project/"
        "qt_project_pkg/resources/images/items/"
        "fire.png";


    // 이미지 로딩
    bool loaded =
        gameRenderer_->
        loadImages(
            tileSheetPath,
            greenBuildingPath,
            redBuildingPath,
            playerSheetPath,
            zombieSheetPath,
            catImagePath,
            cannedFoodImagePath,
            torchImagePath
        );


    if (!loaded)
    {
        qDebug()
            << "Image Load Error";
    }


    // 맵 크기
    ui->label_map->
        setFixedSize(
            1248,
            960
        );


    ui->label_map->
        setAlignment(
            Qt::AlignCenter
        );


    // 메시지 라벨도 같은 크기
    ui->label_game_message->
        setGeometry(
            ui->label_map->
            geometry()
        );


    // 첫 화면
    updateGameScreen();


    // ROS2 Qt Node
    qnode =
        new QNode();


    // Zombie AI Node
    zombieAiNode_ =
        new ZombieAiNode(
            mapManager_,
            cats_
        );


    // 좀비 위치 Topic
    QObject::connect(
        qnode,
        &QNode::zombiePositionsReceived,
        this,
        &MainWindow::updateZombiePositions
    );


    // ROS2 종료
    QObject::connect(
        qnode,
        &QNode::rosShutDown,
        this,
        &MainWindow::close
    );


    // 좀비 생성 Timer
    zombieSpawnRequestTimer_ =
        new QTimer(
            this
        );


    QObject::connect(
        zombieSpawnRequestTimer_,
        &QTimer::timeout,
        this,
        &MainWindow::requestZombieSpawn
    );


    // 반복 좀비 생성 시간
    zombieSpawnRequestTimer_->
        start(
            15000
        );


    // 고양이 이동 Timer
    catFollowTimer_ =
        new QTimer(
            this
        );


    QObject::connect(
        catFollowTimer_,
        &QTimer::timeout,
        this,
        &MainWindow::updateCatFollow
    );


    // 고양이 Follow 종료 Timer
    catFollowEndTimer_ =
        new QTimer(
            this
        );


    catFollowEndTimer_->
        setSingleShot(
            true
        );


    QObject::connect(
        catFollowEndTimer_,
        &QTimer::timeout,
        this,
        &MainWindow::stopCatFollow
    );


    // 어둠 Timer
    darknessTimer_ =
        new QTimer(
            this
        );


    QObject::connect(
        darknessTimer_,
        &QTimer::timeout,
        this,
        &MainWindow::decreaseVision
    );


    // 시야 감소 시간
    darknessTimer_->
        start(
            15000
        );


    // 최초 플레이어와 고양이 위치 전송
    QTimer::singleShot(
        500,
        this,
        [this]()
        {
            qnode->
                publishPlayerPosition(
                    player_->getRow(),
                    player_->getCol()
                );


            qnode->
                publishCatPositions(
                    cats_
                );
        }
    );


    // 플레이어 위치 다시 전송
    QTimer::singleShot(
        1000,
        this,
        [this]()
        {
            qnode->
                publishPlayerPosition(
                    player_->getRow(),
                    player_->getCol()
                );
        }
    );


    // 첫 번째 좀비
    QTimer::singleShot(
        1500,
        this,
        [this]()
        {
            requestZombieSpawn();
        }
    );


    // GAME START 표시
    showGameMessage(
        "GAME START",
        "white"
    );


    // 2초 후 GAME START 숨김
    QTimer::singleShot(
        2000,
        this,
        [this]()
        {
            if (
                !gameOver_ &&
                !gameCleared_)
            {
                hideGameMessage();
            }
        }
    );
}


void MainWindow::showGameMessage(
    QString text,
    QString color)
{
    ui->label_game_message->
        setText(
            text
        );


    ui->label_game_message->
        setStyleSheet(
            QString(
                "QLabel {"
                "color: %1;"
                "font-size: 100px;"
                "font-weight: bold;"
                "background-color: rgba(0, 0, 0, 160);"
                "}"
            ).
            arg(
                color
            )
        );


    ui->label_game_message->
        show();


    ui->label_game_message->
        raise();
}


void MainWindow::hideGameMessage()
{
    ui->label_game_message->
        hide();
}


void MainWindow::decreaseVision()
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    if (
        visionManager_ ==
        nullptr)
    {
        return;
    }


    visionManager_->
        decreaseVision();


    qDebug()
        << "Vision Radius:"
        << visionManager_->
           getVisionRadius();


    updateGameScreen();
}


void MainWindow::setupCats()
{
    cats_.
        clear();


    int attempts =
        0;


    while (
        static_cast<int>(
            cats_.size()
        ) < 2 &&
        attempts < 500)
    {
        attempts++;


        int row =
            std::rand() %
            mapManager_->
            getMapRows();


        int col =
            std::rand() %
            mapManager_->
            getMapCols();


        if (
            mapManager_->
            getCollisionValue(
                row,
                col
            ) == 1)
        {
            continue;
        }


        if (
            isInsideBuildingArea(
                row,
                col
            ))
        {
            continue;
        }


        if (
            row ==
            player_->getRow() &&
            col ==
            player_->getCol())
        {
            continue;
        }


        int playerDistance =
            std::abs(
                player_->
                getRow() -
                row
            )
            +
            std::abs(
                player_->
                getCol() -
                col
            );


        if (
            playerDistance <
            8)
        {
            continue;
        }


        bool tooClose =
            false;


        for (
            int i = 0;
            i <
            static_cast<int>(
                cats_.size()
            );
            i++)
        {
            int catDistance =
                std::abs(
                    cats_[i].
                    getRow() -
                    row
                )
                +
                std::abs(
                    cats_[i].
                    getCol() -
                    col
                );


            if (
                catDistance <
                10)
            {
                tooClose =
                    true;

                break;
            }
        }


        if (tooClose)
        {
            continue;
        }


        Cat cat;


        bool placed =
            cat.
            setStartPosition(
                row,
                col,
                mapManager_
            );


        if (placed)
        {
            cats_.
                push_back(
                    cat
                );


            qDebug()
                << "Cat"
                << cats_.size()
                << "row:"
                << row
                << "col:"
                << col;
        }
    }
}


void MainWindow::setupCannedFoods()
{
    cannedFoods_.
        clear();


    int attempts =
        0;


    while (
        static_cast<int>(
            cannedFoods_.size()
        ) < 5 &&
        attempts < 1000)
    {
        attempts++;


        int row =
            std::rand() %
            mapManager_->
            getMapRows();


        int col =
            std::rand() %
            mapManager_->
            getMapCols();


        if (
            mapManager_->
            getCollisionValue(
                row,
                col
            ) == 1)
        {
            continue;
        }


        if (
            isInsideBuildingArea(
                row,
                col
            ))
        {
            continue;
        }


        if (
            row ==
            player_->getRow() &&
            col ==
            player_->getCol())
        {
            continue;
        }


        bool catPosition =
            false;


        for (
            int i = 0;
            i <
            static_cast<int>(
                cats_.size()
            );
            i++)
        {
            if (
                cats_[i].
                getRow() == row &&
                cats_[i].
                getCol() == col)
            {
                catPosition =
                    true;

                break;
            }
        }


        if (catPosition)
        {
            continue;
        }


        if (
            isCannedFoodPosition(
                row,
                col
            ))
        {
            continue;
        }


        CannedFood cannedFood;


        bool placed =
            cannedFood.
            setPosition(
                row,
                col,
                mapManager_
            );


        if (placed)
        {
            cannedFoods_.
                push_back(
                    cannedFood
                );


            qDebug()
                << "Can"
                << cannedFoods_.size()
                << "row:"
                << row
                << "col:"
                << col;
        }
    }
}


void MainWindow::setupTorches()
{
    torches_.
        clear();


    int torchCount =
        6;


    int attempts =
        0;


    while (
        static_cast<int>(
            torches_.size()
        ) < torchCount &&
        attempts < 1000)
    {
        attempts++;


        int row =
            std::rand() %
            mapManager_->
            getMapRows();


        int col =
            std::rand() %
            mapManager_->
            getMapCols();


        if (
            mapManager_->
            getCollisionValue(
                row,
                col
            ) == 1)
        {
            continue;
        }


        if (
            isInsideBuildingArea(
                row,
                col
            ))
        {
            continue;
        }


        if (
            row ==
            player_->getRow() &&
            col ==
            player_->getCol())
        {
            continue;
        }


        bool catPosition =
            false;


        for (
            int i = 0;
            i <
            static_cast<int>(
                cats_.size()
            );
            i++)
        {
            if (
                cats_[i].
                getRow() == row &&
                cats_[i].
                getCol() == col)
            {
                catPosition =
                    true;

                break;
            }
        }


        if (catPosition)
        {
            continue;
        }


        if (
            isCannedFoodPosition(
                row,
                col
            ))
        {
            continue;
        }


        if (
            isTorchPosition(
                row,
                col
            ))
        {
            continue;
        }


        Torch torch;


        bool placed =
            torch.
            setPosition(
                row,
                col,
                mapManager_
            );


        if (placed)
        {
            torches_.
                push_back(
                    torch
                );


            qDebug()
                << "Torch"
                << torches_.size()
                << "row:"
                << row
                << "col:"
                << col;
        }
    }
}


bool MainWindow::isCannedFoodPosition(
    int row,
    int col)
{
    for (
        int i = 0;
        i <
        static_cast<int>(
            cannedFoods_.size()
        );
        i++)
    {
        if (
            cannedFoods_[i].
            isCollected())
        {
            continue;
        }


        if (
            cannedFoods_[i].
            getRow() == row &&
            cannedFoods_[i].
            getCol() == col)
        {
            return true;
        }
    }


    return false;
}


bool MainWindow::isTorchPosition(
    int row,
    int col)
{
    for (
        int i = 0;
        i <
        static_cast<int>(
            torches_.size()
        );
        i++)
    {
        if (
            torches_[i].
            isCollected())
        {
            continue;
        }


        if (
            torches_[i].
            getRow() == row &&
            torches_[i].
            getCol() == col)
        {
            return true;
        }
    }


    return false;
}


void MainWindow::checkCannedFoodCollection()
{
    for (
        int i = 0;
        i <
        static_cast<int>(
            cannedFoods_.size()
        );
        i++)
    {
        if (
            cannedFoods_[i].
            isCollected())
        {
            continue;
        }


        if (
            cannedFoods_[i].
            getRow() ==
            player_->getRow() &&

            cannedFoods_[i].
            getCol() ==
            player_->getCol())
        {
            cannedFoods_[i].
                collect();


            cannedFoodCount_++;


            qDebug()
                << "통조림 획득"
                << "보유:"
                << cannedFoodCount_;


            return;
        }
    }
}


void MainWindow::checkTorchCollection()
{
    if (
        visionManager_ ==
        nullptr)
    {
        return;
    }


    for (
        int i = 0;
        i <
        static_cast<int>(
            torches_.size()
        );
        i++)
    {
        if (
            torches_[i].
            isCollected())
        {
            continue;
        }


        if (
            torches_[i].
            getRow() ==
            player_->getRow() &&

            torches_[i].
            getCol() ==
            player_->getCol())
        {
            int beforeVision =
                visionManager_->
                getVisionRadius();


            torches_[i].
                collect();


            visionManager_->
                increaseVision(
                    4
                );


            int afterVision =
                visionManager_->
                getVisionRadius();


            qDebug()
                << "횃불 획득"
                << "VISION:"
                << beforeVision
                << "->"
                << afterVision;


            return;
        }
    }
}


void MainWindow::useCannedFood()
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    if (
        activeFollowCatIndex_ !=
        -1)
    {
        qDebug()
            << "이미 고양이가 따라오는 중입니다.";

        return;
    }


    if (
        cannedFoodCount_ <=
        0)
    {
        qDebug()
            << "통조림이 없습니다.";

        return;
    }


    int catIndex =
        -1;


    for (
        int i = 0;
        i <
        static_cast<int>(
            cats_.size()
        );
        i++)
    {
        if (
            cats_[i].
            getRow() ==
            player_->getRow() &&

            cats_[i].
            getCol() ==
            player_->getCol())
        {
            catIndex =
                i;

            break;
        }
    }


    if (
        catIndex ==
        -1)
    {
        qDebug()
            << "고양이와 같은 위치에서 Q키를 사용하세요.";

        return;
    }


    cannedFoodCount_--;


    activeFollowCatIndex_ =
        catIndex;


    qDebug()
        << "통조림 사용"
        << "Cat:"
        << activeFollowCatIndex_
        << "남은 통조림:"
        << cannedFoodCount_;


    catFollowTimer_->
        start(
            300
        );


    catFollowEndTimer_->
        start(
            10000
        );
}


void MainWindow::updateCatFollow()
{
    if (
        activeFollowCatIndex_ <
        0 ||
        activeFollowCatIndex_ >=
        static_cast<int>(
            cats_.size()
        ))
    {
        return;
    }


    Cat& cat =
        cats_[
            activeFollowCatIndex_
        ];


    if (
        cat.getRow() ==
        player_->getRow() &&

        cat.getCol() ==
        player_->getCol())
    {
        if (
            qnode !=
            nullptr)
        {
            qnode->
                publishCatPositions(
                    cats_
                );
        }


        return;
    }


    std::vector<Cat> emptyCats;


    std::vector<std::pair<int, int>> path =
        catPathFinder_.
        findPath(
            cat.getRow(),
            cat.getCol(),

            player_->getRow(),
            player_->getCol(),

            mapManager_,

            emptyCats,

            0
        );


    if (
        path.size() >=
        2)
    {
        int nextRow =
            path[1].
            first;


        int nextCol =
            path[1].
            second;


        cat.
            moveTo(
                nextRow,
                nextCol,
                mapManager_
            );
    }


    if (
        qnode !=
        nullptr)
    {
        qnode->
            publishCatPositions(
                cats_
            );
    }


    updateGameScreen();
}


void MainWindow::stopCatFollow()
{
    if (
        catFollowTimer_ !=
        nullptr)
    {
        catFollowTimer_->
            stop();
    }


    if (
        catFollowEndTimer_ !=
        nullptr)
    {
        catFollowEndTimer_->
            stop();
    }


    activeFollowCatIndex_ =
        -1;


    qDebug()
        << "고양이 따라오기 종료";


    if (
        qnode !=
        nullptr)
    {
        qnode->
            publishCatPositions(
                cats_
            );
    }
}


bool MainWindow::isInsideBuildingArea(
    int row,
    int col)
{
    std::vector<Building>& buildings =
        buildingManager_->
        getGreenBuildings();


    for (
        int i = 0;
        i <
        static_cast<int>(
            buildings.size()
        );
        i++)
    {
        int startRow =
            buildings[i].
            getRow();


        int startCol =
            buildings[i].
            getCol();


        int endRow =
            startRow +
            buildings[i].
            getHeight();


        int endCol =
            startCol +
            buildings[i].
            getWidth();


        if (
            row >= startRow &&
            row < endRow &&
            col >= startCol &&
            col < endCol)
        {
            return true;
        }
    }


    Building& bunker =
        buildingManager_->
        getBunker();


    if (
        row >=
        bunker.getRow() &&

        row <
        bunker.getRow() +
        bunker.getHeight() &&

        col >=
        bunker.getCol() &&

        col <
        bunker.getCol() +
        bunker.getWidth())
    {
        return true;
    }


    return false;
}


void MainWindow::updateZombiePositions(
    QVector<int> positions)
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    if (
        positions.size() %
        2 != 0)
    {
        qDebug()
            << "Zombie Position Data Error";

        return;
    }


    std::vector<Zombie> newZombies;


    for (
        int i = 0;
        i <
        positions.size();
        i += 2)
    {
        Zombie zombie;


        bool placed =
            zombie.
            setStartPosition(
                positions[i],
                positions[i + 1],
                mapManager_
            );


        if (placed)
        {
            newZombies.
                push_back(
                    zombie
                );
        }
    }


    zombies_ =
        newZombies;


    if (
        static_cast<int>(
            zombies_.size()
        ) >= 10)
    {
        if (
            zombieSpawnRequestTimer_ !=
            nullptr)
        {
            zombieSpawnRequestTimer_->
                stop();
        }
    }


    checkZombieCollision();


    updateGameScreen();
}


void MainWindow::updateGameScreen()
{
    if (
        gameRenderer_ ==
        nullptr ||
        visionManager_ ==
        nullptr ||
        player_ ==
        nullptr ||
        mapManager_ ==
        nullptr)
    {
        return;
    }


    // 전체 맵 생성
    QPixmap fullMap =
        gameRenderer_->
        renderMap(
            buildingManager_,
            mapManager_,
            player_,
            zombies_,
            cats_,
            cannedFoods_,
            torches_,
            visionManager_->
            getVisionRadius()
        );


    // 타일 크기
    int tileSize =
        16;


    // 카메라 크기
    int cameraCols =
        39;

    int cameraRows =
        30;


    int cameraWidth =
        cameraCols *
        tileSize;

    int cameraHeight =
        cameraRows *
        tileSize;


    // 플레이어 중심
    int cameraCol =
        player_->
        getCol() -
        cameraCols / 2;


    int cameraRow =
        player_->
        getRow() -
        cameraRows / 2;


    // 왼쪽 제한
    if (
        cameraCol <
        0)
    {
        cameraCol =
            0;
    }


    // 위쪽 제한
    if (
        cameraRow <
        0)
    {
        cameraRow =
            0;
    }


    int maximumCameraCol =
        mapManager_->
        getMapCols() -
        cameraCols;


    int maximumCameraRow =
        mapManager_->
        getMapRows() -
        cameraRows;


    // 오른쪽 제한
    if (
        cameraCol >
        maximumCameraCol)
    {
        cameraCol =
            maximumCameraCol;
    }


    // 아래쪽 제한
    if (
        cameraRow >
        maximumCameraRow)
    {
        cameraRow =
            maximumCameraRow;
    }


    int cameraX =
        cameraCol *
        tileSize;


    int cameraY =
        cameraRow *
        tileSize;


    // 카메라 부분만 자르기
    QPixmap cameraMap =
        fullMap.
        copy(
            cameraX,
            cameraY,
            cameraWidth,
            cameraHeight
        );


    // QLabel 크기로 확대
    cameraMap =
        cameraMap.
        scaled(
            ui->label_map->
            width(),

            ui->label_map->
            height(),

            Qt::IgnoreAspectRatio,

            Qt::FastTransformation
        );


    ui->label_map->
        setPixmap(
            cameraMap
        );


    // 메시지가 표시 중이면 항상 앞으로
    ui->label_game_message->
        raise();


    updateGameInfo();
}


void MainWindow::updateGameInfo()
{
    if (
        player_ ==
        nullptr ||
        visionManager_ ==
        nullptr)
    {
        return;
    }


    ui->label_food->
        setText(
            QString(
                "FOOD : %1 / 3"
            ).
            arg(
                player_->
                getFoodCount()
            )
        );


    ui->label_zombie_count->
        setText(
            QString(
                "ZOMBIE : %1 / 10"
            ).
            arg(
                static_cast<int>(
                    zombies_.size()
                )
            )
        );


    if (gameCleared_)
    {
        ui->label_game_status->
            setText(
                "GAME CLEAR"
            );

        return;
    }


    if (gameOver_)
    {
        ui->label_game_status->
            setText(
                "GAME OVER"
            );

        return;
    }


    ui->label_game_status->
        setText(
            QString(
                "GAME PLAYING | CAN : %1 | VISION : %2"
            ).
            arg(
                cannedFoodCount_
            ).
            arg(
                visionManager_->
                getVisionRadius()
            )
        );
}


void MainWindow::checkZombieCollision()
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    for (
        int i = 0;
        i <
        static_cast<int>(
            zombies_.size()
        );
        i++)
    {
        if (
            zombies_[i].
            getRow() ==
            player_->getRow() &&

            zombies_[i].
            getCol() ==
            player_->getCol())
        {
            gameOver_ =
                true;


            if (
                zombieSpawnRequestTimer_ !=
                nullptr)
            {
                zombieSpawnRequestTimer_->
                    stop();
            }


            if (
                darknessTimer_ !=
                nullptr)
            {
                darknessTimer_->
                    stop();
            }


            stopCatFollow();


            qDebug()
                << "GAME OVER";


            // 큰 게임 오버 화면
            showGameMessage(
                "GAME OVER",
                "red"
            );


            updateGameInfo();


            return;
        }
    }
}


void MainWindow::checkPlayerPosition()
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    std::vector<Building>& buildings =
        buildingManager_->
        getGreenBuildings();


    for (
        int i = 0;
        i <
        static_cast<int>(
            buildings.size()
        );
        i++)
    {
        if (
            buildings[i].
            isDoorPosition(
                player_->getRow(),
                player_->getCol()
            ))
        {
            if (
                buildings[i].
                getSearched())
            {
                return;
            }


            buildings[i].
                setSearched(
                    true
                );


            if (
                buildings[i].
                getHasFood())
            {
                player_->
                    addFood();


                qDebug()
                    << "식량 획득"
                    << player_->
                       getFoodCount()
                    << "/ 3";
            }
            else
            {
                qDebug()
                    << "식량이 없는 건물";
            }


            updateGameInfo();


            return;
        }
    }


    Building& bunker =
        buildingManager_->
        getBunker();


    if (
        bunker.
        isDoorPosition(
            player_->getRow(),
            player_->getCol()
        ))
    {
        if (
            player_->
            getFoodCount() >=
            3)
        {
            gameCleared_ =
                true;


            if (
                zombieSpawnRequestTimer_ !=
                nullptr)
            {
                zombieSpawnRequestTimer_->
                    stop();
            }


            if (
                darknessTimer_ !=
                nullptr)
            {
                darknessTimer_->
                    stop();
            }


            stopCatFollow();


            qDebug()
                << "GAME CLEAR";


            // 큰 게임 클리어 화면
            showGameMessage(
                "GAME CLEAR",
                "lime"
            );


            updateGameInfo();
        }
        else
        {
            qDebug()
                << "식량 부족"
                << player_->
                   getFoodCount()
                << "/ 3";
        }
    }
}


void MainWindow::requestZombieSpawn()
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    if (
        static_cast<int>(
            zombies_.size()
        ) >= 10)
    {
        if (
            zombieSpawnRequestTimer_ !=
            nullptr)
        {
            zombieSpawnRequestTimer_->
                stop();
        }


        return;
    }


    if (
        qnode ==
        nullptr)
    {
        return;
    }


    qnode->
        requestSpawnZombie();
}


void MainWindow::keyPressEvent(
    QKeyEvent* event)
{
    if (
        gameCleared_ ||
        gameOver_)
    {
        return;
    }


    // Q키 통조림 사용
    if (
        event->key() ==
        Qt::Key_Q)
    {
        useCannedFood();


        updateGameInfo();


        return;
    }


    bool moved =
        false;


    // 위
    if (
        event->key() ==
        Qt::Key_W ||
        event->key() ==
        Qt::Key_Up)
    {
        moved =
            player_->
            move(
                -1,
                0,
                mapManager_
            );
    }


    // 아래
    else if (
        event->key() ==
        Qt::Key_S ||
        event->key() ==
        Qt::Key_Down)
    {
        moved =
            player_->
            move(
                1,
                0,
                mapManager_
            );
    }


    // 왼쪽
    else if (
        event->key() ==
        Qt::Key_A ||
        event->key() ==
        Qt::Key_Left)
    {
        moved =
            player_->
            move(
                0,
                -1,
                mapManager_
            );
    }


    // 오른쪽
    else if (
        event->key() ==
        Qt::Key_D ||
        event->key() ==
        Qt::Key_Right)
    {
        moved =
            player_->
            move(
                0,
                1,
                mapManager_
            );
    }


    if (moved)
    {
        // 통조림 획득
        checkCannedFoodCollection();


        // 횃불 획득
        checkTorchCollection();


        // 플레이어 위치 전송
        if (
            qnode !=
            nullptr)
        {
            qnode->
                publishPlayerPosition(
                    player_->getRow(),
                    player_->getCol()
                );
        }


        // 건물 확인
        checkPlayerPosition();


        // 좀비 충돌
        checkZombieCollision();


        // 화면 갱신
        updateGameScreen();
    }


    QMainWindow::
        keyPressEvent(
            event
        );
}


void MainWindow::closeEvent(
    QCloseEvent* event)
{
    if (
        zombieSpawnRequestTimer_ !=
        nullptr)
    {
        zombieSpawnRequestTimer_->
            stop();
    }


    if (
        catFollowTimer_ !=
        nullptr)
    {
        catFollowTimer_->
            stop();
    }


    if (
        catFollowEndTimer_ !=
        nullptr)
    {
        catFollowEndTimer_->
            stop();
    }


    if (
        darknessTimer_ !=
        nullptr)
    {
        darknessTimer_->
            stop();
    }


    QMainWindow::
        closeEvent(
            event
        );
}


MainWindow::~MainWindow()
{
    if (
        zombieSpawnRequestTimer_ !=
        nullptr)
    {
        zombieSpawnRequestTimer_->
            stop();
    }


    if (
        catFollowTimer_ !=
        nullptr)
    {
        catFollowTimer_->
            stop();
    }


    if (
        catFollowEndTimer_ !=
        nullptr)
    {
        catFollowEndTimer_->
            stop();
    }


    if (
        darknessTimer_ !=
        nullptr)
    {
        darknessTimer_->
            stop();
    }


    // AI Node 먼저 삭제
    delete zombieAiNode_;


    // QNode 삭제
    delete qnode;


    delete visionManager_;

    delete player_;

    delete gameRenderer_;

    delete buildingManager_;

    delete mapManager_;

    delete ui;
}