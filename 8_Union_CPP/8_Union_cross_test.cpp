#include <iostream>
#include <cassert>
#include <cmath>
#include "../8_Union_C/8_Union.h"  // C headers
#include "8_Union.h"               // C++ headers
#include <chrono> // For timing in C++

// Function to compare doubles
bool is_equal(double a, double b) {
    return std::abs(a - b) < 1e-6;
}

void test_task_1_points() {
    std::cout << "[TEST] Task 1: Points... ";

    // 1. Data
    double r = 10.0, ang = 0.785; // 45 degrees
    double x = 5.0, y = 5.0;

    // 2. Creating C objects
    Point2D c_p1;
    c_p1.type = COORD_POLAR; c_p1.data.polar.r = r; c_p1.data.polar.angle = ang;

    Point2D c_p2;
    c_p2.type = COORD_CARTESIAN; c_p2.data.cart.x = x; c_p2.data.cart.y = y;

    // 3. Creating C++ objects
    Point2D_CPP cpp_p1 = Polar2D{r, ang};
    Point2D_CPP cpp_p2 = Cart2D{x, y};

    // 4. Calculation
    double res_c = calculate_segment_length_C(c_p1, c_p2);
    double res_cpp = calculate_segment_length_CPP(cpp_p1, cpp_p2);

    // 5. Check
    if (is_equal(res_c, res_cpp)) {
        std::cout << "OK (Val: " << res_c << ")" << std::endl;
    } else {
        std::cout << "FAIL! C: " << res_c << " vs CPP: " << res_cpp << std::endl;
    }
}

void test_task_5_shapes() {
    std::cout << "[TEST] Task 5: Shapes... ";

    // Triangle 3, 4, 5 (Area should be 6)

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

void test_task_3_vectors() {
    std::cout << "[TEST] Task 3: Vectors... ";

    // Vector 1: Coordinates (1, 1)
    Vector v1_c;
    v1_c.type = VECTOR_COORDS;
    v1_c.data.coord.x = 1;
    v1_c.data.coord.y = 1;

    Vector_CPP v1_cpp = VectorCoords{1.0, 1.0};

    // Vector 2: Points (0,0) -> (2,2)
    // --- FIX HERE ---
    Point2D p_start;
    p_start.type = COORD_CARTESIAN;
    p_start.data.cart.x = 0;
    p_start.data.cart.y = 0;

    Point2D p_end;
    p_end.type = COORD_CARTESIAN;
    p_end.data.cart.x = 2;
    p_end.data.cart.y = 2;
    // -----------------------

    Vector v2_c;
    v2_c.type = VECTOR_POINTS;
    v2_c.data.pts.start = p_start;
    v2_c.data.pts.end = p_end;

    Vector_CPP v2_cpp = VectorPoints{ Cart2D{0,0}, Cart2D{2,2} };

    // Check
    bool res_c = are_collinear_C(v1_c, v2_c, v1_c);
    bool res_cpp = are_collinear_CPP(v1_cpp, v2_cpp);

    if (res_c == res_cpp) std::cout << "OK" << std::endl;
    else std::cout << "FAIL" << std::endl;
}

void test_task_4_3d() {
    std::cout << "[TEST] Task 4: 3D Points... ";

    // Point 1: Cartesian (1, 0, 0)
    Point3D p1_c; p1_c.type=SPACE_CART; p1_c.data.cart = {1,0,0};
    Point3D_CPP p1_cpp = Cart3D{1,0,0};

    // Point 2: Spherical (r=1, theta=PI/2, phi=PI) -> (-1, 0, 0)
    // Distance should be 2.0
    double PI = 3.1415926535;
    Point3D p2_c; p2_c.type=SPACE_SPHERICAL; p2_c.data.sphere = {1, PI/2.0, PI};
    Point3D_CPP p2_cpp = Sphere3D{1, PI/2.0, PI};

    double d_c = calculate_distance_3d_C(p1_c, p2_c);
    double d_cpp = calculate_distance_3d_CPP(p1_cpp, p2_cpp);

    if (std::abs(d_c - d_cpp) < 1e-6) std::cout << "OK (" << d_c << ")" << std::endl;
    else std::cout << "FAIL! C:" << d_c << " CPP:" << d_cpp << std::endl;
}

void test_task_6_numbers() {
    std::cout << "[TEST] Task 6: Numbers... ";

    // 10 / 2 = 5
    AnyNumber n1; n1.type=NUM_INT; n1.data.i_val = 10;
    AnyNumber n2; n2.type=NUM_DOUBLE; n2.data.d_val = 2.0;

    AnyNumber_CPP n1_cpp = 10;
    AnyNumber_CPP n2_cpp = 2.0;

    AnyNumber res_c = divide_numbers_C(n1, n2);
    AnyNumber_CPP res_cpp = divide_numbers_CPP(n1_cpp, n2_cpp);

    // Check if result is 5.0
    double val_c = res_c.data.d_val;
    double val_cpp = std::get<double>(res_cpp);

    if (std::abs(val_c - val_cpp) < 1e-9) std::cout << "OK" << std::endl;
    else std::cout << "FAIL" << std::endl;
}

void run_benchmark() {
    std::cout << "\n=== BENCHMARK START (10,000,000 iterations) ===" << std::endl;
    long long iterations = 10000000;

    // --- C Benchmark ---
    Point2D p1_c, p2_c;
    p1_c.type = COORD_CARTESIAN; p1_c.data.cart = {0.0, 0.0};
    p2_c.type = COORD_POLAR;     p2_c.data.polar = {100.0, 0.785};

    auto start_c = std::chrono::high_resolution_clock::now();
    volatile double dummy_c = 0; // volatile so compiler doesn't optimize away the loop
    for(int i=0; i<iterations; ++i) {
        dummy_c += calculate_segment_length_C(p1_c, p2_c);
    }
    auto end_c = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_c = end_c - start_c;

    std::cout << "C (Union) time:   " << diff_c.count() << " s" << std::endl;

    // --- C++ Benchmark ---
    Point2D_CPP p1_cpp = Cart2D{0.0, 0.0};
    Point2D_CPP p2_cpp = Polar2D{100.0, 0.785};

    auto start_cpp = std::chrono::high_resolution_clock::now();
    volatile double dummy_cpp = 0;
    for(int i=0; i<iterations; ++i) {
        dummy_cpp += calculate_segment_length_CPP(p1_cpp, p2_cpp);
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_cpp = end_cpp - start_cpp;

    std::cout << "C++ (Variant) time: " << diff_cpp.count() << " s" << std::endl;

    if (diff_c.count() < diff_cpp.count())
        std::cout << "Result: C is faster by " << (diff_cpp.count() - diff_c.count()) << " s" << std::endl;
    else
        std::cout << "Result: C++ is faster (optimization magic!)" << std::endl;

    std::cout << "=== BENCHMARK END ===" << std::endl;
}

int main() {
    std::cout << "=== CROSS TEST START ===" << std::endl;
    test_task_1_points();
    test_task_3_vectors();
    test_task_4_3d();
    test_task_5_shapes();
    test_task_6_numbers();
    std::cout << "=== CROSS TEST END ===" << std::endl;
    run_benchmark();
    return 0;
}