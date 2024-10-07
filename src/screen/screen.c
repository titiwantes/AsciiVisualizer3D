#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "screen.h"
#include "../shapes/shape.h"

void renderScreen(Screen *screen)
{

    char output[9 + screen->height * (screen->width + 5) + 6];
    char *ptr = output;

    ptr += sprintf(ptr, "\x1b[?25l\x1b[H");

    for (int i = 0; i < screen->height; i++)
    {
        ptr += sprintf(ptr, "%.*s\x1b[0m\n", screen->width, screen->buffer + i * screen->width);
    }

    sprintf(ptr, "\x1b[?25h");

    printf("%s", output);
}

void renderShape(Screen *screen, Shape *shape, Light *light)
{
    float shapeSizeFactor;
    int x, y, index;
    Point3D lightPos = light->position;

    for (int i = 0; i < shape->size; i++)
    {
        Point3D p = rotate(shape, shape->points[i]);
        shapeSizeFactor = PERSPECTIVE_FACTOR / (p.z + shape->distanceFromCam + 5);

        x = (int)(screen->width / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * p.x);
        y = (int)(screen->height / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * p.y);

        index = x + y * screen->width;

        if (x >= 0 && x < screen->width && y >= 0 && y < screen->height)
        {
            if (shapeSizeFactor > screen->depthBuffer[index])
            {
                screen->depthBuffer[index] = shapeSizeFactor;
                Point3D lightDir = {
                    lightPos.x - p.x,
                    lightPos.y - p.y,
                    lightPos.z - p.z};
                lightDir = normalize(lightDir);

                Point3D normal = normalize(p);

                char charToDraw = calculateLighting(normal, lightDir, light->intensity);
                screen->buffer[index] = charToDraw;
            }
        }
    }
    shape->A += 0.05;
    shape->B += 0.03;
    shape->C += 0.02;
}

void cleanup(Screen *screen, Shape *shape)
{
    freeShape(shape);
    if (screen->buffer)
        free(screen->buffer);
    if (screen->depthBuffer)
        free(screen->depthBuffer);
}

void clearBuffer(Screen *screen)
{
    memset(screen->buffer, screen->ch, screen->width * screen->height);
    memset(screen->depthBuffer, 0, screen->width * screen->height * sizeof(float));
}
