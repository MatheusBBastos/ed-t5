#include "object.h"

typedef struct object_t {
    void *content;
    ObjectType type;
    char color1[24], color2[24];
    char id[8];
    char stroke[16];
} *ObjectPtr;

Object Object_Create(char id[], void *content, int type, char color1[], char color2[], char stroke[]) {
    ObjectPtr newObj = malloc(sizeof(struct object_t));
    newObj->content = content;
    newObj->type = type;
    strcpy(newObj->id, id);
    strcpy(newObj->color1, color1);
    strcpy(newObj->color2, color2);
    strcpy(newObj->stroke, stroke);
    return newObj;
}

void *Object_GetContent(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->content;
}

ObjectType Object_GetType(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->type;
}

char *Object_GetColor1(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->color1;
}

char *Object_GetColor2(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->color2;
}

char *Object_GetStroke(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->stroke;
}

char *Object_GetId(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    return object->id;
}

void Object_Destroy(Object objectVoid) {
    ObjectPtr object = (ObjectPtr) objectVoid;
    if (object->type == OBJ_CIRC)
        Circle_Destroy(object->content);
    else if (object->type == OBJ_RECT)
        Rectangle_Destroy(object->content);
    free(object);
}