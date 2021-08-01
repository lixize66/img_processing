#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include <QMouseEvent>
#include<QDebug>
#include<QStack>
#include<QFile>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::ui->pushButton_5->setEnabled(false);
    MainWindow::ui->pushButton_8->setEnabled(false);
    MainWindow::colour_label();
}

MainWindow::~MainWindow()
{
    free(jilu);
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::ui->pushButton_5->setEnabled(false);
    QString filename;
    free(jilu);
    filename = QFileDialog::getOpenFileName(this,
           "选择图像",
           "C:\\",
           "Images (*.png *.bmp *.jpg *.tif *.GIF)"); //选择路径

     if(img1.load(filename)) ;
   {
         img_x=0;img_y=0;//重置移动量
         MainWindow::ui->pushButton_8->setEnabled(true);
         MainWindow::ui->label->setPixmap(QPixmap::fromImage(img1.copy(0,0,500,500)));
         //MainWindow::ui->label->setScaledContents(true);
         img2=QImage(img1.width(),img1.height(),QImage::Format_ARGB32);
         img2_cancel=QImage(img1.width(),img1.height(),QImage::Format_ARGB32);
         MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(0,0,500,500)));
         img1_first=(uchar*)img1.bits();
         img2_first=(uchar*)img2.bits();
         jilu=(int*)malloc(sizeof (int)*img1.width()*img1.height());
         jilu_cancel=(int*)malloc(sizeof (int)*img1.width()*img1.height());
         jilu_ts=(int*)malloc(sizeof (int)*img1.width()*img1.height());
         MainWindow::jilu_copy(jilu,jilu_cancel,img1.width()*img1.height());
         for(int ic=0;ic<img1.width()*img1.height();ic++)
         {
             jilu[ic]=0;
         }
         for(int ic=0;ic<img1.width()*img1.height();ic++)
         {
             jilu_ts[ic]=0;
         }
//         qDebug()<<img1.width()<<img1.height();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{

    QPoint b=e->pos();
    b.setY(b.y()-10);//qt界面多了10像素的边框
    MainWindow::ui->label->setPixmap(QPixmap::fromImage(Imagecopy(img_x+(pointa.x()-b.x()),img_y+(pointa.y()-b.y()),500,500)));
    MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x+(pointa.x()-b.x()),img_y+(pointa.y()-b.y()),500,500)));
}
 void MainWindow::mousePressEvent(QMouseEvent *e)//鼠标点击事件
 {
     pointa= e->pos();
     pointa.setY(pointa.y()-10);
     QPoint pointb;
     qDebug()<<pointa;
     pointb.setX(pointa.x()+img_x-20);
     pointb.setY(pointa.y()+img_y-100);
     MainWindow::ui->textEdit->setText(" ");
     if(pointb.x()>=0&&pointb.x()<=img1.width()-1&&pointb.x()<=499&&pointb.y()>=0&&pointb.y()<=img1.height()-1&&pointb.y()<=499&&begin==1)
     {
         int a=(pointb.x()+pointb.y()*img1.width())*4;
         QString st="(x:"+QString::number(pointb.x())+",y:"+QString::number(pointb.y())+") B:"+QString::number((int)*(img1_first+a))+" G:"+QString::number((int)*(img1_first+a+1))+" R:"+QString::number((int)*(img1_first+a+2))+" A:"+QString::number((int)*(img1_first+a+3));
         MainWindow::ui->textEdit->setText(st);
         MainWindow::ui->pushButton_5->setEnabled(true);
         img2_cancel=img2.copy(0,0,img1.width(),img1.height());

         fenQu(img1,pointb);

     }
//
 }

 void MainWindow::mouseReleaseEvent(QMouseEvent *e)
 {
         QPoint b=e->pos();
         b.setY(b.y()-10);
         int x=img_x+(pointa.x()-b.x());
         int y=img_y+(pointa.y()-b.y());
         imageXY(&x,&y);
         img_x=x;
         img_y=y;
         MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(x,y,500,500)));
 }

 QImage MainWindow::Imagecopy(int x,int y,int width,int height)
 {
     QImage img_copy;
     imageXY(&x,&y);
     img_copy=MainWindow::img1.copy(x,y,width,height);
     return img_copy;
 }

 void MainWindow::imageXY(int *x, int *y)//输入鼠标移动量，修改地址为应移动量
 {

     if(img1.width()>500)
     {
     if(*x>img1.width()-500)*x=img1.width()-500;
     if(*x<0)*x=0;
     }else{
         if(*x<img1.width()-500)*x=img1.width()-500;
         if(*x>0)*x=0;
     }
     if(img1.height()>500)
     {
     if(*y>img1.height()-500)*y=img1.height()-500;
     if(*y<0)*y=0;
     }else{
         if(*y<img1.height()-500)*y=img1.height()-500;
         if(*y>0)*y=0;
     }

 }

 void MainWindow::fenQu(QImage img,QPoint point)//传入点为在图像上的位置
 {

     uchar *first=(uchar*)img.bits();
     int a=point.x()+point.y()*img.width();
     QPoint* p2=(QPoint*)malloc(sizeof(QPoint)*img.width()*img.height());
     for(int ic=0;ic<img.width()*img.height();ic++)
     {
         p2[ic].setX(0);
         p2[ic].setY(0);
     }
     jilu[point.x()+point.y()*img.width()]=1;
     int shuzu[8][2]={ { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };
      QStack<QPoint> s;
     s.push(point);
     int i=0;
     while(!s.isEmpty())
     {

         QPoint r=s.pop();
         p2[i]=r;
         i++;
         int a2=r.x()+r.y()*img.width();
         for (int i2 =0; i2<8; i2++)
         {
             QPoint pt;
             pt.setX(r.x() +shuzu[i2][0]);
             pt.setY(r.y()+ shuzu[i2][1]);
             if(pt.x()>=0&&pt.x()<=img.width()-1&&pt.y()>=0&&pt.y()<=img.height()-1&&jilu[pt.x()+pt.y()*img.width()]!=1)
             {
                 if(fenqu_way==0)//渐变
                 {

                     if(MainWindow::condition((first+(r.x()+r.y()*img.width())*4),(first+(pt.x()+pt.y()*img.width())*4)))
                     {
                         s.push(pt);
                         jilu[pt.x()+pt.y()*img.width()]=1;
                     }
                 }
                 else {//选择
                     if(MainWindow::condition((first+(point.x()+point.y()*img.width())*4),(first+(pt.x()+pt.y()*img.width())*4)))
                     s.push(pt);
                     jilu[pt.x()+pt.y()*img.width()]=1;
                 }

             }
         }

       }
     colouration(p2);
}
 void MainWindow::colouration(QPoint *pt)
 {
     for(int ic=0;ic<img1.width()*img1.height();ic++)
     {
         int a=pt[ic].x()+pt[ic].y()*img1.width();
         *(img2_first+a*4)=*(img1_first+a*4);
         *(img2_first+a*4+1)=*(img1_first+a*4+1);
         *(img2_first+a*4+2)=*(img1_first+a*4+2);
         *(img2_first+a*4+3)=*(img1_first+a*4+3);
//         qDebug()<<pt[ic]<<ic;
     }
 MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x,img_y,500,500)));
 }

