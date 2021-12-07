//
// Created by kent on 2021/12/7.
//

#ifndef MYRAYTRACER_TRIANGLE_H
#define MYRAYTRACER_TRIANGLE_H

#include "Ray.h"
#include "HitObject.h"

// right hand front side
class Triangle : public HitObject{
public:
    Eigen::Vector3d vertex[3];

    bool rayHit(const Ray &r, Eigen::Vector3d *pt_hit) override;

    Eigen::Vector3d normalAtPoint(Eigen::Vector3d pt) const override;
    Eigen::Vector3d normal() const;

    bool PointInTriangle(Eigen::Vector3d pt) const;
};


#endif //MYRAYTRACER_TRIANGLE_H
