#include "svg.h"

void putSVGStart(FILE *file) {
    fputs("<svg " //width=\"30000\" height=\"30000\" 
          "xmlns=\"http://www.w3.org/2000/svg\" "
          "xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", file);
}

void putSVGQueryStart(FILE *file) {
    fputs("<g id=\"query\">\n", file);
}

void putSVGQueryEnd(FILE *file) {
    fputs("</g>\n", file);
}

void putSVGUseQuery(FILE *file) {
    fputs("<use xlink:href=\"#query\"/>\n", file);
}

void putSVGCircle(FILE *file, Circle circ, char color1[], char color2[], char stroke[]) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"%.2lf\"/>\n",
            Circle_GetX(circ),
            Circle_GetY(circ),
            Circle_GetRadius(circ),
            color1,
            stroke,
            color2,
            POLY_OPACITY);
}

void putSVGRectangle(FILE *file, Rectangle rect, char color1[], char color2[], char stroke[]) {
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"%.2lf\"/>\n",
            Rectangle_GetX(rect),
            Rectangle_GetY(rect),
            Rectangle_GetWidth(rect),
            Rectangle_GetHeight(rect),
            color1,
            stroke,
            color2,
            POLY_OPACITY);
}

void putSVGEllipse(FILE *file, double cx, double cy, double rx, double ry, char color1[], char color2[]) {
    fprintf(file, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;stroke-width:%s\"/>\n",
            cx,
            cy,
            rx,
            ry,
            color2,
            color1,
            STROKE_WIDTH);
}

void putSVGText(FILE *file, double x, double y, char text[]) {
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" %s>%s</text>\n",
            x, y, TEXT_FILL_COLOR, TEXT_EXTRA, text);
}

void putSVGBox(FILE *file, double x, double y, double width, double height, bool gaps) {
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"black\" fill=\"none\" stroke-width=\"%s\" stroke-dasharray=\"%s\"/>\n",
            x,
            y,
            width,
            height,
            STROKE_WIDTH,
            gaps ? "5" : "0");
}

void putSVGPoint(FILE *file, double x, double y, bool inside) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\"/>\n",
            x,
            y,
            POINT_RADIUS,
            inside ? "green" : "red");
}

void putSVGLine(FILE *file, double x1, double y1, double x2, double y2) {
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(0,0,0);stroke-width:%s\"/>\n",
            x1,
            y1,
            x2,
            y2,
            STROKE_WIDTH);
}

void putSVGBlock(Block b, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Block_GetX(b),
            Block_GetY(b),
            Block_GetW(b),
            Block_GetH(b),
            Block_GetCStroke(b),
            Block_GetWStroke(b),
            Block_GetCFill(b));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"8\" font-family=\"Arial\" style=\"stroke:#000;stroke-width:0.2px;\">%s</text>\n",
            Block_GetX(b) + 5,
            Block_GetY(b) + 13,
            TEXT_FILL_COLOR,
            Block_GetCep(b));
}

void putSVGHightlight(FILE *file, double x, double y) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"0.5\"/>\n",
            x,
            y,
            HYDRANT_RADIUS + 2,
            HIGHLIGHT_CSTROKE,
            HIGHLIGHT_WSTROKE,
            HIGHLIGHT_CFILL);
}

void putSVGHydrant(Equip h, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if (Equip_GetHighlighted(h))
        putSVGHightlight(file, Equip_GetX(h), Equip_GetY(h));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(h),
            Equip_GetY(h),
            HYDRANT_RADIUS,
            Equip_GetCStroke(h),
            Equip_GetWStroke(h),
            Equip_GetCFill(h));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\" dominant-baseline=\"middle\" text-anchor=\"middle\">H</text>\n",
            Equip_GetX(h),
            Equip_GetY(h) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR);
}

void putSVGTrafficLight(Equip t, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if (Equip_GetHighlighted(t))
        putSVGHightlight(file, Equip_GetX(t), Equip_GetY(t));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(t),
            Equip_GetY(t),
            HYDRANT_RADIUS,
            Equip_GetCStroke(t),
            Equip_GetWStroke(t),
            Equip_GetCFill(t));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\" %s>S</text>\n",
            Equip_GetX(t),
            Equip_GetY(t) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR,
            TEXT_CENTER);
}

