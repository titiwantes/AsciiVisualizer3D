#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "cube.h"
#include "../screen/screen.h"

Shape createCube(int length)
{
    Shape cube;
    int facePoints = 6 * length * length - 12 * length + 8;
    cube.size = length;
    cube.points = (Point3D *)malloc(facePoints * sizeof(Point3D));
    if (!cube.points)
    {
        perror("Failed to allocate memory for cube points");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int x = 0; x < length; x++)
    {
        for (int y = 0; y < length; y++)
        {
            for (int z = 0; z < length; z++)
            {
                if (x == 0 || x == length - 1 || y == 0 || y == length - 1 || z == 0 || z == length - 1)
                {
                    cube.points[index++] = (Point3D){x, y, z};
                }
            }
        }
    }

    cube.A = 0;
    cube.B = 0;
    cube.C = 0;
    cube.distanceFromCam = 500;
    return cube;
}

void renderCube(Screen *screen, Shape *shape)
{
    float shapeSizeFactor;
    int x, y, index;

    int facePoints = 6 * shape->size * shape->size - 12 * shape->size + 8;

    int c_size = sqrt(shape->size / 6);

    for (int i = 0; i < facePoints; i++)
    {
        Point3D a = shape->points[i];
        Point3D p = rotate(shape, i);

        shapeSizeFactor = PERSPECTIVE_FACTOR / (p.z + shape->distanceFromCam + 5);

        x = (int)(screen->width / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * p.x);
        y = (int)(screen->height / 2 + PERSPECTIVE_DISTORTION * shapeSizeFactor * p.y);

        index = x + y * screen->width;

        if (x >= 0 && x < screen->width && y >= 0 && y < screen->height)
        {
            if (shapeSizeFactor > screen->depthBuffer[index])
            {
                screen->depthBuffer[index] = shapeSizeFactor;
                char c;
                if (a.x == 0)
                {
                    c = '.';
                }
                if (a.x == c_size - 1)
                {
                    c = '#';
                }
                if (a.y == 0)
                {
                    c = '-';
                }
                if (a.y == c_size - 1)
                {
                    c = 'O';
                }
                if (a.z == 0)
                {
                    c = '|';
                }
                if (a.z == c_size - 1)
                {
                    c = 'X';
                }
                screen->buffer[index] = c;
            }
        }
    }
    shape->A += 0.01;
    shape->B += 0.1;
    shape->C += 0.005;
}