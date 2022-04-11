project(gui)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
add_compile_definitions(BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(
    Boost 
    COMPONENTS 
    date_time
    chrono
    program_options
    coroutine
    log
    REQUIRED
)
find_package(nlohmann_json REQUIRED)
find_package(Mosquitto REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS net core base xrc)
include(${wxWidgets_USE_FILE})

add_subdirectory(mathplot)
include_directories(${PROJECT_SOURCE_DIR}/mathplot)

add_executable(gui
    src/gui.cpp
    src/Mosquitto.cpp
    src/MosquittoClient.cpp
    src/Widget.cpp
    src/ASRWidget.cpp
    src/ScoreWidget.cpp
    src/Frontend.cpp
)

target_link_libraries(
    gui
    mathplot
    nlohmann_json::nlohmann_json
    ${Mosquitto_LIBRARIES}
    ${Boost_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    -pthread
)


file(COPY templates/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/templates/)
file(COPY config/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/config/)
