#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QString>
#include <QColor>

using namespace std;


class Img
{
    //Q_OBJECT
    private:
        QPixmap *oriFileMap;
        QImage *oriImg, *lastImg;
        QString oriFilePath;
        int width, height;

    public:
        Img(QString filePath);
        QImage* getOriginalImg(){ return oriImg;}
        QImage* getLastImg(){return lastImg;}
        void changeBrightness(int value);
        void convertToGreyScale();

};

#endif // IMAGE_H
