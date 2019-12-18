#include "building.h"

#include <ctype.h>

typedef struct building_t {
    Point point;
    double w;
    double h;
    int num;
    Block block;
    char key[32];
    RBTree commerces;
    RBTree residents;
    bool highlighted;
    bool painted;
} *BuildingPtr;

Building Building_Create(Block block, char face, int num, double f, double p, double mrg) {
    BuildingPtr building = malloc(sizeof(struct building_t));

    double x, y, w, h;
    x = Block_GetX(block);
    y = Block_GetY(block);

    if (toupper(face) == 'N') {
        x += num - f / 2;
        y += Block_GetH(block);
        y -= mrg;
        y -= p;
        w = f;
        h = p;
    } else if (toupper(face) == 'S') {
        x += num - f / 2;
        y += mrg;
        w = f;
        h = p;
    } else if (toupper(face) == 'L') {
        x += mrg;
        y += num - f / 2;
        w = p;
        h = f;
    } else if (toupper(face) == 'O') {
        x += Block_GetW(block);
        x -= mrg;
        x -= p;
        y += num - f / 2;
        w = p;
        h = f;
    } else {
        printf("Face não reconhecida: %c\n", face);
    }

    building->point = Point_Create(x, y);
    building->w = w;
    building->h = h;
    building->num = num;
    building->block = block;

    Building_MakeAddress(building->key, Block_GetCep(block), face, num);

    building->commerces = RBTree_Create(compareStrings);
    building->residents = RBTree_Create(compareStrings);
    building->highlighted = false;
    building->painted = false;

    return building;
}

void Building_MakeAddress(char *address, char *cep, char face, int num) {
    sprintf(address, "%s %c %03d", cep, toupper(face), num);
}

Segment *Building_PutSegments(Building buildingVoid, Segment *vector, double xSource, double ySource) {
    BuildingPtr b = (BuildingPtr) buildingVoid;

    double xMin = Point_GetX(b->point);
    double xMax = xMin + b->w;
    double yMin = Point_GetY(b->point);
    double yMax = yMin + b->h;
    
    Segment southSeg;
    if (yMin != ySource) {
        southSeg = Segment_CreateFromCoords(xMin, yMin, xMax, yMin, xSource, ySource);
        *(vector++) = southSeg;
    }
    
    Segment westSeg;
    if (xMax != xSource) {
        westSeg = Segment_CreateFromCoords(xMax, yMin, xMax, yMax, xSource, ySource);
        *(vector++) = westSeg;
    }

    Segment northSeg;
    if (yMax != ySource) {
        northSeg = Segment_CreateFromCoords(xMin, yMax, xMax, yMax, xSource, ySource);
        *(vector++) = northSeg;
    }

    Segment eastSeg;
    if (xMin != xSource) {
        eastSeg = Segment_CreateFromCoords(xMin, yMin, xMin, yMax, xSource, ySource);
        *(vector++) = eastSeg;
    }

    // Intersecção com a reta
    if (xMin < xSource && ySource > yMin && ySource < yMax) {
        // Segmento leste
        vector = Segment_Cut(eastSeg, vector, xMin, xSource, ySource);
        // Segmento oeste
        vector = Segment_Cut(westSeg, vector, xMax, xSource, ySource);
    }

    return vector;
}

RBTree Building_GetResidents(Building buildingVoid) {
    return ((BuildingPtr) buildingVoid)->residents;
}

RBTree Building_GetCommerces(Building buildingVoid) {
    return ((BuildingPtr) buildingVoid)->commerces;
}

void Building_InsertCommerce(Building buildingVoid, Commerce commerce) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    RBTree_Insert(building->commerces, Commerce_GetCnpj(commerce), commerce);
}

void Building_InsertResident(Building buildingVoid, Person person) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    RBTree_Insert(building->residents, Person_GetCpf(person), person);
}

void Building_RemoveCommerce(Building buildingVoid, Commerce commerce) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    RBTree_Remove(building->commerces, Commerce_GetCnpj(commerce));
}

void Building_RemoveResident(Building buildingVoid, Person person) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    RBTree_Remove(building->residents, Person_GetCpf(person));
}

double Building_GetX(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return Point_GetX(building->point);
}

double Building_GetY(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return Point_GetY(building->point);
}

double Building_GetW(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->w;
}

double Building_GetH(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->h;
}

int Building_GetNum(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->num;
}

Block Building_GetBlock(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->block;
}

char *Building_GetKey(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->key;
}

void Building_SetX(Building buildingVoid, double x) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    Point_SetX(building->point, x);
}

void Building_SetY(Building buildingVoid, double y) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    Point_SetY(building->point, y);
}

void Building_SetW(Building buildingVoid, double w) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->w = w;
}

void Building_SetH(Building buildingVoid, double h) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->h = h;
}

void Building_SetNum(Building buildingVoid, int num) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->num = num;
}

void Building_SetBlock(Building buildingVoid, Block block) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    building->block = block;
}

void Building_SetKey(Building buildingVoid, char *key) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    strcpy(building->key, key);
}

Point Building_GetPoint(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    return building->point;
}

void Building_SetHighlighted(Building buildingVoid, bool highlighted) {
    ((BuildingPtr) buildingVoid)->highlighted = highlighted;
}

bool Building_IsHighlighted(Building buildingVoid) {
    return ((BuildingPtr) buildingVoid)->highlighted;
}

void Building_SetPainted(Building buildingVoid, bool painted) {
    ((BuildingPtr) buildingVoid)->painted = painted;
}

bool Building_IsPainted(Building buildingVoid) {
    return ((BuildingPtr) buildingVoid)->painted;
}

void Building_Describe(Building buildingVoid, char *str) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    sprintf(str, "(%.2lf, %.2lf)\n%s", Point_GetX(building->point), Point_GetY(building->point),
            building->key);
}

void Building_Destroy(Building buildingVoid) {
    BuildingPtr building = (BuildingPtr) buildingVoid;
    Point_Destroy(building->point);
    RBTree_Destroy(building->commerces, NULL);
    RBTree_Destroy(building->residents, NULL);
    free(building);
}
