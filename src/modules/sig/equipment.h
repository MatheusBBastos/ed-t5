#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../aux/point.h"

typedef void *Equip;

Equip Equip_Create(char id[], double x, double y, char cFill[], char cStroke[], char wStroke[]);

char *Equip_GetID(Equip equip);

char *Equip_GetCFill(Equip equip);

char *Equip_GetCStroke(Equip equip);

char *Equip_GetWStroke(Equip equip);

double Equip_GetX(Equip equip);

void Equip_SetX(Equip equip, double x);

double Equip_GetY(Equip equip);

void Equip_SetY(Equip equip, double y);

bool Equip_GetHighlighted(Equip equip);

void Equip_SetHighlighted(Equip equip, bool highlighted);

Point Equip_GetPoint(Equip equip);

void Equip_Describe(Equip equip, char *str);

void Equip_Destroy(Equip equip);

#endif
