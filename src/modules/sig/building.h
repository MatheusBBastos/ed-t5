#ifndef BUILDING_H
#define BUILDING_H

#include <stdio.h>
#include <stdlib.h>
#include "../aux/point.h"
#include "../aux/segment.h"
#include "../data_structures/redblack_tree.h"

typedef void *Block;
typedef void *Building;
typedef void *Commerce;
typedef void *Person;

#include "block.h"
#include "commerce.h"
#include "person.h"

Building Building_Create(Block block, char face, int num, double f, double p, double mrg);

void Building_MakeAddress(char *address, char *cep, char face, int num);

// Posiciona quatro ou seis segmentos no vetor 'vector' a partir do prédio informado,
// levando em conta se os novos segmentos seriam cortados pela semirreta partindo
// das coordenadas de origem e com direção horizontal à esquerda
Segment *Building_PutSegments(Building building, Segment *vector, double x, double y);

RBTree Building_GetResidents(Building building);

RBTree Building_GetCommerces(Building building);

void Building_InsertCommerce(Building building, Commerce commerce);

void Building_InsertResident(Building building, Person person);

void Building_RemoveCommerce(Building building, Commerce commerce);

void Building_RemoveResident(Building building, Person person);

double Building_GetX(Building building);

double Building_GetY(Building building);

double Building_GetW(Building building);

double Building_GetH(Building building);

int Building_GetNum(Building building);

Block Building_GetBlock(Building building);

char *Building_GetKey(Building building);

void Building_SetX(Building building, double x);

void Building_SetY(Building building, double y);

void Building_SetW(Building building, double w);

void Building_SetH(Building building, double h);

void Building_SetNum(Building building, int num);

void Building_SetBlock(Building building, Block block);

void Building_SetKey(Building building, char *key);

Point Building_GetPoint(Building building);

void Building_SetHighlighted(Building building, bool highlighted);

bool Building_IsHighlighted(Building building);

void Building_SetPainted(Building building, bool painted);

bool Building_IsPainted(Building building);

void Building_Describe(Building building, char *str);

void Building_Destroy(Building building);

#endif
