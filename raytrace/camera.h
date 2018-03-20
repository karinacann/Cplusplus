#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"


class camera
{
public:
    camera();
    ray avgRay(float u, float v);

    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;


};

#endif // CAMERA_H
