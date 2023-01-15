#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QString>
#include <QColor>
#include <QMap>
#include <QDebug>
#include<cmath>
#include <typeinfo>
#include <string>

using namespace std;


class Img
{
    //Q_OBJECT
    private:
        QPixmap *oriFileMap;
        QImage oriImg, lastImg;
        QString oriFilePath;
        int width, height, numPixels;
        int adjustValue(int value);
        vector<double> RBGintToDouble(vector<int> values);
        vector<double> RGBtoXYZ(vector<double> color);
        vector<double> XYZtoLAB(vector<double> xyz);
        vector<double> srgb2lab (vector<int> cRGB);
        vector<double> histCumLab();
        vector<int> lab2srgb (vector<double> lab);
        vector<int> XYZtoRGB(vector<double> xyz);
        vector<double> LABtoXYZ(vector<double> lab);
        QImage fromLabToImg(vector<vector<vector<double>>> lab);
        vector<int> histogramCumLAB(vector<int> hist);
    public:
        Img(QString filePath);
        void resetImage();
        QImage getOriImg(){ return oriImg;}
        QImage getLastImg(){return lastImg;}
        int getWidth(){return width;}
        int getHeight(){return height;}
        void changeBrightness(int value);
        QImage convertToGreyScale(QImage img);
        void changeContrast(float value);
        void negative();
        QMap<QRgb,long> histogram();
        int getNumPixels();
        vector<int> normalize(vector<int> hist);
        vector<int> greyHistogram(QImage img);
        vector<int> histogramCum(vector<int> hist);
        void greyImageEqualization();
        QImage greyHistMatching(QImage matchImg);
        QRgb calcColor(int pix, int piy, int x, int y);
        QImage zoomOut(int sx, int sy );
        QImage zoomIn();
        QImage rotate(int right);
        QImage convolution(float kernel[3][3], int tipo);
        QRgb calcColorZoom(QImage img, int x, int y, int isLateral);
        vector<vector<vector<double>>> fromImgToLab(QImage img);
        vector<int> histLab(vector<vector<double>> img);
        QImage labEqualization(QImage img);
        void colorImageEqualization();
};

#endif // IMAGE_H
