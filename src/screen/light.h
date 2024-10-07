#include "../shapes/shape.h"

#define AMBIENT_LIGHT 0.2
#define SPECULAR_EXPONENT 20.0

typedef struct
{
    double x, y, z;
    double dir_x, dir_y, dir_z;
    double intensity;
} Light;

Point3D normalize(Point3D v);
float dotProduct(Point3D a, Point3D b);
char calculateLighting(Point3D normal, Point3D lightDir, float intensity);