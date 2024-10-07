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

Point3D rotate(const Shape *restrict s, Point3D p)
{
    float sinA = sin(s->A), cosA = cos(s->A);
    float sinB = sin(s->B), cosB = cos(s->B);
    float sinC = sin(s->C), cosC = cos(s->C);
    float x = p.y * sinA * sinB * cosC - p.z * cosA * sinB * cosC +
              p.y * cosA * sinC + p.z * sinA * sinC + p.x * cosB * cosC;
    float y = p.y * cosA * cosC + p.z * sinA * cosC -
              p.y * sinA * sinB * sinC + p.z * cosA * sinB * sinC -
              p.x * cosB * sinC;
    float z = p.z * cosA * cosB - p.y * sinA * cosB + p.x * sinB;
    return (Point3D){x, y, z};
}
