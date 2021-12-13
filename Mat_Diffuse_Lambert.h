//
// Created by kent on 2021/12/13.
//

#ifndef MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H
#define MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H

#include "HitObject.h"

class Mat_Diffuse_Lambert : public Material {
public:
    Mat_Diffuse_Lambert(const Eigen::Vector4d &albedo);

    Eigen::Vector4d albedo;
    bool scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                 Ray &out) override;

};


#endif //MYRAYTRACER_MAT_DIFFUSE_LAMBERT_H
