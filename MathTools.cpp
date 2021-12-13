#include "MathTools.h"

#include <boost/random.hpp>

boost::mt19937 gen;
boost::uniform_01<boost::mt19937> dist_boost(gen);

static double *random_nums = nullptr;
static int rand_len = 0;
static int rand_idx = 0;

void MathTools::prepare_rand(int random_size) {
    rand_idx = 0;
    rand_len = random_size;
    if (random_nums)
        delete random_nums;
    random_nums = new double[random_size];
    // prepare random numbers
    for (int i = 0; i < random_size; i++) {
        random_nums[i] = dist_boost();
    }
}

double MathTools::rand_01() {
    return random_nums[rand_idx++ % rand_len];
}

Eigen::Vector3d MathTools::random_unit_sphere() {
    Eigen::Vector3d ret;
    do {
        ret = {MathTools::rand_01() * 2.0 - 1.0, MathTools::rand_01() * 2.0 - 1.0, MathTools::rand_01() * 2.0 - 1.0};
    } while (ret.norm() > 1.0);
    return ret;
}

Eigen::Vector3d MathTools::reflect(Eigen::Vector3d in, Eigen::Vector3d normal) {
    return in - 2 * in.dot(normal) * normal;
}

//
// Created by kent on 2021/12/11.
//

