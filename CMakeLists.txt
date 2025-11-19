cmake_minimum_required(VERSION 3.16)

project(EggCatcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Detect and find Qt version ----
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Multimedia)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    leaderboardmanager.cpp
    mainwindow.h
    leaderboardmanager.h
    mainwindow.ui
    my_label.cpp
    my_label.h
)

# ---- Executable section ----
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(EggCatcher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(EggCatcher SHARED ${PROJECT_SOURCES})
    else()
        add_executable(EggCatcher ${PROJECT_SOURCES})
    endif()
endif()

# ---- Link libraries ----
target_link_libraries(EggCatcher PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

# ---- macOS/iOS Bundle ----
if(DEFINED QT_VERSION AND QT_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EggCatcher)
endif()

set_target_properties(EggCatcher PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ---- Installation ----
include(GNUInstallDirs)
install(TARGETS EggCatcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Finalization (Qt6 only) ----
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EggCatcher)
endif()
