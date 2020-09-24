#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H
#include "QMouseEvent"

class MouseHandler {
public:
    MouseHandler();
    void to_handle(QMouseEvent *mouse_event);
    void get_angles(int *angles);
private:
    int alpha, beta, gama;
    int old_x, old_y, x , y;
    void change_angle_xy();
};

#endif // MOUSEHANDLER_H
