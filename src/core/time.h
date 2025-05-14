typedef struct Time{
    double previousTime;
    unsigned int frameCount;
}Time;

void calc_fps(Time* time);