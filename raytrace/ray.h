#ifndef RAY_H
#define RAY_H
#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class ray
{
protected:
    Vec3 origin;
    Vec3 direction;
public:
    ray(){};
    ray(const Vec3& origin, const Vec3& direction);
    Vec3 ray_at_point(float fractional_distance)const;
    void setDirection(const Vec3& dir);
    Vec3 getDirection()const;

    void setOrigin(const Vec3& ori);
    Vec3 getOrigin()const;


};
#endif // RAY_H
