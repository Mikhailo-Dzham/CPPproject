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

// === ЗАДАЧА 1: Точки I/O ===
Point2D_CPP input_point2d_console() {
    int type;
    std::cout << "  Type (0:Cart, 1:Polar): "; std::cin >> type;
    if (type == 0) {
        double x, y; std::cout << "  X Y: "; std::cin >> x >> y;
        return Cart2D{x, y};
    } else {
        double r, a; std::cout << "  R Angle: "; std::cin >> r >> a;
        return Polar2D{r, a};
    }
}
void save_point2d(std::ofstream& f, const Point2D_CPP& p) {
    f << p.index() << " ";
    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Cart2D>) f << arg.x << " " << arg.y;
        else f << arg.r << " " << arg.angle;
    }, p);
    f << "\n";
}
bool load_point2d(std::ifstream& f, Point2D_CPP& p) {
    int type; if (!(f >> type)) return false;
    if (type == 0) { double x, y; f >> x >> y; p = Cart2D{x, y}; }
    else { double r, a; f >> r >> a; p = Polar2D{r, a}; }
    return true;
}

// === ЗАДАЧА 2: Гроші I/O ===
Money_CPP input_money_console() {
    int type;
    std::cout << "  Type (0:Full, 1:Kop): "; std::cin >> type;
    if(type==0) { int g, k; std::cout << "  Grn Kop: "; std::cin >> g >> k; return MoneyFull{g, k}; }
    else { int k; std::cout << "  Total Kop: "; std::cin >> k; return k; }
}
void save_money(std::ofstream& f, const Money_CPP& m) {
    f << m.index() << " ";
    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, MoneyFull>) f << arg.grn << " " << arg.kop;
        else f << arg;
    }, m);
    f << "\n";
}
bool load_money(std::ifstream& f, Money_CPP& m) {
    int type; if (!(f >> type)) return false;
    if(type==0) { int g, k; f >> g >> k; m = MoneyFull{g,k}; }
    else { int k; f >> k; m = k; }
    return true;
}

// === ЗАДАЧА 3: Вектори I/O ===
Vector_CPP input_vector_console() {
    int type; std::cout << "  Type (0:Coords, 1:Points): "; std::cin >> type;
    if(type==0) { double x,y; std::cout << "  Vx Vy: "; std::cin >> x >> y; return VectorCoords{x,y}; }
    else {
        std::cout << "  > Start:\n"; auto s = input_point2d_console();
        std::cout << "  > End:\n";   auto e = input_point2d_console();
        return VectorPoints{s, e};
    }
}
void save_vector(std::ofstream& f, const Vector_CPP& v) {
    f << v.index() << " ";
    if(v.index() == 0) {
        auto c = std::get<VectorCoords>(v); f << c.x << " " << c.y << "\n";
    } else {
        f << "\n"; // новий рядок для краси
        auto pts = std::get<VectorPoints>(v);
        save_point2d(f, pts.start);
        save_point2d(f, pts.end);
    }
}
bool load_vector(std::ifstream& f, Vector_CPP& v) {
    int type; if(!(f >> type)) return false;
    if(type==0) { double x,y; f >> x >> y; v = VectorCoords{x,y}; }
    else {
        Point2D_CPP s, e;
        if(!load_point2d(f, s) || !load_point2d(f, e)) return false;
        v = VectorPoints{s, e};
    }
    return true;
}

