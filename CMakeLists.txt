cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)

project(contourproject)

set(OPENGL_LIBS_DIR /usr/lib/x86_64-linux-gnu)
SET(OPENGL_LIBS "libGL.so libglut.so libGLEW.so libEGL.so")

set(VIEWER_EXE ViewerApp)
set(RENDER_EXAMPLE_EXE RenderExampleApp)


include_directories(
        include/trimesh2
        include/trimesh2/GL
)
file(GLOB HEADERS
        "include/trimesh2/GL/*.h"
        "include/trimesh2/*.h"
        "src/*.h"
        "mysrc/*.h")
message("HEADERS: ${HEADERS}")

file(GLOB SOURCES
        "src/trimesh2/libsrc/*.cc"
        "src/trimesh2/libsrc/*.inc"
        "src/trimesh2/utilsrc/*.inc"
        "src/*.cpp"
        "src/*.cc")
message("SOURCES: ${SOURCES}")

link_directories(${OPENGL_LIBS_DIR})

add_executable(${VIEWER_EXE}
        ${HEADERS}
        ${SOURCES}
        "src/runners/Viewer.cpp")
target_link_libraries(${VIEWER_EXE} libGL.so libglut.so libGLEW.so)

add_executable(${RENDER_EXAMPLE_EXE}
        ${HEADERS}
        ${SOURCES}
        "src/runners/renderexample.cpp")
target_link_libraries(${RENDER_EXAMPLE_EXE} libGL.so libglut.so libGLEW.so libEGL.so)

add_executable(PPM2BMPExe src/ppm2bmp/ppm2bmp.cpp src/ppm2bmp/ppm2bmp.h)