#include "circle.h"

typedef struct circle_t {
    Point point;
    double radius;
} *CirclePtr;

Circle Circle_Create(double radius, double x, double y) {
    CirclePtr newCircle = malloc(sizeof(struct circle_t));
    newCircle->radius = radius;
    newCircle->point = Point_Create(x, y);
    return newCircle;
}

double Circle_GetRadius(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return circle->radius;
}

double Circle_GetX(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return Point_GetX(circle->point);
}

double Circle_GetY(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return Point_GetY(circle->point);
}

void Circle_SetRadius(Circle circleVoid, double radius) {
    CirclePtr circle = (CirclePtr) circleVoid;
    circle->radius = radius;
}

void Circle_SetX(Circle circleVoid, double x) {
    CirclePtr circle = (CirclePtr) circleVoid;
    Point_SetX(circle->point, x);
}

void Circle_SetY(Circle circleVoid, double y) {
    CirclePtr circle = (CirclePtr) circleVoid;
    Point_SetY(circle->point, y);
}

Point Circle_GetPoint(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return circle->point;
}

void Circle_Destroy(Circle circleVoid) {
    CirclePtr circle = (Circle) circleVoid;
    Point_Destroy(circle->point);
    free(circle);
}