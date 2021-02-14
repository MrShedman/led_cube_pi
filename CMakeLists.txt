cmake_minimum_required(VERSION 3.1)
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(APP_NAME main)
project (${APP_NAME})

find_package(glm REQUIRED)
find_package(YAML-CPP REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

include_directories(
    ${GLM_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
    include/
    src/
    src/math/
    src/rendering/
    src/resource/
    lib/
)

add_executable( ${APP_NAME} 
./src/main.cpp
./src/Application.cpp
./src/Utilities.cpp
./src/math/Angle.cpp
./src/rendering/Image.cpp
./src/rendering/Transform.cpp
./src/resource/ResourcePaths.cpp
./lib/chrono/clock.cpp
./lib/chrono/rate.cpp
./lib/chrono/time.cpp
./lib/filter/filter.cpp
)

target_link_libraries( ${APP_NAME} 
${YAML_CPP_LIBRARIES})