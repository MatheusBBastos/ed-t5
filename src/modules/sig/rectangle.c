#include "rectangle.h"

typedef struct rectangle_t {
    Point point;
    double width, height;
} *RectanglePtr;

Rectangle Rectangle_Create(double width, double height, double x, double y) {
    RectanglePtr newRect = malloc(sizeof(struct rectangle_t));
    newRect->width = width;
    newRect->height = height;
    newRect->point = Point_Create(x, y);
    return newRect;
}

double Rectangle_GetWidth(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->width;
}

double Rectangle_GetHeight(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->height;
}

double Rectangle_GetX(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return Point_GetX(rectangle->point);
}

double Rectangle_GetY(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return Point_GetY(rectangle->point);
}

void Rectangle_SetWidth(Rectangle rectangleVoid, double width) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->width = width;
}

void Rectangle_SetHeight(Rectangle rectangleVoid, double height) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->height = height;
}

void Rectangle_SetX(Rectangle rectangleVoid, double x) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    Point_SetX(rectangle->point, x);
}

void Rectangle_SetY(Rectangle rectangleVoid, double y) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    Point_SetY(rectangle->point, y);
}

Point Rectangle_GetPoint(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->point;
}

void Rectangle_Destroy(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    Point_Destroy(rectangle->point);
    free(rectangle);
}