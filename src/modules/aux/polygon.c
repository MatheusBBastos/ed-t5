#include "polygon.h"

typedef struct segment_t {
    double x1, y1, x2, y2;
    struct segment_t *next;
} *PolySegment;

typedef struct polygon_t {
    PolySegment first, last;
    bool pointInserted;
    double lastX, lastY;
    double minX, maxX;
} *PolygonImpl;

Polygon Polygon_Create() {
    PolygonImpl polygon = malloc(sizeof(struct polygon_t));
    polygon->first = NULL;
    polygon->last = NULL;
    polygon->pointInserted = false;
    return polygon;
}

void Polygon_Connect(Polygon polygonVoid) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;

    // Conectar último ponto ao primeiro
    PolySegment seg = malloc(sizeof(struct segment_t));
    seg->x1 = polygon->lastX;
    seg->y1 = polygon->lastY;
    seg->x2 = polygon->first->x1;
    seg->y2 = polygon->first->y1;
    seg->next = NULL;

    polygon->last->next = seg;
    polygon->last = seg;
}

void Polygon_InsertPoint(Polygon polygonVoid, double x, double y) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    if (polygon->pointInserted) {
        // Não inserir ponto se for igual ao anterior
        if (polygon->lastX != x || polygon->lastY != y) {
            PolySegment seg = malloc(sizeof(struct segment_t));
            seg->next = NULL;
            seg->x1 = polygon->lastX;
            seg->y1 = polygon->lastY;
            seg->x2 = x;
            seg->y2 = y;

            if (polygon->first == NULL) {
                polygon->first = seg;
            } else {
                polygon->last->next = seg;
            }
            polygon->last = seg;

            polygon->minX = min(polygon->minX, x);
            polygon->maxX = max(polygon->maxX, x);
        }
    } else {
        polygon->pointInserted = true;
        polygon->minX = x;
        polygon->maxX = x;
    }
    polygon->lastX = x;
    polygon->lastY = y;
}

void Polygon_ReadFromFile(Polygon polygonVoid, FILE *file) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;

    double x, y;
    fscanf(file, "%lf %lf", &polygon->lastX, &polygon->lastY);

    polygon->minX = polygon->lastX;
    polygon->maxX = polygon->lastX;

    while (fscanf(file, "%lf %lf", &x, &y) > 0) {
        PolySegment seg = malloc(sizeof(struct segment_t));
        seg->next = NULL;
        seg->x1 = polygon->lastX;
        seg->y1 = polygon->lastY;
        seg->x2 = x;
        seg->y2 = y;

        if (polygon->first == NULL) {
            polygon->first = seg;
        } else {
            polygon->last->next = seg;
        }
        polygon->last = seg;

        polygon->lastX = x;
        polygon->lastY = y;
        polygon->minX = min(polygon->minX, x);
        polygon->maxX = max(polygon->maxX, x);
    }

    polygon->pointInserted = true;

    Polygon_Connect(polygonVoid);
}

void Polygon_DumpToFile(Polygon polygonVoid, FILE *file) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    for (PolySegment seg = polygon->first; seg != NULL; seg = seg->next) {
        fprintf(file, "%lf %lf\n", seg->x1, seg->y1);
    }
}

double Polygon_CalculateArea(Polygon polygonVoid) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    double area = 0;
    for (PolySegment seg = polygon->first; seg != NULL; seg = seg->next) {
        area += (seg->x1 * seg->y2 - seg->y1 * seg->x2);
    }
    return fabs(area) / 2;
}

void Polygon_Destroy(Polygon polygonVoid) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    PolySegment seg = polygon->first;
    while (seg != NULL) {
        PolySegment next = seg->next;
        free(seg);
        seg = next;
    }
    free(polygon);
}

int _calculateIntersections(PolygonImpl polygon, double x, double y) {

    int intersections = 0;
    
    for (PolySegment seg = polygon->first; seg != NULL; seg = seg->next) {
        // Segmento do polígono é horizontal e o ponto cruza com ele
        if (seg->y1 == seg->y2 && seg->y1 == y) {
            // Não contar intersecção
            continue;
        // Ponto cruza com o fim do segmento
        } else if (y == seg->y2) {
            // Ponto do segmento está à direita, não contar
            if (x < seg->x1)
                continue;
            // Selecionar próximo segmento
            PolySegment next = seg->next;
            if (next == NULL)
                next = polygon->first;
            // Verificar se o próximo segmento está na mesma direção que o atual
            // (não é um ápice)
            if ((seg->y2 - seg->y1) * (next->y2 - next->y1) > 0) {
                intersections++;
            }
        } else if (y != seg->y1) {
            // Segmento do polígono é vertical
            if (seg->x1 == seg->x2) {
                // Ponto está no intervalo do eixo y do segmento
                if (seg->x1 <= x && y > min(seg->y1, seg->y2) && y < max(seg->y1, seg->y2))
                    intersections++;
            } else {
                // Coeficiente angular do segmento
                double a = (seg->y1 - seg->y2)/(seg->x1 - seg->x2);
                double b = seg->y1 - a * seg->x1;

                // x do ponto de intersecção entre a reta e o segmento
                double xInter = (y - b)/a;
                
                // Verificar que a intersecção ocorre no segmento e está à esquerda do ponto
                if (xInter <= x && xInter > min(seg->x1, seg->x2) && xInter < max(seg->x1, seg->x2))
                    intersections++;
            }
        }
    }

    return intersections;
}

