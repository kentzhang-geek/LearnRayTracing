//
// Created by kent on 2021/12/22.
//

#include "Mat_PBR_CookTorrance.h"
#include "MathTools.h"


bool
Mat_PBR_CookTorrance::scatter(const Ray &in, const Eigen::Vector3d hit_pos, HitObject *hp, Eigen::Vector4d &attenuation,
                              Ray &out) {
    out.origin = hit_pos;
    out.dir = MathTools::random_unit_hemisphere(hp->normalAtPoint(hit_pos));
    Ray iray, oray;
    iray.origin = out.origin + out.dir;
    iray.dir = -out.dir;
    oray.origin = hit_pos;
    oray.dir = -in.dir;
    bool ret = brdf(oray, iray, hit_pos, hp, attenuation);
    return ret;
}

static inline Eigen::Vector3d fresnelSchlick(double cosTheta, Eigen::Vector3d F0) {
    return F0 + (Eigen::Vector3d::Ones() - F0) * pow(1.0 - cosTheta, 5.0);
}

double DistributionGGX(Eigen::Vector3d N, Eigen::Vector3d H, double roughness) {
    double a = roughness * roughness;
    double a2 = a * a;
    double NdotH = std::max(N.dot(H), 0.0);
    double NdotH2 = NdotH * NdotH;

    double nom = a2;
    double denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = M_PI * denom * denom;

    return nom / denom;
}

double GeometrySchlickGGX(double NdotV, double roughness) {
    double r = (roughness + 1.0);
    double k = (r * r) / 8.0;

    double nom = NdotV;
    double denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

double GeometrySmith(Eigen::Vector3d N, Eigen::Vector3d V, Eigen::Vector3d L, double roughness) {
    double NdotV = std::max(N.dot(V), 0.0);
    double NdotL = std::max(N.dot(L), 0.0);
    double ggx2 = GeometrySchlickGGX(NdotV, roughness);
    double ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

bool Mat_PBR_CookTorrance::brdf(const Ray &ray_out, const Ray &ray_in, const Eigen::Vector3d hit_pos, HitObject *hp,
                                Eigen::Vector4d &attenuation) {
    // prepare
    Eigen::Vector3d N = hp->normalAtPoint(hit_pos);
    Eigen::Vector3d half_plane = ((ray_out.dir + -ray_in.dir) / 2.0).normalized();
    Eigen::Vector3d F0 = {fresnel_0, fresnel_0, fresnel_0};
    F0 = MathTools::mix(F0, albedo, metalness);
    // compute
    double D_GGX = DistributionGGX(N, half_plane, roughness);
    double G_GGX = GeometrySmith(N, ray_out.dir, -ray_in.dir, roughness);
    Eigen::Vector3d F = fresnelSchlick(std::max(0.0, half_plane.dot(ray_out.dir)), F0);
    Eigen::Vector3d specular = D_GGX * G_GGX * F /
                               (4.0 * std::max(0.0, N.dot(-ray_in.dir)) * std::max(0.0, N.dot(ray_out.dir)) + 0.001);
    Eigen::Vector3d kS = F;
    Eigen::Vector3d kD = Eigen::Vector3d::Ones() - kS;
    kD *= 1.0 - metalness;

    Eigen::Vector3d result = color_mult(kD, albedo) / M_PI + specular;
    attenuation = {result.x(), result.y(), result.z(), 1.0};
    return true;
}

Mat_PBR_CookTorrance::Mat_PBR_CookTorrance(const Eigen::Vector3d &ain, double roughness, double metal) : albedo(
        ain), roughness(roughness), metalness(metal) {}
