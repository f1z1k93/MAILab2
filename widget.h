#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <plotter.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_8_valueChanged(double arg1);

    void on_doubleSpinBox_9_valueChanged(double arg1);

    void on_tabWidget_currentChanged(int index);

    void on_doubleSpinBox_10_valueChanged(double arg1);

    void on_spinBox_2_valueChanged(int arg1);

private:
    Ui::Widget *ui;

    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *mouse_event);
    void change_angle(int delta_x, int delta_y);
    Frame3D* get_frame();
    int frame_index;
    NPrismFrame* prism;
    SphereFrame* sphere;
    PlaneFrame* plane;
    Plotter plotter;
    int alfa, beta;
};

#endif // WIDGET_H
