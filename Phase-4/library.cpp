#include <iostream>
#include <cmath>

using namespace std;
typedef double num;

class point
{
public:
    num x, y; // x and y coordinates

    point(num x1, num y1) : x(x1), y(y1) {}

    point operator+(const point &other) const
    {
        return point(this->x + other.x, this->y + other.y);
    }
    point operator-(const point &other) const
    {
        return point(this->x - other.x, this->y - other.y);
    }

    friend ostream &operator<<(ostream &os, const point &p)
    {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

class curve
{
public:
    num a, b, c, h, g, f; // Coefficients of the conic curve
    string type;          // Optional name for the curve
    num delta;
    string equation;
    // Constructor
    curve(num a, num b, num c, num h, num g, num f)
        : a(a), b(b), c(c), h(h), g(g), f(f)
    {
        delta = (a * b * c) + (2 * f * g * h) - (a * f * f) - (b * f * f) - (c * h * h);
        equation = to_string(a) + "x^2 + " + to_string(2 * h) + "xy + " +
                   to_string(b) + "y^2 + " + to_string(2 * g) + "x + " +
                   to_string(2 * f) + "y + " + to_string(c) + " = 0";
        num discriminant = (a * b) - (h * h);
        if (h == 0 && a == 0 && b == 0)
        {
            type = "line";
        }
        else if (h == 0 && a != 0 && b != 0 && a == b)
        {
            type = "circle";
        }
        else if (discriminant > 0)
        {
            type = "Ellipse";
        }
        else if (discriminant == 0)
        {
            type = "Parabola";
        }
        else if (discriminant < 0)
        {
            type = "Hyperbola";
        }
    }
    curve(string eq1)
    {
    }
    // Print the curve in standard form
    friend ostream &operator<<(ostream &os, const curve &p)
    {
        os << p.type << ": "
           << p.a << "x^2 + " << 2 * p.h << "xy + " << p.b << "y^2 + "
           << 2 * p.g << "x + " << 2 * p.f << "y + " << p.c << " = 0" << endl;
        return os;
    }
};

class line
{
public:
    num slope;
    curve Curve;
    num c;
    // constructor
    line(point p1, point p2) : slope((p2.y - p1.y) / (p2.x - p1.x)), c(p1.y - (slope * p1.x)), Curve(0, 0, p1.y - (slope * p1.x), 0, slope / 2, -0.5) {}
    line(num m, num c) : slope(m), Curve(0, 0, c, 0, m / 2, -0.5), c(c) {}
    line(curve curve1) : slope(curve1.g * 2), Curve(curve1), c(curve1.c) {}

    friend ostream &operator<<(ostream &os, const line &p)
    {
        os << "line : " << p.Curve.g * 2 << "x + " << p.Curve.f * 2 << "y + " << p.Curve.c << " = 0" << endl;
        return os;
    }
};

class circle
{
public:
    num radius;
    curve Curve;
    point center;
    num eccentricity;

    circle(const point &c, num r) : radius(r), center(c),
                                    Curve(1, 1, -1 * (r * r + c.x * c.x + c.y * c.y), 0, -1 * c.x, -1 * c.y), eccentricity(0) {}

    // Constructor: Create a circle directly from its curve
    circle(const curve &e) : Curve(e),
                             center(point(-1 * e.g / e.a, -1 * e.f / e.b)),
                             radius(sqrt((-1 * (e.c / e.a) + (center.x * center.x) + (center.y * center.y)))), eccentricity(0) {}

    // Display the circle curve

    friend ostream &operator<<(ostream &os, const circle &p)
    {
        os << "Circle: (x - " << p.center.x << ")^2 + (y - " << p.center.y << ")^2 = " << p.radius * p.radius << endl;
        return os;
    }
};

class ellipse
{
public:
    num a, b;     // Semi-major and semi-minor axes
    point center; // Center of the ellipse
    curve Curve;
    num eccentricity;

    ellipse(const point &c, num semi_a, num semi_b)
        : a(semi_a), b(semi_b), center(c),
          Curve(1 / (semi_a * semi_a), 1 / (semi_b * semi_b),
                -1, 0, -2 * c.x / (semi_a * semi_a), -2 * c.y / (semi_b * semi_b)),
          eccentricity(sqrt(1 - ((semi_b * semi_b) / (semi_a * semi_a)))) {}

    ellipse(const curve &e)
        : Curve(e), center(point(-1 * e.g / e.a, -1 * e.f / e.b)),
          a(sqrt(1 / e.a)), b(sqrt(1 / e.b)), eccentricity(sqrt(1 - (e.a / e.b))) {}

    // Display the curve
    friend ostream &operator<<(ostream &os, const ellipse &p)
    {
        os << "Ellipse: (x - " << p.center.x << ")^2/ " << (p.a * p.a) << " + (y - " << p.center.y << ")^2 /" << p.b * p.b << "= 1" << endl;
        return os;
    }
};

class parabola
{
public:
    curve Curve;
    point focus;
    point vertex;
    num eccentricity; // For a parabola, eccentricity is always 1
    num latus_rectum; // Length of the latus rectum

    parabola(const point &f, const point &v)
        : focus(f), vertex(v), eccentricity(1),
          latus_rectum(4 * abs((f.y - v.y))),
          Curve(1, 0, -1 * latus_rectum / 4, 0, 0, -1 * v.y)
    {
        // If parabola is horizontal (focus.x != vertex.x):
        if (f.x != v.x)
        {
            latus_rectum = 4 * abs((f.x - v.x));
            if (f.x > v.x)
            {
                Curve = curve(0, 1, v.x * latus_rectum + (v.y * v.y), 0, -1 * latus_rectum / 2, -1 * v.y);
            }
            else
            {
                Curve = curve(0, 1, -1 * v.x * latus_rectum + (v.y * v.y), 0, latus_rectum / 2, -1 * v.y);
            }
        }
        else
        {
            latus_rectum = 4 * abs((f.y - v.y));
            if (f.y > v.y)
            {
                Curve = curve(1, 0, v.y * latus_rectum + (v.x * v.x), 0, -1 * v.x, -1 * latus_rectum / 2);
            }
            else
            {
                Curve = curve(1, 0, -1 * v.y * latus_rectum + (v.x * v.x), 0, -1 * v.x, latus_rectum / 2);
            }
        }
    }
    // Display the curve
    friend ostream &operator<<(ostream &os, const parabola &p)
    {
        os << p.Curve;
        return os;
    }
};

class hyperbola
{
public:
    num a, b;     // Semi-major and semi-minor axes
    point center; // Center of the hyperbola
    curve eq;     // curve representation
    num eccentricity;
    // Constructor from semi-axes and center
    hyperbola(const point &c, num semi_a, num semi_b)
        : a(semi_a), b(semi_b), center(c),
          eq(1 / (semi_a * semi_a), -1 / (semi_b * semi_b),
             -1, 0, -2 * c.x / (semi_a * semi_a), -2 * c.y / (semi_b * semi_b)),
          eccentricity(sqrt(1 + ((semi_b * semi_b) / (semi_a * semi_a)))) {}

    // Constructor from the general curve
    hyperbola(const curve &e)
        : eq(e), center(point(-1 * e.g / e.a, -1 * e.f / e.b)),
          a(sqrt(1 / e.a)), b(sqrt(1 / e.b)),
          eccentricity(sqrt(1 + (e.a / e.b))) {}

    // Display the curve
    friend ostream &operator<<(ostream &os, const hyperbola &p)
    {
        os << p.eq;
        return os;
    }
};

bool ispoint(curve eq, point p)
{
    return ((eq.a * p.x * p.x) + (2 * eq.h * p.x * p.y) + (eq.b * p.y * p.y) +
            (2 * eq.g * p.x) + (2 * eq.f * p.y) + eq.c) == 0;
}

line tangent(curve Curve, point p)
{
    return line(curve(0, 0, Curve.c, 0, (Curve.a * p.x) + Curve.g, (Curve.b * p.y) + Curve.f));
}

num distance(point p1, point p2)
{
    return num(sqrt(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y))));
}

num distance(line l, point p)
{
    return num(((l.Curve.g * 2 * p.x) + (l.Curve.f * 2 * p.y) + (l.Curve.c)) / sqrt((4 * l.Curve.g * l.Curve.g) + (4 * l.Curve.f * l.Curve.f)));
}

num distance(line l1, line l2)
{
    if (l1.slope == l2.slope)
    {
        return num(abs((l1.c - l2.c)) / sqrt(1 + (l1.slope * l1.slope)));
    }
    return num(0);
}

// num distance(curve eq1,point p){

// }

// num distance(curve eq1 ,line l){

// }

// int main() {
//     point center(0, 0);
//     point focus1(1,0);
//     num a(5), b(3);
//     cout << a;
//     ellipse e1(center, a, b);
//     parabola p(focus1,center);
//     hyperbola h1(center, a, b);
//     cout << e1 ;
//     cout << p ;
//     cout << h1 ;
//     point focus(0, 3);
//     point vertex(0, 1);
//     parabola p1(focus, vertex);
//     cout << p1 ;
//     hyperbola h2(h1);
//     cout << h2;
//     ellipse ek(e1);
//     cout << ek;
//     return 0;
// }
