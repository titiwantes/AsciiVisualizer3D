#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "cube.h"
#include "../screen/screen.h"

Shape createCube(int length, float offset)
{
    Shape cube;
    int facePoints = 6 * length * length - 12 * length + 8;
    cube.size = facePoints;
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
    cube.offset = offset;
    return cube;
}