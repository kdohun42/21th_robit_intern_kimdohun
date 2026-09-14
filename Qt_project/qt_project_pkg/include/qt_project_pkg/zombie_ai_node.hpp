#ifndef ZOMBIE_AI_NODE_HPP_
#define ZOMBIE_AI_NODE_HPP_

#include <QThread>

#include <vector>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/int32_multi_array.hpp"
#include "std_srvs/srv/trigger.hpp"

#include "map_manager.hpp"
#include "zombie.hpp"
#include "cat.hpp"
#include "path_finder.hpp"


class ZombieAiNode : public QThread
{
    Q_OBJECT

public:
    ZombieAiNode(
        MapManager* mapManager,
        std::vector<Cat> cats
    );

    ~ZombieAiNode();

protected:
    void run() override;

private:
    // 플레이어 위치 수신
    void playerPositionCallback(
        std_msgs::msg::Int32MultiArray::SharedPtr msg
    );

    // 고양이 위치 수신
    void catPositionsCallback(
        std_msgs::msg::Int32MultiArray::SharedPtr msg
    );

    // 좀비 생성 Service
    void spawnZombieServiceCallback(
        std::shared_ptr<
            std_srvs::srv::Trigger::Request
        > request,

        std::shared_ptr<
            std_srvs::srv::Trigger::Response
        > response
    );

    void moveZombies();

    bool spawnZombie();

    void publishZombiePositions();

    bool isZombieAtPosition(
        int row,
        int col,
        int ignoreIndex
    );

    bool isNearCat(
        int row,
        int col
    );


    rclcpp::Node::SharedPtr node_;


    rclcpp::Subscription<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr playerPositionSubscriber_;


    rclcpp::Subscription<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr catPositionsSubscriber_;


    rclcpp::Publisher<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr zombiePositionsPublisher_;


    rclcpp::Service<
        std_srvs::srv::Trigger
    >::SharedPtr spawnZombieService_;


    rclcpp::TimerBase::SharedPtr moveTimer_;


    MapManager* mapManager_;


    PathFinder pathFinder_;


    std::vector<Zombie> zombies_;

    std::vector<Cat> cats_;


    int playerRow_;

    int playerCol_;


    bool hasPlayerPosition_;


    int maxZombieCount_;


    // 고양이 주변 3칸
    int catFearRadius_;
};

#endif