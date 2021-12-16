//
// Created by kent on 2021/12/8.
//

#ifndef MYRAYTRACER_MATHTOOLS_H
#define MYRAYTRACER_MATHTOOLS_H

#include <eigen3/Eigen/Eigen>
#include <vector>
#include <string>

inline Eigen::Vector4d color_mult(Eigen::Vector4d c1, Eigen::Vector4d c2) {
    return {c1.x() * c2.x(), c1.y() * c2.y(), c1.z() * c2.z(), c1.w() * c2.w()};
}

namespace MathTools {
    double rand_01();
    void prepare_rand(int random_size);
    Eigen::Vector3d reflect(Eigen::Vector3d in, Eigen::Vector3d normal);
    bool refract(Eigen::Vector3d vi, Eigen::Vector3d n, double ni_over_nt, Eigen::Vector3d &outray);
    std::string to_string(Eigen::Vector3d v);
    Eigen::Vector3d random_unit_sphere();
    bool CheckInRange(double v, double min_v, double max_v);
}

#endif //MYRAYTRACER_MATHTOOLS_H
