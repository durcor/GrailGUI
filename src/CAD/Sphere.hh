#pragma once
#include "opengl/Shape.hh"
#include "opengl/Canvas.hh"
class Sphere : Shape{
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t latRes, longRes, radius;
    public:
        Sphere::Sphere(Canvas* c, uint32_t latRes, uint32_t longRes, uint32_t radius);
        ~Sphere();
        void init();
        void render();
        void cleanup();
};