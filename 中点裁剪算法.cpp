#include <GL/glut.h>
#include <cstdio>
#include<iostream>
using namespace std;
#define LEFT 1//左边界
#define RIGHT 2//右边界
#define BOTTOM 4//下边界
#define TOP 8//上边界

double a1, b1, a2, b2, A1, B1, A2, B2, XL = 100, XR = 300, YB = 100, YT = 200;
double x1_init, y1_init, x2_init, y2_init;

int encode(double x, double y)//按位或操作找到该点的空间区域
{
    int c = 0;
    if (x < XL) c |= LEFT;
    if (x > XR) c |= RIGHT;
    if (y < YB) c |= BOTTOM;
    if (y > YT) c |= TOP;
    return c;
}

int border(double mx, double my) {
    int c = 0;
    if (abs(mx - XL) < 1) {
        c |= LEFT;
    }
    if (abs(mx - XR) < 1) {
        c |= RIGHT;
    }
    if (abs(my - YB) < 1) {
        c |= BOTTOM;
    }
    if (abs(my - YT) < 1) {
        c |= TOP;
    }
    return c;
}
bool pm(double mx, double my) {
    if (abs(mx - XL) < 1 || abs(mx - XR) < 1 || abs(my - YB) < 1 || abs(my - YT) < 1) {
        return false;
    }
    else {
        return true;
    }
}
bool Mid_LineClip1() {
    double mx1, my1;
    int code1, code2, code;
    A2 = a2, B2 = b2;//确保（a2,b2）不被更改
    code1 = encode(a1, b1);//得到该点的空间区域
    code2 = encode(A2, B2);
    mx1 = (a1 + A2) / 2;
    my1 = (b1 + B2) / 2;
    code = encode(mx1, my1);

    if (code1 == 0) {
        A1 = a1, B1 = b1;
        return true;
    }
    else if ((code1 & code2) != 0) {//在窗口外同一侧
        A1 = a1, B1 = b1;
        return false;
    }
    else if ((code1 & border(mx1, my1)) != 0) {//中点在边界上，若靠近点1一侧不在经过窗口
        a1 = mx1;
        b1 = my1;
    }
    else {
        do {
            mx1 = (a1 + A2) / 2;
            my1 = (b1 + B2) / 2;
            code1 = encode(a1, b1);//得到该点的空间区域
            code2 = encode(A2, B2);
            code = encode(mx1, my1);

            if (code == 0) {//中点在窗口内就更新点2(备份)的位置
                A2 = mx1;
                B2 = my1;
                code2 = encode(A2, B2);
            }
            else {
                if ((code & code1) != 0) {//中点在窗口外和点1同一侧就更新点1的位置
                    a1 = mx1;
                    b1 = my1;
                    code1 = encode(a1, b1);
                }
                else {//中点在窗口外和点2在同一侧就更新点2的位置
                    A2 = mx1;
                    B2 = my1;
                    code2 = encode(A2, B2);
                }
            }
        } while (pm(a1, b1));
    }
    A1 = a1, B1 = b1;//保留更新的点1
    return true;
}

bool Mid_LineClip2() {
    double mx2, my2;
    int code1, code2, code;
    code1 = encode(A1, B1);//得到该点的空间区域
    code2 = encode(a2, b2);
    mx2 = (A1 + a2) / 2;
    my2 = (B1 + b2) / 2;
    code = encode(mx2, my2);
    if (code2 == 0) {
        return true;
    }
    else if ((code2 & border(mx2, my2)) != 0) {//中点在边界上，若靠近点2一侧不在经过窗口
        a2 = mx2;
        b2 = my2;
    }
    else {
        do {
            mx2 = (A1 + a2) / 2;
            my2 = (B1 + b2) / 2;
            code1 = encode(A1, B1);//得到该点的空间区域
            code2 = encode(a2, b2);
            code = encode(mx2, my2);
            if (code == 0) {//中点在窗口内就更新点1的位置
                A1 = mx2;
                B1 = my2;
                code1 = encode(A1, B1);
            }
            else {//中点在窗口外和点2同一侧就更新点2的位置
                a2 = mx2;
                b2 = my2;
                code2 = encode(a2, b2);
            }
        } while (pm(a2, b2));
    }
    return true;
}

void mydisplay()  //显示函数
{
    //绘制方形边界
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.6, 0.7);
    glPointSize(2);
    glBegin(GL_LINE_LOOP);
    glVertex2i(XL, YT);
    glVertex2i(XL, YB);
    glVertex2i(XR, YB);
    glVertex2i(XR, YT);
    glEnd();
    glFlush();
    //绘制未裁剪前的线段
    Mid_LineClip1();
    Mid_LineClip2();
    glBegin(GL_LINES);
    glVertex2i(x1_init, y1_init);
    glVertex2i(x2_init, y2_init);
    glEnd();
    glFlush();
    //绘制裁剪后的线段
    glColor3f(0.8, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(a1, b1);
    glVertex2i(a2, b2);
    glEnd();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("中点裁剪算法");
    cout << "请输入直线端点" << endl;
    cin >> a1 >> b1 >> a2 >> b2;
    x1_init = a1, y1_init = b1, x2_init = a2, y2_init = b2;
    glClearColor(0.0, 0.0, 0.0, 0.0);  //设置背景颜色
    glMatrixMode(GL_PROJECTION);       // 设置投影参数
    gluOrtho2D(0.0, 600.0, 0.0, 400.0); // 设置场景的大小
    glutDisplayFunc(mydisplay);
    glutMainLoop();
    return 0;
}
