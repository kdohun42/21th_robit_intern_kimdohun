#include "../include/qt_project_pkg/zombie.hpp"


Zombie::Zombie()
{
    // 좀비 초기 위치
    row_ = 0;
    col_ = 0;

    // 좀비 배치 상태
    placed_ = false;
}


bool Zombie::setStartPosition(
    int row,
    int col,
    MapManager* mapManager)
{
    // 맵 객체 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 시작 위치가 장애물인지 확인
    if (
        mapManager->getCollisionValue(
            row,
            col
        ) == 1)
    {
        return false;
    }


    // 좀비 시작 위치 저장
    row_ = row;
    col_ = col;

    // 배치 완료
    placed_ = true;


    return true;
}


bool Zombie::moveTo(
    int row,
    int col,
    MapManager* mapManager)
{
    // 맵 객체 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 이동할 위치가 장애물인지 확인
    if (
        mapManager->getCollisionValue(
            row,
            col
        ) == 1)
    {
        return false;
    }


    // 좀비 위치 변경
    row_ = row;
    col_ = col;


    return true;
}


int Zombie::getRow()
{
    // 좀비 행 반환
    return row_;
}


int Zombie::getCol()
{
    // 좀비 열 반환
    return col_;
}


bool Zombie::isPlaced()
{
    // 좀비 배치 상태 반환
    return placed_;
}