bool MainWindow::condition(uchar *pt1, uchar *pt2)
 {
     if(abs(*(pt1)-*(pt2))<c_B&&abs(*(pt1+1)-*(pt2+1))<c_G&&abs(*(pt1+2)-*(pt2+2))<c_R&& abs(*(pt1+3)-*(pt2+3))<c_A)
         return true;
     else {
         return false;
     }

 }

void MainWindow::jilu_copy(int *src, int *dest, int count)//src复制给dest
{
    int i;

    for(i = 0; i < count; i++){
        dest[i] = src[i];
    }
}

void MainWindow::ts_range(int *s1, int *s2)
{
    int i;
    for(i=0;i<img1.width()*img1.height();i++)
    {
        if(s1[i]==1)
            if(*(img1_first+i*4)>=MainWindow::ui->spinBox_5->value()&&*(img1_first+i*4)<=MainWindow::ui->spinBox_14->value())
                if(*(img1_first+i*4+1)>=MainWindow::ui->spinBox_6->value()&&*(img1_first+i*4+1)<=MainWindow::ui->spinBox_12->value())
                    if(*(img1_first+i*4+2)>=MainWindow::ui->spinBox_9->value()&&*(img1_first+i*4+2)<=MainWindow::ui->spinBox_13->value())
                        if(*(img1_first+i*4+3)>=MainWindow::ui->spinBox_10->value()&&*(img1_first+i*4+3)<=MainWindow::ui->spinBox_11->value())
        {
                            s2[i]=1;
        }
    }
}

void MainWindow::colour_label()
{
    QString st1=QString::number(MainWindow::ui->spinBox_20->value());
    QString st2=QString::number(MainWindow::ui->spinBox_8->value());
    QString st3=QString::number(MainWindow::ui->spinBox_7->value());
    QString st4=QString::number(MainWindow::ui->spinBox_19->value());
    MainWindow::ui->label_29->setStyleSheet("QLabel{background-color: rgba("+st1+","+st2+","+st3+","+st4+");}");
}

