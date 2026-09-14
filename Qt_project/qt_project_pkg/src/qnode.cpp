#include "../include/qt_project_pkg/qnode.hpp"

#include <QMetaType>

#include <functional>
#include <memory>
#include <chrono>


QNode::QNode()
{
    // QVector<int> Signal 사용 등록
    qRegisterMetaType<QVector<int>>(
        "QVector<int>"
    );


    // ROS2 초기화
    if (!rclcpp::ok())
    {
        int argc = 0;

        char** argv = nullptr;

        rclcpp::init(
            argc,
            argv
        );
    }


    // ROS2 노드
    node_ =
        rclcpp::Node::make_shared(
            "qt_game_node"
        );


    // 플레이어 위치 Publisher
    playerPositionPublisher_ =
        node_->
        create_publisher<
            std_msgs::msg::Int32MultiArray
        >(
            "player_position",
            10
        );


    // 고양이 위치 Publisher
    catPositionsPublisher_ =
        node_->
        create_publisher<
            std_msgs::msg::Int32MultiArray
        >(
            "cat_positions",
            10
        );


    // 좀비 위치 Subscriber
    zombiePositionsSubscriber_ =
        node_->
        create_subscription<
            std_msgs::msg::Int32MultiArray
        >(
            "zombie_positions",
            10,
            std::bind(
                &QNode::
                zombiePositionsCallback,
                this,
                std::placeholders::_1
            )
        );


    // 좀비 생성 Service Client
    spawnZombieClient_ =
        node_->
        create_client<
            std_srvs::srv::Trigger
        >(
            "spawn_zombie"
        );


    start();
}


void QNode::publishPlayerPosition(
    int row,
    int col)
{
    if (node_ == nullptr)
    {
        return;
    }


    std_msgs::msg::Int32MultiArray msg;


    msg.data.push_back(
        row
    );


    msg.data.push_back(
        col
    );


    playerPositionPublisher_->
        publish(
            msg
        );
}


void QNode::publishCatPositions(
    std::vector<Cat>& cats)
{
    if (node_ == nullptr)
    {
        return;
    }


    std_msgs::msg::Int32MultiArray msg;


    for (
        int i = 0;
        i < static_cast<int>(
            cats.size()
        );
        i++)
    {
        msg.data.push_back(
            cats[i].
            getRow()
        );


        msg.data.push_back(
            cats[i].
            getCol()
        );
    }


    catPositionsPublisher_->
        publish(
            msg
        );
}


void QNode::requestSpawnZombie()
{
    if (spawnZombieClient_ == nullptr)
    {
        return;
    }


    // Service 연결 확인
    if (
        !spawnZombieClient_->
        wait_for_service(
            std::chrono::milliseconds(
                500
            )
        ))
    {
        RCLCPP_WARN(
            node_->get_logger(),
            "spawn_zombie service unavailable"
        );

        return;
    }


    std::shared_ptr<
        std_srvs::srv::Trigger::Request
    > request =
        std::make_shared<
            std_srvs::srv::Trigger::Request
        >();


    spawnZombieClient_->
        async_send_request(
            request,

            [this](
                rclcpp::Client<
                    std_srvs::srv::Trigger
                >::SharedFuture future)
            {
                std::shared_ptr<
                    std_srvs::srv::Trigger::Response
                > response =
                    future.get();


                if (response->success)
                {
                    RCLCPP_INFO(
                        node_->get_logger(),
                        "%s",
                        response->
                        message.
                        c_str()
                    );
                }
                else
                {
                    RCLCPP_WARN(
                        node_->get_logger(),
                        "%s",
                        response->
                        message.
                        c_str()
                    );
                }
            }
        );
}


void QNode::zombiePositionsCallback(
    std_msgs::msg::Int32MultiArray::SharedPtr msg)
{
    QVector<int> positions;


    for (
        int i = 0;
        i < static_cast<int>(
            msg->data.size()
        );
        i++)
    {
        positions.
            push_back(
                msg->data[i]
            );
    }


    emit zombiePositionsReceived(
        positions
    );
}


void QNode::run()
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


    emit rosShutDown();
}


QNode::~QNode()
{
    requestInterruption();

    wait();


    if (rclcpp::ok())
    {
        rclcpp::shutdown();
    }
}