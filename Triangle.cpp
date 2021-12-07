//
// Created by kent on 2021/12/7.
//

#include "Triangle.h"

bool Triangle::rayHit(const Ray &r, Eigen::Vector3d *pt_hit) {
    Eigen::Vector3d nor = normal();
    if (r.dir.dot(-nor) < 0.0) {
        return false;
    }
    double o_dis_tr = (vertex[0] - r.origin).dot(-nor);
    double dir_proj = r.dir.dot(-nor);
    Eigen::Vector3d pt = r.point_at_parameter(o_dis_tr / dir_proj);
    if (PointInTriangle(pt)) {
        *pt_hit = pt;
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

    auto ab = pta.cross(ptb);
    auto bc = ptb.cross(ptc);
    auto ca = ptc.cross(pta);

    return ab.dot(bc) > 0 && bc.dot(ca) > 0.0;
}
