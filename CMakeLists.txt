#cmake_minimum_required(VERSION 3.17)
project(contourproject)

#set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE contourapp)


include_directories(
        include/trimesh2
        include/trimesh2/GL
)
file(GLOB HEADERS
        "include/trimesh2/GL/*.h"
        "*.h"
        "include/trimesh2/*.h"
        "src/*.h")
file(GLOB SOURCES
        "src/*.cpp"
        "src/*.cc"
        "src/trimesh2/libsrc/*.cc"
        "src/trimesh2/libsrc/*.inc"
        "src/trimesh2/utilsrc/*.inc")
message("HEADERS: ${HEADERS}")
message("SOURCES: ${SOURCES}")

link_directories(/usr/lib/x86_64-linux-gnu)
add_executable(${EXECUTABLE}
        ${HEADERS}
        ${SOURCES})
target_link_libraries(${EXECUTABLE} libGL.so libglut.so libGLEW.so)
