#pragma once

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 5.0f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f

typedef struct {
    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;

    float pitch;
    float yaw;
    float fov;
} Camera;

Camera* camera_get_instance(void);

void camera_process_input(GLFWwindow* window);
void camera_mouse_input(double xpos, double ypos);
void camera_scroll_input(double xoffset, double yoffset);

void camera_get_view_matrix(Camera* camera, mat4 dest);
