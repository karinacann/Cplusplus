#ifndef METAL_H
#define METAL_H
#include "material.h"


class metal : public material
{
public:
    metal(const Vec3& albed);
    Vec3 reflect(const Vec3& vec, const Vec3& norm)const;
    bool scatter(const ray& rayIn, const hit_record& rec, Vec3& attenuation, ray& scattered)const;
    Vec3 albedo;

};

#endif // METAL_H
