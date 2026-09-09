#include <chrono>
#include <functional>
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstdint>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"

using namespace std::chrono_literals;

class TurtleShape : public rclcpp::Node
{
public:
    TurtleShape()
    : Node("turtle_shape"),
      pose_ready_(false),
      auto_mode_(true),
      auto_index_(0),
      current_shape_(' '),
      side_index_(0),
      target_sides_(0),
      side_length_(2.0),
      turn_angle_(0.0),
      initial_heading_(0.0),
      desired_heading_(0.0),
      start_x_(0.0),
      start_y_(0.0),
      previous_theta_(0.0),
      accumulated_turn_(0.0),
      target_x_(0.0),
      target_y_(0.0),
      target_theta_(0.0),
      pen_r_(0),
      pen_g_(0),
      pen_b_(0),
      pen_width_(1),
      prepare_count_(0)
    {
        cmd_pub_ =
            this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel",
                10
            );

        pose_sub_ =
            this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose",
                10,
                std::bind(
                    &TurtleShape::pose_callback,
                    this,
                    std::placeholders::_1
                )
            );

        pen_client_ =
            this->create_client<turtlesim::srv::SetPen>(
                "/turtle1/set_pen"
            );

        teleport_client_ =
            this->create_client<turtlesim::srv::TeleportAbsolute>(
                "/turtle1/teleport_absolute"
            );

        timer_ =
            this->create_wall_timer(
                50ms,
                std::bind(
                    &TurtleShape::timer_callback,
                    this
                )
            );

        state_ = State::STARTUP;

        RCLCPP_INFO(
            this->get_logger(),
            "Turtle Shape Node Start"
        );
    }

