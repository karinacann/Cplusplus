#ifndef SPHERE_H
#define SPHERE_H
#include <iostream>
#include "OpenGP/Image/Image.h"
using namespace OpenGP;
#include "hitable.h"

class sphere : public hitable{
public:
    sphere();
    sphere(Vec3 position, int rad, material *mat);

    void setPosition(Vec3& position);
    Vec3 getPosition();

    void setRadius(float radius);
    float getRadius();

    virtual bool hit(const ray& r, const float t0, const float t1, hit_record& rec) const;
    //virtual ~sphere();

protected:
    Vec3 spherePosition;
    Vec3 sphereColour;
    float sphereRadius;
    material *material_ptr;

};


#endif

