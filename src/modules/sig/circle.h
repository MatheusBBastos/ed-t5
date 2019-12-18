#ifndef CIRCLE_H
#define CIRCLE_H

#include <stdlib.h>
#include "../aux/point.h"

typedef void *Circle;

Circle Circle_Create(double radius, double x, double y);

double Circle_GetRadius(Circle circle);

double Circle_GetX(Circle circle);

double Circle_GetY(Circle circle);

void Circle_SetRadius(Circle circle, double radius);

void Circle_SetX(Circle circle, double x);

void Circle_SetY(Circle circle, double y);

Point Circle_GetPoint(Circle circle);

void Circle_Destroy(Circle circle);

#endif
