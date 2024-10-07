#include "../shapes/shape.h"
typedef struct
{
    Point3D position;
    float intensity;
} Light;

Point3D normalize(Point3D v);
float dotProduct(Point3D a, Point3D b);
char calculateLighting(Point3D normal, Point3D lightDir, float intensity);