cmake_minimum_required(VERSION 3.16)

project(hw3 VERSION 0.1 LANGUAGES CXX)

# C++17 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 자동 처리
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# mainwindow.ui가 ui 폴더 안에 있으므로 경로 지정
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Qt5 / Qt6 둘 다 지원
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 프로젝트 소스 파일
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp

    include/mainwindow.h

    ui/mainwindow.ui
)

# 실행 파일 생성
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(hw3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()

    if(ANDROID)
        add_library(hw3 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(hw3
            ${PROJECT_SOURCES}
        )
    endif()

endif()

# 헤더 파일 위치
target_include_directories(hw3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Qt Widgets 연결
target_link_libraries(hw3 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Qt6 마무리
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hw3)
endif()
