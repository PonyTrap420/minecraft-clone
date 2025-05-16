#include "input.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    Game* self = (Game*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        self->wireframe = !self->wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, self->wireframe ? GL_LINE : GL_FILL);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    camera_mouse_input(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    (void)window;
    camera_scroll_input(xoffset, yoffset);
}