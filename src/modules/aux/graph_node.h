#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <stdlib.h>
#include "../data_structures/binary_heap.h"
#include "polygon.h"

typedef void *GraphNode;

GraphNode GraphNode_Create(char id[], double x, double y);

void GraphNode_Destroy(GraphNode node);

void GraphNode_InsertEdge(GraphNode nodeVoid, GraphNode other, Block leftBlock, Block rightBlock, 
                          double length, double speed, char name[]);

void GraphNode_CollectNeighbors(GraphNode nodeVoid, BinaryHeap heap, bool byLength);

GraphNode GraphNode_GoTo(GraphNode nodeVoid, char direction[], char streetName[]);

void GraphNode_DestroyEdgesAffected(GraphNode nodeVoid, Polygon polygon);

int GraphNode_CompareAsc(const void *a, const void *b);

int GraphNode_CompareDesc(const void *a, const void *b);

bool GraphNode_IsClosed(GraphNode node);

char *GraphNode_GetId(GraphNode node);

GraphNode GraphNode_GetParent(GraphNode node);

Point GraphNode_GetPoint(GraphNode node);

double GraphNode_GetX(GraphNode node);

double GraphNode_GetY(GraphNode node);

void GraphNode_SetParent(GraphNode node, GraphNode parent);

void GraphNode_SetDistance(GraphNode node, double distance);

void GraphNode_SetClosed(GraphNode node, bool closed);

double GraphNode_GetDistance(GraphNode node);

char *GraphNode_GetStreetName(GraphNode node);

#endif