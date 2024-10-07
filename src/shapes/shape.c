#include <math.h>
#include <stdlib.h>
#include "shape.h"

void freeShape(Shape *shape)
{
    if (shape->points)
    {
        free(shape->points);
    }
}

Point3D rotate(const Shape *restrict s, int index)
{
    float centerX = s->size / 2.0;
    float centerY = s->size / 2.0;
    float centerZ = s->size / 2.0;

    Point3D p = s->points[index];

    p.x -= centerX;
    p.y -= centerY;
    p.z -= centerZ;

    float sinA = sin(s->A), cosA = cos(s->A);
    float sinB = sin(s->B), cosB = cos(s->B);
    float sinC = sin(s->C), cosC = cos(s->C);

    float x = p.y * sinA * sinB * cosC - p.z * cosA * sinB * cosC +
              p.y * cosA * sinC + p.z * sinA * sinC + p.x * cosB * cosC;
    float y = p.y * cosA * cosC + p.z * sinA * cosC -
              p.y * sinA * sinB * sinC + p.z * cosA * sinB * sinC -
              p.x * cosB * sinC;
    float z = p.z * cosA * cosB - p.y * sinA * cosB + p.x * sinB;

    x += centerX;
    y += centerY;
    z += centerZ;

    return (Point3D){x, y, z};
}