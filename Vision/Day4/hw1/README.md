## 사용한 모델
사용한 모델 : yolo26n<br>
https://platform.ultralytics.com/ultralytics/yolo26/yolo26n<br>

학습한 데이터 셋 : coco dataset

## 폴더 구조

카메라에서 이미지 publisher 하는 노드 - camera_node <br>
카메라 이미지 subscriber 및 YOLO 추론 노드 yolo_node <br>

camera_node는 C++로 구현, YOLO 추론 노드는 파이썬으로 구현하여 각각의 패키지를 만들었다. YOLO 패키지 안에 YAML 파일과 launch 파일을 구성하였다. 

├── camera_pkg <br>
│   ├── include <br>
│   │   └── camera_pkg <br>
│   │       └── camera_node <br>
│   │           └── camera_node.hpp <br>
│   └── src <br>
│       └── camera_node <br>
│           └── camera_node.cpp <br>
│ 
└── yolo_pkg<br>
    ├── config <br>
    │   └── config.yaml <br>
    ├── launch <br>
    │   └── hw1.launch.py <br>
    └── yolo_pkg <br>
&nbsp;&nbsp;└── yolo_node <br>
&nbsp;&nbsp;&nbsp;&nbsp; └── yolo_node.py <br>


## 프로그램 빌드 및 실행 방법 
1. 터미널 열었을 때. <br>
``` bash 
source /opt/ros/jazzy/setup.bash

source ~/venvs/yolo/bin/activate
```
ROS2 jazzy 환경 불러오기 및 가상 환경 활성화<br>

2. 워크 스페이스 이동
``` bash
cd ~/Desktop/colcon_ws
``` 
3. 패키지 빌드 
``` bash
colcon build --symlink-install --packages-select camera_pkg yolo_pkg
``` 
4. ROS2 환경 불러오기
``` bash
source install/setup.bash
``` 
5. lainch 파일로 한 번에 실행 및 각 노드 따로 실행
``` bash
ros2 launch yolo_pkg hw1.launch.py

ros2 run camera_pkg camera_node

ros2 run yolo_pkg yolo_node
``` 
각 노드 실행할 때 따른 터미널에서 <br>
진행 따른 터미널에서도 환경 세팅 필요

## YAML 파일
``` YAML
yolo_node:
  ros__parameters:
    topic: "/camera/image"

    class_index:
      - 0
      - 67

    class_name:
      - "person"
      - "cell phone"

    confidence: 0.8
``` 

YAML에 ID 0번과 해당하는 ID를 인식했을 때 출력되는 이름을 설정해놨다. 위의 설정한 다른 객체 말고는 인식되지 않는다. 그리고 아래에 신회도 0.8로 설정해둠으로써 YOLO 내에서 계산된 신뢰도가 0.8 이상일 때 지정한 객체가 인식된 것으로 하도록 설정했다.

## 주요 코드 
``` cpp
if (class_id in self.class_index and confidence >= self.confidence):

                index = self.class_index.index(class_id)
                name = self.class_name[index]
``` 
위의 코드를 사용해 YAML 파일에서 지정한 ID와 신뢰도가 모두 만족되었을 때 화면 출력하도록 하였다.