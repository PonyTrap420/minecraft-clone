#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdint.h>

void calc_fps(int* frameCount, double* previousTime){
    double currentTime = glfwGetTime();

    if (currentTime - *previousTime >= 1.0) {
        printf("FPS: %d\n", *frameCount);
        *frameCount = 0;
        *previousTime = currentTime;
    }
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    double previousTime = glfwGetTime();
    int frameCount = 0;

    while (!glfwWindowShouldClose(window)) {
        frameCount++;
        calc_fps(&frameCount, &previousTime);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
