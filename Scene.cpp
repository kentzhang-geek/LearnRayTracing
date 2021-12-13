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


static Eigen::Vector4d iterRay(Ray &r, Scene *sc, uint32_t max_deep = 10) {
    if (!max_deep)
        return {0.0, 0.0, 0.0, 0.0};
    HitObject *hp;
    Eigen::Vector3d pt;
    if (sc->rayHit(r, hp, pt)) {
        Ray scattered;
        Eigen::Vector4d att;
        Eigen::Vector4d ret = Eigen::Vector4d::Zero();
        if (hp->material->scatter(r, pt, hp, att, scattered))
        {
            ret = color_mult(iterRay(scattered, sc, max_deep - 1), att);
        }
        ret *= 0.5;
        ret.w() = 1.0;
        return ret;
    } else {
        double t = r.dir.y() * 0.5 + 0.5;
        double t1 = 1.0 - t;
        return {
                1.0,
                1.0,
                1.0,
                1.0
        };
        return {
                t1 + 0.5 * t,
                t1 + 0.7 * t,
                t1 + 1.0 * t,
                1.0
        };
    }
}

Eigen::Vector4d Scene::computeLight(HitObject *hp, const Ray &ray_in, Eigen::Vector3d pos) {
    // ?->Eye
    Ray out;
    Eigen::Vector4d attenion;
    hp->material->scatter(ray_in, pos, hp, attenion, out);

    return color_mult(iterRay(out, this), attenion);
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
        return hp->emessive_intensity + computeLight(hp, r, pt);
    }
    return Eigen::Vector4d::Zero();
}
