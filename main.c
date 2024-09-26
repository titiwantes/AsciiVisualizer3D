#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#define PERSPECTIVE_DISTORTION 10
#define PERSPECTIVE_FACTOR 5.0f
#define FRAME_DELAY 33333

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

typedef struct
{
    int width;
    int height;
    int refreshRate;
    char ch;
    char *buffer;
    float *depthBuffer;
} Screen;

Shape createCube(int length)
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
    cube.distanceFromCam = 20;
    return cube;
}

void freeShape(Shape *shape)
{
    if (shape->points)
    {
        free(shape->points);
    }
}

Point3D Rotate(const Shape *restrict s, int index)
{
    Point3D p = s->points[index];

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

Point3D applyPerspective(Point3D p, float distanceFromCam)
{
    float factor = PERSPECTIVE_FACTOR / (p.z + distanceFromCam + 5);
    return (Point3D){p.x * factor, p.y * factor, p.z};
}

void renderScreen(Screen *screen)
{
    printf("\x1b[?25l"); // Cacher le curseur
    printf("\x1b[H");    // Positionner le curseur au coin supérieur gauche

    for (int i = 0; i < screen->height; i++)
    {
        printf("%.*s\x1b[0m\n", screen->width, screen->buffer + i * screen->width);
    }

    printf("\x1b[?25h"); // Montrer le curseur
}

void renderShape(Screen *screen, Shape *shape)
{
    int x, y, index;

    for (int i = 0; i < shape->size; i++)
    {
        Point3D p = Rotate(shape, i);
        Point3D perspectivePoint = applyPerspective(p, shape->distanceFromCam);

        x = (int)(screen->width / 2 + PERSPECTIVE_DISTORTION * perspectivePoint.x);
        y = (int)(screen->height / 2 + PERSPECTIVE_DISTORTION * perspectivePoint.y);
        index = x + y * screen->width;

        if (x >= 0 && x < screen->width && y >= 0 && y < screen->height)
        {
            if (perspectivePoint.z > screen->depthBuffer[index])
            {
                screen->depthBuffer[index] = perspectivePoint.z;
                screen->buffer[index] = '#';
            }
        }
    }

    shape->A += 0.05;
    shape->B += 0.03;
    shape->C += 0.02;
}

void clearBuffer(Screen *screen)
{
    memset(screen->buffer, screen->ch, screen->width * screen->height);
    memset(screen->depthBuffer, 0, screen->width * screen->height * sizeof(float));
}

void cleanup(Screen *screen, Shape *shape)
{
    freeShape(shape);
    if (screen->buffer)
        free(screen->buffer);
    if (screen->depthBuffer)
        free(screen->depthBuffer);
}

int main()
{
    Screen screen = {150, 50, 0, ' ', NULL, NULL};
    Shape cube = createCube(10);
    int screenSize = screen.width * screen.height;

    screen.buffer = (char *)malloc(screenSize * sizeof(char));
    screen.depthBuffer = (float *)malloc(screenSize * sizeof(float));

    if (!screen.buffer || !screen.depthBuffer)
    {
        perror("Failed to allocate memory for screen buffers");
        cleanup(&screen, &cube);
        exit(EXIT_FAILURE);
    }

    int is_rendering = 1;

    while (is_rendering)
    {
        clearBuffer(&screen);
        renderShape(&screen, &cube);
        renderScreen(&screen);
        usleep(FRAME_DELAY);
    }

    cleanup(&screen, &cube);

    return 0;
}
