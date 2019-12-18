#include "person.h"

typedef struct person_t {
    char cpf[16];
    char name[32];
    char surname[32];
    char sex;
    char birthDate[16];
    Block block;
    Building building;
    char address[64];
    char complement[16];
} *PersonPtr;

Person Person_Create(char cpf[], char name[], char surname[], char sex, char birthDate[]) {
    PersonPtr person = malloc(sizeof(struct person_t));
    strcpy(person->cpf, cpf);
    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->sex = sex;
    strcpy(person->birthDate, birthDate);
    person->block = NULL;
    person->building = NULL;
    person->address[0] = '\0';
    person->complement[0] = '\0';
    return person;
}

void Person_DumpToFile(Person personVoid, FILE *file) {
    PersonPtr person = (PersonPtr) personVoid;
    fprintf(file, "\tCPF: %s\n"
                  "\tNome: %s\n"
                  "\tSobrenome: %s\n"
                  "\tSexo: %s\n"
                  "\tData de nascimento: %s\n"
                  "\tEndereço: %s %s\n",
                  person->cpf, person->name, person->surname,
                  person->sex == 'm' ? "masculino" : "feminino",
                  person->birthDate,
                  person->address,
                  person->complement);
}

char *Person_GetCpf(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->cpf;
}

char *Person_GetName(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->name;
}

char *Person_GetSurname(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->surname;
}

char Person_GetSex(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->sex;
}

char *Person_GetBirthDate(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->birthDate;
}

Block Person_GetBlock(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->block;
}

Building Person_GetBuilding(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->building;
}

char *Person_GetAddress(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->address;
}

char *Person_GetComplement(Person personVoid) {
    PersonPtr person = (PersonPtr) personVoid;
    return person->complement;
}

void Person_SetCpf(Person personVoid, char cpf[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->cpf, cpf);
}

void Person_SetName(Person personVoid, char name[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->name, name);
}

void Person_SetSurname(Person personVoid, char surname[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->surname, surname);
}

void Person_SetSex(Person personVoid, char sex) {
    PersonPtr person = (PersonPtr) personVoid;
    person->sex = sex;
}

void Person_SetBirthDate(Person personVoid, char birthDate[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->birthDate, birthDate);
}

void Person_SetBlock(Person personVoid, Block block) {
    PersonPtr person = (PersonPtr) personVoid;
    person->block = block;
}

void Person_SetBuilding(Person personVoid, Building building) {
    PersonPtr person = (PersonPtr) personVoid;
    person->building = building;
}

void Person_SetAddress(Person personVoid, char address[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->address, address);
}

void Person_SetComplement(Person personVoid, char complement[]) {
    PersonPtr person = (PersonPtr) personVoid;
    strcpy(person->complement, complement);
}

void Person_Destroy(Person personVoid) {
    free((PersonPtr) personVoid);
}
