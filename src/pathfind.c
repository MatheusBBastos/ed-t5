#include "pathfind.h"

#define direction(code) code == 'N' ? "Norte" : code == 'S' ? "Sul"\
                        : code == 'L' ? "Leste" : "Oeste"

typedef struct stack_item_t {
    GraphNode node;
    struct stack_item_t *next;
    struct stack_item_t *base;
} *StackItem;

static GraphNode _findClosestNode(RBTree tree, Node node, double x, double y, double *minDist) {
    if (node == NULL)
        return NULL;
    
    GraphNode currentNode = RBTreeN_GetValue(tree, node);
    double dist = euclideanDistance(x, y, GraphNode_GetX(currentNode), GraphNode_GetY(currentNode));
    bool distSet = false;
    if (dist < *minDist) {
        *minDist = dist;
        distSet = true;
    }

    GraphNode closestLeft = _findClosestNode(tree, RBTreeN_GetLeftChild(tree, node), x, y, minDist);
    GraphNode closestRight = _findClosestNode(tree, RBTreeN_GetRightChild(tree, node), x, y, minDist);

    if (closestRight != NULL)
        return closestRight;
    if (closestLeft != NULL)
        return closestLeft;
    if (distSet)
        return currentNode;
    return NULL;
}

static void _resetNode(GraphNode node, void *v) {
    GraphNode_SetDistance(node, INFINITY);
    GraphNode_SetParent(node, NULL);
    GraphNode_SetClosed(node, false);
}

static bool _dijkstra(GraphNode start, GraphNode end, bool quickest) {
    RBTree_Execute(getNodeTree(), _resetNode, NULL);
    GraphNode_SetDistance(start, 0);

    BinaryHeap heap = BinHeap_Create(RBTree_GetLength(getNodeTree()), GraphNode_CompareAsc);
    BinHeap_Insert(heap, start);

    while (!BinHeap_IsEmpty(heap)) {
        GraphNode currentNode = BinHeap_Extract(heap);
        GraphNode_CollectNeighbors(currentNode, heap, !quickest);
    }

    BinHeap_Destroy(heap);

    return GraphNode_IsClosed(end);
}

static StackItem backtrace(GraphNode start, GraphNode end, FILE *svgFile, char color[], bool quickest, bool noWrite) {
    GraphNode currentNode = end;
    StackItem stackTop = NULL;
    StackItem stackBase = NULL;
    while (currentNode != start) {
        GraphNode parent = GraphNode_GetParent(currentNode);

        if (!noWrite)
            putSVGPath(svgFile, GraphNode_GetX(parent), GraphNode_GetY(parent),
                                GraphNode_GetX(currentNode), GraphNode_GetY(currentNode),
                                color, quickest ? 3 : 5);

        StackItem newItem = malloc(sizeof(struct stack_item_t));
        if (stackBase == NULL)
            stackBase = newItem;
        newItem->next = stackTop;
        newItem->node = currentNode;
        newItem->base = stackBase;
        stackTop = newItem;

        currentNode = parent;
    }

    StackItem newItem = malloc(sizeof(struct stack_item_t));
    newItem->next = stackTop;
    newItem->node = start;
    newItem->base = stackBase == NULL ? newItem : stackBase;
    stackTop = newItem;

    return stackTop;
}

static void putPathText(StackItem stackTop, FILE *txtFile, bool quickest, bool freeStack) {
    fprintf(txtFile, "CAMINHO MAIS %s:\n", quickest ? "RÁPIDO" : "CURTO");

    GraphNode currentNode;

    char lastDir = '.';
    double currentDistance = 0;
    while (stackTop->next != NULL) {
        currentNode = stackTop->node;
        GraphNode nextNode = stackTop->next->node;
        char newDir;
        double dx = GraphNode_GetX(nextNode) - GraphNode_GetX(currentNode);
        double dy = GraphNode_GetY(nextNode) - GraphNode_GetY(currentNode);
        double dist;
        if (dx > 0) {
            newDir = 'L';
            dist = dx;
        } else if (dx < 0) {
            newDir = 'O';
            dist = -dx;
        } else if (dy > 0) {
            newDir = 'N';
            dist = dy;
        } else {
            newDir = 'S';
            dist = -dy;
        }

        if (lastDir == '.') {
            currentDistance = dist;
            fprintf(txtFile, "Siga a %s na rua %s",
                direction(newDir),
                GraphNode_GetStreetName(nextNode));
        } else if (newDir != lastDir) {
            fprintf(txtFile, " por %.0lf metros", currentDistance);
            currentDistance = 0;
            if (lastDir == 'L' && newDir == 'O' || lastDir == 'O' && newDir == 'L'
                    || lastDir == 'N' && newDir == 'S' || lastDir == 'S' && newDir == 'N') {
                fprintf(txtFile, " e faça o retorno. \nSiga a %s na rua %s", 
                    direction(newDir),
                    GraphNode_GetStreetName(nextNode));
            } else {
                fprintf(txtFile, " até o cruzamento com a rua %s. \nVire na direção %s e siga",
                    GraphNode_GetStreetName(nextNode),
                    direction(newDir));
            }
        } else {
            currentDistance += dist;
        }
        lastDir = newDir;
        StackItem nextItem = stackTop->next;
        if (freeStack)
            free(stackTop);
        stackTop = nextItem;
    }

    fprintf(txtFile, " por %.0lf metros.\nVocê chegou ao seu destino.\n\n", currentDistance);

    if (freeStack)
        free(stackTop);
}

