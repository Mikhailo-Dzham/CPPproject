#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include "8_Union.h"

// --- Helper: Очищення потоку вводу (щоб не було глюків з меню) ---
void clear_cin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Helper: Отримання файлу для читання ---
std::ifstream get_input_file() {
    std::string fname;
    std::cout << "Enter input filename: ";
    std::cin >> fname;
    std::ifstream f(fname);
    if (!f.is_open()) {
        std::cout << "[ERROR] Could not open file " << fname << std::endl;
    }
    return f;
}

// --- Helper: Отримання потоку для запису (або cout) ---
// Оскільки в C++ не можна просто повернути FILE*, ми зробимо інакше:
// Функція повертає ім'я файлу, якщо обрано файл, або порожній рядок, якщо консоль.
std::string get_output_mode() {
    int choice;
    std::cout << "[OUTPUT] 1. Console  2. File\n> ";
    std::cin >> choice;
    if (choice == 2) {
        std::string fname;
        std::cout << "Enter output filename: ";
        std::cin >> fname;
        return fname;
    }
    return ""; // Empty string = Console
}

// --- Helper: Запис результату (тексту) у вибраний потік ---
void write_result(const std::string& filename, const std::string& text) {
    if (filename.empty()) {
        std::cout << text << std::endl;
    } else {
        std::ofstream f(filename);
        if (f.is_open()) {
            f << text << std::endl;
            f.close();
            std::cout << "[INFO] Saved result to " << filename << std::endl;
        } else {
            std::cout << "[ERROR] Could not write to file!" << std::endl;
            std::cout << text << std::endl; // Fallback to console
        }
    }
}

// ==========================================
// TASK RUNNERS
// ==========================================

void run_task_1() {
    std::cout << "\n--- Task 1: Points Distance (CPP) ---" << std::endl;
    Point2D_CPP p1, p2;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Point 1:\n"; p1 = input_point2d_console();
        std::cout << "Point 2:\n"; p2 = input_point2d_console();
    } else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_point2d(f, p1) || !load_point2d(f, p2)) {
            std::cout << "[ERROR] Failed to read points from file.\n"; return;
        }
    }

    double dist = calculate_segment_length_CPP(p1, p2);

    // Формуємо рядок результату
    std::string res = "Task 1 Result: Distance = " + std::to_string(dist);
    write_result(get_output_mode(), res);
}

void run_task_2() {
    std::cout << "\n--- Task 2: Money Display (CPP) ---" << std::endl;
    Money_CPP m;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) m = input_money_console();
    else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_money(f, m)) {
             std::cout << "[ERROR] Failed to read money.\n"; return;
        }
    }

    std::string out_fname = get_output_mode();
    if (out_fname.empty()) {
        print_money_CPP(m); // Виводимо в консоль нашою функцією
    } else {
        std::ofstream f(out_fname);
        save_money(f, m); // Зберігаємо структуру в файл
        f.close();
        std::cout << "[INFO] Money object dump saved to " << out_fname << std::endl;
    }
}

void run_task_3() {
    std::cout << "\n--- Task 3: Collinear Vectors (CPP) ---" << std::endl;
    Vector_CPP v1, v2;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Vector 1:\n"; v1 = input_vector_console();
        std::cout << "Vector 2:\n"; v2 = input_vector_console();
    } else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_vector(f, v1) || !load_vector(f, v2)) {
            std::cout << "[ERROR] Failed to read vectors.\n"; return;
        }
    }

    bool collinear = are_collinear_CPP(v1, v2);
    std::string res = collinear ? "Task 3 Result: COLLINEAR" : "Task 3 Result: NOT COLLINEAR";
    write_result(get_output_mode(), res);
}

void run_task_4() {
    std::cout << "\n--- Task 4: 3D Distance (CPP) ---" << std::endl;
    Point3D_CPP p1, p2;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Point 1:\n"; p1 = input_point3d_console();
        std::cout << "Point 2:\n"; p2 = input_point3d_console();
    } else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_point3d(f, p1) || !load_point3d(f, p2)) {
            std::cout << "[ERROR] Failed to read 3D points.\n"; return;
        }
    }

    double dist = calculate_distance_3d_CPP(p1, p2);
    std::string res = "Task 4 Result: 3D Distance = " + std::to_string(dist);
    write_result(get_output_mode(), res);
}

void run_task_5() {
    std::cout << "\n--- Task 5: Shape Properties (CPP) ---" << std::endl;
    Shape_CPP s;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) s = input_shape_console();
    else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_shape(f, s)) {
            std::cout << "[ERROR] Failed to read shape.\n"; return;
        }
    }

    double P = calculate_perimeter_CPP(s);
    double A = calculate_area_CPP(s);

    std::string res = "Task 5 Result:\nPerimeter: " + std::to_string(P) + "\nArea: " + std::to_string(A);
    write_result(get_output_mode(), res);
}

void run_task_6() {
    std::cout << "\n--- Task 6: Division (CPP) ---" << std::endl;
    AnyNumber_CPP n1, n2;
    int choice;
    std::cout << "[INPUT] 1. Console  2. File\n> ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Numerator:\n";   n1 = input_number_console();
        std::cout << "Denominator:\n"; n2 = input_number_console();
    } else {
        auto f = get_input_file();
        if (!f.is_open()) return;
        if (!load_number(f, n1) || !load_number(f, n2)) {
            std::cout << "[ERROR] Failed to read numbers.\n"; return;
        }
    }

    auto result = divide_numbers_CPP(n1, n2);

    std::string res_str;
    if (std::holds_alternative<double>(result)) {
        res_str = "Task 6 Result: " + std::to_string(std::get<double>(result));
    } else if (std::holds_alternative<InfType>(result)) {
        res_str = "Task 6 Result: INFINITY";
    } else {
        res_str = "Task 6 Result: NaN (Error)";
    }

    write_result(get_output_mode(), res_str);
}

// ==========================================
// MAIN MENU
// ==========================================

int main() {
    int choice;
    do {
        std::cout << "\n============================\n";
        std::cout << "   PROJECT 8: CPP VARIANT   \n";
        std::cout << "============================\n";
        std::cout << "1. Task 1 (Points 2D)\n";
        std::cout << "2. Task 2 (Money)\n";
        std::cout << "3. Task 3 (Vectors)\n";
        std::cout << "4. Task 4 (Points 3D)\n";
        std::cout << "5. Task 5 (Shapes)\n";
        std::cout << "6. Task 6 (Numbers)\n";
        std::cout << "0. Exit\n";
        std::cout << "Select Task: ";
        std::cin >> choice;

        if(std::cin.fail()) {
            clear_cin();
            std::cout << "Invalid input.\n";
            continue;
        }

        switch(choice) {
            case 1: run_task_1(); break;
            case 2: run_task_2(); break;
            case 3: run_task_3(); break;
            case 4: run_task_4(); break;
            case 5: run_task_5(); break;
            case 6: run_task_6(); break;
            case 0: std::cout << "Goodbye.\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}