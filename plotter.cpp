#include "plotter.h"
#include "matrix.h"
#include "QPainter"
#include <cmath>
#include <QDebug>
#include <iostream>

using namespace std;

Plotter::Plotter(QWidget *w):
    widget(w)

{
}

void Plotter::draw_axis_3d(const Matrix& trans_matrix)
{
    QPainter painter(widget);
    painter.setPen(QPen(QColor(Qt::red)));
    Point from = trans_matrix * Point(-1, 0, 0);
    Point to = trans_matrix * Point(2, 0, 0);
    painter.drawLine(from.x(), from.y(), to.x(), to.y());

    painter.setPen(QPen(QColor(Qt::green)));
    from = trans_matrix * Point(0, -1, 0);
    to = trans_matrix * Point(0, 2, 0);
    painter.drawLine(from.x(), from.y(), to.x(), to.y());

    painter.setPen(QPen(QColor(Qt::blue)));
    from = trans_matrix * Point(0, 0, -1);
    to = trans_matrix * Point(0, 0, 2);
    painter.drawLine(from.x(), from.y(), to.x(), to.y());
    return;
}

void Plotter::draw_scene(Frame3D *frame, Matrix& frame_matr)
{
    double width = widget->width();
    double height = widget->height();
    double scale = 0.03 * std::max(width, height);
    Matrix plotter_matr = Matrix::shift(0.5 * width, 0.5 * height) *
                          Matrix::scale(scale, -scale, scale) * frame_matr;
    draw_axis_3d(plotter_matr);
    QPainter painter(widget);
    frame->draw(painter, frame_matr, plotter_matr);
}
