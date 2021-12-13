//
// Created by kent on 2021/12/7.
//

#include "HitObject.h"
#include "Mat_Diffuse_Lambert.h"

HitObject::HitObject() {
    material = std::shared_ptr<Mat_Diffuse_Lambert>(new Mat_Diffuse_Lambert({1.0, 1.0, 1.0, 1.0}));
}
