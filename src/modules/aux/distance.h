#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdlib.h>
#include "../sig/equipment.h"

typedef void *Distance;

Distance Distance_Create(double dist, Equip equip);

double Distance_GetDist(const void *distance);

Equip Distance_GetEquip(const void *distance);

void Distance_SetDist(Distance distance, double dist);

void Distance_SetEquip(Distance distance, Equip equip);

void Distance_Destroy(Distance distance);

#endif
