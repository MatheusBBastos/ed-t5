#ifndef VERTEX_H
#define VERTEX_H

#include <stdlib.h>
#include <stdbool.h>
#include "segment.h"
#include "../sig/geometry.h"

typedef void *Segment;

typedef void *Vertex;

Vertex Vertex_Create(double x, double y, Segment segment, double angle, double distance);

// Compara dois pontos para determinar qual é anterior. Para ser usado na função qsort
int Vertex_Compare(const void *p1, const void *p2);

double Vertex_GetX(Vertex vertexVoid);

double Vertex_GetY(Vertex vertexVoid);

double Vertex_GetAngle(Vertex vertexVoid);

double Vertex_GetDistance(Vertex vertexVoid);

bool Vertex_IsStarting(Vertex vertexVoid);

Segment Vertex_GetSegment(Vertex vertexVoid);

void Vertex_SetX(Vertex vertexVoid, double x);

void Vertex_SetY(Vertex vertexVoid, double y);

void Vertex_SetAngle(Vertex vertexVoid, double angle);

void Vertex_SetDistance(Vertex vertexVoid, double distance);

void Vertex_SetStarting(Vertex vertexVoid, bool starting);

void Vertex_SetSegment(Vertex vertexVoid, Segment segment);

void Vertex_Destroy(Vertex vertexVoid);

#endif
