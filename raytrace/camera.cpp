#include "camera.h"


//To get different points and return avg
camera::camera()
{
    lowerLeftCorner =  Vec3(-2.0,-1.0,-1.0);
    horizontal = Vec3(4.0,0.0,0.0);
    vertical = Vec3(0.0,2.0,0.0);
    origin = Vec3(0.0,0.0,1.0);

}

//aspect ratio?
ray camera::avgRay(float u, float v){
    return ray(origin, lowerLeftCorner+(u*horizontal)+(v*vertical)-origin);
}
