//
// Created by kent on 2021/12/7.
//

#include "HitObject.h"
#include "Mat_Diffuse_Lambert.h"
#include "Mat_PBR_CookTorrance.h"

HitObject::HitObject() {
    material = std::shared_ptr<Mat_PBR_CookTorrance>(new Mat_PBR_CookTorrance());
}
