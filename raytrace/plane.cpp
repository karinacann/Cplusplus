#include "plane.h"

plane::plane(){}
plane::plane(Vec3 primaryPosition, Vec3 normal, material *mat)
 : normal(normal),
   planePosition(primaryPosition),
   material_ptr(mat)
   //denominator(normal.dot(ray.getDirection()))
{}

void plane::setPosition(Vec3& position){planePosition=position;}
Vec3 plane::getPosition(){return planePosition;}

void plane::setNormal(Vec3& norm){normal=norm;}
Vec3 plane::getNormal(){return normal;}

void plane::setColour(Vec3& colr){planeBaseColour=colr;}
Vec3 plane::getColour(){return planeBaseColour;}

bool plane::hit(const ray& r, const float t0, const float t1, hit_record& rec) const{
    float denominator = normal.dot(r.getDirection());
    bool hit = false;

    Vec3 point = planePosition-r.getOrigin();

 //   float hitpoint = -((r.getOrigin()-planePosition).dot(normal)/(r.getDirection().dot(normal)));
    float hitpoint = (point.dot(normal))/denominator;

    //std :: cout << "hitpoint is: "<<hitpoint;
    //if (hitpoint> t0 && hitpoint<t1){
      if (denominator!=0 && denominator<0){
        rec.closest = hitpoint;
        rec.intersectionPoint = r.ray_at_point(hitpoint);

        rec.pointNormal = normal; //think about this
        rec.material_ptr = material_ptr;

        hit = true;

    }
    return hit;
}
