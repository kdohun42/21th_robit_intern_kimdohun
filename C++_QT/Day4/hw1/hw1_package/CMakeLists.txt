cmake_minimum_required(VERSION 3.5)

project(hw1_package VERSION 0.1 LANGUAGES CXX)

# Qt 자동 처리
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++17 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# UI 파일을 찾을 위치
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Qt Widgets + Network
find_package(
    QT NAMES Qt6 Qt5
    REQUIRED COMPONENTS Widgets Network
)

find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS Widgets Network
)

# 프로젝트 파일 경로
set(PROJECT_SOURCES

    src/main.cpp
    src/mainwindow.cpp

    include/mainwindow.h

    ui/mainwindow.ui
)

# 실행 파일 생성
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(
        hw1_package

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

else()

    if(ANDROID)

        add_library(
            hw1_package
            SHARED

            ${PROJECT_SOURCES}
        )

    else()

        add_executable(
            hw1_package

            ${PROJECT_SOURCES}
        )

    endif()

endif()

# header 파일 위치 등록
target_include_directories(
    hw1_package
    PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Qt 라이브러리 연결
target_link_libraries(
    hw1_package
    PRIVATE

    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# macOS 관련 설정
if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(
        BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.hw1_package
    )

endif()

set_target_properties(
    hw1_package
    PROPERTIES

    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
    ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install 설정
include(GNUInstallDirs)

install(
    TARGETS hw1_package

    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 finalize
if(QT_VERSION_MAJOR EQUAL 6)

    qt_finalize_executable(hw1_package)

endif()
