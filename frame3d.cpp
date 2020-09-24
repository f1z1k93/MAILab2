#include "frame3d.h"
#include "cmath"
#include "QDebug"
#include <iostream>
#include <QPainter>

using namespace std;
bool Frame3D::is_visible_plane(const Point &a1, const Point &b1, const Point &c1,
                               const Matrix&  m)
{
    const Point& a = m * a1;
    const Point& b = m * b1;
    const Point& c = m * c1;
    double ax = b.x() - a.x();
    double bx = c.x() - b.x();
    double ay = b.y() - a.y();
    double by = c.y() - b.y();
    return (ax * by - ay * bx) > 0;
}

NPrismFrame::NPrismFrame(int n, double r, double h):
    Frame3D(2 * NPrismFrame::MAX_N),
    n(n),
    r(r),
    h(h)
{
    set_n(n);
}

int NPrismFrame::set_n(int n)
{
    double delta = 2 * M_PI / n;
    double fi = 0;
    double x, z;
    for (int i = 0; i < n; i++, fi += delta) {
        x = cos(fi) * r;
        z = sin(fi) * r;
        vertex[i] = Point(x, h, z);
        vertex[n + i] = Point(x, -h, z);
    }
    this->n = n;
    return n;
}

double NPrismFrame::set_h(double h)
{
    this->h = h;
    for (int i = 0; i < n; i++)
        (vertex[i])[1][0] = h;

    for (int i = n; i < vertex_num; i++)
        (vertex[i])[1][0] = -h;

    return h;
}

double NPrismFrame::set_r(double r)
{
    double x = r / this->r;
    this->r = r;
    for (int i = 0; i < vertex_num; i++) {
        (vertex[i])[0][0] *= x;
        (vertex[i])[2][0] *= x;
    }
    return r;
}

void NPrismFrame::draw(QPainter& p, const Matrix& frame_matr, const Matrix& plotter_matr)
{
    bool is_up_visible = is_visible_plane(vertex[2], vertex[1], vertex[0],
                                          frame_matr);
    bool is_down_visible = is_visible_plane(vertex[n], vertex[n + 1], vertex[n + 2],
                                            frame_matr);
    for (int i = 1;; i++) {
        int i1 = i - 1;
        if (i == n) i = 0;
        bool is_side_visible = false;
        Point a = plotter_matr * vertex[i1], b = plotter_matr * vertex[i];
        Point c = plotter_matr * vertex[n+i1], d = plotter_matr * vertex[n+i];
        if (is_visible_plane(vertex[i1], vertex[i], vertex[n+i], frame_matr)) {
            p.drawLine(a.x(), a.y(), c.x(), c.y());
            p.drawLine(b.x(), b.y(), d.x(), d.y());
            is_side_visible = true;
        }
        if (is_up_visible || is_side_visible)
            p.drawLine(a.x(), a.y(), b.x(), b.y());
        if (is_down_visible || is_side_visible)
            p.drawLine(c.x(), c.y(), d.x(), d.y());
        if (i == 0) break;
    }
    return;
}


SphereFrame::SphereFrame(int step, double r):
  Frame3D(pow(4, MAX_STEP) * 8 * 3),
  step(step),
  r(r)
{
    MAX_PLANE_NUM = vertex_num / 3;
    is_upper = new bool [MAX_PLANE_NUM];
    set_step(step);
}

int SphereFrame::set_step(int step)
{
    this->step = step;
    this->plane_num = pow(4, step) * 8;
    this->vertex_num = plane_num * 3;
    Point up(0, r, 0);
    Point down(0, -r, 0);
    Point right(r, 0, 0);
    Point left(-r, 0, 0);
    Point forward(0, 0, r);
    Point back(0, 0, -r);
    add_triangles(up, right, forward, true, 0);
    add_triangles(up, forward, left, true, 0);
    add_triangles(up, left, back, true, 0);
    add_triangles(up, back, right, true, 0);
    add_triangles(right, down, forward, false, 0);
    add_triangles(forward, down, left, false, 0);
    add_triangles(left, down, back, false, 0);
    add_triangles(back, down, right, false, 0);
    return step;
}

double SphereFrame::set_r(double r)
{
    double x = r / this->r;
    for (int i = 0; i < vertex_num; i++) {
        (vertex[i])[0][0] *= x;
        (vertex[i])[1][0] *= x;
        (vertex[i])[2][0] *= x;
    }
    this->r = r;
//    qDebug() << r;
    return r;
}

