#include "input.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    Game* game = (Game*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        game->flags ^= GAME_FLAG_WIREFRAME;
        glPolygonMode(GL_FRONT_AND_BACK, (game->flags & GAME_FLAG_WIREFRAME) ? GL_LINE : GL_FILL);
    }
    else if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        game->flags ^= GAME_FLAG_SHOW_CHUNKS;
    }
    
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    (void)mods;

    Game* game = (Game*)glfwGetWindowUserPointer(window);

    BlockHit hit = game->picked_block;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        world_destroy_block(game->world, hit.x,hit.y,hit.z);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        world_place_block(game->world, hit.x, hit.y, hit.z, hit.face, BLOCK_STONE);
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