cmake_minimum_required(VERSION 3.20)
project(non_euclidean_engine)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3  REQUIRED)
find_package(GLEW  REQUIRED)
find_package(glm REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

add_library(STB_IMAGE "pkg/stbi/stb_image.cpp")

add_executable(non_euclidean_engine main.cpp glad/glad.c internal/pkg/camera/camera.h pkg/stbi/stb_image.h internal/pkg/shader/shader.h internal/pkg/filesystem/filesystem.h internal/pkg/entity/entity.h internal/pkg/entity/sphere.h internal/pkg/entity/sphere.cpp internal/pkg/buffer/vertexBuffer.cpp internal/pkg/buffer/vertexBuffer.h internal/pkg/buffer/indexBuffer.cpp internal/pkg/buffer/indexBuffer.h internal/pkg/buffer/vertexAttribBuffer.cpp internal/pkg/buffer/vertexAttribBuffer.h internal/pkg/buffer/attribLayout.h internal/pkg/entity/sphere.cpp internal/pkg/lib_includes.h internal/pkg/entity/cube.h internal/pkg/entity/cube.cpp internal/pkg/core/geometry.h internal/pkg/core/consts.h internal/pkg/entity/tree.h internal/pkg/entity/prism.h)
target_link_libraries(non_euclidean_engine glfw GLEW::GLEW glm::glm STB_IMAGE)
