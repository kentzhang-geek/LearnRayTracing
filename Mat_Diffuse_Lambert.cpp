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
    out.dir = MathTools::random_unit_hemisphere(hp->normalAtPoint(hit_pos));
    attenuation = albedo / M_PI;
    return true;
}

bool Mat_Diffuse_Lambert::brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
                               Eigen::Vector4d &attenuation) {
    // uniform distribute on hemisphere
    attenuation = albedo / M_PI;
    return true;
}

Mat_Diffuse_Lambert::Mat_Diffuse_Lambert(const Eigen::Vector4d &albedo) : albedo(albedo) {}
