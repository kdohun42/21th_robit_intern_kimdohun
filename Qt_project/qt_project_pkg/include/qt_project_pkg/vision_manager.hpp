#ifndef VISION_MANAGER_HPP_
#define VISION_MANAGER_HPP_


class VisionManager
{
public:
    VisionManager();

    // 시간이 지나면 시야 감소
    void decreaseVision();

    // 횃불 사용 시 시야 회복
    void increaseVision(
        int amount
    );

    // 현재 시야
    int getVisionRadius();

private:
    // 현재 시야
    int visionRadius_;

    // 최소 시야
    int minimumVisionRadius_;

    // 횃불로 회복할 수 있는 최대 시야
    int torchMaximumVisionRadius_;
};

#endif