void MainWindow::ts()
{
    int i=0;
    for(i=0;i<img1.width()*img1.height();i++)
    {
        if(jilu_ts[i]==1)
        {
            if((*(img1_first+i*4)+MainWindow::ui->spinBox_15->value()-256)>255)
                *(img2_first+i*4)=255;
            else {
                if((*(img1_first+i*4)+MainWindow::ui->spinBox_15->value()-256)<0)
                    *(img2_first+i*4)=0;
                else {
                    *(img2_first+i*4)=(*(img1_first+i*4)+MainWindow::ui->spinBox_15->value()-256);
                }
            }
            if((*(img1_first+i*4+1)+MainWindow::ui->spinBox_16->value()-256)>255)
                *(img2_first+i*4+1)=255;
            else {
                if((*(img1_first+i*4+1)+MainWindow::ui->spinBox_16->value()-256)<0)
                    *(img2_first+i*4+1)=0;
                else {
                    *(img2_first+i*4+1)=(*(img1_first+i*4+1)+MainWindow::ui->spinBox_16->value()-256);
                }

         }
            if((*(img1_first+i*4+2)+MainWindow::ui->spinBox_17->value()-256)>255)
                *(img2_first+i*4+2)=255;
            else {
                if((*(img1_first+i*4+2)+MainWindow::ui->spinBox_17->value()-256)<0)
                    *(img2_first+i*4+2)=0;
                else {
                    *(img2_first+i*4+2)=(*(img1_first+i*4+2)+MainWindow::ui->spinBox_17->value()-256);
                }

         }
            if((*(img1_first+i*4+2)+MainWindow::ui->spinBox_18->value()-256)>255)
                *(img2_first+i*4+3)=255;
            else {
                if((*(img1_first+i*4+3)+MainWindow::ui->spinBox_18->value()-256)<0)
                    *(img2_first+i*4+3)=0;
                else {
                    *(img2_first+i*4+3)=(*(img1_first+i*4+3)+MainWindow::ui->spinBox_18->value()-256);
                }

         }
    }
    }
    MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x,img_y,500,500)));
}

void MainWindow::on_pushButton_4_clicked()
{

    if(begin==0)
    {
        begin=1;
        MainWindow::ui->pushButton_4->setText("停止分区");
    }
    else {
        begin=0;
        MainWindow::ui->pushButton_4->setText("开始分区");
    }

}

void MainWindow::on_pushButton_2_clicked()//选择分区1
{
    fenqu_way=1;
    MainWindow::ui->label_8->setText("分区方式：选择分区");
}

void MainWindow::on_pushButton_3_clicked()//渐变分区0
{
    fenqu_way=0;
    MainWindow::ui->label_8->setText("分区方式：渐变分区");
}

void MainWindow::on_spinBox_valueChanged(int arg1)//B
{
    c_B=arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)//G
{
    c_G=arg1;
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)//R
{
    c_R=arg1;
}

void MainWindow::on_spinBox_4_valueChanged(int arg1)//A
{
    c_A=arg1;
}

void MainWindow::on_pushButton_5_clicked()//撤回
{
    img2=img2_cancel.copy(0,0,img1.width(),img1.height());
    img2_first=(uchar*)img2.bits();
    MainWindow::jilu_copy(jilu_cancel,jilu,img1.width()*img1.height());
    MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x,img_y,500,500)));
}

void MainWindow::on_spinBox_15_valueChanged(int arg1)
{
    MainWindow::ts_range(jilu,jilu_ts);
    MainWindow::ts();
}

void MainWindow::on_spinBox_16_valueChanged(int arg1)
{
    MainWindow::ts_range(jilu,jilu_ts);
    MainWindow::ts();
}

void MainWindow::on_spinBox_17_valueChanged(int arg1)
{
    MainWindow::ts_range(jilu,jilu_ts);
    MainWindow::ts();
}

void MainWindow::on_spinBox_18_valueChanged(int arg1)
{
    MainWindow::ts_range(jilu,jilu_ts);
    MainWindow::ts();
}

void MainWindow::on_pushButton_9_clicked()
{
    int i=0;
    for(i=0;i<img1.width()*img1.height();i++)
    {
        if(jilu[i]==0)
        {
            *(img2_first+i*4)=*(img1_first+i*4);
            *(img2_first+i*4+1)=*(img1_first+i*4+1);
            *(img2_first+i*4+2)=*(img1_first+i*4+2);
            *(img2_first+i*4+3)=*(img1_first+i*4+3);
        }
    }
    MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x,img_y,500,500)));
}

void MainWindow::on_pushButton_7_clicked()
{
    int i=0;
    for(i=0;i<img1.width()*img1.height();i++)
    {
        if(jilu[i]==0)
        {
            *(img2_first+i*4)=0;
            *(img2_first+i*4+1)=0;
            *(img2_first+i*4+2)=0;
            *(img2_first+i*4+3)=0;
        }
    }
    MainWindow::ui->label_2->setPixmap(QPixmap::fromImage(img2.copy(img_x,img_y,500,500)));
}

void MainWindow::on_pushButton_8_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,"选择保存路径","C:\\",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    qDebug()<<filename;
    img2.save(filename+"/zz.png","PNG",-1);
}

void MainWindow::on_spinBox_7_valueChanged(int arg1)
{
    MainWindow::colour_label();
}

void MainWindow::on_spinBox_8_valueChanged(int arg1)
{
    MainWindow::colour_label();
}

void MainWindow::on_spinBox_20_valueChanged(int arg1)
{
    MainWindow::colour_label();
}

void MainWindow::on_spinBox_19_valueChanged(int arg1)
{
    MainWindow::colour_label();
}
