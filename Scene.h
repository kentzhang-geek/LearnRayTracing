//
// Created by kent on 2021/11/30.
//

#ifndef MYRAYTRACER_SCENE_H
#define MYRAYTRACER_SCENE_H

#include <eigen3/Eigen/Eigen>
#include "HitObject.h"
#include "Sphere.h"
#include "Ray.h"
#include <memory>

/**
 * (0, pixels_height)
 *  ^
 *  |
 *  |
 *  +---------> (pixels_width, 0.0)
 * (0,0)
 */
class Scene {
public:
    Scene() = default;

    Eigen::Vector4d computeLo(Ray &r);

    bool rayHit(const Ray &ray, HitObject *&hitObject, Eigen::Vector3d &pos);

    Eigen::Vector4d directLightDiffuse(HitObject *hp, Eigen::Vector3d pos);

    Ray rayAtPixel(double x, double y);

    Eigen::Vector2d pixelToNormalized(double x, double y);

    double xMaxAtDistance(double dis = 1.0);

    double yMaxAtDistance(double dis = 1.0);

    double fovy = 106.0; // degree
    Eigen::Vector3d cam_pos = Eigen::Vector3d(0.0, 0.0, 0.0);
    Eigen::Vector3d cam_dir = Eigen::Vector3d(1.0, 0.0, 0.0);

    std::vector<std::shared_ptr<HitObject>> lights;
    std::list<std::shared_ptr<HitObject>> objects;
    double pixels_width;
    double pixels_height;
};


#endif //MYRAYTRACER_SCENE_H