void putSVGCellTower(Equip c, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if (Equip_GetHighlighted(c))
        putSVGHightlight(file, Equip_GetX(c), Equip_GetY(c));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(c),
            Equip_GetY(c),
            HYDRANT_RADIUS,
            Equip_GetCStroke(c),
            Equip_GetWStroke(c),
            Equip_GetCFill(c));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\" %s>T</text>\n",
            Equip_GetX(c),
            Equip_GetY(c) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR,
            TEXT_CENTER);
}

void putSVGBuilding(Building b, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Building_GetX(b),
            Building_GetY(b),
            Building_GetW(b),
            Building_GetH(b),
            "black",
            "1",
            Building_IsPainted(b) ? "gold" : "blue");
    if (Building_IsHighlighted(b)) {
        fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
                Building_GetX(b),
                Building_GetY(b),
                Building_GetW(b),
                Building_GetH(b),
                "deeppink",
                "4",
                "none");
        }

    double textX = Building_GetX(b) + Building_GetW(b) / 2;
    double textY = Building_GetY(b) + Building_GetH(b) / 2 + 2;
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" font-size=\"10\" fill=\"%s\" %s %s>%d</text>\n",
            textX, textY, TEXT_FILL_COLOR, TEXT_CENTER, TEXT_EXTRA, Building_GetNum(b));
}

void putSVGWall(Wall w, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(50,50,50);stroke-width:%s\"/>\n",
            Wall_GetX1(w),
            Wall_GetY1(w),
            Wall_GetX2(w),
            Wall_GetY2(w),
            WALL_STROKE_WIDTH);
}

void putSVGSegment(FILE *file, double x1, double y1, double x2, double y2) {
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(200,200,0);stroke-width:%s\"/>\n",
            x1,
            y1,
            x2,
            y2,
            "4");
    
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"4\" fill=\"green\"/>\n",
        x1,
        y1);

    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"3\" fill=\"red\"/>\n",
        x2,
        y2);
}

void putSVGTriangle(FILE *file, double x1, double y1, double x2, double y2, double x3, double y3) {
    fprintf(file, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" style=\"fill: orange; opacity: 0.5\"/>\n",
            x1, y1,
            x2, y2,
            x3, y3);
}

void putSVGBomb(FILE *file, double x, double y) {
    fprintf(file, "<path stroke=\"orange\" stroke-width=\"5\" transform=\"translate(%lf %lf) scale(0.03 0.03)\" d=\"M440.5 88.5l-52 52L415 167c9.4 9.4 9.4 24.6 0 33.9l-17.4 17.4c11.8 26.1 18.4 55.1 "
        "18.4 85.6 0 114.9-93.1 208-208 208S0 418.9 0 304 93.1 96 208 96c30.5 0 59.5 6.6 85.6 18.4L311 97c9.4-9.4 "
        "24.6-9.4 33.9 0l26.5 26.5 52-52 17.1 17zM500 60h-24c-6.6 0-12 5.4-12 12s5.4 12 12 12h24c6.6 0 12-5.4 "
        "12-12s-5.4-12-12-12zM440 0c-6.6 0-12 5.4-12 12v24c0 6.6 5.4 12 12 12s12-5.4 12-12V12c0-6.6-5.4-12-12-12zm33.9 "
        "55l17-17c4.7-4.7 4.7-12.3 0-17-4.7-4.7-12.3-4.7-17 0l-17 17c-4.7 4.7-4.7 12.3 0 17 4.8 4.7 12.4 4.7 17 0zm-67.8 "
        "0c4.7 4.7 12.3 4.7 17 0 4.7-4.7 4.7-12.3 0-17l-17-17c-4.7-4.7-12.3-4.7-17 0-4.7 4.7-4.7 12.3 0 17l17 17zm67.8 "
        "34c-4.7-4.7-12.3-4.7-17 0-4.7 4.7-4.7 12.3 0 17l17 17c4.7 4.7 12.3 4.7 17 0 4.7-4.7 4.7-12.3 0-17l-17-17zM112 "
        "272c0-35.3 28.7-64 64-64 8.8 0 16-7.2 16-16s-7.2-16-16-16c-52.9 0-96 43.1-96 96 0 8.8 7.2 16 16 16s16-7.2 16-16z\"/>\n",
        x - 6.5, y - 8.5);
}

