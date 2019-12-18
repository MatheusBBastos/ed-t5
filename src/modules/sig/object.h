#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "circle.h"
#include "rectangle.h"

typedef enum ObjectType {
    OBJ_CIRC,
    OBJ_RECT,
    OBJ_TEXT
} ObjectType;

typedef void *Object;

Object Object_Create(char id[], void *content, int type, char color1[], char color2[], char stroke[]);

void *Object_GetContent(Object object);

ObjectType Object_GetType(Object object);

char *Object_GetColor1(Object object);

char *Object_GetColor2(Object object);

char *Object_GetStroke(Object object);

char *Object_GetId(Object object);

void Object_Destroy(Object object);

#endif