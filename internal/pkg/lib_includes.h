//
// Created by Pavlov Konstantin on 27.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_LIB_INCLUDES_H
#define NON_EUCLIDEAN_ENGINE_LIB_INCLUDES_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

static void printVector(glm::vec3 vec) {
    std::cout << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z <<std::endl;
}

#endif //NON_EUCLIDEAN_ENGINE_LIB_INCLUDES_H
