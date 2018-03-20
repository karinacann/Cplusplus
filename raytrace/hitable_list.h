#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class hitable_list : public hitable{
public:
    hitable_list();
    hitable_list(hitable **hitlist, int length);
    virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
    int list_length;
    hitable **list;
};



#endif // HITABLE_LIST_H
