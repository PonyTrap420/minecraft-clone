#include <stdio.h>
#include <GLFW/glfw3.h>
#include "time.h"

Time init_time(){
    Time time = {
        glfwGetTime(),
        0,
        0
    };
    return time;
}

void calc_fps(Time* time){
    double currentTime = glfwGetTime();

    if (currentTime - time->previousTimeFPS>= 1.0) {
        printf("FPS: %d\n", time->frameCount);
        time->frameCount = 0;
        time->previousTimeFPS = currentTime;
    }
}
