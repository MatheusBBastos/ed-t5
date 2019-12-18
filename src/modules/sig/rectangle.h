#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdlib.h>
#include "../aux/point.h"

typedef void *Rectangle;

Rectangle Rectangle_Create(double width, double height, double x, double y);

double Rectangle_GetWidth(Rectangle rectangle);

double Rectangle_GetHeight(Rectangle rectangle);

double Rectangle_GetX(Rectangle rectangle);

double Rectangle_GetY(Rectangle rectangle);

void Rectangle_SetWidth(Rectangle rectangle, double width);

void Rectangle_SetHeight(Rectangle rectangle, double height);

void Rectangle_SetX(Rectangle rectangle, double x);

void Rectangle_SetY(Rectangle rectangle, double y);

Point Rectangle_GetPoint(Rectangle rectangle);

void Rectangle_Destroy(Rectangle rectangle);

#endif
