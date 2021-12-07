//
// Created by kent on 2021/11/30.
//

#include "Sphere.h"

bool Sphere::rayHit(const Ray &ray, Eigen::Vector3d *pt_hit) {
    double projed = ray.dir.dot(this->center - ray.origin);
    if (projed > 0) {
        double side = (this->center - ray.origin).norm();
        double len = sqrt(side * side - projed * projed);
        if (len < this->radius) {
            if (pt_hit) {
                // compute point center
                Eigen::Vector3d cpoint = ray.point_at_parameter((this->center - ray.origin).dot(ray.dir));
                double sidelen = (cpoint - this->center).norm();
                double disalter = sqrt(this->radius * this->radius - sidelen * sidelen);
                *pt_hit = cpoint - ray.dir * std::max(0.0, disalter);
            }
            return true;
        }
    }
    return false;
}

Eigen::Vector3d Sphere::normalAtPoint(Eigen::Vector3d pt) const {
    return (pt - center).normalized();
}
