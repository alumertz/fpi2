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
QChartView *oriChartView, *newChartView, *resChartView;
QBarSeries *series;
QChart *oriChart, *newChart, *resChart;
QImage oriImage, proImage, resImage;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString fileName = "/home/ana/Documentos/UFRGS/fpi2/fpi2/test_images/manuelOriginal.png";
    //QMessageBox::information(this, "..", fileName);
    QPixmap oldPic (fileName);
    ui->oriImage->setPixmap(oldPic.scaled(300,300,Qt::KeepAspectRatio));


    image = new Img(fileName);
    oriImage= (*image).getLastImg();
    proImage= (*image).getLastImg();

    QString str;
    ui->label->setText(str);
    updateImage();


    // CHART
    oriChart = createBarCharts();
    newChart = createBarCharts();
    resChart = createBarCharts();

    //ORICHART VIEW
    oriChartView = new QChartView(oriChart);
    oriChartView->setRenderHint(QPainter::Antialiasing);
    oriChartView->setParent(ui->originalChart);

    //NEWCHART VIEW
    newChartView = new QChartView(newChart);
    newChartView->setRenderHint(QPainter::Antialiasing);
    newChartView->setParent(ui->newChart);

    //RESCHART VIEW
    resChartView = new QChartView(resChart);
    resChartView->setRenderHint(QPainter::Antialiasing);
    resChartView->setParent(ui->resChart);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(){
    QPixmap newPic;
    proImage = (*image).getLastImg();

    newPic = newPic.fromImage(proImage.scaled(300,300,Qt::KeepAspectRatio));
    ui->proImage->setPixmap(newPic);

}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QDir::homePath());
    //QMessageBox::information(this, "..", fileName);
    image = new Img (fileName);
    QImage oriImg = image->getOriImg();
    QImage lastImg = image->getLastImg();

    QPixmap oriPic;
    oriPic = oriPic.fromImage(oriImg.scaled(300,300,Qt::KeepAspectRatio));

    QPixmap lastPic;
    lastPic = lastPic.fromImage(lastImg.scaled(300,300,Qt::KeepAspectRatio));

    ui->oriImage->setPixmap(oriPic.scaled(300,300,Qt::KeepAspectRatio));
    ui->proImage->setPixmap(lastPic.scaled(300,300,Qt::KeepAspectRatio));


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
    //Orginal image
    QImage oriGrey = (*image).convertToGreyScale((*image).getOriImg());
    vector<int> hist = (*image).greyHistogram(oriGrey);
    updateChart(hist, oriChart,oriChartView);

    //Last Image
    QImage lastGrey = (*image).convertToGreyScale(proImage);
    vector<int> newHist = (*image).greyHistogram(lastGrey);
    updateChart(newHist, newChart,newChartView);
    updateImage();

    //RESCHART TEMPORARILY
    //vector<int> cumHist = (*image).greyHistogramCum(resImage);
    //updateChart(cumHist, resChart,resChartView);

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
    //QImage lastGrey = (*image).convertToGreyScale((*image).getLastImg());
    (*image).greyImageEqualization();

    vector<int> newHist = (*image).greyHistogram((*image).getLastImg());

    updateChart(newHist, newChart,newChartView);
    updateImage();
}

void MainWindow::on_matchingButton_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this, QDir::homePath());
    //QMessageBox::information(this, "Essa imagem ", fileName);

    //GETS SECOND IMAGE
    QImage matchImg("/home/ana/Documentos/UFRGS/fpi2/fpi2/test_images/portraitTarget.png");


    //DISPLAY SECOND IMAGE SELECTED
    QPixmap secPic ("/home/ana/Documentos/UFRGS/fpi2/fpi2/test_images/portraitTarget.png");
    ui->proImage->setPixmap(secPic.scaled(300,300,Qt::KeepAspectRatio));
    QImage secGrey = (*image).convertToGreyScale(matchImg);
    vector<int> histSec = (*image).greyHistogram(secGrey);
    updateChart(histSec, newChart,newChartView);

    //RESULT IMAGE

    resImage = (*image).greyHistMatching(matchImg);

    QPixmap resPic;
    resPic = resPic.fromImage(resImage.scaled(300,300,Qt::KeepAspectRatio));
    ui->resImage->setPixmap(resPic.scaled(300,300,Qt::KeepAspectRatio));

    vector<int> histRes = (*image).greyHistogram(resImage);
    updateChart(histRes, resChart,resChartView);




}

void MainWindow::on_zoomOutButton_clicked()
{


    int zoomX = ui->zoomXbox->value();
    int zoomY = ui->zoomYbox->value();
    QImage img = (*image).zoomOut(zoomX,zoomY);

    QPixmap newPic;
    newPic = newPic.fromImage(img);
    ui->proImage->setPixmap(newPic);

    QPixmap oldPic;
    oldPic = oldPic.fromImage((*image).getOriImg());
    ui->oriImage->setPixmap(oldPic);
}


void MainWindow::on_zoomInButton_clicked()
{
    QImage img = (*image).zoomIn();

    QPixmap newPic;
    newPic = newPic.fromImage(img);
    ui->proImage->setPixmap(newPic);

    QPixmap oldPic;
    oldPic = oldPic.fromImage((*image).getOriImg());
    ui->oriImage->setPixmap(oldPic);
}

