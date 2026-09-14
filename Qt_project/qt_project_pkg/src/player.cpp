#include "../include/qt_project_pkg/player.hpp"


Player::Player()
{
    // 플레이어 초기 위치
    row_ = 0;
    col_ = 0;

    // 플레이어 초기 식량 개수
    foodCount_ = 0;
}


bool Player::setStartPosition(
    int row,
    int col,
    MapManager* mapManager)
{
    // 맵 객체 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 시작 위치 충돌 확인
    if (
        mapManager->getCollisionValue(
            row,
            col
        ) == 1)
    {
        return false;
    }


    // 플레이어 시작 위치 설정
    row_ = row;
    col_ = col;


    return true;
}


bool Player::move(
    int rowChange,
    int colChange,
    MapManager* mapManager)
{
    // 맵 객체 확인
    if (mapManager == nullptr)
    {
        return false;
    }


    // 다음 행 계산
    int nextRow =
        row_ + rowChange;


    // 다음 열 계산
    int nextCol =
        col_ + colChange;


    // 다음 위치 충돌 확인
    if (
        mapManager->getCollisionValue(
            nextRow,
            nextCol
        ) == 1)
    {
        return false;
    }


    // 플레이어 위치 변경
    row_ = nextRow;
    col_ = nextCol;


    return true;
}


void Player::addFood()
{
    // 식량 획득
    foodCount_++;
}


int Player::getFoodCount()
{
    // 현재 식량 개수 반환
    return foodCount_;
}


int Player::getRow()
{
    // 현재 플레이어 행 반환
    return row_;
}


int Player::getCol()
{
    // 현재 플레이어 열 반환
    return col_;
}