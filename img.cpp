#include "img.h"

Img::Img(QString filePath)
{
    this->oriFilePath = filePath;
    oriImg = new QImage(filePath);
    oriImg->convertToFormat(QImage::Format_Indexed8);
    lastImg = oriImg;
    this->width = oriImg->width();
    this->height = oriImg->height();

}

void Img::convertToGreyScale(){
    QColor oldColor;

    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->oriImg->pixel(x,y));
            int newValue = (oldColor.red()*0.299+oldColor.green()*0.587+oldColor.blue()*0.144);
            QRgb newColor = qRgb(newValue,newValue,newValue);
            lastImg->setPixel(x,y,newColor);
        }
    }
}

void Img::changeBrightness(int value){
    QColor oldColor;

    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->oriImg->pixel(x,y));
            QRgb newColor = qRgb(oldColor.red()+value,oldColor.green()+value,oldColor.blue()+value);
            lastImg->setPixel(x,y,newColor);
        }
    }
}
