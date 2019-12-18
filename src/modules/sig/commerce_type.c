#include "commerce_type.h"

typedef struct commerceType_t {
    char code[16];
    char description[64];
} *CommerceTypePtr;

CommerceType CommerceType_Create(char code[], char description[]) {
    CommerceTypePtr commerceType = malloc(sizeof(struct commerceType_t));
    strcpy(commerceType->code, code);
    strcpy(commerceType->description, description);
    return commerceType;
}

char *CommerceType_GetCode(CommerceType commerceTypeVoid) {
    CommerceTypePtr commerceType = (CommerceTypePtr) commerceTypeVoid;
    return commerceType->code;
}

char *CommerceType_GetDescription(CommerceType commerceTypeVoid) {
    CommerceTypePtr commerceType = (CommerceTypePtr) commerceTypeVoid;
    return commerceType->description;
}

void CommerceType_SetCode(CommerceType commerceTypeVoid, char code[]) {
    CommerceTypePtr commerceType = (CommerceTypePtr) commerceTypeVoid;
    strcpy(commerceType->code, code);
}

void CommerceType_SetDescription(CommerceType commerceTypeVoid, char description[]) {
    CommerceTypePtr commerceType = (CommerceTypePtr) commerceTypeVoid;
    strcpy(commerceType->description, description);
}

void CommerceType_Destroy(CommerceType commerceTypeVoid) {
    free((CommerceTypePtr) commerceTypeVoid);
}
