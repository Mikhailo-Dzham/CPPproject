#include <iostream>
#include <cassert>
#include <cmath>
#include "../8_Union_C/8_Union.h"  // C-headers
#include "8_Union.h"               // C++ headers

// Функція для порівняння double
bool is_equal(double a, double b) {
    return std::abs(a - b) < 1e-6;
}

void test_task_1_points() {
    std::cout << "[TEST] Task 1: Points... ";

    // 1. Дані
    double r = 10.0, ang = 0.785; // 45 градусів
    double x = 5.0, y = 5.0;

    // 2. Створення C об'єктів
    Point2D c_p1;
    c_p1.type = COORD_POLAR; c_p1.data.polar.r = r; c_p1.data.polar.angle = ang;

    Point2D c_p2;
    c_p2.type = COORD_CARTESIAN; c_p2.data.cart.x = x; c_p2.data.cart.y = y;

    // 3. Створення C++ об'єктів
    Point2D_CPP cpp_p1 = Polar2D{r, ang};
    Point2D_CPP cpp_p2 = Cart2D{x, y};

    // 4. Обчислення
    double res_c = calculate_segment_length_C(c_p1, c_p2);
    double res_cpp = calculate_segment_length_CPP(cpp_p1, cpp_p2);

    // 5. Перевірка
    if (is_equal(res_c, res_cpp)) {
        std::cout << "OK (Val: " << res_c << ")" << std::endl;
    } else {
        std::cout << "FAIL! C: " << res_c << " vs CPP: " << res_cpp << std::endl;
    }
}

void test_task_5_shapes() {
    std::cout << "[TEST] Task 5: Shapes... ";

    // Трикутник 3, 4, 5 (Площа має бути 6)

    // C Setup
    Shape c_sh;
    c_sh.type = SHAPE_TRIANGLE;
    c_sh.data.triangle_sides = {3.0, 4.0, 5.0};

    // C++ Setup
    Shape_CPP cpp_sh = Triangle{3.0, 4.0, 5.0};

    double area_c = calculate_area_C(c_sh);
    double area_cpp = calculate_area_CPP(cpp_sh);

    if (is_equal(area_c, area_cpp)) {
        std::cout << "OK (Area: " << area_c << ")" << std::endl;
    } else {
        std::cout << "FAIL! C: " << area_c << " vs CPP: " << area_cpp << std::endl;
    }
}

int main() {
    std::cout << "=== CROSS TEST START ===" << std::endl;
    test_task_1_points();
    test_task_5_shapes();
    std::cout << "=== CROSS TEST END ===" << std::endl;
    return 0;
}