cmake_minimum_required(VERSION 3.5)
project(thermal_conductivity_2D)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp)")

find_package(MPI)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED )

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(thermal_conductivity_2D main.cpp config.cpp visualization.cpp my_queue.h config.h visualization.h my_array.h)
add_executable(generate_field generate_field.cpp)


target_link_libraries(thermal_conductivity_2D ${MPI_C_LIBRARIES})
target_link_libraries(thermal_conductivity_2D Threads::Threads)
target_link_libraries(thermal_conductivity_2D  ${OpenCV_LIBS} )

