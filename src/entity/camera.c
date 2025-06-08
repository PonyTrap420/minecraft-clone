#include "camera.h"
#include <stdlib.h>

static float last_x = 400, last_y = 300;
static int first_mouse = 1;

void camera_process_input(Camera* camera, GLFWwindow* window) {
    vec3 right;
    glm_vec3_cross(camera->cameraFront, camera->cameraUp, right);
    glm_normalize(right);
    
    float camera_speed = SPEED * 0.016f;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_speed *= 2;

    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera_speed /= 4;

    vec3 temp;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(camera->cameraFront, camera_speed, temp);
        glm_vec3_add(camera->cameraPos, temp, camera->cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(camera->cameraFront, camera_speed, temp);
        glm_vec3_sub(camera->cameraPos, temp, camera->cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_scale(right, camera_speed, temp);
        glm_vec3_sub(camera->cameraPos, temp, camera->cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_vec3_scale(right, camera_speed, temp);
        glm_vec3_add(camera->cameraPos, temp, camera->cameraPos);
    }
}

void camera_mouse_input(Camera* camera, double xpos, double ypos) {
    if (first_mouse) {
        last_x = (float)xpos;
        last_y = (float)ypos;
        first_mouse = 0;
    }

    float xoffset = (float)xpos - last_x;
    float yoffset = last_y - (float)ypos;
    last_x = (float)xpos;
    last_y = (float)ypos;

    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    vec3 direction;
    direction[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    direction[1] = sinf(glm_rad(camera->pitch));
    direction[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    glm_normalize_to(direction, camera->cameraFront);
}

void camera_scroll_input(Camera* camera, double xoffset, double yoffset) {
    (void)xoffset;

    camera->fov -= (float)yoffset;
    if (camera->fov < 1.0f)
        camera->fov = 1.0f;
    if (camera->fov > 45.0f)
        camera->fov = 45.0f;
}

void camera_get_view_matrix(Camera* camera, mat4 dest) {
    vec3 center;
    glm_vec3_add(camera->cameraPos, camera->cameraFront, center);
    glm_lookat(camera->cameraPos, center, camera->cameraUp, dest);
}