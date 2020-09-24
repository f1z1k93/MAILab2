#include <cmath>
#include <QPen>
#include "scene.h"
#include "widget.h"
#include "ui_widget.h"
#include "QDebug"
Scene::Scene() :
      show_matrix(DIM + 1, DIM + 1)
{
    scene[0] = new Line1;
    scene[1] = new Line2;
}

Line1::Line1()
{
    params[0] = 1;
    params[1] = 1;
    params[2] = 0.03;
    point_num = 1000;
    max_t = 360;
}

Line2::Line2()
{
    params[0] = 1;
    params[1] = 0.03;
    params[2] = 1;
    params[3] = 2;
    point_num = 1000;
    max_t = 360;
}

void Line1::draw(Widget *widget, const Matrix& show_matrix, int max_x)
{
    QPainter painter(widget);
    Point begin, end;
    double x, y, z;
    double a = params[0], b = params[1], c = params[2];
    Point prev, curr(a, 0, 0);
    double max_t = this->max_t * M_PI / 180;
    double delta_t = max_t / point_num;
    for (double t = 0; t < max_t; t += delta_t) {
        prev = curr;
        x = a * cos(t);
        y = b * sin(t);
        z = c * t;
        curr = Point(x, y, z);
        begin = show_matrix * prev;
        end = show_matrix * curr;
        if (begin[0] > max_x || end[0] > max_x)
            continue;
        painter.drawLine(begin[0], begin[1], end[0], end[1]);
    }
}

void Line2::draw(Widget *widget, const Matrix& show_matrix, int max_x)
{
    QPainter painter(widget);
    Point begin, end;
    double x, y, z;
    double a = params[0], b = params[1], c = params[2];
    double r = params[3];
    bool is_was_continue = false;
    Point prev, curr(0, 0, sqrt(r * r - c));
    double max_t = this->max_t * M_PI / 180;
    double delta_t = max_t / point_num;
    for (double t = 0; t < max_t; t += delta_t) {
        prev = curr;
        x = a * sin(t);
        y = b * t;
        z = r * r - x * x - y * y - c;
        if (z < 0) {
            is_was_continue = true;
            continue;
        }
        curr = Point(x, y, sqrt(z));
        if (is_was_continue) {
            is_was_continue = false;
            continue;
        }
        begin = show_matrix * prev;
        end = show_matrix * curr;
        if (begin[0] > max_x || end[0] > max_x)
            continue;
        painter.drawLine(begin[0], begin[1], end[0], end[1]);
    }
}

void Scene::draw_axis(Widget *widget)
{
    QPainter painter(widget);
    Point a, b;
    a[0] = -0.5; a[1] = 0; a[2] = 0;
    b[0] = 1.5; b[1] = 0; b[2] = 0;
    a = show_matrix * a;
    b = show_matrix * b;
    painter.setPen(QPen(QColor(Qt::red)));
    painter.drawLine(a[0], a[1], b[0], b[1]);

    a[0] = 0; a[1] = -0.5; a[2] = 0;
    b[0] = 0; b[1] = 1.5; b[2] = 0;
    a = show_matrix * a;
    b = show_matrix * b;
    painter.setPen(QPen(QColor(Qt::blue)));
    painter.drawLine(a[0], a[1], b[0], b[1]);

    a[0] = 0; a[1] = 0; a[2] = -0.5;
    b[0] = 0; b[1] = 0; b[2] = 1.5;
    a = show_matrix * a;
    b = show_matrix * b;
    painter.setPen(QPen(QColor(Qt::green)));
    painter.drawLine(a[0], a[1], b[0], b[1]);

}

void Scene::draw(Widget *widget, int max_x)
{
    scene[0]->draw(widget, show_matrix, max_x);
    scene[1]->draw(widget, show_matrix, max_x);
    draw_axis(widget);
}

Matrix Scene::scale_matrix(double width, double height)
{
    Matrix size_matrix(4, 4);
    double scale = 0.2 * std::max(width, height);
    double shift_x = width * 0.5;
    double shift_y = height * 0.5;

    size_matrix.set(0, 0, scale);
    size_matrix.set(1, 1, -scale);
    size_matrix.set(2, 2, -1);
    size_matrix.set(3, 3, 1);
    size_matrix.set(0, 3, shift_x);
    size_matrix.set(1, 3, shift_y);
    return Matrix(size_matrix);
}

Matrix Scene::rot_matrix(double alpha, double beta, double gama)
{
    Matrix rot_matrix(4, 4);
    rot_matrix = get_rotx_matrix(alpha) *
                get_roty_matrix(beta) *
                get_rotz_matrix(gama);
    return Matrix(rot_matrix);
}

void Scene::init_show_matrix(double *size, int* angles)
{
    show_matrix = scale_matrix(size[0], size[1]) *
            rot_matrix(angles[0] * -M_PI / 180,
                        angles[1] * M_PI / 180,
                        angles[2] * M_PI / 180);
}

Line3D* Scene::operator [](int i)
{
    return scene[i];
}

