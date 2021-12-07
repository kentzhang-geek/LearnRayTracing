//
// Created by kent on 2021/11/30.
//

#include "Scene.h"

bool Scene::rayHit(const Ray &ray, HitObject * &hitObject, Eigen::Vector3d &pos) {
    for (auto oit: objects) {
        if (oit->rayHit(ray, &pos)) {
            hitObject = oit.get();
            return true;
        }
    }
    return false;
}

Ray Scene::rayAtPixel(double x, double y) {
    Ray r;
    // get the plane and range
    double raydis = 1.0;
    Eigen::Vector3d center = cam_pos + cam_dir * raydis;
    Eigen::Vector3d up(0.0, 1.0, 0.0);
    Eigen::Vector3d right = cam_dir.cross(up);
    up = right.cross(cam_dir);

    Eigen::Vector2d  xy_normed = pixelToNormalized(x,y);

    Eigen::Vector3d hit_at_one = center
            + right * xy_normed.x() * xMaxAtDistance(1.0)
            + up * xy_normed.y() * yMaxAtDistance(1.0);

    r.origin = cam_pos;
    r.dir = (hit_at_one - cam_pos).normalized();

    return r;
}

Eigen::Vector2d Scene::pixelToNormalized(double x, double y) {
    double ratio_x = (x / pixels_width) * 2.0 - 1.0;
    double ratio_y = (y / pixels_height) * 2.0 - 1.0;
    return Eigen::Vector2d(ratio_x, ratio_y);
}

double Scene::xMaxAtDistance(double dis) {
    return std::tan(fovy / 2.0) * dis;
}

double Scene::yMaxAtDistance(double dis) {
    double ratio = pixels_height / pixels_width;
    return xMaxAtDistance(dis) * ratio;
}
