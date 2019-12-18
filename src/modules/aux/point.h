#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

typedef void *Point;

Point Point_Create(double x, double y);

double Point_GetX(Point point);

double Point_GetY(Point point);

void Point_SetX(Point point, double x);

void Point_SetY(Point point, double y);

// Compara dois pontos (para ser usado em ordenação e estruturas)
int Point_Compare(const void *point1, const void *point2);

void Point_Destroy(Point point);

#endif
