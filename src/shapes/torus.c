#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "torus.h"

Shape createTorus(float R, float r, int numSides, int numRings)
{
    Shape torus;
    torus.size = numSides * numRings;
    torus.points = (Point3D *)malloc(torus.size * sizeof(Point3D));
    if (!torus.points)
    {
        perror("Failed to allocate memory for torus points");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < numRings; i++)
    {
        float u = (float)i / numRings * 2.0f * M_PI;
        for (int j = 0; j < numSides; j++)
        {
            float v = (float)j / numSides * 2.0f * M_PI;
            float x = (R + r * cos(v)) * cos(u);
            float y = (R + r * cos(v)) * sin(u);
            float z = r * sin(v);
            torus.points[index++] = (Point3D){x, y, z};
        }
    }

    torus.A = 0;
    torus.B = 0;
    torus.C = 0;
    torus.distanceFromCam = 10;
    return torus;
}