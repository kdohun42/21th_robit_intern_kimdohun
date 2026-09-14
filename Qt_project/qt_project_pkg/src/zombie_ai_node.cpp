#include "../include/qt_project_pkg/zombie_ai_node.hpp"

#include <functional>
#include <chrono>
#include <cstdlib>
#include <utility>


ZombieAiNode::ZombieAiNode(
    MapManager* mapManager,
    std::vector<Cat> cats)
{
    mapManager_ =
        mapManager;


    // 처음 생성된 고양이 위치
    cats_ =
        cats;


    playerRow_ =
        0;

    playerCol_ =
        0;


    hasPlayerPosition_ =
        false;


    maxZombieCount_ =
        10;


    catFearRadius_ =
        3;


    // ROS2 노드
    node_ =
        rclcpp::Node::make_shared(
            "zombie_ai_node"
        );


    // 플레이어 위치 Subscriber
    playerPositionSubscriber_ =
        node_->
        create_subscription<
            std_msgs::msg::Int32MultiArray
        >(
            "player_position",
            10,

            std::bind(
                &ZombieAiNode::
                playerPositionCallback,
                this,
                std::placeholders::_1
            )
        );


    // 고양이 위치 Subscriber
    catPositionsSubscriber_ =
        node_->
        create_subscription<
            std_msgs::msg::Int32MultiArray
        >(
            "cat_positions",
            10,

            std::bind(
                &ZombieAiNode::
                catPositionsCallback,
                this,
                std::placeholders::_1
            )
        );


    // 좀비 위치 Publisher
    zombiePositionsPublisher_ =
        node_->
        create_publisher<
            std_msgs::msg::Int32MultiArray
        >(
            "zombie_positions",
            10
        );


    // 좀비 생성 Service Server
    spawnZombieService_ =
        node_->
        create_service<
            std_srvs::srv::Trigger
        >(
            "spawn_zombie",

            std::bind(
                &ZombieAiNode::
                spawnZombieServiceCallback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );


    // 좀비 이동
    moveTimer_ =
        node_->
        create_wall_timer(
            std::chrono::milliseconds(
                500
            ),

            std::bind(
                &ZombieAiNode::
                moveZombies,
                this
            )
        );


    start();
}


void ZombieAiNode::playerPositionCallback(
    std_msgs::msg::Int32MultiArray::SharedPtr msg)
{
    if (msg->data.size() < 2)
    {
        return;
    }


    playerRow_ =
        msg->data[0];


    playerCol_ =
        msg->data[1];


    hasPlayerPosition_ =
        true;
}


void ZombieAiNode::catPositionsCallback(
    std_msgs::msg::Int32MultiArray::SharedPtr msg)
{
    // 반드시 row, col 쌍이어야 함
    if (
        msg->data.size() % 2 != 0)
    {
        return;
    }


    std::vector<Cat> newCats;


    for (
        int i = 0;
        i < static_cast<int>(
            msg->data.size()
        );
        i += 2)
    {
        Cat cat;


        bool placed =
            cat.
            setStartPosition(
                msg->data[i],
                msg->data[i + 1],
                mapManager_
            );


        if (placed)
        {
            newCats.
                push_back(
                    cat
                );
        }
    }


    cats_ =
        newCats;
}


void ZombieAiNode::spawnZombieServiceCallback(
    std::shared_ptr<
        std_srvs::srv::Trigger::Request
    > request,

    std::shared_ptr<
        std_srvs::srv::Trigger::Response
    > response)
{
    (void)request;


    if (!hasPlayerPosition_)
    {
        response->success =
            false;

        response->message =
            "Player position not received";

        return;
    }


    if (
        static_cast<int>(
            zombies_.size()
        ) >= maxZombieCount_)
    {
        response->success =
            false;

        response->message =
            "Maximum zombie count reached";

        return;
    }


    bool result =
        spawnZombie();


    if (result)
    {
        response->success =
            true;

        response->message =
            "Zombie spawned";
    }
    else
    {
        response->success =
            false;

        response->message =
            "Zombie spawn failed";
    }
}


bool ZombieAiNode::isNearCat(
    int row,
    int col)
{
    for (
        int i = 0;
        i < static_cast<int>(
            cats_.size()
        );
        i++)
    {
        int distance =
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
            distance <=
            catFearRadius_)
        {
            return true;
        }
    }


    return false;
}