// === ЗАДАЧА 4: 3D I/O ===
Point3D_CPP input_point3d_console() {
    int type; std::cout << "  Type (0:Cart, 1:Polar, 2:Sphere): "; std::cin >> type;
    double a,b,c; std::cout << "  3 vals: "; std::cin >> a >> b >> c;
    if(type==0) return Cart3D{a,b,c};
    if(type==1) return Polar3D{a,b,c};
    return Sphere3D{a,b,c};
}
void save_point3d(std::ofstream& f, const Point3D_CPP& p) {
    f << p.index() << " ";
    std::visit([&](auto&& arg){ f << arg.x << " " << arg.y << " " << arg.z; },
        std::visit([](auto&& s) -> std::variant<Cart3D> {
             // Хак: приводимо структуру до 3 double, щоб не писати if constexpr 3 рази
             // (Припускаємо, що поля йдуть підряд у пам'яті, або просто пишемо явно)
             // Краще явно:
             return Cart3D{0,0,0}; // заглушка, пишемо явно нижче
        }, p)
    );
    // Простий варіант без магії:
    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Cart3D>) f << arg.x << " " << arg.y << " " << arg.z;
        else if constexpr (std::is_same_v<T, Polar3D>) f << arg.r << " " << arg.angle << " " << arg.z;
        else f << arg.r << " " << arg.theta << " " << arg.phi;
    }, p);
    f << "\n";
}
bool load_point3d(std::ifstream& f, Point3D_CPP& p) {
    int type; double a,b,c;
    if(!(f >> type >> a >> b >> c)) return false;
    if(type==0) p=Cart3D{a,b,c};
    else if(type==1) p=Polar3D{a,b,c};
    else p=Sphere3D{a,b,c};
    return true;
}

// === ЗАДАЧА 5: Фігури I/O ===
Shape_CPP input_shape_console() {
    int type; std::cout << "  Type (0:Circ, 1:Sq, 2:Tri, 3:Rect, 4:Trap): "; std::cin >> type;
    if(type==0) { double r; std::cin >> r; return Circle{r}; }
    if(type==1) { double s; std::cin >> s; return Square{s}; }
    if(type==2) { double a,b,c; std::cin >> a >> b >> c; return Triangle{a,b,c}; }
    if(type==3) { double a,b; std::cin >> a >> b; return Rect{a,b}; }
    double a,b,h; std::cin >> a >> b >> h; return Trapezoid{a,b,h};
}
void save_shape(std::ofstream& f, const Shape_CPP& s) {
    f << s.index() << " ";
    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Circle>) f << arg.r;
        else if constexpr (std::is_same_v<T, Square>) f << arg.side;
        else if constexpr (std::is_same_v<T, Triangle>) f << arg.a << " " << arg.b << " " << arg.c;
        else if constexpr (std::is_same_v<T, Rect>) f << arg.a << " " << arg.b;
        else f << arg.a << " " << arg.b << " " << arg.h;
    }, s);
    f << "\n";
}
bool load_shape(std::ifstream& f, Shape_CPP& s) {
    int type; if(!(f >> type)) return false;
    if(type==0) { double r; f>>r; s=Circle{r}; }
    else if(type==1) { double x; f>>x; s=Square{x}; }
    else if(type==2) { double a,b,c; f>>a>>b>>c; s=Triangle{a,b,c}; }
    else if(type==3) { double a,b; f>>a>>b; s=Rect{a,b}; }
    else { double a,b,h; f>>a>>b>>h; s=Trapezoid{a,b,h}; }
    return true;
}

// === ЗАДАЧА 6: Числа I/O ===
AnyNumber_CPP input_number_console() {
    int type; std::cout << "  Type (0:Int, 1:Dbl): "; std::cin >> type;
    if(type==0) { int i; std::cin >> i; return i; }
    else { double d; std::cin >> d; return d; }
}
void save_number(std::ofstream& f, const AnyNumber_CPP& n) {
    f << n.index() << " ";
    if(std::holds_alternative<int>(n)) f << std::get<int>(n);
    else if(std::holds_alternative<double>(n)) f << std::get<double>(n);
    else f << "0";
    f << "\n";
}
bool load_number(std::ifstream& f, AnyNumber_CPP& n) {
    int type; if(!(f >> type)) return false;
    if(type==0) { int i; f>>i; n=i; }
    else if(type==1) { double d; f>>d; n=d; }
    else { int dum; f>>dum; if(type==2) n=InfType{}; else n=NanType{}; }
    return true;
}