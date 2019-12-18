#include "block.h"

#include <ctype.h>

typedef struct block_t {
    char cep[16];
    Point point;
    double w;
    double h;
    char cFill[24];
    char cStroke[24];
    char wStroke[16];
    RBTree buildings;
    RBTree commerces;
    RBTree residents;
} *BlockPtr;

Block Block_Create(char cep[], double x, double y, double w, double h, 
                   char cFill[], char cStroke[], char wStroke[]) {
    BlockPtr block = malloc(sizeof(struct block_t));
    strcpy(block->cep, cep);
    block->point = Point_Create(x, y);
    block->w = w;
    block->h = h;
    strcpy(block->cFill, cFill);
    strcpy(block->cStroke, cStroke);
    strcpy(block->wStroke, wStroke);
    block->buildings = RBTree_Create(compareStrings);
    block->commerces = RBTree_Create(compareStrings);
    block->residents = RBTree_Create(compareStrings);
    return block;
}

void Block_CalculateAddress(Block block, char face, int num, double *x, double *y) {
    *x = Block_GetX(block);
    *y = Block_GetY(block);

    if (toupper(face) == 'N') {
        *x += num;
        *y += Block_GetH(block);
    } else if (toupper(face) == 'S') {
        *x += num;
    } else if (toupper(face) == 'L') {
        *y += num;
        *x += Block_GetW(block);
    } else if (toupper(face) == 'O') {
        *y += num;
    } else {
        printf("Face não reconhecida: %c\n", face);
    }
}

char *Block_GetCep(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cep;
}

char *Block_GetCFill(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cFill;
}

void Block_SetCFill(Block blockVoid, char *cFill) {
    strcpy(((BlockPtr) blockVoid)->cFill, cFill);
}

char *Block_GetCStroke(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cStroke;
}

void Block_SetCStroke(Block blockVoid, char *cStroke) {
    BlockPtr block = (BlockPtr) blockVoid;
    strcpy(block->cStroke, cStroke);
}

char *Block_GetWStroke(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->wStroke;
}

void Block_SetWStroke(Block blockVoid, char *wStroke) {
    strcpy(((BlockPtr) blockVoid)->wStroke, wStroke);
}

double Block_GetX(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return Point_GetX(block->point);
}

void Block_SetX(Block blockVoid, double x) {
    BlockPtr block = (BlockPtr) blockVoid;
    Point_SetX(block->point, x);
}

double Block_GetY(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return Point_GetY(block->point);
}

void Block_SetY(Block blockVoid, double y) {
    BlockPtr block = (BlockPtr) blockVoid;
    Point_SetY(block->point, y);
}

double Block_GetW(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->w;
}

double Block_GetH(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->h;
}

bool Block_GetCoordinates(Block blockVoid, char face, double num, double *x, double *y) {
    BlockPtr block = (BlockPtr) blockVoid;
    *x = Point_GetX(block->point);
    *y = Point_GetY(block->point);

    if (face == 'N') {
        *x += num;
        *y += block->h;
    } else if (face == 'S') {
        *x += num;
    } else if (face == 'O') {
        *x += block->w;
        *y += num;
    } else if (face == 'L') {
        *y += num;
    } else {
        return false;
    }

    return true;
}

Point Block_GetPoint(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->point;
}

RBTree Block_GetBuildings(Block blockVoid) {
    return ((BlockPtr) blockVoid)->buildings;
}

RBTree Block_GetCommerces(Block blockVoid) {
    return ((BlockPtr) blockVoid)->commerces;
}

RBTree Block_GetResidents(Block blockVoid) {
    return ((BlockPtr) blockVoid)->residents;
}

void Block_InsertBuilding(Block blockVoid, Building building) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Insert(block->buildings, Building_GetKey(building), building);
}

void Block_InsertCommerce(Block blockVoid, Commerce commerce) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Insert(block->commerces, Commerce_GetCnpj(commerce), commerce);
}

void Block_InsertResident(Block blockVoid, Person person) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Insert(block->residents, Person_GetCpf(person), person);
}

void Block_RemoveBuilding(Block blockVoid, Building building) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Remove(block->buildings, Building_GetKey(building));
}

void Block_RemoveCommerce(Block blockVoid, Commerce commerce) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Remove(block->commerces, Commerce_GetCnpj(commerce));
}

void Block_RemoveResident(Block blockVoid, Person person) {
    BlockPtr block = (BlockPtr) blockVoid;
    RBTree_Remove(block->residents, Person_GetCpf(person));
}

void Block_Describe(Block blockVoid, char *str) {
    BlockPtr block = (BlockPtr) blockVoid;
    sprintf(str, "(%.2lf, %.2lf)\n%s", Point_GetX(block->point), Point_GetY(block->point),
            block->cep);
}

void Block_Destroy(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    Point_Destroy(block->point);
    RBTree_Destroy(block->buildings, NULL);
    RBTree_Destroy(block->commerces, NULL);
    RBTree_Destroy(block->residents, NULL);
    free(block);
}

int compareStrings(const void *str1, const void *str2) {
    return strcmp((const char *) str1, (const char *) str2);
}
