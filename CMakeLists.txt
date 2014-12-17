cmake_minimum_required(VERSION 2.8)

project(AC_car_game) #alternative controls car game

set(BIN_DIR ${IRG_project_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(SDL2_BUILDING_LIBRARY FALSE)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(DevIL REQUIRED)
find_package(IrrKlang REQUIRED)
find_package(LeapSDK REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR} 
    ${GLEW_INCLUDE_PATH}
    ${OPENGL_INCLUDE_DIRS}
    ${IRRKLANG_INCLUDE_DIRS}
    ${LEAPMOTION_INCLUDE_DIRS}
    "${IL_INCLUDE_DIR}/.."
    "${PROJECT_SOURCE_DIR}"
)

add_subdirectory(Engine)
add_subdirectory(IRG_project)

file(COPY Resources DESTINATION .) 
