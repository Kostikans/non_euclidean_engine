//
// Created by Pavlov Konstantin on 22.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_APP_H
#define NON_EUCLIDEAN_ENGINE_APP_H

#include "../lib_includes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../camera/camera.h"
#include "../shader/shader.h"
#include "../filesystem/filesystem.h"
#include "../entity/sphere.h"
#include "../entity/tree.h"
#include "../input/input.h"
#include "../entity/horosphere.h"
#include "../render/renderContext.h"
#include "../scene/scene.h"

#include <iostream>


class App {
public:
    App() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(800, 600, "NonEuclid Engine", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        srand(time(0));
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window,[](GLFWwindow *window, int width, int height) {
            static_cast<App *>(glfwGetWindowUserPointer(window))->framebuffer_size_callback(window, width,height);
        });

        glfwSetCursorPosCallback(window,[](GLFWwindow *window, double xposIn, double yposIn) {
            static_cast<App *>(glfwGetWindowUserPointer(window))->mouse_callback(window, xposIn,yposIn);
        });

        glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
            static_cast<App *>(glfwGetWindowUserPointer(window))->scroll_callback(window, xoffset, yoffset);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
            static_cast<App *>(glfwGetWindowUserPointer(window))->mouse_button_callback(window, button, action, mods);
        });

        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);

        Shader *shader = new Shader(FileSystem::getPath("internal/pkg/shader/noneuclid.vs").c_str(),
                      FileSystem::getPath("internal/pkg/shader/shader.fs").c_str());
        shader->use();

        context = new RenderContext(shader);
        input = new Input();
        camera = new Camera();
        scene = new Scene();
        scene->setCamera(camera);
    }

    void loop() {
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            if (deltaTime > 0.05) {
                deltaTime = 0.05;
            }

            lastFrame = currentFrame;

            processInput(window);
            double xpos, ypos;
            if (isMouseCaptured(window)) {
                glfwGetCursorPos(window, &xpos, &ypos);
                glfwSetCursorPos(window, 0, 0);
            }

            camera->ProcessStep(*input, xpos, ypos, deltaTime);

            scene->render(context);
            input->ClearInput();

            glfwSwapInterval(1);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            input->SetKey(LEFT_MOVE, true);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            input->SetKey(RIGHT_MOVE, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            input->SetKey(UP_MOVE, true);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            input->SetKey(DOWN_MOVE, true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
            input->SetKey(FORWARD_MOVE, true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            input->SetKey(BACKWARD_MOVE, true);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            input->SetKey(COUNTER_CLOCKWIZE_MOVE, true);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            input->SetKey(CLOCKWIZE_MOVE, true);

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            Tree *tree = new Tree();
            tree->init(glm::identity<glm::mat4x4>(), 4);
            tree->setModelMatrix(camera->GetPos());
            scene->addEntity(tree);
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            Horosphere *horosphere = new Horosphere();
            horosphere->init(FileSystem::getPath("pkg/resources/textures/container.jpeg").c_str());
            horosphere->setModelMatrix(camera->GetPos());
            scene->addEntity(horosphere);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            globalScale *= 1.03f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            globalScale /= 1.03f;

    }

    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        lastX = xpos;
        lastY = ypos;
    }

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        if (!isMouseCaptured(window)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0, 0);
        }
    }

    bool isMouseCaptured(GLFWwindow *window) {
        return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    }

    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }

private:
    GLFWwindow *window;
    RenderContext *context;
    Camera *camera;
    Scene *scene;
    Input *input;

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};


#endif //NON_EUCLIDEAN_ENGINE_APP_H
