cmake_minimum_required(VERSION 3.20)
project(MyRayTracer)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
include_directories(libs/include)
include_directories(libs/include/OpenEXR)
link_directories(libs/lib)
link_libraries(m pthread X11)

add_executable(MyRayTracer main.cpp Ray.cpp Ray.h Sphere.cpp Sphere.h Scene.cpp Scene.h HitObject.cpp HitObject.h Quad.cpp Quad.h Triangle.cpp Triangle.h ThreadPool.h MathTools.h MathTools.cpp Material.cpp Material.h Mat_Diffuse_Lambert.cpp Mat_Diffuse_Lambert.h Mat_Specular_Metal.cpp Mat_Specular_Metal.h Mat_Dielectrics.cpp Mat_Dielectrics.h Mat_PBR_CookTorrance.cpp Mat_PBR_CookTorrance.h)
