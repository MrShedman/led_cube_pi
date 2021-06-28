cmake_minimum_required(VERSION 3.1)
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(APP_NAME main)
project (${APP_NAME})

find_package(glm REQUIRED)
find_package(YAML-CPP REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DLINUX)
add_definitions(-DRASPBERRY)

file(GLOB rpi_matrix_HEADERS lib/matrix/include/*.h)
file(GLOB rpi_matrix_SOURCES lib/matrix/lib/*.c*)

add_library(rpi_matrix STATIC ${rpi_matrix_HEADERS} ${rpi_matrix_SOURCES})

target_link_libraries(rpi_matrix
	pthread
	rt
	m
    stdc++fs)

file(GLOB blynk_HEADERS lib/blynk-library/src/*.h)
file(GLOB blynk_SOURCES lib/blynk-library/src/utility/*.c*)

add_library(blynk STATIC ${blynk_HEADERS} ${blynk_SOURCES})

target_link_libraries(blynk
	pthread
	rt
	wiringPi)

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
    lib/blynk-library/linux/
    lib/blynk-library/src/
)

add_executable( ${APP_NAME} 
./src/main.cpp
./src/Application.cpp
./src/Utilities.cpp
./src/ConfigParser.cpp
./src/i2c.cpp
./src/mpu9250.cpp
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
blynk
)
