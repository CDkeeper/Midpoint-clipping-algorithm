#include <GL/glut.h>
#include <cstdio>
#include<iostream>
using namespace std;
#define LEFT 1//��߽�
#define RIGHT 2//�ұ߽�
#define BOTTOM 4//�±߽�
#define TOP 8//�ϱ߽�

double a1, b1, a2, b2, A1, B1, A2, B2, XL = 100, XR = 300, YB = 100, YT = 200;
double x1_init, y1_init, x2_init, y2_init;

int encode(double x, double y)//��λ������ҵ��õ�Ŀռ�����
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
    A2 = a2, B2 = b2;//ȷ����a2,b2����������
    code1 = encode(a1, b1);//�õ��õ�Ŀռ�����
    code2 = encode(A2, B2);
    mx1 = (a1 + A2) / 2;
    my1 = (b1 + B2) / 2;
    code = encode(mx1, my1);

    if (code1 == 0) {
        A1 = a1, B1 = b1;
        return true;
    }
    else if ((code1 & code2) != 0) {//�ڴ�����ͬһ��
        A1 = a1, B1 = b1;
        return false;
    }
    else if ((code1 & border(mx1, my1)) != 0) {//�е��ڱ߽��ϣ���������1һ�಻�ھ�������
        a1 = mx1;
        b1 = my1;
    }
    else {
        do {
            mx1 = (a1 + A2) / 2;
            my1 = (b1 + B2) / 2;
            code1 = encode(a1, b1);//�õ��õ�Ŀռ�����
            code2 = encode(A2, B2);
            code = encode(mx1, my1);

            if (code == 0) {//�е��ڴ����ھ͸��µ�2(����)��λ��
                A2 = mx1;
                B2 = my1;
                code2 = encode(A2, B2);
            }
            else {
                if ((code & code1) != 0) {//�е��ڴ�����͵�1ͬһ��͸��µ�1��λ��
                    a1 = mx1;
                    b1 = my1;
                    code1 = encode(a1, b1);
                }
                else {//�е��ڴ�����͵�2��ͬһ��͸��µ�2��λ��
                    A2 = mx1;
                    B2 = my1;
                    code2 = encode(A2, B2);
                }
            }
        } while (pm(a1, b1));
    }
    A1 = a1, B1 = b1;//�������µĵ�1
    return true;
}

bool Mid_LineClip2() {
    double mx2, my2;
    int code1, code2, code;
    code1 = encode(A1, B1);//�õ��õ�Ŀռ�����
    code2 = encode(a2, b2);
    mx2 = (A1 + a2) / 2;
    my2 = (B1 + b2) / 2;
    code = encode(mx2, my2);
    if (code2 == 0) {
        return true;
    }
    else if ((code2 & border(mx2, my2)) != 0) {//�е��ڱ߽��ϣ���������2һ�಻�ھ�������
        a2 = mx2;
        b2 = my2;
    }
    else {
        do {
            mx2 = (A1 + a2) / 2;
            my2 = (B1 + b2) / 2;
            code1 = encode(A1, B1);//�õ��õ�Ŀռ�����
            code2 = encode(a2, b2);
            code = encode(mx2, my2);
            if (code == 0) {//�е��ڴ����ھ͸��µ�1��λ��
                A1 = mx2;
                B1 = my2;
                code1 = encode(A1, B1);
            }
            else {//�е��ڴ�����͵�2ͬһ��͸��µ�2��λ��
                a2 = mx2;
                b2 = my2;
                code2 = encode(a2, b2);
            }
        } while (pm(a2, b2));
    }
    return true;
}

void mydisplay()  //��ʾ����
{
    //���Ʒ��α߽�
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
    //����δ�ü�ǰ���߶�
    Mid_LineClip1();
    Mid_LineClip2();
    glBegin(GL_LINES);
    glVertex2i(x1_init, y1_init);
    glVertex2i(x2_init, y2_init);
    glEnd();
    glFlush();
    //���Ʋü�����߶�
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
    glutCreateWindow("�е�ü��㷨");
    cout << "������ֱ�߶˵�" << endl;
    cin >> a1 >> b1 >> a2 >> b2;
    x1_init = a1, y1_init = b1, x2_init = a2, y2_init = b2;
    glClearColor(0.0, 0.0, 0.0, 0.0);  //���ñ�����ɫ
    glMatrixMode(GL_PROJECTION);       // ����ͶӰ����
    gluOrtho2D(0.0, 600.0, 0.0, 400.0); // ���ó����Ĵ�С
    glutDisplayFunc(mydisplay);
    glutMainLoop();
    return 0;
}
