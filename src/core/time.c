#include <stdio.h>
#include <GLFW/glfw3.h>

void calc_fps(int* frameCount, double* previousTime){
    double currentTime = glfwGetTime();

    if (currentTime - *previousTime >= 1.0) {
        printf("FPS: %d\n", *frameCount);
        *frameCount = 0;
        *previousTime = currentTime;
    }
}
