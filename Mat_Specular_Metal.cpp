//
// Created by kent on 2021/12/13.
//

#include "Mat_Specular_Metal.h"
#include "MathTools.h"

bool
Mat_Specular_Metal::scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector3d &attenuation,
                            Ray &out) {
    // random sphere unit
    out.origin = hit_pos;
    Eigen::Vector3d ref = MathTools::reflect(in.dir, hp->normalAtPoint(hit_pos)).normalized();
    out.dir = (ref + MathTools::random_unit_sphere() * fussy).normalized();
    double cosTheta = out.dir.dot(hp->normalAtPoint(hit_pos));
    if (cosTheta < 0.01) {
        return false;
    }
    Eigen::Vector3d half_plane = ((out.dir + -in.dir) / 2.0).normalized();
    attenuation = albedo * pow(std::max(0.0, half_plane.dot(hp->normalAtPoint(hit_pos))), 10.0);
    return true;
}

bool Mat_Specular_Metal::brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
                              Eigen::Vector3d &attenuation) {
    double cosTheta = ray_out.dir.dot(hp->normalAtPoint(hit_pos));
    if (cosTheta < 0.01) {
        return false;
    }
    Eigen::Vector3d ref = MathTools::reflect(ray_in.dir, hp->normalAtPoint(hit_pos)).normalized();
    Eigen::Vector3d half_plane = ((ray_out.dir + -ray_in.dir) / 2.0).normalized();
    attenuation = albedo * pow(std::max(0.0, half_plane.dot(hp->normalAtPoint(hit_pos))), 10.0);
    return true;
}

Mat_Specular_Metal::Mat_Specular_Metal(const Eigen::Vector3d &albedo, double fussy) : albedo(albedo), fussy(fussy) {
    if (this->fussy > 1.0)
        this->fussy = 1.0;
}
