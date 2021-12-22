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
#include "Mat_Diffuse_Lambert.h"
#include "Mat_Specular_Metal.h"
#include "Mat_Dielectrics.h"
#include "Mat_PBR_CookTorrance.h"

//void SetPixel(CImg<float> *img, int x, int y, Eigen::Vector4d color, int color_max = 255) {
//    (*img)(x, y, 0, 0) = color.x();
//    (*img)(x, y, 0, 1) = color.y();
//    (*img)(x, y, 0, 2) = color.z();
//    (*img)(x, y, 0, 3) = color.w();
//}

int main(int argc, char **argv) {
    // init random
    MathTools::prepare_rand(4096);
    const int xmax = 800;
    const int ymax = 400;
//    const int xmax = 2048;
//    const int ymax = 1024;
    gli::texture2d::extent_type dms;
    gli::texture2d willsave(gli::format::FORMAT_RGBA32_SFLOAT_PACK32,
                            gli::extent2d(xmax, ymax),
                            1
    );

    Eigen::Vector4d simple_light = {1.0, 1.00, 1.0, 1.0};
    // construct scene one sphere
    Scene scene;
//    scene.russian_stop_gate = 0.8;
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
    test->material->as<DefaultMaterial>()->roughness = 0.7;
    test->material->as<DefaultMaterial>()->metalness = 0.4;
    scene.objects.push_back(std::shared_ptr<HitObject>(test));
    test = new Sphere;
    test->center = {5.0, -2.0, -2.0};
    test->radius = 1.0;
    test->material->as<DefaultMaterial>()->roughness = 0.1;
    test->material->as<DefaultMaterial>()->metalness = 1.0;
    scene.objects.push_back(std::shared_ptr<HitObject>(test));

    double quad_size = 3.0;
    auto quad = Quad::quick_by_center({5.0, 0.0, -quad_size}, {quad_size, 0.0, 0.0}, {0.0, quad_size, 0.0});
    quad->material->as<DefaultMaterial>()->albedo = {1.0, 0.0, 0.0};
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({7.0, 0.0, 0.0}, {0.0, 0.0, quad_size}, {0.0, quad_size, 0.0});
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, 0.0, quad_size}, {-quad_size, 0.0, 0.0}, {0.0, quad_size, 0.0});
    quad->material->as<DefaultMaterial>()->albedo = {0.0, 1.0, 0.0};
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, -quad_size, 0.0}, {quad_size, 0.0, 0.0}, {0.0, 0.0, -quad_size});
    scene.objects.push_back(quad);
    quad = Quad::quick_by_center({5.0, quad_size, 0.0}, {quad_size, 0.0, 0.0}, {0.0, 0.0, quad_size});
    scene.objects.push_back(quad);

    // light
    double light_size = 0.5;
    quad = Quad::quick_by_center({5.0, quad_size - 2.0, 0.0}, {light_size, 0.0, 0.0}, {0.0, 0.0, light_size});
    double light_int = 20.0;
    quad->emessive_intensity = {light_int, light_int, light_int, 1.0};
    quad->isLight = true;
    scene.lights.push_back(quad);

    // test triangles
    Ray r;
    r.dir = {1.0, 0.0, 0.0};
    r.origin = {0.0, 0.0, 0.0};
    HitObject *htp;
    Eigen::Vector3d  tttt;
    scene.rayHit(r, htp, tttt);


    // multisample offset
    const int max_multi_sample = std::stoi(argv[1]);
#if 0
    const double offsets_len = 1;
    Eigen::Vector2d offsets[1] = {
            {0, 0}
    };
#else
    const double offsets_len = 4;
    Eigen::Vector2d offsets[4] = {
            {-0.33, -0.33},
            {-0.33, 0.33},
            {0.33,  0.33},
            {0.33,  -0.33},
    };
#endif

    ThreadPool tp(24);
    std::list<std::future<void>> results;
    int count = 0;
    bool exit_flag = true;
    std::thread counter([&exit_flag, &count, xmax, ymax] {
        while (exit_flag) {
            std::cout << "Done Ray "
                      << std::to_string((float) (count) / (float) xmax / (float) ymax * 100.0f)
                      << "%"
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    for (int x = 0; x < xmax; x++) {
        for (int y = 0; y < ymax; y++) {
            std::future<void> f = tp.enqueue(
                    [x, y, &count, &scene, &simple_light, &willsave,
                            offsets, offsets_len, max_multi_sample]() {
                        Eigen::Vector4d color = Eigen::Vector4d::Zero();
                        for (int samples = 0; samples < max_multi_sample; samples++) {
                            for (auto offset: offsets) {
                                Ray r = scene.rayAtPixel(x + offset.x(), y + offset.y());
                                color += scene.computeLo(r);
                            }
                        }
                        count++;
                        color = color / offsets_len / (double) max_multi_sample;
                        color = MathTools::Simple_ToneMapping(color);
                        willsave.store(gli::extent2d(x, y), 0, glm::vec4(color.x(), color.y(), color.z(), color.w()));
                        return;
                    });
            results.push_back(std::move(f));
        }
    }
    for (auto &fit: results)
        fit.wait();
    save_dds(willsave, "test.dds");
    std::cout << "ok" << std::endl;
    exit_flag = false;
    counter.join();

    return 0;
}