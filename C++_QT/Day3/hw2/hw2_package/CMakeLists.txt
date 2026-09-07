cmake_minimum_required(VERSION 3.16)

project(hw2 VERSION 0.1 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 자동 처리
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ui 파일 위치
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Qt5 / Qt6 지원
find_package(
    QT NAMES Qt6 Qt5
    REQUIRED COMPONENTS Widgets
)

find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS Widgets
)

# 프로젝트 파일
set(PROJECT_SOURCES

    # cpp 파일
    src/main.cpp
    src/mainwindow.cpp
    src/mazewidget.cpp
    src/dijkstrasolver.cpp
    src/astarsolver.cpp

    # header 파일
    include/mainwindow.h
    include/mazewidget.h
    include/dijkstrasolver.h
    include/astarsolver.h

    # ui 파일
    ui/mainwindow.ui
)

# 실행 파일 생성
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(
        hw2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()

    if(ANDROID)

        add_library(
            hw2
            SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(
            hw2
            ${PROJECT_SOURCES}
        )

    endif()

endif()

# header 파일 검색 경로
target_include_directories(
    hw2
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Qt Widgets 연결
target_link_libraries(
    hw2
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Qt6 마무리
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hw2)
endif()
