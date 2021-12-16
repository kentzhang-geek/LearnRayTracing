//
// Created by kent on 2021/12/7.
//

#include "Triangle.h"
#include "MathTools.h"
#define EPSILON 0.00001

bool Triangle::rayHit(const Ray &r, Eigen::Vector3d *pt_hit) {
    Eigen::Vector3d E1 = vertex[1] - vertex[0];
    Eigen::Vector3d E2 = vertex[2] - vertex[0];
    Eigen::Vector3d S = r.origin - vertex[0];
    Eigen::Vector3d S1 = r.dir.normalized().cross(E2);
    Eigen::Vector3d S2 = S.cross(E1);
    double s1_e1 = S1.dot(E1);
    if (s1_e1 > -EPSILON) {
        s1_e1 = std::max(abs(s1_e1), EPSILON);
    } else {
        s1_e1 = -std::max(abs(s1_e1), EPSILON);
    }
    Eigen::Vector3d tbb = {
            S2.dot(E2) / s1_e1,
            S1.dot(S) / s1_e1,
            S2.dot(r.dir.normalized()) / s1_e1,
    };
    if (tbb.x() >= -EPSILON && tbb.y() >= -EPSILON && tbb.z() >= -EPSILON && (1 - tbb.y() - tbb.z()) >= -EPSILON) {
        if (pt_hit) {
            *pt_hit = r.point_at_parameter(tbb.x());
        }
        return true;
    }
    return false;
}

Eigen::Vector3d Triangle::normalAtPoint(Eigen::Vector3d pt) const {
    return normal();
}

Eigen::Vector3d Triangle::normal() const {
    auto dira = vertex[2] - vertex[1];
    auto dirb = vertex[0] - vertex[1];
    return dira.cross(dirb).normalized();
}

bool Triangle::PointInTriangle(Eigen::Vector3d pt) const {
    auto pta = (vertex[0] - pt);
    auto ptb = (vertex[1] - pt);
    auto ptc = (vertex[2] - pt);

    if ((pta + ptb).norm() < ((vertex[0] - vertex[1]).norm() + 0.0001)) return true;
    if ((pta + ptc).norm() < ((vertex[0] - vertex[2]).norm() + 0.0001)) return true;
    if ((ptc + ptb).norm() < ((vertex[2] - vertex[1]).norm() + 0.0001)) return true;

    auto ab = pta.cross(ptb);
    auto bc = ptb.cross(ptc);
    auto ca = ptc.cross(pta);

    return ab.dot(bc) > 0 && bc.dot(ca) > 0.0;
}
