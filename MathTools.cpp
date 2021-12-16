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

bool MathTools::refract(Eigen::Vector3d vi, Eigen::Vector3d n, double ni_over_nt, Eigen::Vector3d &outray) {
    vi = vi.normalized();
    n = n.normalized();
    double check_sin_out = (1.0 - n.dot(-vi) * n.dot(-vi)) * ni_over_nt * ni_over_nt;
    if (check_sin_out >= 1.0)
    {
        return false;
    }
    Eigen::Vector3d sinVinVec = vi + n.dot(-vi) * n;
    double sinVin = sinVinVec.norm();
    double sinVout = ni_over_nt * sinVin;
    double cosVout = sqrt(1.0 - sinVout * sinVout);
    outray = -n * cosVout + sinVinVec.normalized() * sinVout;
//    std::cout << "Fraction " << MathTools::to_string(vi) << " to " << MathTools::to_string(outray) << std::endl;
    return true;
}

std::string MathTools::to_string(Eigen::Vector3d v) {
    return std::string("(") + std::to_string(v.x()) + ", " + std::to_string(v.y()) + ", " + std::to_string(v.z()) + ")";
}

bool MathTools::CheckInRange(double v, double min_v, double max_v) {
    return (v >= min_v) && (v <= max_v);
}

std::string MathTools::to_string(std::list<Eigen::Vector3d> lpath) {
    std::string lp = "LP {";
    for (auto p : lpath) {
        lp += to_string(p);
        lp += ", ";
    }
    return lp + "}";
}

//
// Created by kent on 2021/12/11.
//

