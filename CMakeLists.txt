cmake_minimum_required(VERSION 3.0)
project(gdk VERSION 1.0)

set(SRC_DIR src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HDRS ${SRC_DIR}/*.h)

foreach(HEADER ${HDRS})
    get_filename_component(INC ${HEADER} PATH)
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${INC})
endforeach()


list(REMOVE_DUPLICATES INCLUDE_DIRS)


find_package(Bullet REQUIRED)
find_package(Panda3D REQUIRED Core Tools Direct Contrib OpenAL OpenGL Bullet)


set(PANDA_LIBS 
    Panda3D::Core::panda
    Panda3D::Core::pandaexpress
    Panda3D::Core::p3dtool
    Panda3D::Direct::p3direct
    Panda3D::Contrib::p3ai
    Panda3D::Bullet::p3bullet
    Panda3D::OpenAL::p3openal_audio
    Panda3D::OpenGL::pandagl
    Panda3D::Core::pandaphysics
)

add_definitions(-fno-rtti -DTIXML_USE_STL)
add_library(gdk SHARED ${SRCS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${PANDA_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
