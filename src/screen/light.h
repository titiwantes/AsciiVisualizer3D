#include "../shapes/shape.h"

#define AMBIENT_LIGHT 0.2
#define SPECULAR_EXPONENT 20.0

typedef struct
{
    // plus de précision pour les calculs
    double x, y, z;
    double dir_x, dir_y, dir_z;
    double intensity;
} Light;
