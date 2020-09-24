#include "mousehandler.h"
#include <cmath>
#include <QDebug>
#include <iostream>

MouseHandler::MouseHandler()
{
    alpha = 0;
    beta = 0;
    gama = 0;
}
void MouseHandler::get_angles(int *angles)
{
    angles[0] = alpha;
    angles[1] = beta;
    angles[2] = gama;
}


void MouseHandler::to_handle(QMouseEvent *mouse_event)
{
    static bool is_first_use = true;
    if (is_first_use) {
        old_x = mouse_event->x();
        old_y = -(mouse_event->y());
        is_first_use = false;
        return;
    }
    int new_x = mouse_event->globalX();
    int new_y = -(mouse_event->globalY());
    x = new_x - old_x, y = new_y - old_y;

    if (mouse_event->buttons().testFlag(Qt::LeftButton)) {
        change_angle_xy();
    }
    old_x = new_x;
    old_y =new_y;

}

void MouseHandler::change_angle_xy()
{
    if (x == y || x == -y)
        return;
    if ((x > 0) && (-x < y) && (y < x)) {
        beta += 2;
    } else if ((x < 0) && (x < y) && (y < -x)){
        beta -= 2;
    } else if (y > 0) {
        alpha += 2;
    } else if (y < 0) {
        alpha -= 2;
    }
    if (alpha > 180) alpha = 180;
    if (beta > 180) beta = 180;
    if (alpha < -180) alpha = -180;
    if (beta < -180) beta = -180;

}
