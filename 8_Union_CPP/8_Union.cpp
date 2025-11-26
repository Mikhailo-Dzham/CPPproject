#include "8_Union.h"
#include <cmath>
#include <algorithm>
#include <iostream>

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

// ==========================
// ЗАДАЧА 2: Гроші
// ==========================
void print_money_CPP(const Money_CPP& m) {
    std::visit(overloaded {
        [](const MoneyFull& f) { std::cout << f.grn << " UAH " << f.kop << " kop" << std::endl; },
        [](int kop_only) {
            std::cout << (kop_only / 100) << " UAH " << (kop_only % 100) << " kop (from int)" << std::endl;
        }
    }, m);
}

// ==========================
// ЗАДАЧА 3: Вектори
// ==========================
// Helper: Отримати пару {dx, dy} з вектора
struct VectorComponentVisitor {
    struct Vec2 { double x, y; };

    // Якщо вектор заданий координатами
    Vec2 operator()(const VectorCoords& c) const { return {c.x, c.y}; }

    // Якщо вектор заданий точками (тут складно, бо точки теж Variant)
    Vec2 operator()(const VectorPoints& pts) const {
        // Нам треба викликати ToCartesianVisitor (з Задачі 1) для точок start і end
        // Оскільки ToCartesianVisitor визначений в цьому файлі (вище), ми його бачимо
        auto c1 = std::visit(ToCartesianVisitor{}, pts.start);
        auto c2 = std::visit(ToCartesianVisitor{}, pts.end);
        return {c2.x - c1.x, c2.y - c1.y};
    }
};

bool are_collinear_CPP(const Vector_CPP& v1, const Vector_CPP& v2) {
    auto vec1 = std::visit(VectorComponentVisitor{}, v1);
    auto vec2 = std::visit(VectorComponentVisitor{}, v2);

    // Cross product logic
    double det = vec1.x * vec2.y - vec2.x * vec1.y;
    return std::abs(det) < 1e-9;
}


// ==========================
// ЗАДАЧА 4: Точки 3D
// ==========================
struct ToCartesian3DVisitor {
    struct Coords3 { double x, y, z; };

    Coords3 operator()(const Cart3D& c) const { return {c.x, c.y, c.z}; }
    Coords3 operator()(const Polar3D& p) const {
        return {p.r * std::cos(p.angle), p.r * std::sin(p.angle), p.z};
    }
    Coords3 operator()(const Sphere3D& s) const {
        return {
            s.r * std::sin(s.theta) * std::cos(s.phi),
            s.r * std::sin(s.theta) * std::sin(s.phi),
            s.r * std::cos(s.theta)
        };
    }
};

double calculate_distance_3d_CPP(const Point3D_CPP& p1, const Point3D_CPP& p2) {
    auto c1 = std::visit(ToCartesian3DVisitor{}, p1);
    auto c2 = std::visit(ToCartesian3DVisitor{}, p2);
    return std::sqrt(std::pow(c2.x - c1.x, 2) +
                     std::pow(c2.y - c1.y, 2) +
                     std::pow(c2.z - c1.z, 2));
}

// ==========================
// ЗАДАЧА 6: Числа
// ==========================
AnyNumber_CPP divide_numbers_CPP(const AnyNumber_CPP& n1, const AnyNumber_CPP& n2) {
    // Helper lambda to extract double value
    auto get_val = [](const auto& val) -> double {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>) return static_cast<double>(val);
        else if constexpr (std::is_same_v<T, double>) return val;
        else return NAN; // Для Inf/Nan повертаємо NAN, щоб сигналізувати помилку
    };

    double v1 = std::visit(get_val, n1);
    double v2 = std::visit(get_val, n2);

    if (std::isnan(v1) || std::isnan(v2)) return NanType{};
    if (std::abs(v2) < 1e-9) return InfType{}; // Ділення на нуль

    return v1 / v2;
}