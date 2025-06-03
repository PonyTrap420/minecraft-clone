#include "game.h"
#include <stdio.h>
#include <stdint.h>
#include <cglm/cglm.h>
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include <noise1234.h>

int init(Game* self)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    self->winHeight = 960;
    self->winWidth = 1280;
    GLFWwindow* window = glfwCreateWindow(self->winWidth, self->winHeight, "Minecraft Clone", NULL, NULL);
    if (!window) {
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    renderer_init();
    texture_init();

    picker_init();
    init_chunk_border_mesh();

    return 0;
}

void tick(Game* self)
{
    self->time.frameCount++;
    calc_fps(&self->time);
}

void update(Game* self)
{

    camera_process_input(self->window);
    self->picked_block = picker_pick_block(self->world, self->camera, 10.0f);
}

void render(Game* self)
{
    renderer_clear();

    mat4 projection;
    glm_perspective(glm_rad(self->camera->fov),(float)self->winWidth / (float)self->winHeight, 0.1f, 1000.0f, projection);

    mat4 view;
    camera_get_view_matrix(self->camera, view);

    shader_bind(self->shader_textured);
    shader_set_uniform_mat4f(self->shader_textured, "view", (const float*)view);
    shader_set_uniform_mat4f(self->shader_textured, "projection", (const float*)projection);

    world_update(self->world, self->camera->cameraPos[0], self->camera->cameraPos[2]);
    world_render(self->world, self->shader_textured);

    if (self->picked_block.hit) {
        picker_render(self->picked_block, self->flags & GAME_FLAG_WIREFRAME, (float*)view, (float*)projection);
    }
    if (self->flags & GAME_FLAG_SHOW_CHUNKS) {
        for (int i = 0; i < self->world->size_x; i++) {
            for (int j = 0; j < self->world->size_z; j++) {
                if ((i + j) % 2 == 0) continue;

                Chunk* chunk = self->world->chunks[i][j];
                if (chunk) {
                    render_chunk_border(chunk->chunk_x, chunk->chunk_z, (float*)view, (float*)projection);
                }
            }
        }
    }
}

int start_game(Game* self)
{
    if(init(self) == -1)
        return -1;

    self->shader_textured = shader_create(SHADER_DIR"basic", shader_separate);
    if (!self->shader_textured) {
        fprintf(stderr, "Failed to create shader\n");
        return -1;
    }
    shader_bind(self->shader_textured);

    self->camera = camera_get_instance();
    glm_vec3_copy((vec3){8.0f, 65.0f, 8.0f}, self->camera->cameraPos);

    char atlas_path[_MAX_PATH];
    sprintf(atlas_path, "%s%s",TEXTURE_DIR,"block_atlas_0.png");
    Texture* atlas_texture = texture_create(atlas_path, TEXTURE_DIFFUSE);
    texture_bind(atlas_texture,0);

    self->world = world_init(5);

    Shader* crosshair_shader = shader_create(SHADER_DIR"crosshair", shader_separate);

    while (!glfwWindowShouldClose(self->window) && glfwGetKey(self->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS) {
        tick(self);
        update(self);
        
        render(self);

        shader_bind(crosshair_shader);
        shader_set_uniform_2f(crosshair_shader, "resolution", self->winWidth, self->winHeight);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(self->window);
        glfwPollEvents();
    }

    glfwDestroyWindow(self->window);
    glfwTerminate();
    return 0;
}