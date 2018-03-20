#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "sphere.h"
#include "ray.h"
#include "hitable.h"
#include "hitable_list.h"
#include "plane.h"
#include "material.h"
#include "metal.h"
#include "camera.h"

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Color red(){return Colour (1.0f, 0.0f, 0.0f);}

/*Peter Shirley, Ray Tracing in One Weekend*/
Vec3 get_color (const ray& r, hitable* environment, int depth){
    hit_record rec;

    if (environment->hit(r,0.001, MAXFLOAT,rec)){

       ray scattered;
       Vec3 attenuation;

            if (((depth<50))&& rec.material_ptr->scatter(r,rec,attenuation,scattered)){
                Vec3 nextColor = get_color(scattered, environment, depth+1);

                  return Vec3((attenuation[0]*nextColor[0]),(attenuation[1]*nextColor[1]),(attenuation[2]*nextColor[2]));

            }else{
                   return Vec3(0,0,0);
                   //return 0.5*Vec3(rec.pointNormal[0]+1,rec.pointNormal[1]+1, rec.pointNormal[2]+1);
            }

    }else{
        Vec3 unit_direction = (r.getDirection()).normalized();
        float t = 0.5*(unit_direction[1]+1.0);
        return (1.0-t)*Vec3(1.0f,1.0f,1.0f) + t*Vec3(0.5,0.7,1.0);
    }

}

int main(int, char**){

    //set resolution
    int wResolution = 80;
    int hResolution = 60;
    float aspectRatio = float(wResolution) / float(hResolution);

    Image<Colour> image(hResolution, wResolution);

    //set bases
    Vec3 W = Vec3(0.0f, 0.0f, -1.0f); //into the screen direction
    Vec3 V = Vec3(0.0f, 1.0f, 0.0f); // upward direction
    Vec3 U = Vec3(1.0f, 0.0f, 0.0f); // right direction

    float d = 1.0f; //distance, focal length between origin and image plane
    Vec3 E = -d*W; //E is Eye is the origin I thought...

    //build image plane screen
    float left = -1.0f*aspectRatio;
    float right = 1.0f*aspectRatio;
    float bottom = -1.0f;
    float top = 1.0f;

    //build scene: list of objects to intersect
    hitable *list[3];

    std::cout<<"before list";
    list[0] = new sphere (Vec3(-1,2.0,-4), 2.0, new metal(Vec3(0.8,0.6,0.2)));
    list[1] = new sphere (Vec3(1,0,-4), 1.5, new metal(Vec3(0.8,0.8,0.8)));
    list[2] = new plane (Vec3(0.0,-2.0,0.0),Vec3(0.0,1.0,0.0), new metal(Vec3(0.8,0.8,0.8)));

   hitable *environment = new hitable_list(list,3);
std::cout<<"world is made...";
    ///Light source
    Vec3 lightPos = Vec3(0.0f, 4.0f, 0.0f);
    float lightInt = 1.0f; //light intensity

    //find hit points of each object
   std::cout<<"hello?";
    for (int row = 0; row < image.rows(); ++row) {
     std:: cout<<"row "<<row<<"\n";
        for (int col = 0; col < image.cols(); ++col) {
            //compute u,v pixel position

            Vec3 pixel = left*U + ((col)*(right-left)/image.cols())*U;
            pixel += bottom*V + (row*(top-bottom)/image.rows())*V;

            //compute ray
          ray r0(E, (pixel - E));// ray with origin E and rayDirection pixel - E

           hit_record record, shadowrec;
         camera cam;
            Vec3 color = Vec3(0,0,0);
 /*          for (int s=0; s< hResolution;s++){
                float u = float(col + drand48())/image.cols();
                float v = float(row + drand48())/image.rows();

              ray r = cam.avgRay(u,v);
              Vec3 p = r.ray_at_point(2.0);

             Vec3 po = r0.ray_at_point(2.0);
               color+= get_color(r,environment,0);

           }

            color/=float(wResolution);
           color =Vec3(std::sqrt(color[0]), std::sqrt(color[1]),std::sqrt(color[2]));


*/
            if (environment->hit(r0,0.0, MAXFLOAT,record)){

                //must be unit vectors, which they are.
                ///****************

                Vec3 lightDirN = (lightPos - record.intersectionPoint).normalized();
                Vec3 point = r0.ray_at_point(record.closest);

                Vec3 ambient = 0.5*Vec3(0.6,0.3,0.3);

                ray shadow = ray(point, (lightPos-point));

                if(!(environment->hit(shadow,0.001,MAXFLOAT,shadowrec))){
                   //new vector with intersection point as origin, and camera-sphere

                    Vec3 shadowDirectionN= shadow.getDirection().normalized();
                    Vec3 rayDirectionN= -r0.getDirection().normalized(); //(-d)
                    Vec3 h = (shadowDirectionN+rayDirectionN).normalized();

                   //  Vec3 h = (lightDir +r0.getDirection()).normalized();

                    float maxNDotH = std::fmaxf(0.0,record.pointNormal.dot(h));

                     Vec3 lambert = lightInt*red()*(std::fmaxf(0.0, record.pointNormal.dot(lightDirN)));
                     Vec3 specular = lightInt*Vec3(0.5,0.5,0.5)*(std::pow(maxNDotH,100));
                //  Vec3 ambient = 0.5*Vec3(0.6,0.3,0.3); //environment.getColor

                    color = lambert+specular+ambient;
                }


                ////*******************************
              //just need to change the red() to the shape color somehow. environment.getColor()
               // std:: cout << "the vector to be printed is ("<<result[0]<<","<<result[1]<<","<<result[2]<<")";


               image(row,col) = color;//extend to handle N light sources! pg 84

           //maybe still too much lambert
           }else {
                float t = 0.5*(pixel[1]+1.0);
                Vec3 blended_value = (1.0-t)*Vec3(1.0f,1.0f,1.0f) + t*Vec3(0.5,0.7,1.0);
                image(row,col) = Colour(blended_value[0], blended_value[1], blended_value[2]);
            }

       }
    }

    bmpwrite("../../out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}


