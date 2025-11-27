#ifndef UNION_C_H
#define UNION_C_H

#include <stdio.h> // Потрібно для FILE*

#ifdef __cplusplus
extern "C" {
#endif

// ==========================
// 1. Точка 2D
// ==========================
typedef enum { COORD_CARTESIAN, COORD_POLAR } Point2DType;
typedef struct {
    Point2DType type;
    union {
        struct { double x; double y; } cart;
        struct { double r; double angle; } polar;
    } data;
} Point2D;

double calculate_segment_length_C(Point2D p1, Point2D p2);
// Файлові операції
void save_point2d(FILE *f, Point2D p);
int load_point2d(FILE *f, Point2D *p); // Повертає 1 якщо успішно, 0 якщо ні


// ==========================
// 2. Гроші
// ==========================
typedef enum { MONEY_FULL, MONEY_KOP } MoneyType;
typedef struct {
    MoneyType type;
    union {
        struct { int grn; int kop; } full;
        int only_kop;
    } data;
} Money;

void print_money_C(Money m);
void save_money(FILE *f, Money m);
int load_money(FILE *f, Money *m);


// ==========================
// 3. Вектор
// ==========================
typedef enum { VECTOR_COORDS, VECTOR_POINTS } VectorType;
typedef struct {
    VectorType type;
    union {
        struct { double x; double y; } coord;
        struct { Point2D start; Point2D end; } pts;
    } data;
} Vector;

int are_collinear_C(Vector v1, Vector v2, Vector v3);
void save_vector(FILE *f, Vector v);
int load_vector(FILE *f, Vector *v);


// ==========================
// 4. Точка 3D
// ==========================
typedef enum { SPACE_CART, SPACE_POLAR, SPACE_SPHERICAL } SpaceType;
typedef struct {
    SpaceType type;
    union {
        struct { double x; double y; double z; } cart;
        struct { double r; double angle; double z; } polar;
        struct { double r; double theta; double phi; } sphere;
    } data;
} Point3D;

double calculate_distance_3d_C(Point3D p1, Point3D p2);
void save_point3d(FILE *f, Point3D p);
int load_point3d(FILE *f, Point3D *p);


// ==========================
// 5. Фігура
// ==========================
typedef enum { SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE, SHAPE_RECT, SHAPE_TRAPEZOID } ShapeType;
typedef struct {
    ShapeType type;
    union {
        double circle_radius;
        double square_side;
        struct { double a; double b; double c; } triangle_sides;
        struct { double a; double b; } rect_sides;
        struct { double a; double b; double h; } trapezoid_dims;
    } data;
} Shape;

double calculate_perimeter_C(Shape s);
double calculate_area_C(Shape s);
void save_shape(FILE *f, Shape s);
int load_shape(FILE *f, Shape *s);


// ==========================
// 6. Число
// ==========================
typedef enum { NUM_INT, NUM_DOUBLE, NUM_STRING_INF, NUM_STRING_NAN } AnyNumType;
typedef struct {
    AnyNumType type;
    union {
        int i_val;
        double d_val;
        char dummy;
    } data;
} AnyNumber;

AnyNumber divide_numbers_C(AnyNumber n1, AnyNumber n2);
void save_number(FILE *f, AnyNumber n);
int load_number(FILE *f, AnyNumber *n);

// --- Додаємо нові прототипи для консольного вводу ---
Point2D input_point2d_console();
Money   input_money_console();
Vector  input_vector_console();
Point3D input_point3d_console();
Shape   input_shape_console();
AnyNumber input_number_console();

#ifdef __cplusplus
}
#endif

#endif // UNION_C_H