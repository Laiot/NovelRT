cmake_minimum_required(VERSION 3.13)

project(NovelRT VERSION 0.0.1)
include(GNUInstallDirs)
include_directories(../include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(Freetype REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# lua doesn't provide native cmake support
find_path(LUA_INCLUDE_DIR lua.h)
include_directories(${LUA_INCLUDE_DIR})

find_library(LUA_LIBRARY lua)
link_libraries(${LUA_LIBRARY})

#Default shaders
#configure_file(Resources/DefaultShaders/BasicFragmentShader.glsl BasicFragmentShader.glsl COPYONLY)
#configure_file(Resources/DefaultShaders/BasicVertexShader.glsl BasicVertexShader.glsl COPYONLY)
#configure_file(Resources/DefaultShaders/FontFragmentShader.glsl FontFragmentShader.glsl COPYONLY)
#configure_file(Resources/DefaultShaders/FontVertexShader.glsl FontVertexShader.glsl COPYONLY)
#configure_file(Resources/DefaultShaders/TexturedFragmentShader.glsl TexturedFragmentShader.glsl COPYONLY)
#configure_file(Resources/DefaultShaders/TexturedVertexShader.glsl TexturedVertexShader.glsl COPYONLY)

#Default font files
#configure_file(Resources/FontFiles/Gayathri-Regular.ttf Gayathri-Regular.ttf COPYONLY)

set(LIBNOVELRT_RESOURCES
    Resources/DefaultShaders/BasicFragmentShader.glsl;
    Resources/DefaultShaders/BasicVertexShader.glsl;
    Resources/DefaultShaders/FontFragmentShader.glsl;
    Resources/DefaultShaders/FontVertexShader.glsl;
    Resources/DefaultShaders/TexturedFragmentShader.glsl;
    Resources/DefaultShaders/TexturedVertexShader.glsl;
    Resources/FontFiles/Gayathri-Regular.ttf;)

set(NOVELRT_LIBRARY_HEADERS
  ../include/NovelRT.h
  ../include/NovelRT/DebugService.h
  ../include/NovelRT/LoggingService.h
  ../include/NovelRT/NovelRunner.h
  ../include/NovelRT/Transform.h
  ../include/NovelRT/WorldObject.h
  ../include/NovelRT/Audio/AudioService.h
  ../include/NovelRT/Graphics/BasicFillRect.h
  ../include/NovelRT/Graphics/Camera.h
  ../include/NovelRT/Graphics/CameraFrameState.h
  ../include/NovelRT/Graphics/GraphicsCharacterRenderData.h
  ../include/NovelRT/Graphics/GraphicsCharacterRenderDataHelper.h
  ../include/NovelRT/Graphics/ImageRect.h
  ../include/NovelRT/Graphics/RenderingService.h
  ../include/NovelRT/Graphics/RenderObject.h
  ../include/NovelRT/Graphics/RGBAConfig.h
  ../include/NovelRT/Graphics/ShaderProgram.h
  ../include/NovelRT/Graphics/TextRect.h
  ../include/NovelRT/Input/BasicInteractionRect.h
  ../include/NovelRT/Input/InteractionObject.h
  ../include/NovelRT/Input/InteractionService.h
  ../include/NovelRT/Input/KeyCode.h
  ../include/NovelRT/Input/KeyState.h
  ../include/NovelRT/Maths/GeoBounds.h
  ../include/NovelRT/Maths/GeoMatrix4.h
  ../include/NovelRT/Maths/GeoVector.h
  ../include/NovelRT/Timing/StepTimer.h
  ../include/NovelRT/Utilities/Lazy.h
  ../include/NovelRT/Utilities/Misc.h
  ../include/NovelRT/Windowing/WindowingService.h)

  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES ${NOVELRT_LIBRARY_HEADERS})

set(NOVELRT_LIBRARY_SOURCES
  NovelRT/DebugService.cpp
  NovelRT/LoggingService.cpp
  NovelRT/NovelRunner.cpp
  NovelRT/Transform.cpp
  NovelRT/WorldObject.cpp
  NovelRT/Audio/AudioService.cpp
  NovelRT/Graphics/BasicFillRect.cpp
  NovelRT/Graphics/Camera.cpp
  NovelRT/Graphics/ImageRect.cpp
  NovelRT/Graphics/RenderingService.cpp
  NovelRT/Graphics/RenderObject.cpp
  NovelRT/Graphics/RGBAConfig.cpp
  NovelRT/Graphics/TextRect.cpp
  NovelRT/Input/BasicInteractionRect.cpp
  NovelRT/Input/InteractionObject.cpp
  NovelRT/Input/InteractionService.cpp
  NovelRT/Maths/GeoBounds.cpp
  NovelRT/Timing/StepTimer.cpp
  NovelRT/Windowing/WindowingService.cpp)

  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES ${NOVELRT_LIBRARY_SOURCES})

set(NOVELRT_LINK_LIBRARIES
  Freetype::Freetype
  glad::glad
  glm
  SDL2::SDL2-static
  SDL2::SDL2_image
  SDL2::SDL2_mixer
  SDL2::SDL2main
  spdlog::spdlog
)

if(NOT WIN32)
    set(NOVELRT_LINK_LIBRARIES
      ${NOVELRT_LINK_LIBRARIES}
      m
    )
endif()

if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(NOVELRT_LIBRARY_LINK_LIBRARIES
  ${NOVELRT_LINK_LIBRARIES}
)

add_library(libNovelRT SHARED ${NOVELRT_LIBRARY_HEADERS} ${NOVELRT_LIBRARY_SOURCES})
target_link_libraries(libNovelRT ${NOVELRT_LIBRARY_LINK_LIBRARIES})
