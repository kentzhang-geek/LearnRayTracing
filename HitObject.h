//
// Created by kent on 2021/12/7.
//

#ifndef MYRAYTRACER_HITOBJECT_H
#define MYRAYTRACER_HITOBJECT_H

#include <eigen3/Eigen/Eigen>
#include "Ray.h"

class HitObject {
public:
    virtual bool rayHit(const Ray &r, Eigen::Vector3d * pt_hit = nullptr) = 0;
    virtual Eigen::Vector3d normalAtPoint(Eigen::Vector3d pt) const = 0;
};


#endif //MYRAYTRACER_HITOBJECT_H