void putSVGNode(FILE *file, double x, int y, bool red, char *description) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%d\" r=\"%d\" fill=\"%s\"/>\n", 
            x, y, TREE_NODE_RADIUS, red ? "red" : "black");
    char part1[64], part2[64];
    int n;
    sscanf(description, "%63[^\n]%n", part1, &n);
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"white\" font-family=\"Arial\" font-size=\"%d\" dominant-baseline=\"middle\" "
                  "text-anchor=\"middle\" style=\"stroke:#000;stroke-width:0.25\">%s</text>\n", 
                  x, y - TREE_TEXT_SIZE / 2.0, TREE_TEXT_SIZE, part1);
        
    if (strlen(description) > n + 1) {
        sscanf(description + n + 1, "%63[^\n]", part2);
        fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"white\" font-family=\"Arial\" font-size=\"%d\" dominant-baseline=\"middle\" "
                       "text-anchor=\"middle\" style=\"stroke:#000;stroke-width:0.25\">%s</text>\n", 
                       x, y + TREE_TEXT_SIZE / 2.0, TREE_TEXT_SIZE, part2);
    }
}

static double _generateTreeSVG(RBTree tree, FILE *file, Node node, int height, double xMin, double *x, void (*describe)(Value, char*)) {
    if (node == NULL) {
        *x = xMin;
        putSVGNode(file, *x, TREE_TOP_MARGIN + height * TREE_Y_SPACING, false, "nil");
        return xMin + TREE_X_SPACING * 2;
    }
    double xLeft;
    double xMaxLeft = _generateTreeSVG(tree, file, RBTreeN_GetLeftChild(tree, node), height + 1, xMin, &xLeft, describe);

    double xRight;
    double xMaxRight = _generateTreeSVG(tree, file, RBTreeN_GetRightChild(tree, node), height + 1, xMaxLeft, &xRight, describe);

     *x = (xLeft + xRight) / 2;
     int y = TREE_TOP_MARGIN + height * TREE_Y_SPACING;
     int yChild = y + TREE_Y_SPACING;

    putSVGLine(file, *x, y, xLeft, yChild - TREE_NODE_RADIUS);
    putSVGLine(file, *x, y, xRight, yChild - TREE_NODE_RADIUS);

    char description[64];
    describe(RBTreeN_GetValue(tree, node), description);

    putSVGNode(file, *x, y, RBTreeN_GetColor(node) == RED, description);
    
    return xMaxRight + TREE_X_SPACING;
}


void putSVGRBTree(FILE *file, RBTree tree, void (*describe)(Value, char*)) {
    putSVGStart(file);
    double x;
    _generateTreeSVG(tree, file, RBTree_GetRoot(tree), 0, TREE_LEFT_MARGIN, &x, describe);
    putSVGEnd(file);
}

void putSVGPolygon(FILE *file, Polygon polygon, char color[]) {
    fprintf(file, "<polygon points=\"");
    for (void *seg = Polygon_GetFirstSeg(polygon); seg != NULL; seg = PolySeg_GetNext(seg)) {
        fprintf(file, "%lf,%lf %lf,%lf ", PolySeg_GetX1(seg), PolySeg_GetY1(seg),
                                          PolySeg_GetX2(seg), PolySeg_GetY2(seg));
    }
    fprintf(file, "\" style=\"fill: %s; fill-opacity: 0.5\" stroke-width=\"0.3\" stroke=\"black\"/>\n", color);
}

void putSVGCross(FILE *file, Building building) {
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(255,0,0);stroke-width:%s\"/>\n",
            Building_GetX(building),
            Building_GetY(building),
            Building_GetX(building) + Building_GetW(building),
            Building_GetY(building) + Building_GetH(building),
            STROKE_WIDTH);
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(255,0,0);stroke-width:%s\"/>\n",
            Building_GetX(building) + Building_GetW(building),
            Building_GetY(building),
            Building_GetX(building),
            Building_GetY(building) + Building_GetH(building),
            STROKE_WIDTH);
}

void putSVGPath(FILE *file, double x1, double y1, double x2, double y2, char color[], int thickness) {
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:%s;stroke-width:%d\"/>\n",
            x1,
            y1,
            x2,
            y2,
            color,
            thickness);
}

void putSVGEnd(FILE *file) {
    fputs("</svg>\n", file);
}