#include "../include/qt_project_pkg/building.hpp"


Building::Building()
{
    // 건물 기본 위치
    row_ = 0;
    col_ = 0;

    // 건물 기본 크기
    width_ = 0;
    height_ = 0;

    // 식량 초기 상태
    hasFood_ = false;

    // 건물 탐색 초기 상태
    searched_ = false;

    // 기본 건물 종류
    type_ = GREEN_BUILDING;
}


Building::Building(
    int row,
    int col,
    int width,
    int height,
    BuildingType type)
{
    // 건물 위치 저장
    row_ = row;
    col_ = col;

    // 건물 크기 저장
    width_ = width;
    height_ = height;

    // 식량 초기 상태
    hasFood_ = false;

    // 건물 탐색 초기 상태
    searched_ = false;

    // 건물 종류 저장
    type_ = type;
}


void Building::setRow(int row)
{
    // 건물 행 설정
    row_ = row;
}


void Building::setCol(int col)
{
    // 건물 열 설정
    col_ = col;
}


int Building::getRow()
{
    // 건물 행 반환
    return row_;
}


int Building::getCol()
{
    // 건물 열 반환
    return col_;
}


int Building::getWidth()
{
    // 건물 가로 크기 반환
    return width_;
}


int Building::getHeight()
{
    // 건물 세로 크기 반환
    return height_;
}


void Building::setHasFood(bool hasFood)
{
    // 식량 존재 여부 설정
    hasFood_ = hasFood;
}


bool Building::getHasFood()
{
    // 식량 존재 여부 반환
    return hasFood_;
}


void Building::setSearched(bool searched)
{
    // 건물 탐색 여부 설정
    searched_ = searched;
}


bool Building::getSearched()
{
    // 건물 탐색 여부 반환
    return searched_;
}


BuildingType Building::getType()
{
    // 건물 종류 반환
    return type_;
}


void Building::setCollisionMask(
    std::vector<std::vector<int>> mask)
{
    // 건물 충돌 배열 저장
    collisionMask_ = mask;
}


std::vector<std::vector<int>>&
Building::getCollisionMask()
{
    // 건물 충돌 배열 반환
    return collisionMask_;
}


bool Building::isDoorPosition(
    int playerRow,
    int playerCol)
{
    // 건물 내부 문 행 계산
    int doorRow1 =
        row_ + 5;

    int doorRow2 =
        row_ + 6;


    // 건물 내부 문 열 계산
    int doorCol1 =
        col_ + 2;

    int doorCol2 =
        col_ + 3;


    // 플레이어가 문 위치에 있는지 확인
    if (
        (playerRow == doorRow1 ||
         playerRow == doorRow2) &&
        (playerCol == doorCol1 ||
         playerCol == doorCol2))
    {
        return true;
    }


    return false;
}