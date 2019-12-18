#include "segment.h"

typedef struct segment_t {
    Vertex pStart;
    Vertex pEnd;
    double xBiombo;
    double yBiombo;
    double distance;
} *SegmentPtr;

Segment Segment_Create(Vertex pStart, Vertex pEnd) {
    SegmentPtr segment = malloc(sizeof(struct segment_t));
    segment->pStart = pStart;
    segment->pEnd = pEnd;
    segment->xBiombo = Vertex_GetX(pStart);
    segment->yBiombo = Vertex_GetY(pStart);
    return segment;
}

Segment Segment_CreateFromCoords(double x1, double y1, double x2, double y2, double x, double y) {
    SegmentPtr segment = malloc(sizeof(struct segment_t));
    double angle1 = calculateAngle(y1 - y, x1 - x);
    double angle2 = calculateAngle(y2 - y, x2 - x);
    double distance1 = euclideanDistance(x1, y1, x, y);
    double distance2 = euclideanDistance(x2, y2, x, y);
    Vertex p1 = Vertex_Create(x1, y1, segment, angle1, distance1);
    Vertex p2 = Vertex_Create(x2, y2, segment, angle2, distance2);
    if (angle1 > angle2 || angle1 == angle2 && distance1 < distance2) {
        segment->pStart = p2;
        Vertex_SetStarting(p2, true);
        segment->pEnd = p1;
        Vertex_SetStarting(p1, false);
    } else {
        segment->pStart = p1;
        Vertex_SetStarting(p1, true);
        segment->pEnd = p2;
        Vertex_SetStarting(p2, false);
    }
    segment->xBiombo = Vertex_GetX(segment->pStart);
    segment->yBiombo = Vertex_GetY(segment->pStart);
    segment->distance = euclideanDistance(x, y, (x1 + x2) / 2, (y1 + y2) / 2);
    return segment;
}

double Segment_CheckXIntersection(Segment segmentVoid, double y) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    Vertex p1 = segment->pStart;
    Vertex p2 = segment->pEnd;

    if (Vertex_GetX(p1) == Vertex_GetX(p2)) {
        if (y > min(Vertex_GetY(p1), Vertex_GetY(p2)) && y <= max(Vertex_GetY(p1), Vertex_GetY(p2)))
            return Vertex_GetX(p1);
        else
            return -1;
    }

    double a = (Vertex_GetY(p1) - Vertex_GetY(p2))/(Vertex_GetX(p1) - Vertex_GetX(p2));
    double b = Vertex_GetY(p1) - a * Vertex_GetX(p1);

    return (y - b)/a;
}

Segment *Segment_Cut(Segment segment, Segment *vector, double xInter, double xSource, double ySource) {
    Vertex oldPStart = Segment_GetPStart(segment);
    Vertex oldPEnd = Segment_GetPEnd(segment);

    if (Vertex_GetY(oldPEnd) == ySource) {
        if (Vertex_GetY(oldPStart) > Vertex_GetY(oldPEnd)) {
            Vertex_SetAngle(oldPEnd, PI);
            return vector;
        }
        Vertex_SetAngle(oldPEnd, -PI);
        Vertex_SetStarting(oldPEnd, true);
        Segment_SetPStart(segment, oldPEnd);

        Vertex_SetStarting(oldPStart, false);
        Segment_SetPEnd(segment, oldPStart);

        return vector;
    }

    double distance = fabs(xInter - xSource);
    Vertex pAbove = Vertex_Create(xInter, ySource, segment, -PI, distance);
    Segment_SetPStart(segment, pAbove);
    Segment_SetPEnd(segment, oldPStart);
    Vertex_SetStarting(pAbove, true);
    Vertex_SetStarting(oldPStart, false);

    Vertex pBelow = Vertex_Create(xInter, ySource, NULL, PI, distance);
    Segment newSeg = Segment_Create(oldPEnd, pBelow);
    double newDist = euclideanDistance(xSource, ySource, (Vertex_GetX(oldPEnd) + Vertex_GetX(pBelow))/2, (Vertex_GetY(oldPEnd) + Vertex_GetY(pBelow))/2);
    Segment_SetDistance(newSeg, newDist);
    Vertex_SetSegment(pBelow, newSeg);
    Vertex_SetSegment(oldPEnd, newSeg);
    Vertex_SetStarting(oldPEnd, true);
    Vertex_SetStarting(pBelow, false);
    *(vector++) = newSeg;

    return vector;
}

Vertex Segment_GetPStart(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return segment->pStart;
}

Vertex Segment_GetPEnd(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return segment->pEnd;
}

double Segment_GetXBiombo(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return segment->xBiombo;
}

double Segment_GetYBiombo(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return segment->yBiombo;
}

double Segment_GetDistance(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return segment->distance;
}

void Segment_SetPStart(Segment segmentVoid, Vertex pStart) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    segment->pStart = pStart;
    segment->xBiombo = Vertex_GetX(pStart);
    segment->yBiombo = Vertex_GetY(pStart);
}

void Segment_SetPEnd(Segment segmentVoid, Vertex pEnd) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    segment->pEnd = pEnd;
}

void Segment_SetXBiombo(Segment segmentVoid, double xBiombo) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    segment->xBiombo = xBiombo;
}

void Segment_SetYBiombo(Segment segmentVoid, double yBiombo) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    segment->yBiombo = yBiombo;
}

void Segment_SetDistance(Segment segmentVoid, double distance) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    segment->distance = distance;
}

Vertex Segment_GetKey(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    return &segment->pEnd;
}

void Segment_Destroy(Segment segmentVoid) {
    SegmentPtr segment = (SegmentPtr) segmentVoid;
    Vertex_Destroy(segment->pStart);
    Vertex_Destroy(segment->pEnd);
    free(segment);
}
