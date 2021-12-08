//
// Created by kent on 2021/12/8.
//

#ifndef MYRAYTRACER_MATHTOOLS_H
#define MYRAYTRACER_MATHTOOLS_H

#include <eigen3/Eigen/Eigen>

inline Eigen::Vector4d color_mult(Eigen::Vector4d c1, Eigen::Vector4d c2) {
    return {c1.x() * c2.x(), c1.y() * c2.y(), c1.z() * c2.z(), c1.w() * c2.w()};
}

#endif //MYRAYTRACER_MATHTOOLS_H
