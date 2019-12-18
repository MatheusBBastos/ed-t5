#include "vertex.h"

typedef struct vertex_t {
    double x;
    double y;
    double angle;
    double distance;
    bool starting;
    Segment segment;
} *VertexPtr;

Vertex Vertex_Create(double x, double y, Segment segment, double angle, double distance) {
    VertexPtr vertex = malloc(sizeof(struct vertex_t));
    vertex->x = x;
    vertex->y = y;
    vertex->angle = angle;
    vertex->distance = distance;
    vertex->starting = false;
    vertex->segment = segment;
    return vertex;
}

int Vertex_Compare(const void *p1Void, const void *p2Void) {
    VertexPtr p1 = (VertexPtr) *((Vertex*) p1Void);
    VertexPtr p2 = (VertexPtr) *((Vertex*) p2Void);

    if (p1->angle < p2->angle)
        return -1;
    else if (p1->angle > p2->angle)
        return 1;
    else if (p1->distance < p2->distance)
        return 1;
    else if (p1->distance > p2->distance)
        return -1;
    else if (p1->starting && !p2->starting)
        return 1;
    else if (!p1->starting && p2->starting)
        return -1;
    else if (Segment_GetDistance(p1->segment) < Segment_GetDistance(p2->segment))
        return 1;
    else if (Segment_GetDistance(p1->segment) > Segment_GetDistance(p2->segment))
        return -1;
    else
        return 0;
}

double Vertex_GetX(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->x;
}

double Vertex_GetY(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->y;
}

double Vertex_GetAngle(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->angle;
}

double Vertex_GetDistance(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->distance;
}

bool Vertex_IsStarting(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->starting;
}

Segment Vertex_GetSegment(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    return vertex->segment;
}

void Vertex_SetX(Vertex vertexVoid, double x) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->x = x;
}

void Vertex_SetY(Vertex vertexVoid, double y) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->y = y;
}

void Vertex_SetAngle(Vertex vertexVoid, double angle) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->angle = angle;
}

void Vertex_SetDistance(Vertex vertexVoid, double distance) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->distance = distance;
}

void Vertex_SetStarting(Vertex vertexVoid, bool starting) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->starting = starting;
}

void Vertex_SetSegment(Vertex vertexVoid, Segment segment) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    vertex->segment = segment;
}

void Vertex_Destroy(Vertex vertexVoid) {
    VertexPtr vertex = (VertexPtr) vertexVoid;
    free(vertex);
}
