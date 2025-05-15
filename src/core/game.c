#include "game.h"
#include <stdio.h>
#include <stdint.h>
#include <cglm/cglm.h>
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include "../gfx/shader.h"
#include "../gfx/index_buffer.h"
#include "../input/input.h"

int init(Game* self)
{
     if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Minecraft Clone", NULL, NULL);
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

    glfwSetWindowUserPointer(self->window, self);

    self->time = init_time();
    self->wireframe = false;

    glfwSetKeyCallback(self->window, key_callback);

    return 0;
}

void tick(Game* self)
{
    self->time.frameCount++;
    calc_fps(&self->time);
}

void render()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
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

    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = { 
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    struct VAO vao = vao_init();

    struct VBO vbo = vbo_init(vertices, sizeof(vertices));

    struct IndexBuffer ib = ib_init(indices, 6);

    VertexBufferLayout* layout = vbl_init();
    vbl_push_float(layout, 3);

    vao_addbuffer(vao, vbo, layout);
    vao_bind(&vao);
    ib_bind(&ib);

 
    while (!glfwWindowShouldClose(self->window) && glfwGetKey(self->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS) {
        tick(self);
        
        render(self);

        glfwSwapBuffers(self->window);
        glfwPollEvents();
    }

    glfwDestroyWindow(self->window);
    glfwTerminate();
    return 0;
}