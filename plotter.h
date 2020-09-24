#ifndef PLOTTER_H
#define PLOTTER_H
#include "frame3d.h"
#include "QWidget"

class Plotter {
public:
    Plotter(QWidget *w);
    void draw_scene(Frame3D *frame, Matrix& frame_matr);
private:
    QWidget* widget;
    void draw_frame_3d(const Matrix&);
    void draw_axis_3d(const Matrix&);

};

#endif // PLOTTER_H
