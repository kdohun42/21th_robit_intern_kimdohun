#ifndef QNODE_HPP_
#define QNODE_HPP_

#include <QThread>
#include <QVector>

#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/int32_multi_array.hpp"

#include "std_srvs/srv/trigger.hpp"

#include "cat.hpp"


class QNode : public QThread
{
    Q_OBJECT

public:
    QNode();

    ~QNode();

    // 플레이어 위치 전송
    void publishPlayerPosition(
        int row,
        int col
    );

    // 고양이 위치 전송
    void publishCatPositions(
        std::vector<Cat>& cats
    );

    // 좀비 생성 요청
    void requestSpawnZombie();

signals:
    void rosShutDown();

    void zombiePositionsReceived(
        QVector<int> positions
    );

protected:
    void run() override;

private:
    void zombiePositionsCallback(
        std_msgs::msg::Int32MultiArray::SharedPtr msg
    );


    rclcpp::Node::SharedPtr node_;


    rclcpp::Publisher<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr playerPositionPublisher_;


    rclcpp::Publisher<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr catPositionsPublisher_;


    rclcpp::Subscription<
        std_msgs::msg::Int32MultiArray
    >::SharedPtr zombiePositionsSubscriber_;


    rclcpp::Client<
        std_srvs::srv::Trigger
    >::SharedPtr spawnZombieClient_;
};

#endif