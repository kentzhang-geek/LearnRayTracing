#include "MathTools.h"

#include <boost/random.hpp>

boost::mt19937 gen;
boost::uniform_01<boost::mt19937> dist_boost(gen);

static std::vector<double> random_nums;
static int rand_idx = 0;

void MathTools::prepare_rand(int random_size) {
    rand_idx = 0;
    random_nums.resize(random_size);
    // prepare random numbers
    for (int i = 0; i < random_size; i++) {
        random_nums[i] = dist_boost();
    }
}

double MathTools::rand_01() {
    return random_nums[rand_idx++ % random_nums.size()];
}

//
// Created by kent on 2021/12/11.
//

