import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Float32
from std_msgs.msg import Bool


class MinimalPublisher(Node):

    def __init__(self):

        # ROS2 Node 생성
        super().__init__('python_publisher')

        # String Publisher
        self.publisher_string = self.create_publisher(
            String,
            'topic_string',
            10
        )

        # Int32 Publisher
        self.publisher_int32 = self.create_publisher(
            Int32,
            'topic_int32',
            10
        )

        # Float32 Publisher
        self.publisher_float32 = self.create_publisher(
            Float32,
            'topic_float32',
            10
        )

        # Bool Publisher
        self.publisher_bool = self.create_publisher(
            Bool,
            'topic_bool',
            10
        )

        # 0.5초마다 timer_callback 실행
        self.timer = self.create_timer(
            0.5,
            self.timer_callback
        )

        self.count = 0


    def timer_callback(self):

        # -------------------------
        # String 메시지
        # -------------------------

        message1 = String()

        message1.data = (
            'Hello, world! ' + str(self.count)
        )


        # -------------------------
        # Int32 메시지
        # -------------------------

        message2 = Int32()

        message2.data = self.count


        # -------------------------
        # Float32 메시지
        # -------------------------

        message3 = Float32()

        message3.data = 3.14


        # -------------------------
        # Bool 메시지
        # -------------------------

        message4 = Bool()

        message4.data = True


        # -------------------------
        # 터미널 출력
        # -------------------------

        self.get_logger().info(
            f"String publisher: '{message1.data}'"
        )

        self.get_logger().info(
            f"Int publisher: '{message2.data}'"
        )

        self.get_logger().info(
            f"Float publisher: '{message3.data:.2f}'"
        )

        self.get_logger().info(
            f"Bool publisher: '{message4.data}'"
        )


        # -------------------------
        # 실제 Topic Publish
        # -------------------------

        self.publisher_string.publish(message1)

        self.publisher_int32.publish(message2)

        self.publisher_float32.publish(message3)

        self.publisher_bool.publish(message4)


        # count 증가
        self.count += 1


def main(args=None):

    # ROS2 초기화
    rclpy.init(args=args)

    # Node 생성
    node = MinimalPublisher()

    # Node 계속 실행
    rclpy.spin(node)

    # Node 제거
    node.destroy_node()

    # ROS2 종료
    rclpy.shutdown()


if __name__ == '__main__':
    main()