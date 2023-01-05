#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtCharts>
//#include <QChartView>
//#include <QtCharts/QBarSeries>
//#include <QtCharts/QBarSet>
//#include <QtCharts/QLegend>
//#include <QtCharts/QBarCategoryAxis>
//using namespace QtCharts;

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
