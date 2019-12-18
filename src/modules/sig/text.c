#include "text.h"

typedef struct text_t {
    Point point;
    char string[128];
} *TextPtr;

Text Text_Create(double x, double y, char string[]) {
    TextPtr text = malloc(sizeof(struct text_t));
    text->point = Point_Create(x, y);
    strcpy(text->string, string);
    return text;
}

double Text_GetX(Text textVoid) {
    TextPtr text = (TextPtr) textVoid;
    return Point_GetX(text->point);
}

double Text_GetY(Text textVoid) {
    TextPtr text = (TextPtr) textVoid;
    return Point_GetY(text->point);
}

char *Text_GetString(Text textVoid) {
    TextPtr text = (TextPtr) textVoid;
    return text->string;
}

void Text_SetX(Text textVoid, double x) {
    TextPtr text = (TextPtr) textVoid;
    Point_SetX(text->point, x);
}

void Text_SetY(Text textVoid, double y) {
    TextPtr text = (TextPtr) textVoid;
    Point_SetY(text->point, y);
}

void Text_SetString(Text textVoid, char string[]) {
    TextPtr text = (TextPtr) textVoid;
    strcpy(text->string, string);
}

Point Text_GetPoint(Text textVoid) {
    TextPtr text = (TextPtr) textVoid;
    return text->point;
}

void Text_Destroy(Text textVoid) {
    TextPtr text = (TextPtr) textVoid;
    free(text);
}
