#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "screen/screen.h"
#include "shapes/shape.h"
#include "shapes/cube.h"
#include "shapes/torus.h"

int main()
{
    Screen screen = {150, 80, 0, ' ', NULL, NULL};
    Light light = {100.0, 10.0, -100.0, 0.5, -0.5, -10.0, 1.0};
    Shape cube = createCube(50, 30);
    Shape torus = createTorus(1.1f, 0.5f, 100, 100);
    int screenSize = screen.width * screen.height;

    screen.buffer = (char *)malloc(screenSize * sizeof(char));
    screen.depthBuffer = (float *)malloc(screenSize * sizeof(float));

    if (!screen.buffer || !screen.depthBuffer)
    {
        perror("Failed to allocate memory for screen buffers");
        cleanup(&screen, &torus);
        cleanup(&screen, &cube);
        exit(EXIT_FAILURE);
    }

    int is_rendering = 1;

    while (is_rendering)
    {
        clearBuffer(&screen);
        renderShape(&screen, &torus, &light);
        renderShape(&screen, &cube, &light);
        renderScreen(&screen);
        usleep(FRAME_DELAY);
    }

    cleanup(&screen, &torus);
    cleanup(&screen, &cube);
    return 0;
}
