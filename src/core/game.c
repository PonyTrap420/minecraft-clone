#include "game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdint.h>
#include <cglm/cglm.h>
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include "../gfx/shader.h"
#include "../gfx/index_buffer.h"

void calc_fps(int* frameCount, double* previousTime);

int start_game()
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Minecraft Clone", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

   if (glewInit() != GLEW_OK)
    {
        return -1;
    }
    printf("%s\n", (const char*)glGetString(GL_VERSION));
    
    double previousTime = glfwGetTime();
    int frameCount = 0;
    
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
    vao_bind(vao);
    ib_bind(&ib);

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS) {
        frameCount++;
        calc_fps(&frameCount, &previousTime);

        glClear(GL_COLOR_BUFFER_BIT);
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}