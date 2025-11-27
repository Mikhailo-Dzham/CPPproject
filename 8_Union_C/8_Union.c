#include "8_Union.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Допоміжна функція (не видно ззовні) ---
// Перетворює будь-яку точку в структуру {x, y}
static void to_cartesian(Point2D p, double *x, double *y) {
    if (p.type == COORD_CARTESIAN) {
        *x = p.data.cart.x;
        *y = p.data.cart.y;
    } else {
        // Переведення з полярної в декартову
        *x = p.data.polar.r * cos(p.data.polar.angle);
        *y = p.data.polar.r * sin(p.data.polar.angle);
    }
}

// ==========================
// ЗАДАЧА 1: Точки
// ==========================
double calculate_segment_length_C(Point2D p1, Point2D p2) {
    double x1, y1, x2, y2;
    to_cartesian(p1, &x1, &y1);
    to_cartesian(p2, &x2, &y2);

    // Формула відстані: sqrt((x2-x1)^2 + (y2-y1)^2)
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void save_point2d(FILE *f, Point2D p) {
    fprintf(f, "%d ", p.type); // 1. Пишемо тип
    if (p.type == COORD_CARTESIAN) {
        fprintf(f, "%lf %lf\n", p.data.cart.x, p.data.cart.y);
    } else {
        fprintf(f, "%lf %lf\n", p.data.polar.r, p.data.polar.angle);
    }
}

int load_point2d(FILE *f, Point2D *p) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0; // Помилка читання

    p->type = (Point2DType)type_int;
    if (p->type == COORD_CARTESIAN) {
        if (fscanf(f, "%lf %lf", &p->data.cart.x, &p->data.cart.y) != 2) return 0;
    } else {
        if (fscanf(f, "%lf %lf", &p->data.polar.r, &p->data.polar.angle) != 2) return 0;
    }
    return 1;
}

// ==========================
// ЗАДАЧА 5: Фігури
// ==========================
double calculate_perimeter_C(Shape s) {
    switch (s.type) {
        case SHAPE_CIRCLE:
            return 2 * M_PI * s.data.circle_radius;
        case SHAPE_SQUARE:
            return 4 * s.data.square_side;
        case SHAPE_TRIANGLE:
            return s.data.triangle_sides.a + s.data.triangle_sides.b + s.data.triangle_sides.c;
        case SHAPE_RECT:
            return 2 * (s.data.rect_sides.a + s.data.rect_sides.b);
        case SHAPE_TRAPEZOID:
            // Для периметра трапеції треба знати 4 сторони,
            // але в умові часто дають основи і висоту.
            // Якщо маємо тільки a, b, h, то бічні сторони вважаємо рівнобічними (спрощення)
            // c^2 = h^2 + ((b-a)/2)^2
            {
                double a = s.data.trapezoid_dims.a;
                double b = s.data.trapezoid_dims.b;
                double h = s.data.trapezoid_dims.h;
                double cathet = fabs(b - a) / 2.0;
                double side = sqrt(h*h + cathet*cathet);
                return a + b + 2 * side;
            }
        default:
            return 0.0;
    }
}

double calculate_area_C(Shape s) {
    switch (s.type) {
        case SHAPE_CIRCLE:
            return M_PI * pow(s.data.circle_radius, 2);
        case SHAPE_SQUARE:
            return pow(s.data.square_side, 2);
        case SHAPE_TRIANGLE:
            {
                // Формула Герона
                double a = s.data.triangle_sides.a;
                double b = s.data.triangle_sides.b;
                double c = s.data.triangle_sides.c;
                double p = (a + b + c) / 2.0;
                return sqrt(p * (p - a) * (p - b) * (p - c));
            }
        case SHAPE_RECT:
            return s.data.rect_sides.a * s.data.rect_sides.b;
        case SHAPE_TRAPEZOID:
            return ((s.data.trapezoid_dims.a + s.data.trapezoid_dims.b) / 2.0) * s.data.trapezoid_dims.h;
        default:
            return 0.0;
    }
}

void save_shape(FILE *f, Shape s) {
    fprintf(f, "%d ", s.type);
    switch (s.type) {
    case SHAPE_CIRCLE: fprintf(f, "%lf\n", s.data.circle_radius); break;
    case SHAPE_SQUARE: fprintf(f, "%lf\n", s.data.square_side); break;
    case SHAPE_TRIANGLE: fprintf(f, "%lf %lf %lf\n", s.data.triangle_sides.a, s.data.triangle_sides.b, s.data.triangle_sides.c); break;
    case SHAPE_RECT: fprintf(f, "%lf %lf\n", s.data.rect_sides.a, s.data.rect_sides.b); break;
    case SHAPE_TRAPEZOID: fprintf(f, "%lf %lf %lf\n", s.data.trapezoid_dims.a, s.data.trapezoid_dims.b, s.data.trapezoid_dims.h); break;
    }
}

