#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>

typedef void *Person;
typedef void *Building;

#include "building.h"

Person Person_Create(char cpf[], char name[], char surname[], char sex, char birthDate[]);

void Person_DumpToFile(Person person, FILE *file);

char *Person_GetCpf(Person person);

char *Person_GetName(Person person);

char *Person_GetSurname(Person person);

char Person_GetSex(Person person);

char *Person_GetBirthDate(Person person);

Block Person_GetBlock(Person person);

Building Person_GetBuilding(Person person);

char *Person_GetAddress(Person person);

char *Person_GetComplement(Person person);

void Person_SetCpf(Person person, char cpf[]);

void Person_SetName(Person person, char name[]);

void Person_SetSurname(Person person, char surname[]);

void Person_SetSex(Person person, char sex);

void Person_SetBirthDate(Person person, char birthDate[]);

void Person_SetBlock(Person person, Block block);

void Person_SetBuilding(Person person, Building building);

void Person_SetAddress(Person person, char address[]);

void Person_SetComplement(Person person, char complement[]);

void Person_Destroy(Person person);

#endif
