#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "8_Union.h"

struct TestCase {
    int task_id;
    std::string input_file;
    std::string output_file;
    std::string description;
};

void run_test_case(const TestCase& tc) {
    std::cout << "[RUNNING] " << tc.description << " (Task " << tc.task_id << ")... ";

    std::ifstream in(tc.input_file);
    if (!in.is_open()) {
        std::cout << "\n  [ERROR] Cannot open input: " << tc.input_file << std::endl;
        return;
    }

    std::ofstream out(tc.output_file);
    if (!out.is_open()) {
        std::cout << "\n  [ERROR] Cannot open output: " << tc.output_file << std::endl;
        return;
    }

    switch(tc.task_id) {
        case 1: {
            Point2D_CPP p1, p2;
            if (load_point2d(in, p1) && load_point2d(in, p2)) {
                double dist = calculate_segment_length_CPP(p1, p2);
                out << "Input: " << tc.input_file << "\nResult Distance: " << dist << std::endl;
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse)" << std::endl;
            break;
        }
        case 2: {
            Money_CPP m;
            if (load_money(in, m)) {
                out << "Index: " << m.index() << "\n";
                save_money(out, m); // Using our save function
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse)" << std::endl;
            break;
        }
        case 3: {
            Vector_CPP v1, v2; // In C++ version we check 2 vectors (example)
            if (load_vector(in, v1) && load_vector(in, v2)) {
                bool res = are_collinear_CPP(v1, v2);
                out << "Collinear: " << (res ? "YES" : "NO") << std::endl;
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse - need 2 vectors)" << std::endl;
            break;
        }
        case 4: {
            Point3D_CPP p1, p2;
            if (load_point3d(in, p1) && load_point3d(in, p2)) {
                double dist = calculate_distance_3d_CPP(p1, p2);
                out << "3D Dist: " << dist << std::endl;
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse)" << std::endl;
            break;
        }
        case 5: {
            Shape_CPP s;
            if (load_shape(in, s)) {
                out << "Area: " << calculate_area_CPP(s) << "\nPerim: " << calculate_perimeter_CPP(s) << std::endl;
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse)" << std::endl;
            break;
        }
        case 6: {
            AnyNumber_CPP n1, n2;
            if (load_number(in, n1) && load_number(in, n2)) {
                auto res = divide_numbers_CPP(n1, n2);
                out << "Result: ";
                if (std::holds_alternative<double>(res)) out << std::get<double>(res);
                else if (std::holds_alternative<InfType>(res)) out << "INFINITY";
                else out << "NaN";
                out << std::endl;
                std::cout << "OK -> " << tc.output_file << std::endl;
            } else std::cout << "FAIL (Parse)" << std::endl;
            break;
        }
    }
}

int main() {
    std::vector<TestCase> tests = {
        {1, "test_t1_mixed.txt", "res_cpp_t1.txt", "Task 1: Mixed Coordinates"},
        {2, "test_t2_full.txt",  "res_cpp_t2a.txt", "Task 2: Full Money"},
        {2, "test_t2_kop.txt",   "res_cpp_t2b.txt", "Task 2: Kopecks Only"},
        // For vectors, C++ can read the first two vectors from a file with three vectors, that's fine
        {3, "test_t3_collinear.txt", "res_cpp_t3.txt", "Task 3: Collinear Vectors"},
        {4, "test_t4_sphere.txt", "res_cpp_t4.txt", "Task 4: Spherical 3D"},
        {5, "test_t5_all.txt",    "res_cpp_t5.txt", "Task 5: Trapezoid Area"},
        {6, "test_t6_int_dbl.txt","res_cpp_t6a.txt", "Task 6: Int / Double"},
        {6, "test_t6_zero.txt",   "res_cpp_t6b.txt", "Task 6: Division by Zero"}
    };

    std::cout << "=== STARTING FAST BATCH TESTING (CPP) ===\n";
    for(const auto& t : tests) {
        run_test_case(t);
    }
    std::cout << "=== ALL TESTS FINISHED ===\n";
    return 0;
}