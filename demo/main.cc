#include "panic.h"

#include "demo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

int main(int argc, char *argv[])
{
    bool fullscreen = false;

    int opt;
    while ((opt = getopt(argc, argv, "f")) != -1) {
        switch (opt) {
            case 'f':
                fullscreen = true;
                break;

            default:
                break;
        }
    }

    if (!glfwInit())
        panic("glfwInit failed");

    glfwSetErrorCallback([](int error, const char *description) {
        panic("GLFW error: %s", description);
    });

    int window_width, window_height;
    GLFWmonitor *monitor;

    if (!fullscreen) {
        monitor = nullptr;
        window_width = 910;
        window_height = 512;
    } else {
        monitor = glfwGetPrimaryMonitor();
        const auto mode = glfwGetVideoMode(monitor);
        window_width = mode->width;
        window_height = mode->height;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "1997", monitor, nullptr);
    if (!window)
        panic("glfwCreateWindow failed");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    {
        demo d(fb_width, fb_height);

        while (!glfwWindowShouldClose(window)) {
            d.redraw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
