cmake_minimum_required(VERSION 3.22)
project(chat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        Multimedia
        MultimediaWidgets
        REQUIRED)

add_executable(chat ${SRC} ${HEADERS})
target_link_libraries(chat
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
		Qt::Multimedia
		Qt::MultimediaWidgets
)



