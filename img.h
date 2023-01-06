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
        QImage getOriginalImg(){ return oriImg;}
        QImage getLastImg(){return lastImg;}
        void changeBrightness(int value);
        void convertToGreyScale();
        void changeContrast(int value);
        void negative();
        QMap<QRgb,long> histogram();
        vector<int> greyHistogram();
        int getNumPixels();
        vector<int> greyHistogramCum();
        void greyImageEqualization();

};

#endif // IMAGE_H
