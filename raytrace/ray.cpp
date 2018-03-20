#include "ray.h"
#include "OpenGP/Image/Image.h"


//Peter Shirley
/*
ray::ray(Vec){
}
Vec3 origin;
Vec3 direction;

*/

ray :: ray(const Vec3& org, const Vec3& dir)
    : origin(org), direction(dir)
{}

Vec3 ray :: ray_at_point (float fractional_distance)const
{
    return origin + fractional_distance*direction;
}

void ray :: setDirection(const Vec3& dir){direction = dir;}

Vec3 ray :: getDirection()const{return this->direction;}

void ray :: setOrigin(const Vec3 &ori){origin = ori;}

Vec3 ray :: getOrigin() const{ return this->origin;}


