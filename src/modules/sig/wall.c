#include "wall.h"

typedef struct wall_t {
    Point point1;
    Point point2;
} *WallPtr;

Wall Wall_Create(double x1, double y1, double x2, double y2) {
    WallPtr wall = malloc(sizeof(struct wall_t));
    wall->point1 = Point_Create(x1, y1);
    wall->point2 = Point_Create(x2, y2);
    return wall;
}

void Wall_Describe(Wall wallVoid, char *str) {
    WallPtr wall = (WallPtr) wallVoid;
    sprintf(str, "(%.2lf, %.2lf)\n(%.2lf, %.2lf)\n", 
            Point_GetX(wall->point1), Point_GetY(wall->point1),
            Point_GetX(wall->point2), Point_GetY(wall->point2));
}

Segment *Wall_PutSegments(Wall wallVoid, Segment *vector, double xSource, double ySource) {
    WallPtr wall = (WallPtr) wallVoid;
    double x1 = Point_GetX(wall->point1), y1 = Point_GetY(wall->point1);
    double x2 = Point_GetX(wall->point2), y2 = Point_GetY(wall->point2);

    if (x1 == x2 && x1 == xSource || y1 == y2 && y1 == ySource)
        return vector;

    Segment seg = Segment_CreateFromCoords(x1, y1, x2, y2, xSource, ySource);
    (*vector++) = seg;

    double xInter = Segment_CheckXIntersection(seg, ySource);
    // Intersecção com a reta
    if (xInter <= xSource && xInter <= max(x1, x2) && xInter >= min(x1, x2)) {
        vector = Segment_Cut(seg, vector, xInter, xSource, ySource);
    }

    return vector;
}

double Wall_GetX1(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return Point_GetX(wall->point1);
}

double Wall_GetY1(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return Point_GetY(wall->point1);
}

double Wall_GetX2(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return Point_GetX(wall->point2);
}

double Wall_GetY2(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return Point_GetY(wall->point2);
}

void Wall_SetX1(Wall wallVoid, double x1) {
    WallPtr wall = (WallPtr) wallVoid;
    Point_SetX(wall->point1, x1);
}

void Wall_SetY1(Wall wallVoid, double y1) {
    WallPtr wall = (WallPtr) wallVoid;
    Point_SetY(wall->point1, y1);
}

void Wall_SetX2(Wall wallVoid, double x2) {
    WallPtr wall = (WallPtr) wallVoid;
    Point_SetX(wall->point2, x2);
}

void Wall_SetY2(Wall wallVoid, double y2) {
    WallPtr wall = (WallPtr) wallVoid;
    Point_SetY(wall->point2, y2);
}

Point Wall_GetPoint1(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    return wall->point1;
}

void Wall_Destroy(Wall wallVoid) {
    WallPtr wall = (WallPtr) wallVoid;
    Point_Destroy(wall->point1);
    Point_Destroy(wall->point2);
    free(wall);
}