PathStack fullPathFind(GraphNode start, GraphNode end, FILE *svgFile, FILE *txtFile, char color[], bool quickest, bool freeStack) {
    if (_dijkstra(start, end, quickest)) {
        StackItem stackTop = backtrace(start, end, svgFile, color, quickest, false);
        putPathText(stackTop, txtFile, quickest, freeStack);
        return stackTop;
    } else {
        fprintf(txtFile, "Caminho não encontrado\n\n");
    }
    return NULL;
}

GraphNode findClosestNodeToPoint(Point point) {
    double x = Point_GetX(point), y = Point_GetY(point);
    double minDist = INFINITY;
    return _findClosestNode(getNodeTree(), RBTree_GetRoot(getNodeTree()), x, y, &minDist);
}

PathStack findPathStack(GraphNode start, GraphNode end, bool quickest) {
    if (_dijkstra(start, end, quickest)) {
        return backtrace(start, end, NULL, NULL, quickest, true);
    }

    return NULL;
}

GraphNode peekPathStack(PathStack stackTop) {
    return ((StackItem) stackTop)->node;
}

PathStack popPathStack(PathStack stackTopVoid) {
    StackItem stackTop = (StackItem) stackTopVoid;
    StackItem next = stackTop->next;
    free(stackTop);
    return next;
}

GraphNode getPathStackBase(PathStack stackTop) {
    return ((StackItem) stackTop)->base->node;
}

void destroyPathStack(PathStack stackTopVoid) {
    StackItem stackTop = (StackItem) stackTopVoid;
    while (stackTop != NULL) {
        StackItem next = stackTop->next;
        free(stackTop);
        stackTop = next;
    }
}

PathStack findPath(Point pointA, Point pointB, FILE *svgFile, FILE *txtFile, 
                   char color1[], char color2[], PathFindMode mode) {
    double x1 = Point_GetX(pointA), y1 = Point_GetY(pointA);
    double x2 = Point_GetX(pointB), y2 = Point_GetY(pointB);
    double minDist = INFINITY;
    GraphNode start = _findClosestNode(getNodeTree(), RBTree_GetRoot(getNodeTree()), x1, y1, &minDist);
    minDist = INFINITY;
    GraphNode end = _findClosestNode(getNodeTree(), RBTree_GetRoot(getNodeTree()), x2, y2, &minDist);

    PathStack shortestPathStack = fullPathFind(start, end, svgFile, txtFile, color1, false, mode != SHORTEST);
    if (shortestPathStack != NULL) {
        putSVGPath(svgFile, x1, y1, GraphNode_GetX(start), GraphNode_GetY(start), color1, 5);
        putSVGPath(svgFile, x2, y2, GraphNode_GetX(end), GraphNode_GetY(end), color1, 5);
    }

    PathStack quickestPathStack = fullPathFind(start, end, svgFile, txtFile, color2, true, mode != QUICKEST);
    if (quickestPathStack != NULL) {
        putSVGPath(svgFile, x1, y1, GraphNode_GetX(start), GraphNode_GetY(start), color2, 3);
        putSVGPath(svgFile, x2, y2, GraphNode_GetX(end), GraphNode_GetY(end), color2, 3);
    }

    if (mode == SHORTEST)
        return shortestPathStack;
    else if (mode == QUICKEST)
        return quickestPathStack;
    else
        return NULL;
}