int load_shape(FILE *f, Shape *s) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0;
    s->type = (ShapeType)type_int;

    switch (s->type) {
    case SHAPE_CIRCLE: return fscanf(f, "%lf", &s->data.circle_radius) == 1;
    case SHAPE_SQUARE: return fscanf(f, "%lf", &s->data.square_side) == 1;
    case SHAPE_TRIANGLE: return fscanf(f, "%lf %lf %lf", &s->data.triangle_sides.a, &s->data.triangle_sides.b, &s->data.triangle_sides.c) == 3;
    case SHAPE_RECT: return fscanf(f, "%lf %lf", &s->data.rect_sides.a, &s->data.rect_sides.b) == 2;
    case SHAPE_TRAPEZOID: return fscanf(f, "%lf %lf %lf", &s->data.trapezoid_dims.a, &s->data.trapezoid_dims.b, &s->data.trapezoid_dims.h) == 3;
    }
    return 0;
}

// ==========================
// ЗАДАЧА 2: Гроші
// ==========================
void print_money_C(Money m) {
    if (m.type == MONEY_FULL) {
        printf("%d UAH %02d kop\n", m.data.full.grn, m.data.full.kop);
    } else {
        // Конвертація копійок у гривні при виводі
        int grn = m.data.only_kop / 100;
        int kop = m.data.only_kop % 100;
        printf("%d UAH %02d kop (from %d total)\n", grn, kop, m.data.only_kop);
    }
}

void save_money(FILE *f, Money m) {
    fprintf(f, "%d ", m.type);
    if (m.type == MONEY_FULL) {
        fprintf(f, "%d %d\n", m.data.full.grn, m.data.full.kop);
    } else {
        fprintf(f, "%d\n", m.data.only_kop);
    }
}

int load_money(FILE *f, Money *m) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0;
    m->type = (MoneyType)type_int;

    if (m->type == MONEY_FULL) {
        if (fscanf(f, "%d %d", &m->data.full.grn, &m->data.full.kop) != 2) return 0;
    } else {
        if (fscanf(f, "%d", &m->data.only_kop) != 1) return 0;
    }
    return 1;
}

// ==========================
// ЗАДАЧА 3: Вектори
// ==========================
// Допоміжна: отримати X та Y вектора
static void get_vector_components(Vector v, double *vx, double *vy) {
    if (v.type == VECTOR_COORDS) {
        *vx = v.data.coord.x;
        *vy = v.data.coord.y;
    } else {
        // Треба вирахувати координати з двох точок (End - Start)
        // Використовуємо функцію to_cartesian з Задачі 1 (вона має бути вище у файлі)
        // Оскільки to_cartesian static, ми можемо її викликати тут
        // Але! В C "static" функції видно тільки в цьому файлі.

        double x1, y1, x2, y2;

        // Start point
        if (v.data.pts.start.type == COORD_CARTESIAN) {
             x1 = v.data.pts.start.data.cart.x;
             y1 = v.data.pts.start.data.cart.y;
        } else {
             x1 = v.data.pts.start.data.polar.r * cos(v.data.pts.start.data.polar.angle);
             y1 = v.data.pts.start.data.polar.r * sin(v.data.pts.start.data.polar.angle);
        }

        // End point
        if (v.data.pts.end.type == COORD_CARTESIAN) {
             x2 = v.data.pts.end.data.cart.x;
             y2 = v.data.pts.end.data.cart.y;
        } else {
             x2 = v.data.pts.end.data.polar.r * cos(v.data.pts.end.data.polar.angle);
             y2 = v.data.pts.end.data.polar.r * sin(v.data.pts.end.data.polar.angle);
        }

        *vx = x2 - x1;
        *vy = y2 - y1;
    }
}

int are_collinear_C(Vector v1, Vector v2, Vector v3) {
    // Вектори колінеарні, якщо їхній векторний добуток (determinant 2x2) дорівнює 0.
    // Перевіряємо попарно: v1 || v2 AND v2 || v3
    double x1, y1, x2, y2, x3, y3;
    get_vector_components(v1, &x1, &y1);
    get_vector_components(v2, &x2, &y2);
    get_vector_components(v3, &x3, &y3);

    double det12 = x1 * y2 - x2 * y1;
    double det23 = x2 * y3 - x3 * y2;

    // Використовуємо малий epsilon для порівняння double
    double eps = 1e-9;
    return (fabs(det12) < eps) && (fabs(det23) < eps);
}

