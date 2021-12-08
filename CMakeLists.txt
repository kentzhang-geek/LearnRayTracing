cmake_minimum_required(VERSION 3.20)
project(MyRayTracer)

set(CMAKE_CXX_STANDARD 17)
include_directories(libs/include)
include_directories(libs/include/OpenEXR)
link_directories(libs/lib)
link_libraries(m pthread X11)

add_executable(MyRayTracer main.cpp Ray.cpp Ray.h Sphere.cpp Sphere.h Scene.cpp Scene.h HitObject.cpp HitObject.h Quad.cpp Quad.h Triangle.cpp Triangle.h ThreadPool.h MathTools.h)
