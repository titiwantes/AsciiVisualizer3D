#include <math.h>
#include "light.h"

Point3D normalize(Point3D v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= magnitude;
    v.y /= magnitude;
    v.z /= magnitude;
    return v;
}

float dotProduct(Point3D a, Point3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

char calculateLighting(Point3D normal, Point3D lightDir, float intensity)
{
    float dot = dotProduct(normal, lightDir);
    if (dot < 0)
        dot = 0;

    float lightFactor = dot * intensity;

    if (lightFactor > 0.8)
        return '@';
    else if (lightFactor > 0.6)
        return '#';
    else if (lightFactor > 0.4)
        return '*';
    else if (lightFactor > 0.2)
        return '.';
    else
        return '.';
}