void save_vector(FILE *f, Vector v) {
    fprintf(f, "%d ", v.type);
    if (v.type == VECTOR_COORDS) {
        fprintf(f, "%lf %lf\n", v.data.coord.x, v.data.coord.y);
    } else {
        // Тут хитро: вектор зберігає дві точки. Рекурсивно викликаємо save_point2d
        save_point2d(f, v.data.pts.start);
        save_point2d(f, v.data.pts.end);
    }
}

int load_vector(FILE *f, Vector *v) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0;
    v->type = (VectorType)type_int;

    if (v->type == VECTOR_COORDS) {
        if (fscanf(f, "%lf %lf", &v->data.coord.x, &v->data.coord.y) != 2) return 0;
    } else {
        if (!load_point2d(f, &v->data.pts.start)) return 0;
        if (!load_point2d(f, &v->data.pts.end)) return 0;
    }
    return 1;
}

// ==========================
// ЗАДАЧА 4: Точки 3D
// ==========================
static void to_cartesian_3d(Point3D p, double *x, double *y, double *z) {
    switch (p.type) {
        case SPACE_CART:
            *x = p.data.cart.x; *y = p.data.cart.y; *z = p.data.cart.z;
            break;
        case SPACE_POLAR: // Циліндричні
            *x = p.data.polar.r * cos(p.data.polar.angle);
            *y = p.data.polar.r * sin(p.data.polar.angle);
            *z = p.data.polar.z;
            break;
        case SPACE_SPHERICAL:
            // x = r * sin(theta) * cos(phi)
            // y = r * sin(theta) * sin(phi)
            // z = r * cos(theta)
            *x = p.data.sphere.r * sin(p.data.sphere.theta) * cos(p.data.sphere.phi);
            *y = p.data.sphere.r * sin(p.data.sphere.theta) * sin(p.data.sphere.phi);
            *z = p.data.sphere.r * cos(p.data.sphere.theta);
            break;
    }
}

double calculate_distance_3d_C(Point3D p1, Point3D p2) {
    double x1, y1, z1, x2, y2, z2;
    to_cartesian_3d(p1, &x1, &y1, &z1);
    to_cartesian_3d(p2, &x2, &y2, &z2);
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

void save_point3d(FILE *f, Point3D p) {
    fprintf(f, "%d ", p.type);
    switch (p.type) {
    case SPACE_CART: fprintf(f, "%lf %lf %lf\n", p.data.cart.x, p.data.cart.y, p.data.cart.z); break;
    case SPACE_POLAR: fprintf(f, "%lf %lf %lf\n", p.data.polar.r, p.data.polar.angle, p.data.polar.z); break;
    case SPACE_SPHERICAL: fprintf(f, "%lf %lf %lf\n", p.data.sphere.r, p.data.sphere.theta, p.data.sphere.phi); break;
    }
}

int load_point3d(FILE *f, Point3D *p) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0;
    p->type = (SpaceType)type_int;

    switch (p->type) {
    case SPACE_CART: return fscanf(f, "%lf %lf %lf", &p->data.cart.x, &p->data.cart.y, &p->data.cart.z) == 3;
    case SPACE_POLAR: return fscanf(f, "%lf %lf %lf", &p->data.polar.r, &p->data.polar.angle, &p->data.polar.z) == 3;
    case SPACE_SPHERICAL: return fscanf(f, "%lf %lf %lf", &p->data.sphere.r, &p->data.sphere.theta, &p->data.sphere.phi) == 3;
    }
    return 0;
}

// ==========================
// ЗАДАЧА 6: Числа (Ділення)
// ==========================
AnyNumber divide_numbers_C(AnyNumber n1, AnyNumber n2) {
    AnyNumber res;
    double val1 = 0, val2 = 0;

    // 1. Перевірка на валідність вхідних даних
    if (n1.type >= NUM_STRING_INF || n2.type >= NUM_STRING_INF) {
        res.type = NUM_STRING_NAN; // Операції з нечислами дають NaN
        return res;
    }

    // 2. Отримання значень
    if (n1.type == NUM_INT) val1 = (double)n1.data.i_val;
    else val1 = n1.data.d_val;

    if (n2.type == NUM_INT) val2 = (double)n2.data.i_val;
    else val2 = n2.data.d_val;

    // 3. Ділення
    if (fabs(val2) < 1e-9) {
        res.type = NUM_STRING_INF; // Ділення на 0
    } else {
        res.type = NUM_DOUBLE;
        res.data.d_val = val1 / val2;
    }
    return res;
}

void save_number(FILE *f, AnyNumber n) {
    fprintf(f, "%d ", n.type);
    if (n.type == NUM_INT) fprintf(f, "%d\n", n.data.i_val);
    else if (n.type == NUM_DOUBLE) fprintf(f, "%lf\n", n.data.d_val);
    else fprintf(f, "0\n"); // Для INF/NAN записуємо пустишку
}

