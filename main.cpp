#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "Ray.h"
#include "Scene.h"

#include <iostream>
#include <gli/gl.hpp>
#include <gli/gli.hpp>
#include "Quad.h"
#include "ThreadPool.h"
#include "MathTools.h"


//void SetPixel(CImg<float> *img, int x, int y, Eigen::Vector4d color, int color_max = 255) {
//    (*img)(x, y, 0, 0) = color.x();
//    (*img)(x, y, 0, 1) = color.y();
//    (*img)(x, y, 0, 2) = color.z();
//    (*img)(x, y, 0, 3) = color.w();
//}

int main() {
    // init random
    MathTools::prepare_rand(1000);
    const int xmax = 800;
    const int ymax = 400;
    gli::texture2d::extent_type dms;
    gli::texture2d willsave(gli::format::FORMAT_RGBA32_SFLOAT_PACK32,
                            gli::extent2d(xmax, ymax),
                            1
    );

    Eigen::Vector4d simple_light = {1.0, 1.00, 1.0, 1.0};
    // construct scene one sphere
    Scene scene;
    scene.pixels_width = xmax;
    scene.pixels_height = ymax;
    scene.cam_pos = {-4.0, 0.0, 0.0};
    scene.cam_dir = {1.0, 0.0, 0.0};

    Sphere *test = new Sphere;
    test->center = {5.0, -2.0, 0.0};
    test->radius = 1.0;
    scene.objects.push_back(std::shared_ptr<HitObject>(test));
    test = new Sphere;
    test->center = {5.0, -2.0, 2.0};
    test->radius = 1.0;
    scene.objects.push_back(std::shared_ptr<HitObject>(test));

    double quad_size = 3.0;
    auto quad = Quad::quick_by_center({5.0, 0.0, -quad_size}, {quad_size, 0.0, 0.0}, {0.0, quad_size, 0.0});
    quad->albedo = {1.0, 0.0, 0.0, 1.0};
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({7.0, 0.0, 0.0}, {0.0, 0.0, quad_size}, {0.0, quad_size, 0.0});
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, 0.0, quad_size}, {-quad_size, 0.0, 0.0}, {0.0, quad_size, 0.0});
    quad->albedo = {0.0, 1.0, 0.0, 1.0};
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, -quad_size, 0.0}, {quad_size, 0.0, 0.0}, {0.0, 0.0, -quad_size});
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, quad_size, 0.0}, {quad_size, 0.0, 0.0}, {0.0, 0.0, quad_size});
    scene.objects.push_back(quad);

    // light
    double light_size = 0.5;
    quad = Quad::quick_by_center({5.0, quad_size - 0.3, 0.0}, {light_size, 0.0, 0.0}, {0.0, 0.0, light_size});
    quad->emessive_intensity = {10.0, 10.0, 10.0, 1.0};
    quad->isLight = true;
    scene.lights.push_back(quad);

    // multisample offset
    Eigen::Vector2d offsets[4] = {
            {-0.33, -0.33},
            {-0.33, 0.33},
            {0.33,  0.33},
            {0.33,  -0.33},
    };

    ThreadPool tp(12);
    std::list<std::future<void>> results;
    int count = 0;

    for (int x = 0; x < xmax; x++) {
        for (int y = 0; y < ymax; y++) {
            std::future<void> f = tp.enqueue([x, y, &count, &scene, &simple_light, &willsave, offsets]() {
                Eigen::Vector4d color = Eigen::Vector4d::Zero();
                for (auto offset: offsets) {
                    Ray r = scene.rayAtPixel(x + offset.x(), y + offset.y());
                    color += scene.computeLo(r);
                }
                std::cout << "Done Ray " << std::to_string(count++) << std::endl;
                color = color / 4.0;
                willsave.store(gli::extent2d(x, y), 0, glm::vec4(color.x(), color.y(), color.z(), color.w()));
                return;
            });
            results.push_back(std::move(f));
        }
    }
    for (auto &fit: results)
        fit.wait();

    save_dds(willsave, "test.dds");
    return 0;
}