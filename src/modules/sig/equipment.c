#include "equipment.h"

typedef struct equip_t {
    char id[16];
    Point point;
    char cFill[24];
    char cStroke[24];
    char wStroke[16];
    bool highlighted;
} *EquipPtr;

Equip Equip_Create(char id[], double x, double y, char cFill[], char cStroke[], char wStroke[]) {
    EquipPtr equip = malloc(sizeof(struct equip_t));
    strcpy(equip->id, id);
    equip->point = Point_Create(x, y);
    equip->highlighted = false;
    strcpy(equip->cFill, cFill);
    strcpy(equip->cStroke, cStroke);
    strcpy(equip->wStroke, wStroke);
    return equip;
}

char *Equip_GetID(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->id;
}

char *Equip_GetCFill(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->cFill;
}

char *Equip_GetCStroke(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->cStroke;
}

char *Equip_GetWStroke(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->wStroke;
}

double Equip_GetX(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return Point_GetX(equip->point);
}

void Equip_SetX(Equip equipVoid, double x) {
    EquipPtr equip = (EquipPtr) equipVoid;
    Point_SetX(equip->point, x);
}

double Equip_GetY(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return Point_GetY(equip->point);
}

void Equip_SetY(Equip equipVoid, double y) {
    EquipPtr equip = (EquipPtr) equipVoid;
    Point_SetY(equip->point, y);
}

bool Equip_GetHighlighted(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->highlighted;
}

void Equip_SetHighlighted(Equip equipVoid, bool highlighted) {
    EquipPtr equip = (EquipPtr) equipVoid;
    equip->highlighted = highlighted;
}

Point Equip_GetPoint(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->point;
}

void Equip_Describe(Equip equipVoid, char *str) {
    EquipPtr equip = (EquipPtr) equipVoid;
    sprintf(str, "(%.2lf, %.2lf)\n%s", Point_GetX(equip->point), Point_GetY(equip->point),
            equip->id);
}

void Equip_Destroy(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    Point_Destroy(equip->point);
    free(equip);
}