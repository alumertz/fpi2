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

    public:
        Img(QString filePath);
        void resetImage();
        QImage getOriImg(){ return oriImg;}
        QImage getLastImg(){return lastImg;}
        void changeBrightness(int value);
        QImage convertToGreyScale(QImage img);
        void changeContrast(int value);
        void negative();
        QMap<QRgb,long> histogram();
        int getNumPixels();
        vector<int> greyHistogram(QImage img);
        vector<int> greyHistogramCum(QImage img);
        void greyImageEqualization();
        QImage greyHistMatching(QImage matchImg);
        QRgb calcColor(int pix, int piy, int x, int y);
        QImage zoomOut(int sx, int sy );
};

#endif // IMAGE_H
