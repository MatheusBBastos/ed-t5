#ifndef COMMERCETYPE_H
#define COMMERCETYPE_H

#include <stdlib.h>
#include <string.h>

typedef void *CommerceType;

CommerceType CommerceType_Create(char code[], char description[]);

char *CommerceType_GetCode(CommerceType commerceType);

char *CommerceType_GetDescription(CommerceType commerceType);

void CommerceType_SetCode(CommerceType commerceType, char code[]);

void CommerceType_SetDescription(CommerceType commerceType, char description[]);

void CommerceType_Destroy(CommerceType commerceType);

#endif
