typedef struct Time{
    double previousTime;
    unsigned int frameCount;
}Time;


Time init_time();
void calc_fps(Time* time);