import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Float32
from std_msgs.msg import Bool


class MinimalSubscriber(Node):

    def __init__(self):

        super().__init__('python_subscriber')


        # -------------------------
        # String Subscriber
        # -------------------------

        self.subscription_string = self.create_subscription(
            String,
            'topic_string',
            self.topic_callback_string,
            10
        )


        # -------------------------
        # Int32 Subscriber
        # -------------------------

        self.subscription_int32 = self.create_subscription(
            Int32,
            'topic_int32',
            self.topic_callback_int,
            10
        )


        # -------------------------
        # Float32 Subscriber
        # -------------------------

        self.subscription_float32 = self.create_subscription(
            Float32,
            'topic_float32',
            self.topic_callback_float,
            10
        )


        # -------------------------
        # Bool Subscriber
        # -------------------------

        self.subscription_bool = self.create_subscription(
            Bool,
            'topic_bool',
            self.topic_callback_bool,
            10
        )


    # =============================
    # String Callback
    # =============================

    def topic_callback_string(self, msg):

        self.get_logger().info(
            f"String subscriber: '{msg.data}'"
        )


    # =============================
    # Int Callback
    # =============================

    def topic_callback_int(self, msg):

        self.get_logger().info(
            f"Int subscriber: '{msg.data}'"
        )


    # =============================
    # Float Callback
    # =============================

    def topic_callback_float(self, msg):

        self.get_logger().info(
            f"Float subscriber: '{msg.data:.2f}'"
        )




    def topic_callback_bool(self, msg):

        self.get_logger().info(
            f"Bool subscriber: '{msg.data}'"
        )


def main(args=None):

    rclpy.init(args=args)

    node = MinimalSubscriber()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == '__main__':
    main()