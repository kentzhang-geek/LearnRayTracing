//
// Created by kent on 2021/11/30.
//

#include "Scene.h"
#include <glm/glm.hpp>
#include "Quad.h"
#include "MathTools.h"
#include <iostream>
#include <string>
#include "Mat_Dielectrics.h"
#include "Mat_Diffuse_Lambert.h"

static void CHECK_WHY(bool cond) {
//    if (cond)
//        std::cout << "why?";
}

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


static Eigen::Vector4d
iterRay(Ray &path_out_to_here, Scene *sc, std::list<Eigen::Vector3d> &light_path, uint32_t max_deep = 50) {
    if (!max_deep) {
        return {0.0, 0.0, 0.0, 0.0};
    }
    HitObject *here;
    Eigen::Vector3d pt;
    if (sc->rayHit(path_out_to_here, here, pt)) {
        Eigen::Vector4d ret = Eigen::Vector4d::Zero();
        // compute direct illumination
        Ray rayout;
        rayout.origin = pt;
        rayout.dir = -path_out_to_here.dir;
        ret = sc->computeLight(here, rayout, pt);
        // russian
        if (MathTools::rand_01() > sc->russian_stop_gate)
            return ret;
        light_path.push_back(pt);
        // iteration
        Ray path_here_to_other;
        Eigen::Vector4d attetion_another;
        if (here->material->scatter(path_out_to_here, pt, here, attetion_another, path_here_to_other)) {
            Eigen::Vector4d down_level = iterRay(path_here_to_other, sc, light_path, max_deep - 1);
            Eigen::Vector4d down_l_o = color_mult(attetion_another, down_level);
            double rcp_pdf = 2.0 * M_PI;
            double cosTheta = here->normalAtPoint(pt).dot(path_here_to_other.dir);
            ret += down_l_o / sc->russian_stop_gate * cosTheta * rcp_pdf;
        }
        return ret;
    }
    return Eigen::Vector4d::Zero();
}

Eigen::Vector4d Scene::computeLight(HitObject *hp, const Ray &ray_out, Eigen::Vector3d pos) {
    // choice one light source
    int light_idx = lights.size() * std::floor(MathTools::rand_01());
    Quad *only_quad_now = dynamic_cast<Quad *>(lights[light_idx].get());
    Eigen::Vector3d dA_light = only_quad_now->randomPick_dA();
    // visible test
    Ray r;      // ray in test
    r.origin = pos;
    r.dir = (dA_light - pos).normalized();
    HitObject *check = nullptr;
    Eigen::Vector3d pttest;
    if (rayHit(r, check, pttest) && (check == only_quad_now)) {
        // pass visible test
        Eigen::Vector4d attention;
        r.origin = dA_light;    // now is from light to this
        r.dir = (pos - dA_light).normalized();
        if (hp->material->brdf(ray_out, r, pos, hp, attention)) {
            // compute brdf result
            double receive_cosTheta = hp->normalAtPoint(pos).dot(-r.dir);
            double distance_falloff = 1.0 / std::max(1.0, pow((dA_light - ray_out.origin).norm(), 2.0));
            double light_cosTheta = r.dir.dot(only_quad_now->normalAtPoint(r.origin));
            double rcp_pdf = 1.0 / only_quad_now->area();
            return color_mult(attention, only_quad_now->emessive_intensity)
                   * std::max(0.0, receive_cosTheta)
                   * distance_falloff
                   * std::max(0.0, light_cosTheta)
                   * rcp_pdf;
        }
    }

    return Eigen::Vector4d::Zero(); // no light in
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
    std::list<Eigen::Vector3d> light_path;
    if (rayHit(r, hp, pt)) {
        return hp->emessive_intensity + iterRay(r, this, light_path);
    }
    return Eigen::Vector4d::Zero();
}
