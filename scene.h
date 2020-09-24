#ifndef FIGURE_H
#define FIGURE_H
#include <QPainter>
#include "matrix.h"
#include "transmatrix.h"
struct Widget;

class Line3D {
public:
    double params[10];
    int point_num;
    int max_t;
    virtual void draw(Widget *widget, const Matrix& show_matrix, int max_x) = 0;
};

class Line1 : public Line3D {
public:
    Line1();
    void draw(Widget *widget, const Matrix& show_matrix, int max_x);
};

class Line2 : public Line3D {
public:
    Line2();
    void draw(Widget *widget, const Matrix& show_matrix, int max_x);
};


class Scene {
public:
    Scene ();
    Line3D *operator[] (int i);
    void init_show_matrix(double *size, int *angles);
    void draw(Widget *widget, int max_x);
private:
    Line3D *scene[10];

    Matrix show_matrix;
    void draw_axis(Widget *widget);
    Matrix scale_matrix(double width, double height);
    Matrix rot_matrix(double alpha, double beta, double gama);
};


#endif // FIGURE_H
