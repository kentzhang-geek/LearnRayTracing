//
// Created by kent on 2021/12/15.
//

#ifndef MYRAYTRACER_MAT_DIELECTRICS_H
#define MYRAYTRACER_MAT_DIELECTRICS_H

#include "HitObject.h"

class Mat_Dielectrics  : public Material{
public:
    double refract_idx = 1.0;
    Mat_Dielectrics(double refractIdx);

    bool scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                 Ray &out) override;
};


#endif //MYRAYTRACER_MAT_DIELECTRICS_H
