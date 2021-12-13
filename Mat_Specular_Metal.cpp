//
// Created by kent on 2021/12/13.
//

#include "Mat_Specular_Metal.h"
#include "MathTools.h"

bool
Mat_Specular_Metal::scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                            Ray &out) {
    // random sphere unit
    out.origin = hit_pos;
    Eigen::Vector3d ref = MathTools::reflect(in.dir, hp->normalAtPoint(hit_pos)).normalized();
    out.dir = (ref + MathTools::random_unit_sphere() * fussy).normalized();
    double cosTheta = out.dir.dot(hp->normalAtPoint(hit_pos));
    if (cosTheta < 0.01) {
        return false;
    }
    attenuation = albedo * cosTheta * ref.dot(out.dir);
    return true;
}

Mat_Specular_Metal::Mat_Specular_Metal(const Eigen::Vector4d &albedo, double fussy) : albedo(albedo), fussy(fussy) {
    if (this->fussy > 1.0)
        this->fussy = 1.0;
}
