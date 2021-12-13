//
// Created by kent on 2021/12/13.
//

#include "Mat_Diffuse_Lambert.h"
#include "MathTools.h"

bool
Mat_Diffuse_Lambert::scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                             Ray &out) {
    // random sphere unit
    out.origin = hit_pos;
    out.dir = (MathTools::random_unit_sphere() + hp->normalAtPoint(hit_pos)).normalized();
    attenuation = albedo * out.dir.dot(hp->normalAtPoint(hit_pos));
    if (albedo.z() < 0.8) {
        int xx = 0.0;
        int yy = 0.0;
    }
    return true;
}

Mat_Diffuse_Lambert::Mat_Diffuse_Lambert(const Eigen::Vector4d &albedo) : albedo(albedo) {}
