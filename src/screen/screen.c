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

Point3D centerShape(Shape *shape)
{
    float minX = shape->points[0].x, maxX = shape->points[0].x;
    float minY = shape->points[0].y, maxY = shape->points[0].y;
    float minZ = shape->points[0].z, maxZ = shape->points[0].z;

    for (int i = 1; i < shape->size; i++)
    {
        if (shape->points[i].x < minX)
            minX = shape->points[i].x;
        if (shape->points[i].x > maxX)
            maxX = shape->points[i].x;
        if (shape->points[i].y < minY)
            minY = shape->points[i].y;
        if (shape->points[i].y > maxY)
            maxY = shape->points[i].y;
        if (shape->points[i].z < minZ)
            minZ = shape->points[i].z;
        if (shape->points[i].z > maxZ)
            maxZ = shape->points[i].z;
    }

    return (Point3D){(maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f};
}

void renderShape(Screen *screen, Shape *shape, Light *light)
{
    float shapeSizeFactor;
    int x, y, index;

    Point3D center = centerShape(shape);

    double light_length = sqrt(light->dir_x * light->dir_x + light->dir_y * light->dir_y + light->dir_z * light->dir_z);
    if (light_length != 0)
    {
        light->dir_x /= light_length;
        light->dir_y /= light_length;
        light->dir_z /= light_length;
    }

    for (int i = 0; i < shape->size; i++)
    {
        Point3D a = shape->points[i];
        Point3D p = rotate(shape, (Point3D){a.x - center.x, a.y - center.y, a.z - center.z});
        shapeSizeFactor = PERSPECTIVE_FACTOR / ((p.z + center.z) + shape->distanceFromCam + 5);

        x = (int)(screen->width / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * (p.x + center.x) + shape->offset);
        y = (int)(screen->height / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * (p.y + center.y));

        index = x + y * screen->width;

        if (x >= 0 && x < screen->width && y >= 0 && y < screen->height)
        {
            if (shapeSizeFactor > screen->depthBuffer[index])
            {
                screen->depthBuffer[index] = shapeSizeFactor;

                double normal_x = p.x;
                double normal_y = p.y;
                double normal_z = p.z;

                double length = sqrt(normal_x * normal_x + normal_y * normal_y + normal_z * normal_z);
                if (length != 0)
                {
                    normal_x /= length;
                    normal_y /= length;
                    normal_z /= length;
                }

                double light_intensity = normal_x * light->dir_x + normal_y * light->dir_y + normal_z * light->dir_z;

                light_intensity = fmax(light_intensity * light->intensity, AMBIENT_LIGHT);

                double view_x = -p.x;
                double view_y = -p.y;
                double view_z = -p.z;
                length = sqrt(view_x * view_x + view_y * view_y + view_z * view_z);
                if (length != 0)
                {
                    view_x /= length;
                    view_y /= length;
                    view_z /= length;
                }

                double reflect_x = 2 * light_intensity * normal_x - light->dir_x;
                double reflect_y = 2 * light_intensity * normal_y - light->dir_y;
                double reflect_z = 2 * light_intensity * normal_z - light->dir_z;

                double specular = pow(fmax(0, view_x * reflect_x + view_y * reflect_y + view_z * reflect_z), SPECULAR_EXPONENT);
                light_intensity = fmin(1, light_intensity + specular);

                if (light_intensity > 0)
                {
                    if (light_intensity < 0.2)
                    {
                        screen->buffer[index] = '.';
                    }
                    else if (light_intensity < 0.5)
                    {
                        screen->buffer[index] = '*';
                    }
                    else if (light_intensity < 0.8)
                    {
                        screen->buffer[index] = '#';
                    }
                    else
                    {
                        screen->buffer[index] = '@';
                    }
                }
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
