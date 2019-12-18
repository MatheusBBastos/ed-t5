#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <string.h>
#include "../aux/point.h"

typedef void *Text;

Text Text_Create(double x, double y, char string[]);

double Text_GetX(Text text);

double Text_GetY(Text text);

char *Text_GetString(Text text);

void Text_SetX(Text text, double x);

void Text_SetY(Text text, double y);

void Text_SetString(Text text, char string[]);

Point Text_GetPoint(Text text);

void Text_Destroy(Text text);

#endif
