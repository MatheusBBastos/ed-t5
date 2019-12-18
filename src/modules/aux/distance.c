#include "distance.h"

typedef struct distance_t {
    double dist;
    Equip equip;
} *DistancePtr;

Distance Distance_Create(double dist, Equip equip) {
    DistancePtr distance = malloc(sizeof(struct distance_t));
    distance->dist = dist;
    distance->equip = equip;
    return distance;
}

double Distance_GetDist(const void *distanceVoid) {
    DistancePtr distance = (DistancePtr) distanceVoid;
    return distance->dist;
}

Equip Distance_GetEquip(const void *distanceVoid) {
    DistancePtr distance = (DistancePtr) distanceVoid;
    return distance->equip;
}

void Distance_SetDist(Distance distanceVoid, double dist) {
    DistancePtr distance = (DistancePtr) distanceVoid;
    distance->dist = dist;
}

void Distance_SetEquip(Distance distanceVoid, Equip equip) {
    DistancePtr distance = (DistancePtr) distanceVoid;
    distance->equip = equip;
}

void Distance_Destroy(Distance distanceVoid) {
    DistancePtr distance = (DistancePtr) distanceVoid;
    free(distance);
}
