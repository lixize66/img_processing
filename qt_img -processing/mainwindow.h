#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileDialog>
#include<QPainter>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    int begin=0;
    int fenqu_way=0;//分区方式
     QImage img1,img2,img2_cancel;//选择图片
        QPoint pointa;//鼠标点击点
        QPoint *p2;
        int img_x=0,img_y=0;
        int* jilu,*jilu_ts;//记录数组首地址
        int*jilu_cancel;
        int c_A=255,c_R=10,c_G=10,c_B=10;//颜色条件
        uchar* img1_first,*img2_first;
 void mouseMoveEvent(QMouseEvent *e);
 void mousePressEvent(QMouseEvent *e);//鼠标点击事件
 void mouseReleaseEvent(QMouseEvent *e);//鼠标释放事件，存放图片偏移量
 QImage Imagecopy(int x,int y,int width,int height);
 void imageXY(int *x,int *y);
 void fenQu(QImage img,QPoint point_b);
 void colouration(QPoint*pt);
 bool condition(uchar*pt1,uchar*pt2);
 void jilu_copy(int src[], int dest[], int count);
 void ts_range(int*s1,int*s2);
 void colour_label();
 void ts();
//        void paintEvent(QPaintEvent *);
private slots:
        void on_pushButton_clicked();
        void on_pushButton_4_clicked();
        void on_pushButton_2_clicked();
        void on_pushButton_3_clicked();
        void on_spinBox_valueChanged(int arg1);
        void on_spinBox_2_valueChanged(int arg1);
        void on_spinBox_3_valueChanged(int arg1);
        void on_spinBox_4_valueChanged(int arg1);
        void on_pushButton_5_clicked();
        void on_spinBox_15_valueChanged(int arg1);
        void on_spinBox_16_valueChanged(int arg1);
        void on_spinBox_17_valueChanged(int arg1);
        void on_spinBox_18_valueChanged(int arg1);
        void on_pushButton_9_clicked();
        void on_pushButton_7_clicked();
        void on_pushButton_8_clicked();
        void on_spinBox_7_valueChanged(int arg1);
        void on_spinBox_8_valueChanged(int arg1);
        void on_spinBox_20_valueChanged(int arg1);
        void on_spinBox_19_valueChanged(int arg1);
};

#endif // MAINWINDOW_H

