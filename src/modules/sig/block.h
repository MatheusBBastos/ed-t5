#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../aux/point.h"
#include "../data_structures/redblack_tree.h"

typedef void *Block;
typedef void *Building;

#include "building.h"

Block Block_Create(char cep[], double x, double y, double w, double h, char cFill[], char cStroke[], char wStroke[]);

void Block_CalculateAddress(Block block, char face, int num, double *x, double *y);

RBTree Block_GetBuildings(Block block);

RBTree Block_GetCommerces(Block block);

RBTree Block_GetResidents(Block block);

void Block_InsertBuilding(Block block, Building building);

void Block_InsertCommerce(Block block, Commerce commerce);

void Block_InsertResident(Block block, Person person);

void Block_RemoveBuilding(Block block, Building building);

void Block_RemoveCommerce(Block block, Commerce commerce);

void Block_RemoveResident(Block block, Person person);

char *Block_GetCep(Block block);

char *Block_GetCFill(Block block);

void Block_SetCFill(Block block, char *cFill);

char *Block_GetCStroke(Block block);

void Block_SetCStroke(Block block, char *cStroke);

char *Block_GetWStroke(Block block);

void Block_SetWStroke(Block block, char *wStroke);

double Block_GetX(Block block);

void Block_SetX(Block block, double x);

double Block_GetY(Block block);

void Block_SetY(Block block, double y);

double Block_GetW(Block block);

double Block_GetH(Block block);

// Coloca as coordenadas do numero 'num' da face 'face' da quadra 'block' em (x, y)
bool Block_GetCoordinates(Block block, char face, double num, double *x, double *y);

Point Block_GetPoint(Block block);

void Block_Describe(Block blockVoid, char *str);

void Block_Destroy(Block block);

int compareStrings(const void *str1, const void *str2);

#endif
