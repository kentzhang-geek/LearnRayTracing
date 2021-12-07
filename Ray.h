//
// Created by kent on 2021/11/30.
//

#ifndef MYRAYTRACER_RAY_H
#define MYRAYTRACER_RAY_H

#include <eigen3/Eigen/Eigen>

class Ray {
public:
    Ray() {};

    Ray(const Eigen::Vector3d &origin, const Eigen::Vector3d &dir) : origin(origin), dir(dir.normalized()) {}

    Eigen::Vector3d point_at_parameter(double t) const { return origin + t * dir; } ;

    Eigen::Vector3d origin;
    Eigen::Vector3d dir;
};


#endif //MYRAYTRACER_RAY_H
