#ifndef UNION_CPP_H
#define UNION_CPP_H

#include <variant>
#include <string>
#include <vector>
#include <fstream>

// --- ЗАДАЧА 1: Точки ---
struct Cart2D { double x, y; };
struct Polar2D { double r, angle; };
using Point2D_CPP = std::variant<Cart2D, Polar2D>;

double calculate_segment_length_CPP(const Point2D_CPP& p1, const Point2D_CPP& p2);

// --- ЗАДАЧА 2: Гроші ---
struct MoneyFull { int grn, kop; };
using Money_CPP = std::variant<MoneyFull, int>; // int = тільки копійки

void print_money_CPP(const Money_CPP& m);

// --- ЗАДАЧА 3: Вектори ---
struct VectorCoords { double x, y; };
struct VectorPoints { Point2D_CPP start, end; };
using Vector_CPP = std::variant<VectorCoords, VectorPoints>;

bool are_collinear_CPP(const Vector_CPP& v1, const Vector_CPP& v2);

// --- ЗАДАЧА 4: Точки 3D ---
struct Cart3D { double x, y, z; };
struct Polar3D { double r, angle, z; }; // Циліндричні
struct Sphere3D { double r, theta, phi; }; // Сферичні
using Point3D_CPP = std::variant<Cart3D, Polar3D, Sphere3D>;

double calculate_distance_3d_CPP(const Point3D_CPP& p1, const Point3D_CPP& p2);

// --- ЗАДАЧА 5: Фігури ---
struct Circle { double r; };
struct Square { double side; };
struct Triangle { double a, b, c; };
struct Rect { double a, b; };
struct Trapezoid { double a, b, h; };
using Shape_CPP = std::variant<Circle, Square, Triangle, Rect, Trapezoid>;

double calculate_perimeter_CPP(const Shape_CPP& s);
double calculate_area_CPP(const Shape_CPP& s);

// --- ЗАДАЧА 6: Числа ---
// Використовуємо monostate для позначення "Inf" або "NaN", бо вони не несуть даних, тільки тип
struct InfType {};
struct NanType {};
using AnyNumber_CPP = std::variant<int, double, InfType, NanType>;

AnyNumber_CPP divide_numbers_CPP(const AnyNumber_CPP& n1, const AnyNumber_CPP& n2);

// Функції для консольного вводу (інтерактивні)
Point2D_CPP input_point2d_console();
Money_CPP   input_money_console();
Vector_CPP  input_vector_console();
Point3D_CPP input_point3d_console();
Shape_CPP   input_shape_console();
AnyNumber_CPP input_number_console();

// Функції для файлового I/O
void save_point2d(std::ofstream& f, const Point2D_CPP& p);
bool load_point2d(std::ifstream& f, Point2D_CPP& p);

void save_money(std::ofstream& f, const Money_CPP& m);
bool load_money(std::ifstream& f, Money_CPP& m);

void save_vector(std::ofstream& f, const Vector_CPP& v);
bool load_vector(std::ifstream& f, Vector_CPP& v);

void save_point3d(std::ofstream& f, const Point3D_CPP& p);
bool load_point3d(std::ifstream& f, Point3D_CPP& p);

void save_shape(std::ofstream& f, const Shape_CPP& s);
bool load_shape(std::ifstream& f, Shape_CPP& s);

void save_number(std::ofstream& f, const AnyNumber_CPP& n);
bool load_number(std::ifstream& f, AnyNumber_CPP& n);

#endif // UNION_CPP_H