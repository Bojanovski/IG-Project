project(IRG_project)

# Define the source files.
set(IRG_project_SRCS
    Main.cpp
)

# Define the header files.
set(IRG_project_HDRS
)

file(COPY SimpleShader.frag SimpleShader.vert DESTINATION .)

add_executable(IRG_project ${IRG_project_SRCS} ${IRG_project_HDRS})

target_link_libraries(IRG_project
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)
