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
    QString fileName = "/home/ana/Documentos/UFRGS/fpi2/fpi2/test_images/cameraman.jpg";
    //QMessageBox::information(this, "..", fileName);
    QPixmap oldPic (fileName);
    ui->oldImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));

    image = new Img(fileName);
    QImage teste= (*image).getLastImg();
    teste.convertToFormat(QImage::Format_Indexed8);

    QString str;
    str.setNum(teste.depth());
    ui->label->setText(str);
    updateImage();

/*
    QPixmap newPic;
    QImage * timg = (*image).getLastImg();

    newPic = newPic.fromImage((*timg).scaled(300,300,Qt::KeepAspectRatio));
    ui->newImage->setPixmap(newPic);*/

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(){
    QPixmap newPic;
    QImage timg = (*image).getLastImg();

    newPic = newPic.fromImage(timg.scaled(300,300,Qt::KeepAspectRatio));
    ui->newImage->setPixmap(newPic);
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QDir::homePath());
    //QMessageBox::information(this, "..", fileName);
    QPixmap oldPic (fileName);
    ui->oldImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));
    ui->newImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));
    image = new Img (fileName);

}

void MainWindow::on_brigthButton_clicked()
{
    int value = ui->brightBox->value();
    (*image).changeBrightness(value);
    updateImage();
}

void MainWindow::on_histogramButton_clicked()
{
    vector<int> hist = (*image).greyHistogram();



    //BAR SET
    QBarSet* greyBarSet = new QBarSet("Gray");
    greyBarSet->setColor(Qt::black);

    int numPixels = (*image).getNumPixels();
    int largeFreq = hist[0];
    int heightGraph

    for (int x=0;x<255;x++){
        if(hist[x]>largeFreq){
            largeFreq = hist[x];
        }
    }
    for (int x=0;x<255;x++){
        *grayBarSet << hist[x] * 100.0 / numberPixels_f;
    }
    //END BAR SET

    //SERIES
    QStackedBarSeries* series = new QStackedBarSeries();
    series->append(greyBarSet);

    //create chart
    QChart *chart = new QChart();
    chart->setTitle("Histogram");
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    //pChart->removeAllSeries();

    // add series to the chart and update axisY

    axisY->setRange(0, 255);
    chart->addSeries(pSeries);

    //VIEW
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->originalChart);

}

void MainWindow::on_resetButton_clicked()
{
    (*image).resetImage();
    updateImage();



}

void MainWindow::on_contrastButton_clicked()
{
    int value = ui->contrastBox->value();
    (*image).changeContrast(value);
    updateImage();
}

void MainWindow::on_negativeButton_clicked()
{
    (*image).negative();
    updateImage();
}
