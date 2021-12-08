//
// Created by kent on 2021/12/7.
//

#include "Quad.h"
#include <boost/random.hpp>

boost::mt19937 gen;
boost::uniform_01<boost::mt19937> dist_boost(gen);

#define RANDOM_NUM_LEN 1000

bool Quad::rayHit(const Ray &r, Eigen::Vector3d *pt_hit) {
    if (two_triangle[0].rayHit(r, pt_hit))
        return true;
    if (two_triangle[1].rayHit(r, pt_hit))
        return true;
    return false;
}

Eigen::Vector3d Quad::randomPick_dA() {
    return center + x * (random_nums[rand_idx++ % RANDOM_NUM_LEN]) + y * (random_nums[rand_idx++ % RANDOM_NUM_LEN]);
}

Eigen::Vector3d Quad::normalAtPoint(Eigen::Vector3d pt) const {
    return two_triangle[0].normal();
}

std::shared_ptr<Quad> Quad::quick_by_center(Eigen::Vector3d center, Eigen::Vector3d x, Eigen::Vector3d y) {
    std::shared_ptr<Quad> q(new Quad);
    q->two_triangle[0].vertex[0] = center - x - y;
    q->two_triangle[0].vertex[1] = center + x - y;
    q->two_triangle[0].vertex[2] = center + x + y;

    q->two_triangle[1].vertex[0] = center - x - y;
    q->two_triangle[1].vertex[1] = center + x + y;
    q->two_triangle[1].vertex[2] = center - x + y;

    q->center = center;
    q->x = x;
    q->y = y;
    return q;
}

Quad::Quad() {
    // prepare random numbers
    for (int i = 0; i < RANDOM_NUM_LEN; i++) {
        this->random_nums.push_back(dist_boost() * 2.0 - 1.0);
    }
}
