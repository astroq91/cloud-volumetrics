cmake_minimum_required(VERSION 3.3)
project(Clouds)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
add_subdirectory("vendor/glm")

add_executable(Clouds)
target_sources(Clouds
  PRIVATE
    src/main.cpp
    src/main.cpp
    vendor/glad/src/glad.c
)

target_include_directories(Clouds
  PRIVATE
    vendor/glad/include
)

target_link_libraries(Clouds
  PRIVATE
    glfw
    OpenGL
    glm
)

target_compile_definitions(Clouds 
  PRIVATE
    GLM_ENABLE_EXPERIMENTAL=1
)

