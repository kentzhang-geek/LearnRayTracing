//
// Created by kent on 2021/12/22.
//

#ifndef MYRAYTRACER_MAT_PBR_COOKTORRANCE_H
#define MYRAYTRACER_MAT_PBR_COOKTORRANCE_H

#include "HitObject.h"

class Mat_PBR_CookTorrance : public Material {
public:
    Mat_PBR_CookTorrance(const Eigen::Vector3d &albedo = Eigen::Vector3d::Ones(), double roughness = 1.0, double metalness = 0.0);

    Eigen::Vector3d albedo = Eigen::Vector3d::Ones();
    double roughness = 1.0;
    double metalness = 0.0;
    double fresnel_0 = 0.04;

    bool scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                 Ray &out) override;

    bool brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
              Eigen::Vector4d &attenuation) override;

};


#endif //MYRAYTRACER_MAT_PBR_COOKTORRANCE_H
