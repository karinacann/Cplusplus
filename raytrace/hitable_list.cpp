#include "hitable_list.h"

hitable_list::hitable_list(){}
hitable_list::hitable_list(hitable **hitlist, int length){
    list_length=length;
    list=hitlist;
}

bool hitable_list :: hit(const ray& r, float t0, float t1, hit_record& rec)const {
    //iterate through list of objects
    //if it hits, check if less than t1, and update hit point to t1 if so.
   hit_record curr_rec;
   bool hit_something = false;

   float currentClose= t1;

    for (int i = 0; i < list_length; i++){

        if (list[i]->hit(r,t0,t1,curr_rec)){
        //     std::cout<<"i="<<i<<", currentClose is"<<currentClose;
            //something has hit
            hit_something = true;

         //   std::cout<<". curr_rec.closest from hit is:"<<curr_rec.closest<<"\n";

            if (curr_rec.closest<currentClose){
                currentClose = curr_rec.closest;
                rec = curr_rec;
            }

           // std::cout<< ". updated currentClose is now"<<currentClose;
            //update new record

        }

    }
    return hit_something;
}

