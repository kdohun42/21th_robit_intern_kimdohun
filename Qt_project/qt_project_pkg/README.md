<프로젝트 빌드 및 실행>
cd ~/Desktop/colcon_ws
colcon build \--symlink-install \--packages-select qt_project_pkg
source install/setup.bash
ros2 run qt_project_pkg qt_project_pkg

<프로젝트 파일 구조>
qt_project_pkg/
│
├── CMakeLists.txt
│   └── Qt, ROS2 라이브러리 연결 및 전체 소스 파일 빌드 설정
│
├── package.xml
│   └── ROS2 패키지 정보와 rclcpp, std_msgs, std_srvs 등의 의존성 설정
│
├── include/
│   └── qt_project_pkg/
│       │
│       ├── main_window.hpp
│       │   └── MainWindow 클래스 선언, 게임 전체 진행과 UI 제어에 필요한 변수 및 함수 선언
│       │
│       ├── qnode.hpp
│       │   └── Qt와 ROS2를 연결하는 QNode 클래스 선언, Topic 및 Service 통신 함수 선언
│       │
│       ├── zombie_ai_node.hpp
│       │   └── 좀비 AI ROS2 노드 선언, 플레이어 추적 및 좀비 생성 관련 함수 선언
│       │
│       ├── map_manager.hpp
│       │   └── 게임 맵 크기와 충돌 맵을 관리하는 MapManager 클래스 선언
│       │
│       ├── building.hpp
│       │   └── 개별 건물의 위치, 크기, 식량 여부, 문 위치 등의 정보를 관리하는 클래스 선언
│       │
│       ├── building_manager.hpp
│       │   └── 여러 초록 건물과 빨간 벙커의 생성 및 관리를 담당하는 클래스 선언
│       │
│       ├── player.hpp
│       │   └── 플레이어 위치, 이동, 식량 개수 등을 관리하는 Player 클래스 선언
│       │
│       ├── zombie.hpp
│       │   └── 개별 좀비의 위치와 이동 정보를 관리하는 Zombie 클래스 선언
│       │
│       ├── cat.hpp
│       │   └── 고양이 위치와 이동 상태를 관리하는 Cat 클래스 선언
│       │
│       ├── canned_food.hpp
│       │   └── 통조림 아이템의 위치와 획득 여부를 관리하는 클래스 선언
│       │
│       ├── torch.hpp
│       │   └── 횃불 아이템의 위치와 획득 여부를 관리하는 클래스 선언
│       │
│       ├── path_finder.hpp
│       │   └── A* 알고리즘을 이용하여 좀비와 고양이의 이동 경로를 계산하는 클래스 선언
│       │
│       ├── vision_manager.hpp
│       │   └── 플레이어 시야 범위 감소 및 횃불 획득에 따른 시야 회복을 관리하는 클래스 선언
│       │
│       └── game_renderer.hpp
│           └── 맵, 건물, 캐릭터, 아이템, 어둠 등을 화면에 출력하는 렌더링 클래스 선언
│
├── src/
│   │
│   ├── main.cpp
│   │   └── Qt 프로그램을 시작하고 MainWindow를 생성하는 프로그램 시작 파일
│   │
│   ├── main_window.cpp
│   │   └── 키 입력, 플레이어 이동, 아이템 획득, 게임 오버/클리어, 카메라, Timer 등 게임 전체 진행 구현
│   │
│   ├── qnode.cpp
│   │   └── ROS2 Topic Publisher/Subscriber와 좀비 생성 Service Client 기능 구현
│   │
│   ├── zombie_ai_node.cpp
│   │   └── 플레이어 위치를 받아 A* 경로 탐색으로 좀비를 이동시키고 좀비 생성 Service를 처리
│   │
│   ├── map_manager.cpp
│   │   └── 60×78 게임 맵과 충돌 정보를 생성하고 충돌 여부를 확인하는 기능 구현
│   │
│   ├── building.cpp
│   │   └── 건물의 위치, 크기, 문 위치, 식량 여부와 조사 상태 등의 기능 구현
│   │
│   ├── building_manager.cpp
│   │   └── 초록 건물 여러 개와 빨간 벙커를 배치하고 건물 충돌 정보를 맵에 적용
│   │
│   ├── player.cpp
│   │   └── 플레이어 시작 위치 설정, 이동 가능 여부 확인, 이동 및 식량 획득 기능 구현
│   │
│   ├── zombie.cpp
│   │   └── 좀비 시작 위치 설정과 이동 위치 변경 기능 구현
│   │
│   ├── cat.cpp
│   │   └── 고양이 시작 위치 설정과 플레이어를 따라 이동하는 데 필요한 위치 변경 기능 구현
│   │
│   ├── canned_food.cpp
│   │   └── 통조림의 맵 배치 위치와 플레이어가 획득했는지 여부를 관리
│   │
│   ├── torch.cpp
│   │   └── 횃불의 맵 배치 위치와 획득 상태를 관리
│   │
│   ├── path_finder.cpp
│   │   └── A* 알고리즘을 구현하여 시작 위치에서 목표 위치까지의 최단 이동 경로 계산
│   │
│   ├── vision_manager.cpp
│   │   └── 일정 시간마다 시야를 감소시키고 횃불 획득 시 시야를 증가시키는 기능 구현
│   │
│   └── game_renderer.cpp
│       └── 잔디, 건물, 플레이어, 좀비, 고양이, 통조림, 횃불, 어둠 효과를 QPixmap에 출력
│
├── ui/
│   └── mainwindow.ui
│       └── Qt Designer로 제작한 게임 GUI 화면 구성
│           ├── label_map : 실제 게임 화면 출력
│           ├── label_food : 획득한 식량 개수 출력
│           ├── label_zombie_count : 현재 좀비 수 출력
│           ├── label_game_status : 통조림 개수, 시야, 게임 상태 출력
│           └── label_game_message : GAME START, GAME OVER, GAME CLEAR를 크게 출력
│
└── resources/
    │
    ├── images.qrc
    │   └── Qt에서 사용하는 이미지 리소스를 등록하고 관리하는 파일
    │
    └── images/
        │
        ├── Background_Green_TileSet.png
        │   └── 게임 맵의 잔디 배경을 구성하기 위한 타일 이미지
        │
        ├── buildings/
        │   │
        │   ├── building_green.png
        │   │   └── 식량을 탐색할 수 있는 일반 초록 건물 이미지
        │   │
        │   └── building_red.png
        │       └── 식량 3개를 모은 후 도착해야 하는 최종 벙커 이미지
        │
        ├── player/
        │   └── player.png
        │       └── 플레이어 캐릭터 이미지
        │
        ├── zombie/
        │   └── zombie.png
        │       └── 플레이어를 추적하는 좀비 캐릭터 이미지
        │
        ├── cat/
        │   └── cat.png
        │       └── 좀비가 접근하지 못하는 영역을 생성하고 통조림 사용 시 플레이어를 따라오는 고양이 이미지
        │
        └── items/
            │
            ├── can.png
            │   └── 고양이를 일정 시간 플레이어에게 따라오게 하기 위해 사용하는 통조림 아이템 이미지
            │
            └── fire.png
                └── 획득하면 플레이어의 시야 범위를 증가시키는 횃불 아이템 이미지