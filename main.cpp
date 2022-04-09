#include "internal/pkg/lib_includes.h"
#include "pkg/stbi/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "internal/pkg/camera/camera.h"
#include "internal/pkg/shader/shader.h"
#include "internal/pkg/filesystem/filesystem.h"
#include "internal/pkg/entity/sphere.h"
#include "internal/pkg/entity/prism.h"
#include "internal/pkg/entity/tree.h"
#include "internal/pkg/entity/cube.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window,int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
bool isMouseCaptured(GLFWwindow* window);
glm::vec3 getRandomColor();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<Entity*> models;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    srand( time(0) );
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);


    Shader ourShader(FileSystem::getPath("internal/pkg/shader/noneuclid.vs").c_str(), FileSystem::getPath("internal/pkg/shader/shader.fs").c_str());


    std::vector<glm::vec3> cubePositions;
    std::vector<Cube*> cubes;

    const int cubeCount = 20;

    float z = -2.0f;
    float radius = 20.0f;
    float x = 0.0f;
    float stackAngle;
    float stackStep = M_PI * 2 / cubeCount;
    for(int i = 0; i < cubeCount; i++){
        stackAngle = M_PI - i * stackStep;
        x = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);
        float const y = 0.0f;
        Cube *cube = new Cube();
        glm::vec3 color = getRandomColor();
        float sizeX = 1.0f;
        float sizeY = 1.0f;
        float sizeZ = 1.0f;
        cube->init(sizeX,sizeY,sizeZ,color);
        cube->translate(glm::vec3(x,y,z));
        cubes.emplace_back(cube);
    }

    ourShader.use();

//    Tree *tree = new Tree();
//    tree->init(glm::identity<glm::mat4x4>(),4);
//    models.emplace_back(tree);
    float anti = 1.0f;
    float near = 0.1f;
    float far = 100.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        if (deltaTime > 0.05) {
            deltaTime = 0.05;
        }

        lastFrame = currentFrame;

        processInput(window);
        double xpos,ypos;
        if (isMouseCaptured(window)) {
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwSetCursorPos(window, 0, 0);
        }

        camera.ProcessMouse(xpos,ypos,deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        float ratio = (double)SCR_WIDTH / (double)SCR_HEIGHT;
        double zoom = camera.getCameraZoom();
        Matrix4d projection = VectorMath::perspective(ratio * zoom, zoom, 0.01, 10);
        ourShader.setMat4Eugen("P", projection.cast<float>());

        Matrix4d view = camera.GetViewMatrix();

        ourShader.setMat4Eugen("V", view.cast<float>());

        for (Entity* model : models) {
			model->draw(ourShader);
		}
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.leftPress = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.rightPress = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.upPress = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.downPress = true;
    if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        camera.forwardPress = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        camera.backwardPress = true;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.counterClockWise = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.clockWise = true;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        LorentzSign = 0.0f;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        LorentzSign = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        LorentzSign = -1.0f;


    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        Tree *tree = new Tree();
        tree->init(glm::identity<glm::mat4x4>(),4);
        tree->setModelMatrix(camera.GetPos());
        models.emplace_back(tree);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        globalScale *= 1.03f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        globalScale /= 1.03f;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::vec3 getRandomColor()
{
    int COLORS = 256;
    return glm::vec3(float(rand()%COLORS)/COLORS,float(rand()%COLORS)/COLORS,float(rand()%COLORS)/COLORS);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    lastX = xpos;
    lastY = ypos;
}

void mouse_button_callback(GLFWwindow* window,int button, int action, int mods){
    if (!isMouseCaptured(window)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, 0, 0);
    }
}

bool isMouseCaptured(GLFWwindow* window) {
    return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}