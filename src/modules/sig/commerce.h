#ifndef COMMERCE_H
#define COMMERCE_H

#include <stdlib.h>

typedef void *Commerce;
typedef void *Building;
typedef void *Person;

#include "building.h"
#include "commerce_type.h"
#include "person.h"

Commerce Commerce_Create(CommerceType type, char address[], Block block, Building building, char name[], char cnpj[], Person owner);

void Commerce_DumpToFile(Commerce commerce, FILE *file);

CommerceType Commerce_GetType(Commerce commerce);

char *Commerce_GetAddress(Commerce commerce);

Building Commerce_GetBlock(Commerce commerce);

Building Commerce_GetBuilding(Commerce commerce);

char *Commerce_GetName(Commerce commerce);

char *Commerce_GetCnpj(Commerce commerce);

Person Commerce_GetOwner(Commerce commerce);

void Commerce_SetType(Commerce commerce, CommerceType type);

void Commerce_SetAddress(Commerce commerce, char address[]);

void Commerce_SetBlock(Commerce commerce, Block block);

void Commerce_SetBuilding(Commerce commerce, Building building);

void Commerce_SetName(Commerce commerce, char name[]);

void Commerce_SetCnpj(Commerce commerce, char cnpj[]);

void Commerce_SetOwner(Commerce commerce, Person owner);

void Commerce_Destroy(Commerce commerce);

#endif
