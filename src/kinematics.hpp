#include <math.h>

using real_t = float;

struct Vec3
{
    Vec3(real_t x, real_t y, real_t z)
    : x{x}
    , y{y}
    , z{z}
    {}

    Vec3(real_t v)
    : Vec3{v, v, v}
    {}

    real_t x, y, z;
};

struct Leg
{
    real_t coxa_length, femur_length, tibia_length;
};

struct Angles
{
    Angles(real_t j0, real_t j1, real_t j2)
    : j0{j0}
    , j1{j1}
    , j2{j2}
    {}

    real_t j0 = 0.0, j1 = 0.0, j2 = 0.0;
};

real_t sqr(real_t x)
{
    return x * x;
}

real_t sign(real_t x)
{
    return x < 0.0 ? -1.0 : 1.0;
}

real_t angle(real_t a, real_t b, real_t c)
{
    return acos((sqr(a) + sqr(b) - sqr(c)) / (2 * a * b));
}

bool check_triangle(real_t a, real_t b, real_t c)
{
    return c < a + b && a < b + c && b < a + c;
}

// Target coordinates are in the leg space
Angles leg_ik(Leg leg, Vec3 target)
{
    // TODO : clamp target

    // cylindrical coordinates
    real_t phi = atan2(target.y, target.x);
    real_t r2 = sqr(target.x) + sqr(target.y);
    real_t r = sqrt(r2);
    real_t z = target.z;

    real_t lr = r - leg.coxa_length;

    // TODO : correctly handle lr < 0
    real_t sz = leg.femur_length - target.z > 0.0 ? -1.0 : 1.0;

    real_t lr2 = sqr(lr);
    real_t d2 = sqr(target.z) + lr2;
    real_t d = sqrt(d2);
    real_t dl = sqrt(sqr(target.z) + r2);

    if (!check_triangle(d, leg.femur_length, leg.tibia_length))
    {
        return Angles{0, 0, 0};
    }

    // knee doesn't bend backwards (for now)
    real_t j2 = angle(leg.femur_length, leg.tibia_length, d) - M_PI; // potential sign change

    real_t ta2 = atan2(target.z, lr);
    real_t ta = angle(leg.femur_length, d, leg.tibia_length);

    real_t j1 = ta + ta2;

    return Angles{phi, j1, j2};
}

struct Body {
    real_t side;
    Leg leg;
};

struct Actuations {
    Angles legs[4];

    Actuations() = default;
    explicit Actuations(Angles a)
    : legs{a, a, a, a}
    {}

    Actuations(Angles a0, Angles a1, Angles a2, Angles a3)
    : legs{a0, a1, a2, a3}
    {}
};
