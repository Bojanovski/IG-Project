project(Engine)

set(Engine_HDRS
    Engine.h
)

set(Engine_Common_SRCS
    Common/ErrorCheck.cpp
    Common/MathFunctions.cpp
    Common/MathFunctions.tpp
)

set(Engine_Common_HDRS
    Common/ErrorCheck.h
    Common/MathFunctions.h
)
  
set(Engine_Core_SRCS
    Core/SDLHandler.cpp
    Core/Camera.cpp
    Core/CameraHandler.cpp
    Core/DefaultCameraHandler.cpp
    Core/EventHandler.cpp
    Core/EventListener.cpp
    Core/Updateable.cpp
)

set(Engine_Core_HDRS
    Core/Core.h
    Core/SDLHandler.h
    Core/Camera.h
    Core/CameraHandler.h
    Core/DefaultCameraHandler.h
    Core/EventHandler.h
    Core/EventListener.h
    Core/Updateable.h
)

set(Engine_GL_SRCS
    GL/Shader.cpp
    GL/VertexShader.cpp
    GL/GeometryShader.cpp
    GL/FragmentShader.cpp
    GL/Program.cpp
)

set(Engine_GL_HDRS
    GL/Shader.h
    GL/VertexShader.h
    GL/GeometryShader.h
    GL/FragmentShader.h
    GL/Program.h
)

source_group(Common FILES ${Engine_Common_SRCS} ${Engine_Common_HDRS})
source_group(Core FILES ${Engine_Core_SRCS} ${Engine_Core_HDRS})
source_group(GL FILES ${Engine_GL_SRCS} ${Engine_GL_HDRS})

add_library(Engine STATIC
    ${Engine_HDRS}
    ${Engine_Core_SRCS}
    ${Engine_Core_HDRS}
    ${Engine_Common_SRCS}
    ${Engine_Common_HDRS}
    ${Engine_GL_SRCS}
    ${Engine_GL_HDRS}
)

target_link_libraries(Engine 
    ${SDL2_LIBRARY} 
    ${GLEW_LIBRARY} 
    ${OPENGL_LIBRARIES}
)
