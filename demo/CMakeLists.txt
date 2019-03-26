set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR})

set(DEMO_SOURCES
    main.cc
    panic.cc
    util.cc
    gl_shader_program.cc
    gl_texture.cc
    demo.cc
    effect.cc
    arrows.cc
    text.cc)

add_executable(demo ${DEMO_SOURCES})

target_link_libraries(
    demo
    ${OPENGL_LIBRARIES}
    ${GLFW3_LIBRARY}
    ${GLEW_LIBRARY})
