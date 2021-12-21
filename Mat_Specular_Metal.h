//
// Created by kent on 2021/12/13.
//

#ifndef MYRAYTRACER_MAT_SPECULAR_METAL_H
#define MYRAYTRACER_MAT_SPECULAR_METAL_H

#include "HitObject.h"

class Mat_Specular_Metal : public Material{
public:
    Mat_Specular_Metal(const Eigen::Vector4d &albedo, double fussy = 0.01);

    bool scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                 Ray &out) override;

    bool brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
              Eigen::Vector4d &attenuation) override;

    Eigen::Vector4d albedo;
    double fussy = 0.0;
};


#endif //MYRAYTRACER_MAT_SPECULAR_METAL_H
