#include "mainwindow.h"
#include "img.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QImage>

Img *image;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString fileName = "/home/ana/Documentos/UFRGS/fpi2/fpi2/test_images/Gramado_22k.jpg";
    //QMessageBox::information(this, "..", fileName);
    QPixmap oldPic (fileName);
    ui->oldImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));

    image = new Img(fileName);
    //image.convertToGreyScale();

    //QPixmap newPic;
    //QImage * timg = pic.getLastImg();

    //newPic = newPic.fromImage((*timg).scaled(200,200,Qt::KeepAspectRatio));
    //ui->newImage->setPixmap(newPic);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void Mainwindow::updateImage(){
    QImage * timg = (*image).getLastImg();
    //QPixmap newPic = newPic.fromImage((*timg).scaled(200,200,Qt::KeepAspectRatio));
    //ui->newImage->setPixmap(newPic);
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QDir::homePath());
    //QMessageBox::information(this, "..", fileName);
    QPixmap oldPic (fileName);
    ui->oldImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));

    image = new Img (fileName);

}

void MainWindow::on_brigthButton_clicked()
{
    int value = ui->brightBox->value();
    cout << value;
    //image.changeBrightness(value);

}

void MainWindow::on_histogramButton_clicked()
{

}
//setPixmap(QPixmap::fromImage(myImage));
//QImage *nimg;
//nimg = new QImage(fileName);
//img.convertToFormat(QImage::Format_Indexed8);