private:
    enum class State
    {
        STARTUP,
        WAIT_PEN_OFF,
        WAIT_TELEPORT,
        WAIT_PEN_ON,
        STRAIGHT,
        TURN,
        CIRCLE,
        WAIT_INPUT,
        MANUAL_WAIT_PEN
    };

    void pose_callback(
        const turtlesim::msg::Pose::SharedPtr msg)
    {
        current_pose_ = *msg;
        pose_ready_ = true;
    }

    void timer_callback()
    {
        if (!pose_ready_)
        {
            return;
        }

        switch (state_)
        {
        case State::STARTUP:
        {
            if (!pen_client_->service_is_ready() ||
                !teleport_client_->service_is_ready())
            {
                return;
            }

            start_auto_shape();
            break;
        }

        case State::WAIT_PEN_OFF:
        {
            stop_turtle();

            prepare_count_++;

            if (prepare_count_ >= 3)
            {
                prepare_count_ = 0;

                teleport_turtle(
                    target_x_,
                    target_y_,
                    target_theta_
                );

                state_ = State::WAIT_TELEPORT;
            }

            break;
        }

        case State::WAIT_TELEPORT:
        {
            stop_turtle();

            double distance =
                std::hypot(
                    current_pose_.x - target_x_,
                    current_pose_.y - target_y_
                );

            double angle_error =
                std::abs(
                    normalize_angle(
                        target_theta_ -
                        current_pose_.theta
                    )
                );

            if (distance < 0.1 &&
                angle_error < 0.1)
            {
                set_pen(
                    pen_r_,
                    pen_g_,
                    pen_b_,
                    pen_width_,
                    false
                );

                prepare_count_ = 0;
                state_ = State::WAIT_PEN_ON;
            }

            break;
        }

        case State::WAIT_PEN_ON:
        {
            stop_turtle();

            prepare_count_++;

            if (prepare_count_ >= 3)
            {
                prepare_count_ = 0;
                start_shape_motion();
            }

            break;
        }

        case State::STRAIGHT:
        {
            draw_straight();
            break;
        }

        case State::TURN:
        {
            rotate_to_target();
            break;
        }

        case State::CIRCLE:
        {
            draw_circle();
            break;
        }

        case State::WAIT_INPUT:
        {
            stop_turtle();

            char key;

            std::cout << "\n============================\n";
            std::cout << "W : Square\n";
            std::cout << "A : Triangle\n";
            std::cout << "S : Circle\n";
            std::cout << "D : Pentagon\n";
            std::cout << "Input : ";

            std::cin >> key;

            key =
                static_cast<char>(
                    std::toupper(
                        static_cast<unsigned char>(key)
                    )
                );

            if (key != 'W' &&
                key != 'A' &&
                key != 'S' &&
                key != 'D')
            {
                std::cout << "Wrong Input" << std::endl;
                break;
            }

            configure_shape(key);

            set_pen(
                pen_r_,
                pen_g_,
                pen_b_,
                pen_width_,
                false
            );

            prepare_count_ = 0;
            state_ = State::MANUAL_WAIT_PEN;

            break;
        }

        case State::MANUAL_WAIT_PEN:
        {
            stop_turtle();

            prepare_count_++;

            if (prepare_count_ >= 3)
            {
                prepare_count_ = 0;
                start_shape_motion();
            }

            break;
        }
        }
    }

    void configure_shape(char key)
    {
        current_shape_ = key;
        side_index_ = 0;

        if (key == 'W')
        {
            target_sides_ = 4;
            side_length_ = 2.0;
            turn_angle_ = PI / 2.0;

            pen_r_ = 255;
            pen_g_ = 0;
            pen_b_ = 0;
            pen_width_ = 3;

            RCLCPP_INFO(
                this->get_logger(),
                "Square Start"
            );
        }
        else if (key == 'A')
        {
            target_sides_ = 3;
            side_length_ = 2.0;
            turn_angle_ = 2.0 * PI / 3.0;

            pen_r_ = 255;
            pen_g_ = 255;
            pen_b_ = 0;
            pen_width_ = 5;

            RCLCPP_INFO(
                this->get_logger(),
                "Triangle Start"
            );
        }
        else if (key == 'S')
        {
            target_sides_ = 0;
            turn_angle_ = 0.0;

            pen_r_ = 0;
            pen_g_ = 0;
            pen_b_ = 255;
            pen_width_ = 7;

            RCLCPP_INFO(
                this->get_logger(),
                "Circle Start"
            );
        }
        else if (key == 'D')
        {
            target_sides_ = 5;
            side_length_ = 1.8;
            turn_angle_ = 2.0 * PI / 5.0;

            pen_r_ = 255;
            pen_g_ = 0;
            pen_b_ = 255;
            pen_width_ = 4;

            RCLCPP_INFO(
                this->get_logger(),
                "Pentagon Start"
            );
        }
    }

    void start_auto_shape()
    {
        if (auto_index_ == 0)
        {
            configure_shape('W');

            target_x_ = 2.0;
            target_y_ = 7.5;
            target_theta_ = 0.0;
        }
        else if (auto_index_ == 1)
        {
            configure_shape('A');

            target_x_ = 6.0;
            target_y_ = 7.5;
            target_theta_ = 0.0;
        }
        else if (auto_index_ == 2)
        {
            configure_shape('S');

            target_x_ = 5.5;
            target_y_ = 2.5;
            target_theta_ = 0.0;
        }

        set_pen(
            0,
            0,
            0,
            1,
            true
        );

        prepare_count_ = 0;
        state_ = State::WAIT_PEN_OFF;
    }

    void start_shape_motion()
    {
        side_index_ = 0;

        initial_heading_ =
            current_pose_.theta;

        if (current_shape_ == 'S')
        {
            previous_theta_ =
                current_pose_.theta;

            accumulated_turn_ = 0.0;

            state_ = State::CIRCLE;

            return;
        }

        start_x_ =
            current_pose_.x;

        start_y_ =
            current_pose_.y;

        desired_heading_ =
            initial_heading_;

        state_ =
            State::STRAIGHT;
    }

    void draw_straight()
    {
        geometry_msgs::msg::Twist msg;

        double distance =
            std::hypot(
                current_pose_.x - start_x_,
                current_pose_.y - start_y_
            );

        if (distance < side_length_)
        {
            msg.linear.x = 1.5;

            double heading_error =
                normalize_angle(
                    desired_heading_ -
                    current_pose_.theta
                );

            msg.angular.z =
                std::clamp(
                    2.0 * heading_error,
                    -0.5,
                    0.5
                );

            cmd_pub_->publish(msg);
        }
        else
        {
            stop_turtle();

            desired_heading_ =
                normalize_angle(
                    initial_heading_ +
                    turn_angle_ *
                    static_cast<double>(
                        side_index_ + 1
                    )
                );

            state_ = State::TURN;
        }
    }

    void rotate_to_target()
    {
        geometry_msgs::msg::Twist msg;

        double error =
            normalize_angle(
                desired_heading_ -
                current_pose_.theta
            );

        if (std::abs(error) > 0.02)
        {
            msg.linear.x = 0.0;

            msg.angular.z =
                std::clamp(
                    2.0 * error,
                    -1.0,
                    1.0
                );

            cmd_pub_->publish(msg);
        }
        else
        {
            stop_turtle();

            side_index_++;

            if (side_index_ >= target_sides_)
            {
                finish_shape();
                return;
            }

            start_x_ =
                current_pose_.x;

            start_y_ =
                current_pose_.y;

            state_ =
                State::STRAIGHT;
        }
    }

    void draw_circle()
    {
        geometry_msgs::msg::Twist msg;

        double delta =
            normalize_angle(
                current_pose_.theta -
                previous_theta_
            );

        accumulated_turn_ +=
            std::abs(delta);

        previous_theta_ =
            current_pose_.theta;

        if (accumulated_turn_ < 2.0 * PI)
        {
            msg.linear.x = 1.5;
            msg.angular.z = 1.0;

            cmd_pub_->publish(msg);
        }
        else
        {
            stop_turtle();
            finish_shape();
        }
    }

    void finish_shape()
    {
        stop_turtle();

        if (current_shape_ == 'W')
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Square Finished!"
            );
        }
        else if (current_shape_ == 'A')
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Triangle Finished!"
            );
        }
        else if (current_shape_ == 'S')
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Circle Finished!"
            );
        }
        else if (current_shape_ == 'D')
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Pentagon Finished!"
            );
        }

        if (auto_mode_)
        {
            auto_index_++;

            if (auto_index_ <= 2)
            {
                start_auto_shape();
            }
            else
            {
                auto_mode_ = false;
                current_shape_ = ' ';
                state_ = State::WAIT_INPUT;

                RCLCPP_INFO(
                    this->get_logger(),
                    "Initial Shapes Finished!"
                );

                RCLCPP_INFO(
                    this->get_logger(),
                    "W / A / S / D Input Mode"
                );
            }
        }
        else
        {
            current_shape_ = ' ';
            state_ = State::WAIT_INPUT;
        }
    }

    void stop_turtle()
    {
        geometry_msgs::msg::Twist msg;

        msg.linear.x = 0.0;
        msg.angular.z = 0.0;

        cmd_pub_->publish(msg);
    }

    void set_pen(
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t width,
        bool off)
    {
        if (!pen_client_->service_is_ready())
        {
            return;
        }

        auto request =
            std::make_shared<
                turtlesim::srv::SetPen::Request
            >();

        request->r = r;
        request->g = g;
        request->b = b;
        request->width = width;
        request->off = off ? 1 : 0;

        pen_client_->async_send_request(
            request
        );
    }

    void teleport_turtle(
        float x,
        float y,
        float theta)
    {
        if (!teleport_client_->service_is_ready())
        {
            return;
        }

        auto request =
            std::make_shared<
                turtlesim::srv::TeleportAbsolute::Request
            >();

        request->x = x;
        request->y = y;
        request->theta = theta;

        teleport_client_->async_send_request(
            request
        );
    }

    double normalize_angle(double angle)
    {
        while (angle > PI)
        {
            angle -= 2.0 * PI;
        }

        while (angle < -PI)
        {
            angle += 2.0 * PI;
        }

        return angle;
    }

    rclcpp::Publisher<
        geometry_msgs::msg::Twist
    >::SharedPtr cmd_pub_;

    rclcpp::Subscription<
        turtlesim::msg::Pose
    >::SharedPtr pose_sub_;

    rclcpp::Client<
        turtlesim::srv::SetPen
    >::SharedPtr pen_client_;

    rclcpp::Client<
        turtlesim::srv::TeleportAbsolute
    >::SharedPtr teleport_client_;

    rclcpp::TimerBase::SharedPtr timer_;

    turtlesim::msg::Pose current_pose_;

    bool pose_ready_;

    State state_;

    bool auto_mode_;

    int auto_index_;

    char current_shape_;

    int side_index_;

    int target_sides_;

    double side_length_;

    double turn_angle_;

    double initial_heading_;

    double desired_heading_;

    double start_x_;

    double start_y_;

    double previous_theta_;

    double accumulated_turn_;

    float target_x_;

    float target_y_;

    float target_theta_;

    uint8_t pen_r_;

    uint8_t pen_g_;

    uint8_t pen_b_;

    uint8_t pen_width_;

    int prepare_count_;

    static constexpr double PI =
        3.14159265358979323846;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<TurtleShape>()
    );

    rclcpp::shutdown();

    return 0;
}