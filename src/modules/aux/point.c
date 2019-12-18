#include "point.h"

typedef struct point_t {
    double x;
    double y;
} *PointPtr;

Point Point_Create(double x, double y) {
    PointPtr point = malloc(sizeof(struct point_t));
    point->x = x;
    point->y = y;
    return point;
}

double Point_GetX(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->x;
}

double Point_GetY(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    return point->y;
}

void Point_SetX(Point pointVoid, double x) {
    PointPtr point = (PointPtr) pointVoid;
    point->x = x;
}

void Point_SetY(Point pointVoid, double y) {
    PointPtr point = (PointPtr) pointVoid;
    point->y = y;
}

int Point_Compare(const void *point1Void, const void *point2Void) {
    PointPtr point1 = (PointPtr) point1Void;
    PointPtr point2 = (PointPtr) point2Void;
    
    if (point1->x < point2->x) {
        return -1;
    } else if (point1->x > point2->x) {
        return 1;
    } else if (point1->y < point2->y) {
        return -1;
    } else if (point1->y > point2->y) {
        return 1;
    } else {
        return 0;
    }
}

void Point_Destroy(Point pointVoid) {
    PointPtr point = (PointPtr) pointVoid;
    free(point);
}
