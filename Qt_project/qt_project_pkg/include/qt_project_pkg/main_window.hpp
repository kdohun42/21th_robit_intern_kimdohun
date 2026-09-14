#ifndef QT_PROJECT_PKG_MAIN_WINDOW_HPP_
#define QT_PROJECT_PKG_MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include <QString>

#include <vector>

#include "qnode.hpp"
#include "zombie_ai_node.hpp"

#include "map_manager.hpp"
#include "building_manager.hpp"
#include "game_renderer.hpp"

#include "player.hpp"
#include "zombie.hpp"
#include "cat.hpp"
#include "canned_food.hpp"
#include "torch.hpp"

#include "path_finder.hpp"
#include "vision_manager.hpp"


namespace Ui
{
class MainWindowDesign;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
        QWidget* parent = nullptr
    );

    ~MainWindow();


protected:
    // 창 닫기
    void closeEvent(
        QCloseEvent* event
    ) override;


    // 키 입력
    void keyPressEvent(
        QKeyEvent* event
    ) override;


private:
    // 게임 화면 갱신
    void updateGameScreen();


    // 게임 정보 갱신
    void updateGameInfo();


    // 게임 시작, 게임 오버, 게임 클리어 메시지
    void showGameMessage(
        QString text,
        QString color
    );


    // 게임 메시지 숨기기
    void hideGameMessage();


    // 건물 및 벙커 위치 확인
    void checkPlayerPosition();


    // 좀비 위치 수신
    void updateZombiePositions(
        QVector<int> positions
    );


    // 좀비 충돌 확인
    void checkZombieCollision();


    // 좀비 생성 요청
    void requestZombieSpawn();


    // 고양이 배치
    void setupCats();


    // 고양이 따라오기
    void updateCatFollow();


    // 고양이 따라오기 종료
    void stopCatFollow();


    // 통조림 사용
    void useCannedFood();


    // 통조림 배치
    void setupCannedFoods();


    // 통조림 획득
    void checkCannedFoodCollection();


    // 통조림 위치 확인
    bool isCannedFoodPosition(
        int row,
        int col
    );


    // 횃불 배치
    void setupTorches();


    // 횃불 획득
    void checkTorchCollection();


    // 횃불 위치 확인
    bool isTorchPosition(
        int row,
        int col
    );


    // 건물 영역 확인
    bool isInsideBuildingArea(
        int row,
        int col
    );


    // 시야 감소
    void decreaseVision();


    Ui::MainWindowDesign* ui;


    // ROS2 Qt Node
    QNode* qnode;


    // Zombie AI Node
    ZombieAiNode* zombieAiNode_;


    // 맵
    MapManager* mapManager_;


    // 건물
    BuildingManager* buildingManager_;


    // 렌더러
    GameRenderer* gameRenderer_;


    // 플레이어
    Player* player_;


    // 시야
    VisionManager* visionManager_;


    // 좀비
    std::vector<Zombie> zombies_;


    // 고양이
    std::vector<Cat> cats_;


    // 통조림
    std::vector<CannedFood> cannedFoods_;


    // 횃불
    std::vector<Torch> torches_;


    // 고양이 이동 A*
    PathFinder catPathFinder_;


    // 좀비 생성 Timer
    QTimer* zombieSpawnRequestTimer_;


    // 고양이 이동 Timer
    QTimer* catFollowTimer_;


    // 고양이 따라오기 종료 Timer
    QTimer* catFollowEndTimer_;


    // 시야 감소 Timer
    QTimer* darknessTimer_;


    // 통조림 개수
    int cannedFoodCount_;


    // 따라오는 고양이 번호
    int activeFollowCatIndex_;


    // 게임 클리어
    bool gameCleared_;


    // 게임 오버
    bool gameOver_;
};


#endif