cmake_minimum_required(VERSION 3.5)

project(hw2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(
    QT NAMES Qt6 Qt5
    REQUIRED COMPONENTS Widgets
)

find_package(
    Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS Widgets
)

set(PROJECT_SOURCES

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    mazewidget.cpp
    mazewidget.h

    dijkstrasolver.cpp
    dijkstrasolver.h

    astarsolver.cpp
    astarsolver.h
)

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

target_link_libraries(
    hw2
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

if(${QT_VERSION} VERSION_LESS 6.1.0)

    set(
        BUNDLE_ID_OPTION

        MACOSX_BUNDLE_GUI_IDENTIFIER
        com.example.hw2
    )

endif()

set_target_properties(
    hw2
    PROPERTIES

    ${BUNDLE_ID_OPTION}

    MACOSX_BUNDLE_BUNDLE_VERSION
    ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(
    TARGETS hw2

    BUNDLE DESTINATION .

    LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)

    qt_finalize_executable(hw2)

endif()
