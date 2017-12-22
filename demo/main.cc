#include "panic.h"

#include "demo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace {

constexpr int window_width = 512;
constexpr int window_height = 256;

}

int
main(int argc, char *argv[])
{
    if (!glfwInit())
        panic("glfwInit failed");

    glfwSetErrorCallback([](int error, const char *description) {
        panic("GLFW error: %s", description);
    });

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "test", nullptr, nullptr);
    if (!window)
        panic("glfwCreateWindow failed");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });

    {
        demo d(window_width, window_height);

        while (!glfwWindowShouldClose(window)) {
            d.redraw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
