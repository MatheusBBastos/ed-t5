#include "graph_node.h"
#include "../sig/block.h"
#include <float.h>
#include <string.h>

typedef struct graph_node_t *GraphNodeImpl;

typedef struct edge_t {
    char name[64];
    Block leftBlock;
    Block rightBlock;
    double length;
    double speed;
    GraphNodeImpl node;
} *Edge;

typedef struct edge_list_item_t {
    Edge edge;
    struct edge_list_item_t *previous, *next;
} *EdgeListItem;

typedef struct edge_list_t {
    EdgeListItem first, last;
} *EdgeList;

typedef struct graph_node_t {
    char id[32];
    Point point;
    EdgeList edges;
    double distance;
    struct graph_node_t *parent;
    bool closed;
    char *streetName;
} *GraphNodeImpl;

GraphNode GraphNode_Create(char id[], double x, double y) {
    GraphNodeImpl node = malloc(sizeof(struct graph_node_t));
    strcpy(node->id, id);
    node->point = Point_Create(x, y);
    node->edges = malloc(sizeof(struct edge_list_t));
    node->edges->first = NULL;
    node->edges->last = NULL;
    node->closed = false;
    node->streetName = NULL;
    return node;
}

void GraphNode_Destroy(GraphNode nodeVoid) {
    GraphNodeImpl node = (GraphNodeImpl) nodeVoid;
    EdgeListItem current = node->edges->first;
    while (current != NULL) {
        EdgeListItem next = current->next;
        free(current->edge);
        free(current);
        current = next;
    }
    free(node->edges);
    Point_Destroy(node->point);
    if (node->streetName != NULL)
        free(node->streetName);
    free(node);
}

void GraphNode_InsertEdge(GraphNode nodeVoid, GraphNode other, Block leftBlock, Block rightBlock, 
                          double length, double speed, char name[]) {
    GraphNodeImpl node = (GraphNodeImpl) nodeVoid;
    
    Edge edge = malloc(sizeof(struct edge_t));
    edge->node = (GraphNodeImpl) other;
    edge->leftBlock = leftBlock;
    edge->rightBlock = rightBlock;
    edge->length = length;
    edge->speed = speed;
    strcpy(edge->name, name);

    EdgeListItem item = malloc(sizeof(struct edge_list_item_t));
    item->edge = edge;
    item->next = NULL;

    if (node->edges->first == NULL) {
        item->previous = NULL;
        node->edges->first = item;
        node->edges->last = item;
    } else {
        item->previous = node->edges->last;
        node->edges->last->next = item;
        node->edges->last = item;
    }
}

GraphNode GraphNode_GoTo(GraphNode nodeVoid, char direction[], char streetName[]) {
    GraphNodeImpl node = (GraphNodeImpl) nodeVoid;

    double x1 = Point_GetX(node->point), y1 = Point_GetY(node->point);

    EdgeListItem current = node->edges->first;
    while (current != NULL) {
        GraphNodeImpl currentNode = current->edge->node;
        double x2 = Point_GetX(currentNode->point), y2 = Point_GetY(currentNode->point);

        if (strcmp(direction, "l") == 0 && x2 < x1 && y1 == y2
                || strcmp(direction, "o") == 0 && x2 > x1 && y1 == y2
                || strcmp(direction, "n") == 0 && y2 > y1 && x1 == x2
                || strcmp(direction, "s") == 0 && y2 < y1 && x1 == x2
                || strcmp(direction, "ne") == 0 && x2 < x1 && y1 < y2
                || strcmp(direction, "no") == 0 && x2 > x1 && y1 < y2
                || strcmp(direction, "se") == 0 && x2 < x1 && y1 > y2
                || strcmp(direction, "so") == 0 && x2 > x1 && y1 > y2) {
            strcpy(streetName, current->edge->name);
            return currentNode;
        }

        current = current->next;
    }

    return NULL;
}

void GraphNode_CollectNeighbors(GraphNode nodeVoid, BinaryHeap heap, bool byLength) {
    GraphNodeImpl node = (GraphNodeImpl) nodeVoid;

    node->closed = true;
    EdgeListItem current = node->edges->first;
    while (current != NULL) {
        GraphNodeImpl currentNode = current->edge->node;

        bool newNode = currentNode->closed == false && currentNode->parent == NULL;

        double newdist;
        if (byLength) {
            newdist = node->distance + current->edge->length;
        } else {
            newdist = node->distance + current->edge->length / current->edge->speed;
            if (current->edge->speed == 0) {
                newdist = INFINITY;
            }
        }

        if (isfinite(newdist) && (isinf(currentNode->distance) || currentNode->distance > newdist)) {
            currentNode->distance = newdist;
            currentNode->parent = node;
            if (currentNode->streetName == NULL)
                currentNode->streetName = malloc(64 * sizeof(char));
            strcpy(currentNode->streetName, current->edge->name);
        } else if (newNode) {
            newNode = false;
        }

        if (newNode) {
            BinHeap_Insert(heap, currentNode);
        }
        current = current->next;
    }
}

void GraphNode_DestroyEdgesAffected(GraphNode nodeVoid, Polygon polygon) {
    GraphNodeImpl node = (GraphNodeImpl) nodeVoid;

    double x1 = Point_GetX(node->point);
    double y1 = Point_GetY(node->point);

    bool nodeInside = Polygon_IsPointInside(polygon, x1, y1);

    EdgeListItem current = node->edges->first;
    while (current != NULL) {
        double x2 = Point_GetX(current->edge->node->point);
        double y2 = Point_GetY(current->edge->node->point);

        if (nodeInside || Polygon_IsPointInside(polygon, x2, y2) || 
                Polygon_DoesSegmentIntersect(polygon, x1, y1, x2, y2)) {
            current->edge->length = INFINITY;
            current->edge->speed = 0;
        }
        current = current->next;
    }
}

int GraphNode_CompareAsc(const void *a, const void *b) {
    return ((GraphNodeImpl) a)->distance - ((GraphNodeImpl) b)->distance;
}

int GraphNode_CompareDesc(const void *a, const void *b) {
    return ((GraphNodeImpl) b)->distance - ((GraphNodeImpl) a)->distance;
}

bool GraphNode_IsClosed(GraphNode node) {
    return ((GraphNodeImpl) node)->closed;
}

char *GraphNode_GetId(GraphNode node) {
    return ((GraphNodeImpl) node)->id;
}

GraphNode GraphNode_GetParent(GraphNode node) {
    return ((GraphNodeImpl) node)->parent;
}

Point GraphNode_GetPoint(GraphNode node) {
   return ((GraphNodeImpl) node)->point;
}

double GraphNode_GetX(GraphNode node) {
    return Point_GetX(((GraphNodeImpl) node)->point);
}

double GraphNode_GetY(GraphNode node) {
    return Point_GetY(((GraphNodeImpl) node)->point);
}

void GraphNode_SetParent(GraphNode node, GraphNode parent) {
    ((GraphNodeImpl) node)->parent = parent;
}

void GraphNode_SetDistance(GraphNode node, double distance) {
    ((GraphNodeImpl) node)->distance = distance;
}

void GraphNode_SetClosed(GraphNode node, bool closed) {
    ((GraphNodeImpl) node)->closed = closed;
}

double GraphNode_GetDistance(GraphNode node) {
    return ((GraphNodeImpl) node)->distance;
}

char *GraphNode_GetStreetName(GraphNode node) {
    return ((GraphNodeImpl) node)->streetName;
}
