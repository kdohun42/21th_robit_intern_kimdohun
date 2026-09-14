#include "../include/qt_project_pkg/vision_manager.hpp"


VisionManager::VisionManager()
{
    // 게임 시작 시 시야
    visionRadius_ =
        13;


    // 최소 시야
    minimumVisionRadius_ =
        0;


    // 횃불 회복 최대치
    torchMaximumVisionRadius_ =
        12;
}


void VisionManager::decreaseVision()
{
    // 최소 시야보다 클 때만 감소
    if (
        visionRadius_ >
        minimumVisionRadius_)
    {
        visionRadius_--;
    }
}


void VisionManager::increaseVision(
    int amount)
{
    // 현재 시야가 이미 12 이상이면
    // 횃불을 먹어도 시야값을 낮추지 않음
    if (
        visionRadius_ >=
        torchMaximumVisionRadius_)
    {
        return;
    }


    // 시야 회복
    visionRadius_ +=
        amount;


    // 최대 회복 제한
    if (
        visionRadius_ >
        torchMaximumVisionRadius_)
    {
        visionRadius_ =
            torchMaximumVisionRadius_;
    }
}


int VisionManager::getVisionRadius()
{
    return visionRadius_;
}