double _crossProduct(double x1, double y1, double x2, double y2) {
    return x1 * y2 - x2 * y1;
}

bool _checkBoundingBoxIntersection(PolySegment a, PolySegment b) {
    return min(a->x1, a->x2) < max(b->x1, b->x2) 
            && max(a->x1, a->x2) > min(b->x1, b->x2) 
            && min(a->y1, a->y2) < max(b->y1, b->y2)
            && max(a->y1, a->y2) > min(b->y1, b->y2);
}

bool _isPointOnLine(PolySegment seg, double x, double y) {
    double ax = seg->x2 - seg->x1, ay = seg->y2 - seg->y1;
    double bx = x - seg->x1, by = y - seg->y1;
    double r = _crossProduct(ax, ay, bx, by);
    return fabs(r) < DBL_EPSILON;
}

bool _isPointRightOfLine(PolySegment seg, double x, double y) {
    double ax = seg->x2 - seg->x1, ay = seg->y2 - seg->y1;
    double bx = x - seg->x1, by = y - seg->y1;
    return _crossProduct(ax, ay, bx, by) < 0;
}

bool _segmentTouchesOrCrossesLine(PolySegment a, PolySegment b) {
    return _isPointOnLine(a, b->x1, b->y1)
            || _isPointOnLine(a, b->x2, b->y2)
            || (_isPointRightOfLine(a, b->x1, b->y1) ^ _isPointRightOfLine(a, b->x2, b->y2));
}

bool _checkIntersection(PolygonImpl polygon, double x1, double y1, double x2, double y2) {
    struct segment_t testingSeg = {x1, y1, x2, y2};
    for (PolySegment seg = polygon->first; seg != NULL; seg = seg->next) {
        if (_checkBoundingBoxIntersection(&testingSeg, seg)
                && _segmentTouchesOrCrossesLine(&testingSeg, seg)
                && _segmentTouchesOrCrossesLine(seg, &testingSeg))
            return true;
    }
    return false;
}

bool Polygon_DoesSegmentIntersect(Polygon polygon, double x1, double y1, double x2, double y2) {
    return _checkIntersection((PolygonImpl) polygon, x1, y1, x2, y2);
}

bool Polygon_IsPointInside(Polygon polygonVoid, double x, double y) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;

    int i = _calculateIntersections(polygon, x, y);
    return i % 2 == 1;
}

bool _isRectInside(PolygonImpl polygon, double x, double y, double w, double h, bool partially) {
    if (partially) {
        // Testar se o retângulo está parcialmente dentro, portanto se qualquer uma
        // das condições forem cumpridas o resultado é positivo
        if (_calculateIntersections(polygon, x, y) % 2 == 1)
            return true;
        if (_calculateIntersections(polygon, x + w, y) % 2 == 1)
            return true;
        if (_calculateIntersections(polygon, x, y + h) % 2 == 1)
            return true;
        if (_calculateIntersections(polygon, x + w, y + h) % 2 == 1)
            return true;
        
        if (_checkIntersection(polygon, x, y, x + w, y))
            return true;
        if (_checkIntersection(polygon, x + w, y, x + w, y + h))
            return true;
        if (_checkIntersection(polygon, x + w, y + h, x, y + h))
            return true;
        if (_checkIntersection(polygon, x, y + h, x, y))
            return true;

        return false;
    }
    
    // Retângulo inteiramente dentro, portanto todas as condições devem ser cumpridas
    if (_calculateIntersections(polygon, x, y) % 2 != 1)
        return false;
    if (_calculateIntersections(polygon, x + w, y) % 2 != 1)
        return false;
    if (_calculateIntersections(polygon, x, y + h) % 2 != 1)
        return false;
    if (_calculateIntersections(polygon, x + w, y + h) % 2 != 1)
        return false;
    
    if (_checkIntersection(polygon, x, y, x + w, y))
        return false;
    if (_checkIntersection(polygon, x + w, y, x + w, y + h))
        return false;
    if (_checkIntersection(polygon, x + w, y + h, x, y + h))
        return false;
    if (_checkIntersection(polygon, x, y + h, x, y))
        return false;
    
    return true;
}

double Polygon_GetMinX(Polygon polygon) {
    return ((PolygonImpl) polygon)->minX;
}

double Polygon_GetMaxX(Polygon polygon) {
    return ((PolygonImpl) polygon)->maxX;
}

bool Polygon_IsBlockInside(Polygon polygonVoid, Block block, bool partially) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    return _isRectInside(polygon, Block_GetX(block), Block_GetY(block), 
                         Block_GetW(block), Block_GetH(block), partially);
}

bool Polygon_IsBuildingInside(Polygon polygonVoid, Building building) {
    PolygonImpl polygon = (PolygonImpl) polygonVoid;
    return _isRectInside(polygon, Building_GetX(building), Building_GetY(building), 
                         Building_GetW(building), Building_GetH(building), false);
}

void *Polygon_GetFirstSeg(Polygon polygon) {
    return ((PolygonImpl) polygon)->first;
}

double PolySeg_GetX1(void *seg) {
    return ((PolySegment) seg)->x1;
}

double PolySeg_GetY1(void *seg) {
    return ((PolySegment) seg)->y1;
}

double PolySeg_GetX2(void *seg) {
    return ((PolySegment) seg)->x2;
}

double PolySeg_GetY2(void *seg) {
    return ((PolySegment) seg)->y2;
}

void *PolySeg_GetNext(void *seg) {
    return ((PolySegment) seg)->next;
}
