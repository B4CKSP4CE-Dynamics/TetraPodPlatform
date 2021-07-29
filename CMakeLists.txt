cmake_minimum_required(VERSION 3.7)

project(pavuk)

set(EASY3D_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
set(EASY3D_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party/Easy3D)

add_executable(viewer viewer.cpp)
target_link_libraries(viewer PRIVATE easy3d::core easy3d::fileio easy3d::renderer easy3d::viewer dwarf dw)