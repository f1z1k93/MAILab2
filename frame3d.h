#ifndef FRAME3D_H
#define FRAME3D_H
#include "QPainter"
#include "matrix.h"
#include <stdio.h>
class Frame3D {
public:
    virtual ~Frame3D() {delete[] vertex; }
    virtual void draw(QPainter& p,
                      const Matrix& frame_matr,
                      const Matrix& scene_matr) {}
protected:
    Frame3D(int a): vertex_num(a) { vertex = new Point[a]; }
    Frame3D() {}
    bool is_visible_plane(const Point&, const Point&, const Point&, const Matrix&);
    int vertex_num;
    Point* vertex;
};

class NPrismFrame : public Frame3D {
public:
    static const int MAX_N = 10;
    NPrismFrame(int n, double r, double h);
    double set_h(double h);
    double set_r(double r);
    int set_n(int n);
    void draw(QPainter& p,
              const Matrix& frame_matr,
              const Matrix& plotter_matr);
private:
    int n;
    double r, h;
};

class SphereFrame : public Frame3D {
public:
    SphereFrame(int step, double r);
    ~SphereFrame() {delete[] is_upper; }
    double set_r(double r);
    int set_step(int step);
    static const int MAX_STEP = 5;
    int MAX_PLANE_NUM;
private:
    bool* is_upper;
    int plane_num;
    int step;
    double r;
    void save_triangle(Point&, Point&, Point&, int);
    void add_triangles(Point&, Point&, Point&, int, int);
    void draw(QPainter& p,
              const Matrix& frame_matr,
              const Matrix& plotter_matr);
};

class PlaneFrame: public Frame3D {
public:
    PlaneFrame(int length, int width, double max_x, double max_z);
    int set_hlength(int length);
    int set_hwidth(int width);
    double set_max_x(double max_x);
    double set_max_z(double max_z);
    void draw(QPainter& p,
              const Matrix& frame_matr,
              const Matrix& plotter_matr);
private:
    const static int MAX_HLENGTH = 50;
    const static int MAX_HWIDTH = 50;
    double max_x, max_z;
    int hlength, hwidth;
    double y(double x, double z);
    void set_plane(int length, int width, double max_x, double max_z);
};

#endif // FRAME3D_H
