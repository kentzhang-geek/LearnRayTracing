//
// Created by kent on 2021/12/13.
//

#ifndef MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H
#define MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H

#include "HitObject.h"

class Mat_Diffuse_Lambert : public Material {
public:
    Mat_Diffuse_Lambert(const Eigen::Vector3d &albedo = {1.0, 1.0, 1.0});

    Eigen::Vector3d albedo;
    bool scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector3d &attenuation,
                 Ray &out) override;

    bool brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
              Eigen::Vector3d &attenuation) override;

};


#endif //MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H
