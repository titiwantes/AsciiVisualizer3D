#ifndef SCREEN_H
#define SCREEN_H
#include "../shapes/shape.h"
#include "light.h"

#define FRAME_DELAY 1500
#define PERSPECTIVE_DISTORTION 60
#define PERSPECTIVE_FACTOR 5.0f

typedef struct
{
    int width;
    int height;
    int refreshRate;
    char ch;
    char *buffer;
    float *depthBuffer;
} Screen;

void renderScreen(Screen *screen);
void renderShape(Screen *screen, Shape *shape, Light *light);
void clearBuffer(Screen *screen);
void cleanup(Screen *screen, Shape *shape);

#endif