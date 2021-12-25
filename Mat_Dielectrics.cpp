//
// Created by kent on 2021/12/15.
//

#include "Mat_Dielectrics.h"
#include "MathTools.h"

bool Mat_Dielectrics::scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector3d &attenuation,
                              Ray &out) {
    attenuation = {1.0, 1.0, 1.0};
    Eigen::Vector3d outray;
    Eigen::Vector3d n = hp->normalAtPoint(hit_pos);
    bool is_refract = false;
    if (in.dir.dot(n) > 0) {
        // out this
        is_refract = MathTools::refract(in.dir, -n, 1.0 / refract_idx, outray);
    } else {
        is_refract = MathTools::refract(in.dir, n, refract_idx, outray);
    }
    out.origin = hit_pos;
    if (is_refract)
    {
        out.dir = outray;
    }
    else
    {
        out.dir = MathTools::reflect(in.dir, n);
    }

    return true;
}

Mat_Dielectrics::Mat_Dielectrics(double refractIdx) : refract_idx(refractIdx) {}
