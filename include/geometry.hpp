#pragma once

#include "matrix.hpp"


namespace Geometrix
{
using Vector2D = Math::Vector2D<float>;
using Vector3D = Math::Vector3D<float>;
using Vector4D = Math::Vector4D<float>;

using Point2D = Math::Vector2D<std::size_t>;
using Point3D = Math::Vector3D<std::size_t>;


template<typename T, std::size_t Dim>
struct Ray
{
    Math::Vector<T,Dim> origin;
    Math::Vector<T,Dim> direction;
    Math::Vector<T,Dim> at(T t) const noexcept {return origin + direction*t;}
};

template<typename T, std::size_t Dim>
struct Line
{
    Math::Vector<T,Dim> a;
    Math::Vector<T,Dim> b;
};

template<typename T, std::size_t Dim>
struct Circle
{
    Math::Vector<T,Dim> origin;
    T                   radius;
};


template<typename T, std::size_t Dim>
struct Triangle
{
    Math::Vector<T,Dim> angle[3];
};

/*      width
 *    |<----->| p2
 * p1 +-------+--
 *    |       | |
 *    |       | | height
 *    |       | |
 *    +-------+--
 *  p4         p3
 *
 *  p1 = topleft
 *  p3 = botright
 */

template<typename T, std::size_t Dim>
struct Rectangle
{
    Math::Vector<T,Dim> topleft;
    Math::Vector<T,Dim> botright;

    auto height() const noexcept { return botright.y() - topleft.y(); }
    auto width() const noexcept { return botright.x() - topleft.x(); }
    auto p1() const noexcept {return topleft;}
    auto p2() const noexcept {return Math::Vector<T,Dim>(topleft.y(), botright.x());}
    auto p3() const noexcept {return botright;}
    auto p4() const noexcept {return Math::Vector<T,Dim>(topleft.x(), botright.y());}
};

template<typename T>
using Rectangle2D = Rectangle<T,2>;

template<typename T>
using Line2D = Line<T,2>;

template<typename T>
using Ray2D = Ray<T,2>;

template<typename T>
using Circle2D = Circle<T,2>;

template<typename T>
inline constexpr bool intersect(const Math::Vector<T,2> &p, const Rectangle2D<T> &rect) noexcept
{
    if(p.x() > rect.topleft.x() &&
       p.x() < rect.botright.x() &&
       p.y() > rect.topleft.y() &&
       p.y() < rect.botright.y())
        return true;
    return false;
}

template<typename T>
inline constexpr bool intersect(const Math::Vector<T,2> &p, const Circle2D<T> &crcl) noexcept
{
    const T dx = std::abs(p.x() - crcl.origin.x());
    if(dx > crcl.radius) return false;
    const T dy = std::abs(p.y() - crcl.origin.y());
    if(dy > crcl.radius) return false;
    if(dx+dy<=crcl.radius) return true;
    return dx*dx + dy*dy <= crcl.radius*crcl.radius;
}

//AABB Algorithm
template<typename T>
inline constexpr bool intersect(const Rectangle2D<T> &lhs, const Rectangle2D<T> &rhs) noexcept
{
    const auto x1 = lhs.topleft.x();
    const auto y1 = lhs.topleft.y();
    const auto x2 = rhs.topleft.x();
    const auto y2 = rhs.topleft.y();

    if((x1 < rhs.botright.x()) &&
        (lhs.botright.x() > x2) &&
        (y1 < rhs.botright.y()) &&
        (lhs.botright.y() > y2))
        return true;
    return false;
}


template<typename T>
inline constexpr bool intersect(const Circle2D<T> &lhs, const Circle2D<T> &rhs) noexcept
{
    const auto diff = (lhs.origin + lhs.radius) - (rhs.origin + rhs.radius);
    const auto dist = diff.norm();
    if(dist < (lhs.radius + rhs.radius))
        return true;
    return false;
}

template<typename T>
inline constexpr bool intersect(const Rectangle2D<T> &r, const Circle2D<T> &c) noexcept
{
    Math::Vector<T,2> n( std::max(r.topleft.x(),std::min(c.origin.x(), r.botright.x())),
                         std::max(r.topleft.y(),std::min(c.origin.y(), r.botright.y())));

    Math::Vector<T,2> d = n-c.origin;
    return d.norm() <= c.radius*c.radius;
}

template<typename T>
inline constexpr bool intersect(const Circle2D<T> &c, const Rectangle2D<T> &r) noexcept
{
    return intersect(r,c);
}

template<typename T, std::size_t Dim>
inline constexpr bool intersect(const Circle<T,Dim> &c, const Ray<T,Dim> &l) noexcept
{
    auto oc = l.origin - c.origin;
    T a = Math::dot(l.direction,l.direction);
    T b = Math::dot(oc,l.direction);
    T e = Math::dot(oc,oc) - c.radius * c.radius;
    T d = b*b-a*e;

    if(d>0) return true;

    return false;
}

//Möller Trumbore algorithm
template<typename T, std::size_t Dim>
inline constexpr auto intersect(const Ray<T,Dim> &ray, const Triangle<T,Dim> &triangle)
{
    using Vec = Math::Vector<T,Dim>;
    Vec e1 = triangle.angle[1] - triangle.angle[0];
    Vec e2 = triangle.angle[2] - triangle.angle[0];

    Vec normVec = Math::cross(ray.direction,e2);
    T det = dot(e1,normVec);

    if(det < 1e-8 && det > -1e-8) //if det == 0
        return T{0};

    auto invDet = 1/det;
    Vec tvec = ray.origin - triangle.angle[0];
    auto u = dot(tvec,normVec)*invDet;

    if((u<0) || (u>1))
        return T{0};

    Vec qvec = cross(tvec,triangle.angle[1]);
    auto v = dot(ray.direction,qvec) * invDet;
    if((v<0) || (u+v>1))
        return T{0};
    return dot(e2,qvec) * invDet;
}

using FRectangle2D = Rectangle2D<float>;
using FRay2D = Ray2D<float>;
}
