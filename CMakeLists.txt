#cmake_minimum_required(VERSION 3.17)
project(contourproject)

#set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE contourapp)


#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS_RELEASE " -Wno-unused-result -O3 -s -DNDEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE " -Wno-unused-result -O3 -s -DNDEBUG -fprofile-use")

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