bool ZombieAiNode::spawnZombie()
{
    if (mapManager_ == nullptr)
    {
        return false;
    }


    if (!hasPlayerPosition_)
    {
        return false;
    }


    if (
        static_cast<int>(
            zombies_.size()
        ) >= maxZombieCount_)
    {
        return false;
    }


    // 최대 200회 위치 탐색
    for (
        int attempt = 0;
        attempt < 200;
        attempt++)
    {
        int spawnRow = 0;

        int spawnCol = 0;


        int side =
            std::rand() % 4;


        // 위쪽
        if (side == 0)
        {
            spawnRow = 0;

            spawnCol =
                std::rand() %
                mapManager_->
                getMapCols();
        }

        // 아래쪽
        else if (side == 1)
        {
            spawnRow =
                mapManager_->
                getMapRows() - 1;

            spawnCol =
                std::rand() %
                mapManager_->
                getMapCols();
        }

        // 왼쪽
        else if (side == 2)
        {
            spawnRow =
                std::rand() %
                mapManager_->
                getMapRows();

            spawnCol = 0;
        }

        // 오른쪽
        else
        {
            spawnRow =
                std::rand() %
                mapManager_->
                getMapRows();

            spawnCol =
                mapManager_->
                getMapCols() - 1;
        }


        // 건물
        if (
            mapManager_->
            getCollisionValue(
                spawnRow,
                spawnCol
            ) == 1)
        {
            continue;
        }


        // 다른 좀비
        if (
            isZombieAtPosition(
                spawnRow,
                spawnCol,
                -1
            ))
        {
            continue;
        }


        // 고양이 주변
        if (
            isNearCat(
                spawnRow,
                spawnCol
            ))
        {
            continue;
        }


        // 플레이어와 거리
        int distance =
            std::abs(
                playerRow_ -
                spawnRow
            )
            +
            std::abs(
                playerCol_ -
                spawnCol
            );


        if (distance < 12)
        {
            continue;
        }


        Zombie zombie;


        bool placed =
            zombie.
            setStartPosition(
                spawnRow,
                spawnCol,
                mapManager_
            );


        if (!placed)
        {
            continue;
        }


        zombies_.
            push_back(
                zombie
            );


        publishZombiePositions();


        return true;
    }


    return false;
}


bool ZombieAiNode::isZombieAtPosition(
    int row,
    int col,
    int ignoreIndex)
{
    for (
        int i = 0;
        i < static_cast<int>(
            zombies_.size()
        );
        i++)
    {
        if (i == ignoreIndex)
        {
            continue;
        }


        if (
            zombies_[i].
            getRow() == row &&
            zombies_[i].
            getCol() == col)
        {
            return true;
        }
    }


    return false;
}


void ZombieAiNode::moveZombies()
{
    if (!hasPlayerPosition_)
    {
        return;
    }


    if (mapManager_ == nullptr)
    {
        return;
    }


    for (
        int i = 0;
        i < static_cast<int>(
            zombies_.size()
        );
        i++)
    {
        // 현재 고양이 위치를 기준으로 A*
        std::vector<std::pair<int, int>> path =
            pathFinder_.
            findPath(
                zombies_[i].
                getRow(),

                zombies_[i].
                getCol(),

                playerRow_,
                playerCol_,

                mapManager_,

                cats_,

                catFearRadius_
            );


        if (path.size() < 2)
        {
            continue;
        }


        int nextRow =
            path[1].first;


        int nextCol =
            path[1].second;


        if (
            isZombieAtPosition(
                nextRow,
                nextCol,
                i
            ))
        {
            continue;
        }


        zombies_[i].
            moveTo(
                nextRow,
                nextCol,
                mapManager_
            );
    }


    publishZombiePositions();
}


void ZombieAiNode::publishZombiePositions()
{
    std_msgs::msg::Int32MultiArray msg;


    for (
        int i = 0;
        i < static_cast<int>(
            zombies_.size()
        );
        i++)
    {
        msg.data.push_back(
            zombies_[i].
            getRow()
        );


        msg.data.push_back(
            zombies_[i].
            getCol()
        );
    }


    zombiePositionsPublisher_->
        publish(
            msg
        );
}


void ZombieAiNode::run()
{
    while (
        rclcpp::ok() &&
        !isInterruptionRequested())
    {
        rclcpp::spin_some(
            node_
        );


        QThread::msleep(
            20
        );
    }
}


ZombieAiNode::~ZombieAiNode()
{
    requestInterruption();

    wait();
}