//
// Created by kent on 2021/12/7.
//

#ifndef MYRAYTRACER_QUAD_H
#define MYRAYTRACER_QUAD_H

#include <eigen3/Eigen/Eigen>
#include "HitObject.h"
#include "Triangle.h"
#include <memory>

class Quad : public HitObject {
public:
    Quad();
    uint rand_idx = 0;
    std::vector<double> random_nums;

    Eigen::Vector3d randomPick_dA();
    static std::shared_ptr<Quad> quick_by_center(Eigen::Vector3d center, Eigen::Vector3d x, Eigen::Vector3d y);

    Triangle two_triangle[2];
    Eigen::Vector3d center;
    Eigen::Vector3d x;
    Eigen::Vector3d y;

    bool rayHit(const Ray &r, Eigen::Vector3d *pt_hit) override;

    Eigen::Vector3d normalAtPoint(Eigen::Vector3d pt) const override;
};


#endif //MYRAYTRACER_QUAD_H
