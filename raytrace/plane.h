#ifndef PLANE_H
#define PLANE_H
#include "OpenGP/Image/Image.h"
#include "hitable.h"

class plane : public hitable
{
public:
    plane();
    plane(Vec3 primaryPosition, Vec3 normal, material *mat);

    void setPosition(Vec3& position);
    Vec3 getPosition();

    void setNormal(Vec3& norm);
    Vec3 getNormal();

    void setColour(Vec3& colr);
    Vec3 getColour();

    virtual bool hit(const ray& r, const float t0, const float t1, hit_record& rec) const;


    Vec3 normal;
    Vec3 planePosition;
    Vec3 planeBaseColour;
    float denominator;
    material *material_ptr;
    //plane equals a point from camera,
    //and normal
};

#endif // PLANE_H