int load_number(FILE *f, AnyNumber *n) {
    int type_int;
    if (fscanf(f, "%d", &type_int) != 1) return 0;
    n->type = (AnyNumType)type_int;

    if (n->type == NUM_INT) return fscanf(f, "%d", &n->data.i_val) == 1;
    else if (n->type == NUM_DOUBLE) return fscanf(f, "%lf", &n->data.d_val) == 1;
    else { int dummy; fscanf(f, "%d", &dummy); return 1; } // Пропускаємо пустишку
}

// === ІНТЕРАКТИВНИЙ ВВІД ===

Point2D input_point2d_console() {
    Point2D p;
    int type;
    printf("  Select Point Type (0: Cartesian, 1: Polar): ");
    scanf("%d", &type);
    p.type = (Point2DType)type;
    if (p.type == COORD_CARTESIAN) {
        printf("  Enter X Y: ");
        scanf("%lf %lf", &p.data.cart.x, &p.data.cart.y);
    } else {
        printf("  Enter R Angle: ");
        scanf("%lf %lf", &p.data.polar.r, &p.data.polar.angle);
    }
    return p;
}

Money input_money_console() {
    Money m;
    int type;
    printf("  Select Money Type (0: Full, 1: Kop Only): ");
    scanf("%d", &type);
    m.type = (MoneyType)type;
    if (m.type == MONEY_FULL) {
        printf("  Enter UAH KOP: ");
        scanf("%d %d", &m.data.full.grn, &m.data.full.kop);
    } else {
        printf("  Enter Total Kopecks: ");
        scanf("%d", &m.data.only_kop);
    }
    return m;
}

Vector input_vector_console() {
    Vector v;
    int type;
    printf("  Select Vector Type (0: Coords, 1: Points): ");
    scanf("%d", &type);
    v.type = (VectorType)type;
    if (v.type == VECTOR_COORDS) {
        printf("  Enter Vx Vy: ");
        scanf("%lf %lf", &v.data.coord.x, &v.data.coord.y);
    } else {
        printf("  > Start Point:\n");
        v.data.pts.start = input_point2d_console();
        printf("  > End Point:\n");
        v.data.pts.end = input_point2d_console();
    }
    return v;
}

Point3D input_point3d_console() {
    Point3D p;
    int type;
    printf("  Select 3D Type (0: Cart, 1: Polar, 2: Sphere): ");
    scanf("%d", &type);
    p.type = (SpaceType)type;
    if (p.type == SPACE_CART) {
        printf("  Enter X Y Z: ");
        scanf("%lf %lf %lf", &p.data.cart.x, &p.data.cart.y, &p.data.cart.z);
    } else if (p.type == SPACE_POLAR) {
        printf("  Enter R Angle Z: ");
        scanf("%lf %lf %lf", &p.data.polar.r, &p.data.polar.angle, &p.data.polar.z);
    } else {
        printf("  Enter R Theta Phi: ");
        scanf("%lf %lf %lf", &p.data.sphere.r, &p.data.sphere.theta, &p.data.sphere.phi);
    }
    return p;
}

Shape input_shape_console() {
    Shape s;
    int type;
    printf("  Select Shape (0:Circle, 1:Square, 2:Triangle, 3:Rect, 4:Trapezoid): ");
    scanf("%d", &type);
    s.type = (ShapeType)type;
    switch(s.type) {
        case SHAPE_CIRCLE: printf("  Enter R: "); scanf("%lf", &s.data.circle_radius); break;
        case SHAPE_SQUARE: printf("  Enter Side: "); scanf("%lf", &s.data.square_side); break;
        case SHAPE_TRIANGLE: printf("  Enter A B C: "); scanf("%lf %lf %lf", &s.data.triangle_sides.a, &s.data.triangle_sides.b, &s.data.triangle_sides.c); break;
        case SHAPE_RECT: printf("  Enter A B: "); scanf("%lf %lf", &s.data.rect_sides.a, &s.data.rect_sides.b); break;
        case SHAPE_TRAPEZOID: printf("  Enter A B H: "); scanf("%lf %lf %lf", &s.data.trapezoid_dims.a, &s.data.trapezoid_dims.b, &s.data.trapezoid_dims.h); break;
    }
    return s;
}

AnyNumber input_number_console() {
    AnyNumber n;
    int type;
    printf("  Select Number Type (0: Int, 1: Double): ");
    scanf("%d", &type);
    n.type = (AnyNumType)type;
    if (n.type == NUM_INT) {
        printf("  Enter Int: "); scanf("%d", &n.data.i_val);
    } else {
        printf("  Enter Double: "); scanf("%lf", &n.data.d_val);
    }
    return n;
}