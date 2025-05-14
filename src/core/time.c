#include <stdio.h>
#include <GLFW/glfw3.h>
#include "time.h"

void calc_fps(Time* time){
    double currentTime = glfwGetTime();

    if (currentTime - time->previousTime>= 1.0) {
        printf("FPS: %d\n", time->frameCount);
        time->frameCount = 0;
        time->previousTime = currentTime;
    }
}
