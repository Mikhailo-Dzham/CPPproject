#include "8_Union.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Допоміжна функція (не видно ззовні) ---
// Перетворює будь-яку точку в структуру {x, y}
static void to_cartesian(Point2D p, double *x, double *y) {
    if (p.type == COORD_CARTESIAN) {
        *x = p.data.cart.x;
        *y = p.data.cart.y;
    } else {
        // Переведення з полярної в декартову
        *x = p.data.polar.r * cos(p.data.polar.angle);
        *y = p.data.polar.r * sin(p.data.polar.angle);
    }
}

// ==========================
// ЗАДАЧА 1: Точки
// ==========================
double calculate_segment_length_C(Point2D p1, Point2D p2) {
    double x1, y1, x2, y2;
    to_cartesian(p1, &x1, &y1);
    to_cartesian(p2, &x2, &y2);
    
    // Формула відстані: sqrt((x2-x1)^2 + (y2-y1)^2)
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// ==========================
// ЗАДАЧА 5: Фігури
// ==========================
double calculate_perimeter_C(Shape s) {
    switch (s.type) {
        case SHAPE_CIRCLE:
            return 2 * M_PI * s.data.circle_radius;
        case SHAPE_SQUARE:
            return 4 * s.data.square_side;
        case SHAPE_TRIANGLE:
            return s.data.triangle_sides.a + s.data.triangle_sides.b + s.data.triangle_sides.c;
        case SHAPE_RECT:
            return 2 * (s.data.rect_sides.a + s.data.rect_sides.b);
        case SHAPE_TRAPEZOID:
            // Для периметра трапеції треба знати 4 сторони, 
            // але в умові часто дають основи і висоту. 
            // Якщо маємо тільки a, b, h, то бічні сторони вважаємо рівнобічними (спрощення)
            // c^2 = h^2 + ((b-a)/2)^2
            {
                double a = s.data.trapezoid_dims.a;
                double b = s.data.trapezoid_dims.b;
                double h = s.data.trapezoid_dims.h;
                double cathet = fabs(b - a) / 2.0;
                double side = sqrt(h*h + cathet*cathet);
                return a + b + 2 * side;
            }
        default:
            return 0.0;
    }
}

double calculate_area_C(Shape s) {
    switch (s.type) {
        case SHAPE_CIRCLE:
            return M_PI * pow(s.data.circle_radius, 2);
        case SHAPE_SQUARE:
            return pow(s.data.square_side, 2);
        case SHAPE_TRIANGLE: 
            {
                // Формула Герона
                double a = s.data.triangle_sides.a;
                double b = s.data.triangle_sides.b;
                double c = s.data.triangle_sides.c;
                double p = (a + b + c) / 2.0;
                return sqrt(p * (p - a) * (p - b) * (p - c));
            }
        case SHAPE_RECT:
            return s.data.rect_sides.a * s.data.rect_sides.b;
        case SHAPE_TRAPEZOID:
            return ((s.data.trapezoid_dims.a + s.data.trapezoid_dims.b) / 2.0) * s.data.trapezoid_dims.h;
        default:
            return 0.0;
    }
}