import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Image
from cv_bridge import CvBridge

import cv2
from ultralytics import YOLO

import time


class YoloNode(Node):

    def __init__(self):
        super().__init__('yolo_node')

        self.bridge = CvBridge()
        self.model = YOLO("yolo26n.pt")

        # 파라미터 선언
        self.declare_parameter('topic', '/camera/image')
        self.topic = self.get_parameter('topic').value

        self.declare_parameter('class_index', [0])
        self.class_index = self.get_parameter('class_index').value

        self.declare_parameter('class_name',['person'])
        self.class_name = self.get_parameter('class_name').value

        self.declare_parameter('confidence', 0.8)
        self.confidence = self.get_parameter('confidence').value


        self.subscription = self.create_subscription( Image, self.topic, self.image_callback, 10)
        self.get_logger().info('YOLO Subscriber Node Start')
        
    def image_callback(self, msg):

        frame = self.bridge.imgmsg_to_cv2( msg, desired_encoding='bgr8')

        start_time = time.perf_counter() # 추론 시작 시간

        results = self.model(frame,conf=self.confidence,classes=self.class_index,device='cpu',verbose=False)

        end_time = time.perf_counter() # 추론 종료 시간

        inference_time = (end_time - start_time) * 1000.0
        self.get_logger().info(f'Inference Time: 'f'{inference_time:.2f} ms')


        #result_frame = results[0].plot()


        for box in results[0].boxes:

            # ID
            class_id = int(box.cls[0])

            # Confidence
            confidence = float(box.conf[0])

            # Bounding Box 좌표
            x1, y1, x2, y2 = (box.xyxy[0].cpu().numpy().astype(int))

            if (class_id in self.class_index and confidence >= self.confidence):

                index = self.class_index.index(class_id)
                name = self.class_name[index]

                # 출력할 문자열
                label = (f'{name} ' f'({confidence:.2f})')
                cv2.rectangle(frame,(x1, y1),(x2, y2), (0, 255, 0),2)
                text_y = max( y1 - 10, 20)
                cv2.putText(frame,label,(x1, text_y),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        cv2.imshow('YOLO', frame)
        cv2.waitKey(1)


def main(args=None):
    rclpy.init(args=args)
    node = YoloNode()
    rclpy.spin(node) 
    node.destroy_node()
    cv2.destroyAllWindows()
    rclpy.shutdown()


if __name__ == '__main__':
    main()