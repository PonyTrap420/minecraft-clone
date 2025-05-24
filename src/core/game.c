#include "game.h"
#include <stdio.h>
#include <stdint.h>
#include <cglm/cglm.h>
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include "../gfx/shader.h"
#include "../gfx/index_buffer.h"
#include "../input/input.h"
#include "../world/blockmesh.h"
#include "../gfx/renderer.h"
#include "../world/world.h"
#include "../gfx/texture.h"
#include "../gfx/atlas.h"

int init(Game* self)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    self->winHeight = 960;
    self->winWidth = 1280;
    GLFWwindow* window = glfwCreateWindow(self->winWidth, self->winHeight, "Minecraft Clone", NULL, NULL);
    if (!self->window ) {
        glfwTerminate();
        return -1;
    }
    
    self->window = window;

    glfwMakeContextCurrent(self->window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    printf("%s\n", (const char*)glGetString(GL_VERSION));

    glfwSetWindowUserPointer(window, self);

    self->time = init_time();
    self->wireframe = false;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(self->window, key_callback);

    renderer_init();
    texture_init();

    return 0;
}

void tick(Game* self)
{
    self->time.frameCount++;
    calc_fps(&self->time);

    camera_process_input(self->window);
}

void render()
{
   renderer_clear();
}

int start_game(Game* self)
{
    if(init(self) == -1)
        return -1;

    Shader* shader = shader_create(SHADER_DIR"basic", shader_separate);
    if (!shader) {
        fprintf(stderr, "Failed to create shader\n");
        return -1;
    }
    shader_bind(shader);

    Camera* cam = camera_get_instance();
    glm_vec3_copy((vec3){8.0f, 65.0f, 8.0f}, cam->cameraPos);

    char atlas_path[_MAX_PATH];
    sprintf(atlas_path, "%s%s",TEXTURE_DIR,"block_atlas_0.png");
    Texture* atlas_texture = texture_create(atlas_path, TEXTURE_DIFFUSE);
    texture_bind(atlas_texture,0);

    World* world = world_init(5);
    
    mat4 projection;
    glm_perspective(glm_rad(cam->fov),(float)self->winWidth / (float)self->winHeight, 0.1f, 100.0f, projection);


    while (!glfwWindowShouldClose(self->window) && glfwGetKey(self->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS) {
        tick(self);
        
        render(self);

        mat4 view;
        camera_get_view_matrix(cam, view);

        shader_set_uniform_mat4f(shader, "view", (const float*)view);
        shader_set_uniform_mat4f(shader, "projection", (const float*)projection);

        world_update(world, cam->cameraPos[0], cam->cameraPos[2]);
        world_render(world, shader);
        
        glfwSwapBuffers(self->window);
        glfwPollEvents();
    }

    glfwDestroyWindow(self->window);
    glfwTerminate();
    return 0;
}