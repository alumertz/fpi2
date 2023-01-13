#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QImage>
#include <QtCharts>
#include <QChartView>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_histogramButton_clicked();

    void on_openButton_clicked();

    void on_brigthButton_clicked();

    void updateImage();

    void on_resetButton_clicked();

    void on_contrastButton_clicked();

    void on_negativeButton_clicked();

    void updateChart(std::vector<int> &hist, QChart * chart, QChartView *chartView);

    void on_equalizeButton_clicked();

    void on_matchingButton_clicked();

    void on_zoomOutButton_clicked();

    void on_zoomInButton_clicked();

    void on_rotateRightButton_clicked();

    void on_rotateLeftButton_clicked();

    void on_convolutionButton_clicked();

    void on_laplacianButton_clicked();

    void on_highPassButton_clicked();

    void on_prewittXButton_clicked();

    void on_prewittYButton_clicked();

    void on_sobelXButton_clicked();

    void on_sobelYButton_clicked();

    void on_gaussianButton_clicked();

private:
    Ui::MainWindow *ui;
    QChart* createBarCharts() const;
    void convolution(float arr[3][3], int tipo);
};
#endif // MAINWINDOW_H
