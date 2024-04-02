# NoCSim
# Ahmed Hussein (amhussein4@gmail.com)
# April 1st 2024

cmake_minimum_required(VERSION 2.9)

project(nocsim, CXX)

# run cmake on dependencies
add_subdirectory(network)

# define an executable target, its include directories and link libraries
add_executable(nocsim nocsim.cpp)
target_include_directories(nocsim PUBLIC ${PROJECT_SOURCE_DIR}/network)
target_link_libraries(nocsim PRIVATE network)
