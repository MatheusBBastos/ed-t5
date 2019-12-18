#include "geometry.h"

double euclideanDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double manhattanDistance(double x1, double y1, double x2, double y2) {
    return fabs(x1 - x2) + fabs(y1 - y2);
}

double clamp(double value, double a, double b) {
	double clamped = value > b ? b : value;
	clamped = value < a ? a : clamped;
	return clamped;
}

bool checkOverlap(Object a, Object b) {
    if (Object_GetType(a) == OBJ_CIRC && Object_GetType(b) == OBJ_CIRC) {
        Circle c1 = (Circle) Object_GetContent(a);
        Circle c2 = (Circle) Object_GetContent(b);
        double dist = euclideanDistance(Circle_GetX(c1), Circle_GetY(c1), Circle_GetX(c2), Circle_GetY(c2));
        return (dist < Circle_GetRadius(c1) + Circle_GetRadius(c2));
    } else if (Object_GetType(a) == OBJ_RECT && Object_GetType(b) == OBJ_RECT) {
        Rectangle r1 = (Rectangle) Object_GetContent(a);
        Rectangle r2 = (Rectangle) Object_GetContent(b);
        return Rectangle_GetX(r1) < Rectangle_GetX(r2) + Rectangle_GetWidth(r2) &&
               Rectangle_GetX(r1) + Rectangle_GetWidth(r1) > Rectangle_GetX(r2) &&
               Rectangle_GetY(r1) < Rectangle_GetY(r2) + Rectangle_GetHeight(r2) &&
               Rectangle_GetY(r1) + Rectangle_GetHeight(r1) > Rectangle_GetY(r2);
    } else {
        // Deixar o a como círculo e b como retângulo
        if (Object_GetType(a) == OBJ_RECT) {
            Object temp = a;
            a = b;
            b = temp;
        }
        Circle circ = (Circle) Object_GetContent(a);
        Rectangle rect = (Rectangle) Object_GetContent(b);

        double nearestX = clamp(Circle_GetX(circ), Rectangle_GetX(rect), Rectangle_GetX(rect) + Rectangle_GetWidth(rect));
        double nearestY = clamp(Circle_GetY(circ), Rectangle_GetY(rect), Rectangle_GetY(rect) + Rectangle_GetHeight(rect));

        double dist = euclideanDistance(nearestX, nearestY, Circle_GetX(circ), Circle_GetY(circ));

        return dist < Circle_GetRadius(circ);
    }
}

bool checkInside(Object obj, double x, double y) {
    if (Object_GetType(obj) == OBJ_CIRC) {
        Circle circ = (Circle) Object_GetContent(obj);
        return euclideanDistance(x, y, Circle_GetX(circ), Circle_GetY(circ)) < Circle_GetRadius(circ);
    } else {
        Rectangle rect = (Rectangle) Object_GetContent(obj);
        return x > Rectangle_GetX(rect) && x < Rectangle_GetX(rect) + Rectangle_GetWidth(rect) && 
            y > Rectangle_GetY(rect) && y < Rectangle_GetY(rect) + Rectangle_GetHeight(rect);
    }
}

void getCenter(Object obj, double *x, double *y) {
    if (Object_GetType(obj) == OBJ_CIRC) {
        Circle circ = (Circle) Object_GetContent(obj);
        *x = Circle_GetX(circ);
        *y = Circle_GetY(circ);
    } else {
        Rectangle rect = (Rectangle) Object_GetContent(obj);
        *x = Rectangle_GetX(rect) + Rectangle_GetWidth(rect)/2;
        *y = Rectangle_GetY(rect) + Rectangle_GetHeight(rect)/2;
    }
}

void getSurroundingRect(Object obj, Rectangle resultRect) {
    if (Object_GetType(obj) == OBJ_CIRC) {
        Circle circ = (Circle) Object_GetContent(obj);
        Rectangle_SetX(resultRect, Circle_GetX(circ) - Circle_GetRadius(circ));
        Rectangle_SetY(resultRect, Circle_GetY(circ) - Circle_GetRadius(circ));
        Rectangle_SetWidth(resultRect, Circle_GetRadius(circ) * 2);
        Rectangle_SetHeight(resultRect, Circle_GetRadius(circ) * 2);
    } else {
        Rectangle rect = (Rectangle) Object_GetContent(obj);
        Rectangle_SetX(resultRect, Rectangle_GetX(rect));
        Rectangle_SetY(resultRect, Rectangle_GetY(rect));
        Rectangle_SetWidth(resultRect, Rectangle_GetWidth(rect));
        Rectangle_SetHeight(resultRect, Rectangle_GetHeight(rect));
    }
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

double calculateAngle(double y, double x) {
    return atan2(y, x);
}

bool nearlyEqual(double a, double b) {
    double absA = fabs(a);
    double absB = fabs(b);
    double diff = fabs(a - b);

    if (a == b) {
        return true;
    } else if (a == 0 || b == 0 || (absA + absB < DBL_MIN)) {
        return diff < (DBL_EPSILON * DBL_MIN);
    } else {
        return diff / min((absA + absB), DBL_MAX) < DBL_EPSILON;
    }
}
