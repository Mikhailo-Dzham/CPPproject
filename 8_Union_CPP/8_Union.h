#ifndef UNION_CPP_H
#define UNION_CPP_H

#include <variant>
#include <iostream>
#include <vector>
#include <string>

// --- ЗАДАЧА 1: Точки ---
struct Cart2D { double x, y; };
struct Polar2D { double r, angle; };

// Point2D може бути АБО Cart2D, АБО Polar2D
using Point2D_CPP = std::variant<Cart2D, Polar2D>;

// Прототип функції
double calculate_segment_length_CPP(const Point2D_CPP& p1, const Point2D_CPP& p2);


// --- ЗАДАЧА 5: Фігури ---
struct Circle { double r; };
struct Square { double side; };
struct Triangle { double a, b, c; };
struct Rect { double a, b; };
struct Trapezoid { double a, b, h; };

using Shape_CPP = std::variant<Circle, Square, Triangle, Rect, Trapezoid>;

// Прототипи функцій
double calculate_perimeter_CPP(const Shape_CPP& s);
double calculate_area_CPP(const Shape_CPP& s);

#endif // UNION_CPP_H