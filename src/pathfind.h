#ifndef PATHFIND_H
#define PATHFIND_H

#include "modules/aux/point.h"
#include "modules/util/svg.h"
#include "data.h"

typedef enum PathFindMode {
    ALL, QUICKEST, SHORTEST
} PathFindMode;

typedef void *PathStack;

PathStack findPathStack(GraphNode start, GraphNode end, bool fastest);

GraphNode peekPathStack(PathStack stackTop);

PathStack popPathStack(PathStack stackTop);

GraphNode getPathStackBase(PathStack stackTop);

void destroyPathStack(PathStack stackTop);

PathStack findPath(Point pointA, Point pointB, FILE *svgFile, FILE *txtFile, 
                   char color1[], char color2[], PathFindMode mode);

#endif