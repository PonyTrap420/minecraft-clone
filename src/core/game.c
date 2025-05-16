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

int init(Game* self)
{
     if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    self->winHeight = 480;
    self->winWidth = 640;
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
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
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

    BlockMesh* mesh = block_mesh_create();

    size_t vertex_data_size = sizeof(float) * mesh->vertex_count * 3;

    struct VAO vao = vao_init();
    struct VBO vbo = vbo_init(mesh->vertices, vertex_data_size);
    struct IndexBuffer ib = ib_init(mesh->indices, mesh->index_count);

    VertexBufferLayout* layout = vbl_init();
    vbl_push_float(layout, 3);

    vao_addbuffer(vao, vbo, layout);
    vao_bind(&vao);
    ib_bind(&ib);

    Camera* cam = camera_get_instance();
    mat4 projection;
    glm_perspective(glm_rad(cam->fov),(float)self->winWidth / (float)self->winHeight, 0.1f, 100.0f, projection);

    mat4 model;
    glm_mat4_identity(model);

    while (!glfwWindowShouldClose(self->window) && glfwGetKey(self->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS) {
        tick(self);
        
        render(self);

        mat4 view;
        camera_get_view_matrix(cam, view);
        shader_bind(shader);
        shader_set_uniform_mat4f(shader, "view", (const float*)view);
        shader_set_uniform_mat4f(shader, "projection", (const float*)projection);
        shader_set_uniform_mat4f(shader, "model", (const float*)model);


        glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(self->window);
        glfwPollEvents();
    }

    glfwDestroyWindow(self->window);
    glfwTerminate();
    return 0;
}