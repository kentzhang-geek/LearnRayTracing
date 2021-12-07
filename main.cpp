#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "Ray.h"
#include "Scene.h"

#include <iostream>
#include <gli/gl.hpp>
#include <gli/gli.hpp>


//void SetPixel(CImg<float> *img, int x, int y, Eigen::Vector4d color, int color_max = 255) {
//    (*img)(x, y, 0, 0) = color.x();
//    (*img)(x, y, 0, 1) = color.y();
//    (*img)(x, y, 0, 2) = color.z();
//    (*img)(x, y, 0, 3) = color.w();
//}

int main() {
    const int xmax = 800;
    const int ymax = 400;
    gli::texture2d::extent_type dms;
    gli::texture2d willsave(gli::format::FORMAT_RGBA32_SFLOAT_PACK32,
                            gli::extent2d (xmax, ymax),
                            1
    );

    // construct scene one sphere
    Scene scene;
    scene.pixels_width = xmax;
    scene.pixels_height = ymax;
    scene.cam_pos = {0.0, 0.0, 0.0};
    scene.cam_dir = {1.0, 0.0, 0.0};

    Sphere *test = new Sphere;
    test->center = {10.0, 0.0, 0.0};
    test->radius = 1.0;
    scene.objects.push_back(std::shared_ptr<HitObject>(test));

    // multisample offset
    Eigen::Vector2d offsets[4] = {
            {-0.33, -0.33},
            {-0.33, 0.33},
            {0.33,  0.33},
            {0.33,  -0.33},
    };

    for (int x = 0; x < xmax; x++) {
        for (int y = 0; y < ymax; y++) {
            double total = 0.0;
            Eigen::Vector4d color = Eigen::Vector4d::Zero();
//            for (auto offset: offsets) {
//                Ray r = scene.rayAtPixel(x + offset.x(), y + offset.y());
//                Eigen::Vector3d test;
//                HitObject * hp;
//                if (scene.rayHit(r, hp, test)) {
//                    total += 1.0;
//                }
//            }
//            if (total > 0.1) {
            Ray r = scene.rayAtPixel(x, y);
            HitObject *hp;
            Eigen::Vector3d pt;
            if (scene.rayHit(r, hp, pt)) {
                pt = hp->normalAtPoint(pt);
//                color = Eigen::Vector4d(1, 1, 1, 1.0);
                color = Eigen::Vector4d(pt.x(), pt.y(), pt.z(), 1.0);
            }
            willsave.store(gli::extent2d(x,y), 0, glm::vec4(color.x(), color.y(), color.z(), color.w()));
        }
    }
    save_dds(willsave, "test.dds");
    return 0;
}