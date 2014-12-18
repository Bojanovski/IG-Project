project(IRG_project)

# Define the source files.
set(IRG_project_SRCS
    Main.cpp
    GameEngine.cpp
    PostProcessAA.cpp
    Car.cpp
    RacingTrack.cpp
    CarCameraHandler.cpp
    GraphicsSettings.cpp
    Terrain.cpp
)

# Define the header files.
set(IRG_project_HDRS
    Car.h
    GameEngine.h
    PostProcessAA.h
    RacingTrack.h
    CarCameraHandler.h
    GraphicsSettings.h
    Terrain.h
)

file(COPY Shaders DESTINATION .)

add_executable(IRG_project ${IRG_project_SRCS} ${IRG_project_HDRS})

target_link_libraries(IRG_project
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${IRRKLANG_LIBRARIES}
	${LEAPMOTION_LIBRARIES}
)
