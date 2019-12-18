#ifndef POLYGON_H
#define POLYGON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#include "../sig/block.h"
#include "../sig/building.h"
#include "../sig/geometry.h"

typedef void *Polygon;

Polygon Polygon_Create();

void Polygon_InsertPoint(Polygon polygon, double x, double y);

void Polygon_Connect(Polygon polygon);

void Polygon_ReadFromFile(Polygon polygon, FILE *file);

void Polygon_DumpToFile(Polygon polygon, FILE *file);

double Polygon_CalculateArea(Polygon polygon);

void Polygon_Destroy(Polygon polygon);

bool Polygon_IsPointInside(Polygon polygon, double x, double y);

bool Polygon_IsBlockInside(Polygon polygon, Block block, bool partially);

bool Polygon_IsBuildingInside(Polygon polygon, Building building);

double Polygon_GetMinX(Polygon polygon);

double Polygon_GetMaxX(Polygon polygon);

bool Polygon_DoesSegmentIntersect(Polygon polygon, double x1, double y1, double x2, double y2);

void *Polygon_GetFirstSeg(Polygon polygon);

double PolySeg_GetX1(void *seg);

double PolySeg_GetY1(void *seg);

double PolySeg_GetX2(void *seg);

double PolySeg_GetY2(void *seg);

void *PolySeg_GetNext(void *seg);

#endif