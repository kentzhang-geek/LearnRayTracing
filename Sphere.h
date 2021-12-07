//
// Created by kent on 2021/11/30.
//

#ifndef MYRAYTRACER_SPHERE_H
#define MYRAYTRACER_SPHERE_H

#include <eigen3/Eigen/Eigen>
#include "Ray.h"
#include "HitObject.h"

class Sphere : public HitObject{
public:
    Sphere() = default;
    virtual bool rayHit(const Ray &r, Eigen::Vector3d * pt_hit = nullptr);
    virtual Eigen::Vector3d normalAtPoint(Eigen::Vector3d pt) const;

    Eigen::Vector3d center;
    double radius;
};


#endif //MYRAYTRACER_SPHERE_H
