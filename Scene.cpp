//
// Created by kent on 2021/11/30.
//

#include "Scene.h"
#include <glm/glm.hpp>
#include "Quad.h"
#include "MathTools.h"

bool Scene::rayHit(const Ray &ray, HitObject *&hitObject, Eigen::Vector3d &pos) {
    typedef std::pair<Eigen::Vector3d, std::shared_ptr<HitObject>> HitPair;
    std::vector<HitPair> hit_list;
    for (auto oit: lights) {
        Eigen::Vector3d pt;
        if (oit->rayHit(ray, &pt)) {
            hit_list.push_back({pt, oit});
        }
    }
    for (auto oit: objects) {
        Eigen::Vector3d pt;
        if (oit->rayHit(ray, &pt)) {
            hit_list.push_back({pt, oit});
        }
    }
    if (!hit_list.empty()) {
        std::sort(hit_list.begin(), hit_list.end(), [&ray](HitPair v1, HitPair v2) {
            return (v1.first - ray.origin).norm() < (v2.first - ray.origin).norm();
        });
        pos = hit_list.front().first;
        hitObject = hit_list.front().second.get();
        return true;
    }
    return false;
}

Eigen::Vector4d Scene::directLightDiffuse(HitObject *hp, Eigen::Vector3d pos) {
    Eigen::Vector4d color = Eigen::Vector4d::Zero();
    for (int i = 0; i < lights.size(); i++) {
        auto l = lights[i];
        // only about quad light now
        Quad *q = dynamic_cast<Quad *>(l.get());
        Eigen::Vector3d dApt = q->randomPick_dA();
        // visible detect
        bool visible = false;
        Ray r;
        r.origin = pos;
        r.dir = (dApt - pos).normalized();
        HitObject *hp2;
        Eigen::Vector3d vistestpt;
        if (rayHit(r, hp2, vistestpt)) {
            if (hp2 == l.get()) {
                // visible
                visible = true;
            }
        }
        // compute light
        if (visible) {
            double light_travel = (dApt - pos).norm();
            double dis_fall = 1.0 / std::max(1.0, light_travel * light_travel);
            color =
                    color_mult(hp->albedo, l->emessive_intensity) * dis_fall *
                    std::max(0.0, r.dir.dot(hp->normalAtPoint(pos))) //brdf;
                    * (r.dir.dot(-l->normalAtPoint(dApt)) > 0.01 ? 1.0 : 0.0) // back light
                    ;
        }
    }
    return color;
}

Ray Scene::rayAtPixel(double x, double y) {
    Ray r;
    // get the plane and range
    double raydis = 1.0;
    Eigen::Vector3d center = cam_pos + cam_dir * raydis;
    Eigen::Vector3d up(0.0, 1.0, 0.0);
    Eigen::Vector3d right = cam_dir.cross(up);
    up = right.cross(cam_dir);

    Eigen::Vector2d xy_normed = pixelToNormalized(x, y);

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
    return std::tan(glm::radians(fovy / 2.0)) * dis;
}

double Scene::yMaxAtDistance(double dis) {
    double ratio = pixels_height / pixels_width;
    return xMaxAtDistance(dis) * ratio;
}

Eigen::Vector4d Scene::computeLo(Ray &r) {
    HitObject *hp;
    Eigen::Vector3d pt;
    if (rayHit(r, hp, pt)) {
        Eigen::Vector3d n = hp->normalAtPoint(pt);
        return hp->emessive_intensity + directLightDiffuse(hp, pt);
    }
    return Eigen::Vector4d::Zero();
}
