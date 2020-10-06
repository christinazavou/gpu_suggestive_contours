#cmake_minimum_required(VERSION 3.17)
project(contourproject)

#set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE contourapp)


include_directories(
        include/trimesh2
        include/trimesh2/GL
)
file(GLOB HEADERS
        "*.h"
        "include/trimesh2/*.h"
        "include/trimesh2/GL/*.h"
        "src/*.h")
file(GLOB SOURCES
        "*.cc"
        "src/*.cpp"
        "src/*.cc"
        "src/trimesh2/libsrc/*.cc"
        "src/trimesh2/libsrc/*.inc"
        "src/trimesh2/utilsrc/*.inc")
message("HEADERS: ${HEADERS}")
message("SOURCES: ${SOURCES}")
add_executable(${EXECUTABLE}
        ${HEADERS}
        ${SOURCES})

#target_link_libraries(${EXECUTABLE} -lglu32 -lopengl32 -lfreeglut)
#target_link_libraries(${EXECUTABLE} /usr/lib/libglui.a /usr/lib/libglut.a)
#this one gives "skipping incompatible //usr/lib/libglui.a when searching for -lglui"

#g++ -O3 -Wno-unused-result *.cc src/*.cc src/*.cpp src/trimesh2/libsrc/*.cc src/trimesh2/libsrc/*.inc src/trimesh2/utilsrc/*.inc -Iinclude/trimesh2 -Iinclude/trimesh2/GL /usr/lib/libglui.a /usr/lib/libglut.a
#g++ -O3 -Wno-unused-result *.cc src/*.cc src/*.cpp src/trimesh2/libsrc/*.cc -Iinclude/trimesh2 -Iinclude/trimesh2/GL /usr/lib/libglui.a /usr/lib/libglut.a
#this one gives " i386 architecture of input file `/usr/lib/libglut.a(glut_event.o)' is incompatible with i386:x86-64 output"
