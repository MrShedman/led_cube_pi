cmake_minimum_required(VERSION 3.1)
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(APP_NAME main)
project (${APP_NAME})

find_package(glm REQUIRED)
find_package(YAML-CPP REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

file(GLOB rpi_matrix_HEADERS lib/matrix/include/*.h)
file(GLOB rpi_matrix_SOURCES lib/matrix/lib/*.c*)

add_library(rpi_matrix STATIC ${rpi_matrix_HEADERS} ${rpi_matrix_SOURCES})

target_link_libraries(rpi_matrix
	pthread
	rt
	m)

include_directories(
    ${GLM_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
    include/
    src/
    src/effects/
    src/math/
    src/rendering/
    src/resource/
    lib/
    lib/matrix/include/
)

add_executable( ${APP_NAME} 
./src/main.cpp
./src/Application.cpp
./src/Utilities.cpp
./src/effects/EffectBase.cpp
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
${YAML_CPP_LIBRARIES}
rpi_matrix
)
