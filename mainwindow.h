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

private:
    Ui::MainWindow *ui;
    QChart *createBarChart(std::vector<int> &hist) const;
};
#endif // MAINWINDOW_H
