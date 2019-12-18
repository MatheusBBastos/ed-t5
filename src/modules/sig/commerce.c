#include "commerce.h"

typedef struct commerce_t {
    CommerceType type;
    char address[64];
    Block block;
    Building building;
    char name[64];
    char cnpj[24];
    Person owner;
} *CommercePtr;

Commerce Commerce_Create(CommerceType type, char address[], Block block, Building building, char name[], char cnpj[], Person owner) {
    CommercePtr commerce = malloc(sizeof(struct commerce_t));
    commerce->type = type;
    strcpy(commerce->address, address);
    commerce->block = block;
    commerce->building = building;
    strcpy(commerce->name, name);
    strcpy(commerce->cnpj, cnpj);
    commerce->owner = owner;
    return commerce;
}

void Commerce_DumpToFile(Commerce commerceVoid, FILE *file) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    fprintf(file, "Dados do estabelecimento de CNPJ %s:\n"
                  "\tNome: %s\n"
                  "\tEndereço: %s\n"
                  "\tTipo: %s\n",
                  commerce->cnpj,
                  commerce->name,
                  commerce->address,
                  CommerceType_GetDescription(commerce->type));
}

CommerceType Commerce_GetType(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->type;
}

char *Commerce_GetAddress(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->address;
}

Building Commerce_GetBlock(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->block;
}

Building Commerce_GetBuilding(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->building;
}

char *Commerce_GetName(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->name;
}

char *Commerce_GetCnpj(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->cnpj;
}

Person Commerce_GetOwner(Commerce commerceVoid) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    return commerce->owner;
}

void Commerce_SetType(Commerce commerceVoid, CommerceType type) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    commerce->type = type;
}

void Commerce_SetAddress(Commerce commerceVoid, char address[]) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    strcpy(commerce->address, address);
}

void Commerce_SetBlock(Commerce commerceVoid, Block block) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    commerce->block = block;
}

void Commerce_SetBuilding(Commerce commerceVoid, Building building) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    commerce->building = building;
}

void Commerce_SetName(Commerce commerceVoid, char name[]) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    strcpy(commerce->name, name);
}

void Commerce_SetCnpj(Commerce commerceVoid, char cnpj[]) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    strcpy(commerce->cnpj, cnpj);
}

void Commerce_SetOwner(Commerce commerceVoid, Person owner) {
    CommercePtr commerce = (CommercePtr) commerceVoid;
    commerce->owner = owner;
}

void Commerce_Destroy(Commerce commerceVoid) {
    free((CommercePtr) commerceVoid);
}
