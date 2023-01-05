#include "mainwindow.h"
#include "img.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QImage>
#include <QChartView>
#define QSHADES 255


QT_CHARTS_USE_NAMESPACE

Img *image;
QChartView *chartView;
QBarSeries *series;
QChart *chart;

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
    //teste.convertToFormat(QImage::Format_Indexed8);

    QString str;
    str.setNum(teste.depth());
    ui->label->setText(str);
    updateImage();

    vector<int> hist = (*image).greyHistogram();

    // CHART
    chart = createBarChart(hist);


    //CHART VIEW
    chartView = new QChartView(chart);
    //chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->originalChart);



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

QChart *MainWindow::createBarChart(std::vector<int> &hist)const{
    //HERE
    //vector<int> hist = (*image).greyHistogram();
    int largeSet = hist[0];

    // SET
    QBarSet *greySet = new QBarSet("Grey");
    greySet->setColor(Qt::red);
    greySet->setBorderColor(Qt::red);

    // Assign values for each bar
    for (int x=0;x<QSHADES; x++){
        *greySet << hist[x];
        if (hist[x]>largeSet){
            largeSet = hist[x];
        }

    }



    // SERIES
    //QBarSeries *series = new QBarSeries();
    series = new QBarSeries();
    series->append(greySet);

    greySet = new QBarSet("Grey");
    for (int x=0;x<QSHADES; x++){
        *greySet << x;
    }
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Batting Avg by Year");
    chart->createDefaultAxes();



    //AXIS
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setRange(0,QSHADES);
    axisY->setRange(0,1000);
    axisX->setTickCount(10);
    axisY->setTickCount(10);
    axisX->setLabelFormat("%d");
    axisY->setLabelFormat("%d");

    // SET AXIS
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    // CONFIG
    chart->legend()->setVisible(false);
    QRectF r1(0,0,300,280); //(qreal x, qreal y, qreal width, qreal height)
    chart->setPlotArea(r1);

    return chart;
}

void MainWindow::on_histogramButton_clicked()
{

    //SET
    QBarSet *set = new QBarSet("other");
    set->setColor(Qt::red);
    set->setBorderColor(Qt::red);

    vector<int> vet;
    for (int x =0;x< QSHADES;x++){
        *set << x;
    }

    // SERIES
    series = new QBarSeries();
    series->append(set);

    chart->removeAllSeries();
    chart->addSeries(series);


    chartView->update();

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
