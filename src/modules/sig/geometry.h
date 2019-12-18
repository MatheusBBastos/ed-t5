#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "object.h"
#include "circle.h"
#include "rectangle.h"

#define PI 3.1415192653589793

// Distância Euclidiana entre dois pontos (x1, y1) e (x2, y2)
double euclideanDistance(double x1, double y1, double x2, double y2);

// Distância de Manhattan entre dois pontos (x1, y1) e (x2, y2)
double manhattanDistance(double x1, double y1, double x2, double y2);

// Retorna o valor 'value' limitado no intervalo [a, b]
double clamp(double value, double a, double b);

// Verifica se as duas formas geométricas se sobrepõem
bool checkOverlap(Object a, Object b);

// Verifica se o ponto (x, y) está dentro da forma 'obj'
bool checkInside(Object obj, double x, double y);

// Coloca o ponto central da forma geométrica em (x, y)
void getCenter(Object obj, double *x, double *y);

// Define 'surroundingRect' como o retângulo que cerca a forma geométrica
void getSurroundingRect(Object obj, Rectangle resultRect);

// Retorna o máximo de dois valores
double max(double a, double b);

// Retorna o mínimo de dois valores
double min(double a, double b);

// Calcula o ângulo de um ponto em relação à origem
double calculateAngle(double y, double x);

bool nearlyEqual(double a, double b);

#endif