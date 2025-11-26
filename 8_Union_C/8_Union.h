#ifndef UNION_C_H
#define UNION_C_H

//This block need for connect file in C++ project withaut linking errors
#ifdef __cplusplus
extern "C" {
#endif

// ==========================================
// 1. Точка (Декартова / Полярна)
// ==========================================
typedef enum { COORD_CARTESIAN, COORD_POLAR } CoordinateType;

typedef struct {
    CoordinateType type;
    union {
        struct { double x; double y; } cart;      // Декартові
        struct { double r; double angle; } polar; // Полярні
    } data;
} Point2D;

// Функції для задачі 1
double calculate_segment_length_C(Point2D p1, Point2D p2);


// ==========================================
// 2. Гроші (Гривні+Копійки / Тільки Копійки)
// ==========================================
typedef enum { MONEY_FULL, MONEY_KOP } MoneyType;

typedef struct {
    MoneyType type;
    union {
        struct { int grn; int kop; } full;
        int only_kop;
    } data;
} Money;

// Функції для задачі 2
void print_money_C(Money m);


// ==========================================
// 3. Вектор (По точці / По другій точці)
// ==========================================
// Тут трохи хитріше. Універсальний вектор може бути заданий:
// а) Координатами (x, y)
// б) Двома точками (Start, End) - використаємо Point2D з першої задачі
typedef enum { VECTOR_COORDS, VECTOR_POINTS } VectorType;

typedef struct {
    VectorType type;
    union {
        struct { double x; double y; } coord;
        struct { Point2D start; Point2D end; } pts;
    } data;
} Vector;

// Функції для задачі 3
int are_collinear_C(Vector v1, Vector v2, Vector v3);


// ==========================================
// 4. Точка у просторі (Декарт / Полярні / Сферичні)
// ==========================================
typedef enum { SPACE_CART, SPACE_POLAR, SPACE_SPHERICAL } SpaceType;

typedef struct {
    SpaceType type;
    union {
        struct { double x; double y; double z; } cart;
        struct { double r; double angle; double z; } polar; // Циліндричні/Полярні в 3D
        struct { double r; double theta; double phi; } sphere; // Сферичні
    } data;
} Point3D;

// Функції для задачі 4
double calculate_distance_3d_C(Point3D p1, Point3D p2);


// ==========================================
// 5. Пласка Фігура
// ==========================================
typedef enum { SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE, SHAPE_RECT, SHAPE_TRAPEZOID } ShapeType;

typedef struct {
    ShapeType type;
    union {
        double circle_radius;
        double square_side;
        struct { double a; double b; double c; } triangle_sides;
        struct { double a; double b; } rect_sides;
        struct { double a; double b; double h; } trapezoid_dims; // основи a, b і висота h
    } data;
} Shape;

// Функції для задачі 5
double calculate_perimeter_C(Shape s);
double calculate_area_C(Shape s);


// ==========================================
// 6. Число або Рядок
// ==========================================
typedef enum { NUM_INT, NUM_DOUBLE, NUM_STRING_INF, NUM_STRING_NAN } AnyNumType;

typedef struct {
    AnyNumType type;
    union {
        int i_val;
        double d_val;
        // Рядки тут не зберігаємо в union напряму як масиви,
        // бо вони константні для типів "Нескінченність" та "Невизначеність"
        // Але можна зберігати код помилки або пустишку
        char dummy;
    } data;
} AnyNumber;

// Функції для задачі 6
AnyNumber divide_numbers_C(AnyNumber n1, AnyNumber n2);


#ifdef __cplusplus
}
#endif

#endif // UNION_C_H