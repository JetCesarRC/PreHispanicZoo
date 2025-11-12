#pragma once

#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);

    int Initialise();

    GLfloat getBufferWidth() const { return static_cast<GLfloat>(bufferWidth); }
    GLfloat getBufferHeight() const { return static_cast<GLfloat>(bufferHeight); }

    bool getShouldClose() const { return glfwWindowShouldClose(mainWindow) != 0; }

    bool* getsKeys() { return keys; }

    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow* mainWindow;

    GLint width;
    GLint height;

    GLint bufferWidth;
    GLint bufferHeight;

    bool keys[1024];

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    void createCallbacks();

    static void ManejaTeclado(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};