void SphereFrame::save_triangle(Point& a, Point& b, Point& c, int is_upper)
{
    static int i = 0;
    static int j = 0;
    vertex[i++] = a;
    vertex[i++] = b;
    vertex[i++] = c;
    this->is_upper[j++] = is_upper;
    if (j == plane_num)
        j = 0;
    if (i == vertex_num)
        i = 0;
    //cout << edge_matr << endl;
    return;
}

void SphereFrame::add_triangles(Point& a, Point& b, Point& c, int is_upper, int step)
{
    if (step == this->step) {
        //TODO: заполнить таблицу
        save_triangle(a, b, c, is_upper);
        return;
    }
    step++;
    Point ab = 0.5 * (Point) (a + b);
    ab = (r / ab.len()) * ab;
    Point bc = 0.5 * (Point) (b + c);
    bc = (r / bc.len()) * bc;
    Point ac = 0.5 * (Point) (a + c);
    ac = (r / ac.len()) * ac;

    add_triangles(a, ab, ac, is_upper, step);
    add_triangles(ab, b, bc, is_upper, step);
    add_triangles(ac, bc, c, is_upper, step);
    if (is_upper) {
        add_triangles(ab, bc, ac, false, step);
    } else {
        add_triangles(ac, ab, bc, true, step);
    }
    return;
}

void SphereFrame::draw(QPainter &p, const Matrix &frame_matr, const Matrix &plotter_matr)
{
    for (int i = 0, j = 0; j < plane_num; j++) {
        Point& a = vertex[i++];
        Point& b = vertex[i++];
        Point& c = vertex[i++];
        if (is_visible_plane(c, b, a, frame_matr)) {
            const Point& pa = plotter_matr * a;
            const Point& pb = plotter_matr * b;
            const Point& pc = plotter_matr * c;
            p.drawLine(pa.x(), pa.y(), pb.x(), pb.y());
            p.drawLine(pb.x(), pb.y(), pc.x(), pc.y());
            p.drawLine(pc.x(), pc.y(), pa.x(), pa.y());
        }
    }
}

double PlaneFrame::y(double x, double z)
{
    return x*x + z*z;
}

PlaneFrame::PlaneFrame(int hlength, int hwidth, double max_x, double max_z):
  Frame3D((2 * MAX_HLENGTH + 1) * (2 * MAX_HWIDTH + 1)),
  hlength(hlength),
  hwidth(hwidth),
  max_x(max_x),
  max_z(max_z)
{
    set_plane(hlength, hwidth, max_x, max_z);
    return;
}

int PlaneFrame::set_hlength(int hlength)
{
    set_plane(hlength, hwidth, max_x, max_z);
    this->hlength = hlength;
    return hlength;
}

int PlaneFrame::set_hwidth(int hwidth)
{
    set_plane(hlength, hwidth, max_x, max_z);
    this->hwidth = hwidth;
    return hwidth;
}

double PlaneFrame::set_max_x(double max_x)
{
    set_plane(hlength, hwidth, max_x, max_z);
    this->max_x = max_x;
    return max_x;
}

double PlaneFrame::set_max_z(double max_z)
{
    set_plane(hlength, hwidth, max_x, max_z);
    this->max_z = max_z;
    return max_z;
}

void PlaneFrame::set_plane(int hlength, int hwidth, double max_x, double max_z)
{
    int k = 0;
    double delta_x = max_x / hlength;
    double delta_z = max_z / hwidth;
    double x = -max_x, z = -max_z;
    for (int i = -hwidth; i <= hwidth; z += delta_z, i++) {
        for (int j = -hlength; j <= hlength; x += delta_x, j++) {
            vertex[k++] = Point(x, y(x, z), z);
        }
    }
    return;
}

void PlaneFrame::draw(QPainter &p, const Matrix &frame_matr, const Matrix &plotter_matr)
{
    for (int i = 0; i < vertex_num - 2 * hlength; i++) {
        Point a = vertex[i];
        Point b = vertex[i + 1];
        Point c = vertex[i + 2 * hlength + 1];
        if (0)
            continue;
        if ((0 <= i && i < 2) || i % (2 * hlength) == 1) {
            Point pd = plotter_matr * vertex[i + 2 * hlength + 2];
            if (i == 0) {
                Point pb = plotter_matr * pb;
                p.drawLine(pb.x(), pb.y(), pd.x(), pd.y());
            }
            if (i % (2 * hlength) == 1) {
                Point pc = plotter_matr * pc;
                p.drawLine(pc.x(), pc.y(), pd.x(), pd.y());
            }
        } else {
            Point pa = plotter_matr * a;
            Point pb = plotter_matr * b;
            Point pc = plotter_matr * c;
            p.drawLine(pa.x(), pa.y(), pb.x(), pb.y());
            p.drawLine(pa.x(), pa.y(), pc.x(), pc.y());
        }
    }
}
