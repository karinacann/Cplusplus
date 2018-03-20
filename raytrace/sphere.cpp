#include <iostream>
#include "sphere.h"

using namespace std;
using namespace OpenGP;

sphere::sphere(){}

sphere::sphere(Vec3 position, const int rad, material *mat)
    : spherePosition(position),sphereRadius(rad), material_ptr(mat)
{}

void sphere::setPosition(Vec3& position){spherePosition = position;}
Vec3 sphere::getPosition(){return spherePosition;}

void sphere::setRadius(float radius){sphereRadius=radius;}
float sphere::getRadius(){return sphereRadius;}



// Sphere part Ref Peter Shirley for this idea of a function, and lab

bool sphere :: hit(const ray& r, const float t0, const float t1, hit_record& rec) const{
    Vec3 cameraSpot = r.getOrigin() - spherePosition;
    float a = r.getDirection().dot(r.getDirection());
    float b = cameraSpot.dot(r.getDirection());
    float c = cameraSpot.dot(cameraSpot) - sphereRadius*sphereRadius;

    float discriminent = b*b - a*c;
  // bool hit = false;
    if (discriminent>=0){//then there is an intersecting point,
        //so should find the closest hit point and add it to the record
        float hitpoint = (-b  - std::sqrtf(discriminent))/a;
       // std::cout<<"hitpoint - = "<< hitpoint<<" and hitpoint + = ";
   //std::cout<<"hitpoint is"<<hitpoint;
       //if hitpoint is positive [0,inf) compute normal, otherwise we dont care
        if (hitpoint> t0 && hitpoint<t1){
           //record the info

            rec.closest = hitpoint;
            rec.intersectionPoint = r.ray_at_point(hitpoint);
            rec.pointNormal = (rec.intersectionPoint-spherePosition)/sphereRadius; // this is the unit normal
            rec.material_ptr = material_ptr;

            //  cout<< "pointNormal is"<< rec.pointNormal[0]<<rec.pointNormal[1]<<rec.pointNormal[2];
            return true;
        }

        hitpoint = (-b + std::sqrtf(discriminent))/a;
      //  std::cout<<hitpoint<<", so we want smallest one\n";
       if (hitpoint> t0 && hitpoint<t1){
           //record the info
            rec.closest = hitpoint;
            rec.intersectionPoint = r.ray_at_point(hitpoint);
            rec.pointNormal = (rec.intersectionPoint-spherePosition)/sphereRadius;
            rec.material_ptr = material_ptr;

            return true;
        }
       // std::cout<<"rec.closest = "<<rec.closest<<"\n ";
      // cout << "pointNormal is "<<rec.pointNormal[0]<<" "<<rec.pointNormal[1]<<" "<<rec.pointNormal[2] <<"\n";

    }//else not valid

    return false;
}

/*
sphere::~sphere(){
    std::cout << "~sphere()" << std::endl;

}

*/


/*void sphere::setColour(Vec3& colour){
    sphereColour = colour;
}

Vec3 sphere::getColour(){
    return sphereColour;
}*/
