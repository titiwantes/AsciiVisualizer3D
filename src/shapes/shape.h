#ifndef SHAPE_H
#define SHAPE_H

typedef struct
{
    float x;
    float y;
    float z;
} Point3D;

typedef struct
{
    Point3D *points;
    int size;
    float A;
    float B;
    float C;
    float distanceFromCam;
} Shape;

Point3D rotate(const Shape *s, Point3D p);
void freeShape(Shape *shape);

#endif
