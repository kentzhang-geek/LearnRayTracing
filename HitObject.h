//
// Created by kent on 2021/12/7.
//

#ifndef MYRAYTRACER_HITOBJECT_H
#define MYRAYTRACER_HITOBJECT_H

#include <eigen3/Eigen/Eigen>
#include "Ray.h"
#include <memory>

class HitObject;

class Mat_PBR_CookTorrance;
class Mat_Diffuse_Lambert;

typedef Mat_PBR_CookTorrance DefaultMaterial;

class Material {
public:
    virtual bool
    scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector3d &attenuation, Ray &out) = 0;

    virtual bool
    brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
         Eigen::Vector3d &attenuation) = 0;

    template<typename cast_to>
    cast_to *as() {
        return dynamic_cast<cast_to *>(this);
    }
};

class HitObject {
public:
    HitObject();

    virtual bool rayHit(const Ray &r, Eigen::Vector3d *pt_hit = nullptr) = 0;

    virtual Eigen::Vector3d normalAtPoint(Eigen::Vector3d pt) const = 0;

    std::shared_ptr<Material> material;
    Eigen::Vector3d emessive_intensity = {0.0, 0.0, 0.0};
    bool isLight = false;
};


#endif //MYRAYTRACER_HITOBJECT_H
