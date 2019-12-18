#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdbool.h>
#include "../aux/polygon.h"
#include "../data_structures/redblack_tree.h"
#include "../sig/block.h"
#include "../sig/building.h"
#include "../sig/circle.h"
#include "../sig/equipment.h"
#include "../sig/rectangle.h"
#include "../sig/wall.h"

#define STROKE_WIDTH "1"
#define WALL_STROKE_WIDTH "3"
#define TEXT_FILL_COLOR "white"
#define TEXT_EXTRA "font-family=\"Arial\" style=\"stroke:#000;stroke-width:0.2px;\""
#define POINT_RADIUS 2.0
#define HYDRANT_RADIUS 5.0
#define POLY_OPACITY 0.6
#define HIGHLIGHT_CSTROKE "gold"
#define HIGHLIGHT_WSTROKE "2"
#define HIGHLIGHT_CFILL "yellow"
#define TEXT_CENTER "dominant-baseline=\"middle\" text-anchor=\"middle\""

#define TREE_TOP_MARGIN 10
#define TREE_LEFT_MARGIN 6
#define TREE_X_SPACING 15
#define TREE_Y_SPACING 35
#define TREE_NODE_RADIUS 12
#define TREE_TEXT_SIZE 8

void putSVGStart(FILE *file);

void putSVGQueryStart(FILE *file);

void putSVGQueryEnd(FILE *file);

void putSVGUseQuery(FILE *file);

void putSVGCircle(FILE *file, Circle circ, char color1[], char color2[], char stroke[]);

void putSVGRectangle(FILE *file, Rectangle rect, char color1[], char color2[], char stroke[]);

void putSVGEllipse(FILE *file, double cx, double cy, double rx, double ry, char color1[], char color2[]);

void putSVGText(FILE *file, double x, double y, char text[]);

void putSVGPoint(FILE *file, double x, double y, bool inside);

void putSVGBox(FILE *file, double x, double y, double width, double height, bool gaps);

void putSVGLine(FILE *file, double x1, double y1, double x2, double y2);

void putSVGBlock(Block b, void *file);

void putSVGHydrant(Equip h, void *file);

void putSVGTrafficLight(Equip t, void *file);

void putSVGCellTower(Equip c, void *file);

void putSVGBuilding(Building b, void *file);

void putSVGWall(Wall w, void *file);

void putSVGSegment(FILE *file, double x1, double y1, double x2, double y2);

void putSVGTriangle(FILE *file, double x1, double y1, double x2, double y2, double x3, double y3);

void putSVGBomb(FILE *file, double x, double y);

void putSVGNode(FILE *file, double x, int y, bool red, char *description);

void putSVGRBTree(FILE *file, RBTree tree, void (*describe)(Value, char*));

void putSVGPolygon(FILE *file, Polygon polygon, char color[]);

void putSVGCross(FILE *file, Building building);

void putSVGPath(FILE *file, double x1, double y1, double x2, double y2, char color[], int thickness);

void putSVGEnd(FILE *file);

#endif