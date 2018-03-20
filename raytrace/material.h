#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hitable.h"

class material{
public:

    virtual bool scatter (const ray& rayIn, const hit_record& rec, Vec3& attenuation, ray& scattered) const=0;
};

class lambertian : public material{
    virtual bool scatter (const ray& rayIn, const hit_record& rec, Vec3& attenuation, ray& scattered) const{

    }

};

#endif // MATERIAL_H
