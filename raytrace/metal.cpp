     #include "metal.h"


    metal::metal(const Vec3& albed) : albedo(albed)
    {

    }

    Vec3 metal:: reflect(const Vec3& vec, const Vec3& norm)const{
        return vec - 2*vec.dot(norm)*norm;
    }

    bool metal :: scatter(const ray& rayIn, const hit_record& rec, Vec3& attenuation, ray& scattered)const{
        Vec3 reflected = reflect((rayIn.getDirection()).normalized(),rec.pointNormal);
        scattered = ray(rec.pointNormal, reflected);
        attenuation = albedo;
       // std::cout<<"in metal scatter";
        return ((scattered.getDirection()).dot(rec.pointNormal)>0);

    }



