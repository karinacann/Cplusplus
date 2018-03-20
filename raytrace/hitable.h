#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "OpenGP/Image/Image.h"

//Peter Shirley
class material;

struct hit_record{
    OpenGP :: Vec3 intersectionPoint;
    OpenGP :: Vec3 pointNormal;
    float closest;
    material *material_ptr;
    //time
 };

class hitable
{
public:
    virtual bool hit(const ray& r, const float t0, const float t1, hit_record& rec) const = 0;
    //virtual box bounding-box
};

#endif
