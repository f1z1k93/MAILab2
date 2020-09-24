#include "widget.h"
#include "ui_widget.h"
#include "frame3d.h"
#include "plotter.h"
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    plotter(this)
{
    ui->setupUi(this);
    alfa = 0;
    beta = 0;
    frame_index = 0;
    double r = ui->doubleSpinBox_10->value();
    int step = ui->spinBox_2->value();
    sphere = new SphereFrame(step, r);

    int n = ui->spinBox->value();
    double h = ui->doubleSpinBox_9->value();
    r = ui->doubleSpinBox_8->value();
    prism = new NPrismFrame(n, r, h);
    int length = 2, width = 2;
    double max_x = 2, max_z = 2;
    plane = new PlaneFrame(length, width, max_x, max_z);
}

Widget::~Widget()
{
    delete ui;
    delete sphere;
    delete prism;
    delete plane;
    return;
}

void Widget::paintEvent(QPaintEvent *)
{
    //QApplication::exit();
    Matrix frame_matr = Matrix::rot(M_PI * alfa / 180,
                          M_PI * beta / 180);
    //sphere->set_r(1);
    plotter.draw_scene(get_frame(), frame_matr);
    return;
}

Frame3D* Widget::get_frame()
{
    int frame_index = 2;
    if (frame_index == 0) {
        return prism;
    } else if (frame_index == 1) {
        return sphere;
    } else if (frame_index == 2) {
        return plane;
    }
    return NULL;
}

void Widget::mouseMoveEvent(QMouseEvent *mouse_event)
{
    static bool is_first_use = true;
    static int old_x, old_y;
    if (is_first_use) {
        old_x = mouse_event->x();
        old_y = -(mouse_event->y());
        is_first_use = false;
        return;
    }
    int new_x = mouse_event->globalX();
    int new_y = mouse_event->globalY();
    int delta_x = new_x - old_x;
    int delta_y = new_y - old_y;

    if (mouse_event->buttons().testFlag(Qt::LeftButton)) {
        change_angle(delta_x, delta_y);
    }
    old_x = new_x;
    old_y = new_y;
    this->update();
}

void Widget::change_angle(int x, int y)
{
    if (x == y || x == -y)
        return;
    if ((x > 0) && (-x < y) && (y < x)) {
        beta += 2;
    } else if ((x < 0) && (x < y) && (y < -x)){
        beta -= 2;
    } else if (y > 0) {
        alfa += 2;
    } else if (y < 0) {
        alfa -= 2;
    }
    if (alfa > 180) alfa = 180;
    if (beta > 180) beta = 180;
    if (alfa < -180) alfa = -180;
    if (beta < -180) beta = -180;
    //cerr << alfa << ' ' << beta;
}

void Widget::on_spinBox_valueChanged(int arg1)
{
    //NPrismFrame::n = arg1;
    prism->set_n(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_8_valueChanged(double arg1)
{
    //NPrismFrame::r = arg1;
    prism->set_r(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_9_valueChanged(double arg1)
{
    //scene[0]->set_h(arg1)
    prism->set_h(arg1);
    this->update();
}

void Widget::on_tabWidget_currentChanged(int index)
{
    frame_index = index;
    this->update();
}


void Widget::on_doubleSpinBox_10_valueChanged(double arg1)
{
//    SphereFrame::r = arg1;
    sphere->set_r(arg1);
    this->update();
}

void Widget::on_spinBox_2_valueChanged(int arg1)
{
//    SphereFrame::step = arg1;
    sphere->set_step(arg1);
    this->update();
}
