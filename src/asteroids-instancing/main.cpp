#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "camera.h"
#include "stb_image.h"
#include "objects.hpp"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// CAMERA
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Mouse input configuration
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OPENGL STATE
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader planetShader("planet.vs", "planet.fs");
    Shader asteroidShader("asteroid.vs", "planet.fs");

    // ROCK MODEL
    std::string path = "/home/loe/raytracer/resources/rock/rock.obj";
    Model rock(path.c_str());

    // PLANET MODEL
    path = "/home/loe/raytracer/resources/planet/planet.obj";
    Model planet(path.c_str());

    planetShader.use();
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 2.2f, 2.0f),
        glm::vec3(22.3f, 3.3f, -4.0f),
        glm::vec3(-14.0f, 6.0f, -12.0f),
        glm::vec3(10.0f, 50.0f, -3.0f)};

    unsigned int amount = 600000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 50.0;
    float offset = 2.5f;

    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        modelMatrices[i] = model;
    }

    // vertex buffer object
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4),
                 &modelMatrices[0], GL_STATIC_DRAW);
    for (unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // vertex attributes
        std::size_t v4s = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(1 * v4s));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(2 * v4s));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(3 * v4s));
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // render the triangle
        planetShader.use();

        // view matrix
        glm::mat4 view = camera.GetViewMatrix();
        planetShader.setMat4("view", view);
        planetShader.setVec3("viewPos", camera.Position);
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        planetShader.setMat4("projection", projection);

        // LIGHTS
        // directional light
        planetShader.setVec3("dirLight.direction", -0.2f, 10.0f, -0.3f);
        planetShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        planetShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        planetShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        planetShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        planetShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[0].constant", 1.0f);
        planetShader.setFloat("pointLights[0].linear", 0.09f);
        planetShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        planetShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        planetShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        planetShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[1].constant", 1.0f);
        planetShader.setFloat("pointLights[1].linear", 0.09f);
        planetShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        planetShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        planetShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        planetShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[2].constant", 1.0f);
        planetShader.setFloat("pointLights[2].linear", 0.09f);
        planetShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        planetShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        planetShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        planetShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[3].constant", 1.0f);
        planetShader.setFloat("pointLights[3].linear", 0.09f);
        planetShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        planetShader.setVec3("spotLight.position", camera.Position);
        planetShader.setVec3("spotLight.direction", camera.Front);
        planetShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        planetShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        planetShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("spotLight.constant", 1.0f);
        planetShader.setFloat("spotLight.linear", 0.09f);
        planetShader.setFloat("spotLight.quadratic", 0.032f);
        planetShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        planetShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // render the Planet model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        planetShader.setMat4("model", model);
        planet.Draw(planetShader);

        // render Rocks
        asteroidShader.use();
        // LIGHTS
        // directional light
        asteroidShader.setVec3("dirLight.direction", -0.2f, 10.0f, -0.3f);
        asteroidShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        asteroidShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        asteroidShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        asteroidShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        asteroidShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        asteroidShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        asteroidShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        asteroidShader.setFloat("pointLights[0].constant", 1.0f);
        asteroidShader.setFloat("pointLights[0].linear", 0.09f);
        asteroidShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        asteroidShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        asteroidShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        asteroidShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        asteroidShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        asteroidShader.setFloat("pointLights[1].constant", 1.0f);
        asteroidShader.setFloat("pointLights[1].linear", 0.09f);
        asteroidShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        asteroidShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        asteroidShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        asteroidShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        asteroidShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        asteroidShader.setFloat("pointLights[2].constant", 1.0f);
        asteroidShader.setFloat("pointLights[2].linear", 0.09f);
        asteroidShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        asteroidShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        asteroidShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        asteroidShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        asteroidShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        asteroidShader.setFloat("pointLights[3].constant", 1.0f);
        asteroidShader.setFloat("pointLights[3].linear", 0.09f);
        asteroidShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        asteroidShader.setVec3("spotLight.position", camera.Position);
        asteroidShader.setVec3("spotLight.direction", camera.Front);
        asteroidShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        asteroidShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        asteroidShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        asteroidShader.setFloat("spotLight.constant", 1.0f);
        asteroidShader.setFloat("spotLight.linear", 0.09f);
        asteroidShader.setFloat("spotLight.quadratic", 0.032f);
        asteroidShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        asteroidShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        asteroidShader.setMat4("view", view);
        asteroidShader.setVec3("viewPos", camera.Position);
        asteroidShader.setMat4("projection", projection);
        for (unsigned int i = 0; i < rock.meshes.size(); i++)
        {
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] modelMatrices;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffest, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}