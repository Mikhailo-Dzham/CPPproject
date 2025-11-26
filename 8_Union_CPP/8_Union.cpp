#include "8_Union.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==========================
// ЗАДАЧА 1: Точки (через std::visit)
// ==========================

// Допоміжна структура (Visitor), яка знає, як перетворити будь-що в пари (x, y)
struct ToCartesianVisitor {
    struct Coords { double x, y; };

    Coords operator()(const Cart2D& c) const { return {c.x, c.y}; }
    Coords operator()(const Polar2D& p) const {
        return {p.r * std::cos(p.angle), p.r * std::sin(p.angle)};
    }
};

double calculate_segment_length_CPP(const Point2D_CPP& p1, const Point2D_CPP& p2) {
    // std::visit автоматично вибирає потрібний метод із ToCartesianVisitor
    auto c1 = std::visit(ToCartesianVisitor{}, p1);
    auto c2 = std::visit(ToCartesianVisitor{}, p2);

    return std::sqrt(std::pow(c2.x - c1.x, 2) + std::pow(c2.y - c1.y, 2));
}

// ==========================
// ЗАДАЧА 5: Фігури (через std::visit + lambda)
// ==========================

// Тут покажемо інший спосіб: std::visit з "overloaded" лямбдами
// Це дуже потужний інструмент C++17
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

double calculate_area_CPP(const Shape_CPP& s) {
    return std::visit(overloaded {
        [](const Circle& c) { return M_PI * c.r * c.r; },
        [](const Square& sq) { return sq.side * sq.side; },
        [](const Rect& r) { return r.a * r.b; },
        [](const Trapezoid& t) { return ((t.a + t.b) / 2.0) * t.h; },
        [](const Triangle& t) {
            double p = (t.a + t.b + t.c) / 2.0;
            return std::sqrt(p * (p - t.a) * (p - t.b) * (p - t.c));
        }
    }, s);
}

double calculate_perimeter_CPP(const Shape_CPP& s) {
    return std::visit(overloaded {
        [](const Circle& c) { return 2 * M_PI * c.r; },
        [](const Square& sq) { return 4 * sq.side; },
        [](const Rect& r) { return 2 * (r.a + r.b); },
        [](const Triangle& t) { return t.a + t.b + t.c; },
        [](const Trapezoid& t) {
             double cathet = std::abs(t.b - t.a) / 2.0;
             double side = std::sqrt(t.h*t.h + cathet*cathet);
             return t.a + t.b + 2 * side;
        }
    }, s);
}