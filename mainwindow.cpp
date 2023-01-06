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
QChartView *oriChartView, *newChartView;
QBarSeries *series;
QChart *oriChart, *newChart;

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


    // CHART
    oriChart = createBarCharts();
    newChart = createBarCharts();

    //ORICHART VIEW
    oriChartView = new QChartView(oriChart);
    oriChartView->setRenderHint(QPainter::Antialiasing);
    oriChartView->setParent(ui->originalChart);

    //NEWCHART VIEW
    newChartView = new QChartView(newChart);
    newChartView->setRenderHint(QPainter::Antialiasing);
    newChartView->setParent(ui->newChart);



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

QChart* MainWindow::createBarCharts()const{
    //HERE

    // SET
    QBarSet *greySet = new QBarSet("Grey");
    greySet->setColor(Qt::red);
    greySet->setBorderColor(Qt::red);

    // SERIES
    series = new QBarSeries();

    QChart *chart = new QChart();

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

    //QBarSeries *nSeries = new QBarSeries();
    //newChart->setAxisX(axisX, nSeries);
    //newChart->setAxisY(axisY, nSeries);

    // CONFIG
    chart->legend()->setVisible(false);
    chart->setPlotArea(QRectF (0,0,300,280));//QRECTF(qreal x, qreal y, qreal width, qreal height)
    return chart;
}

void MainWindow::updateChart(std::vector<int> &hist, QChart * chart, QChartView *chartView){
    //SET
    QBarSet *set = new QBarSet("other");
    set->setColor(Qt::red);
    set->setBorderColor(Qt::red);

    for (int x =0;x< QSHADES;x++){
        *set << hist[x];
    }

    // SERIES
    series = new QBarSeries();
    series->append(set);

    chart->removeAllSeries();
    chart->addSeries(series);
    chartView->update();

}

void MainWindow::on_histogramButton_clicked()
{
    vector<int> hist = (*image).greyHistogram();
    updateChart(hist, oriChart,oriChartView);


    vector<int> newHist = (*image).greyHistogram();

    updateChart(newHist, newChart,newChartView);
    updateImage();

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




void MainWindow::on_equalizeButton_clicked()
{
    (*image).greyImageEqualization();
    vector<int> newHist = (*image).greyHistogram();

    updateChart(newHist, newChart,newChartView);
    updateImage();
}

