#include "img.h"

Img::Img(QString filePath)
{
    this->oriFilePath = filePath;

    oriImg.load(filePath);
    lastImg.load(filePath);
    /*oriImg = new QImage(filePath);
    oriImg->convertToFormat(QImage::Format_Indexed8);
    lastImg = oriImg;*/


    this->width = oriImg.width();
    this->height = oriImg.height();
    this->numPixels = this->width * this->height;

}

int Img::getNumPixels(){
    return numPixels;
}

void Img::resetImage(){
    this->lastImg = this->oriImg;
}

void Img::convertToGreyScale(){
    QColor oldColor;
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->lastImg.pixel(x,y));
            int newValue = (oldColor.red()*0.299+oldColor.green()*0.587+oldColor.blue()*0.114);
            QRgb newColor = qRgb(newValue,newValue,newValue);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

void Img::changeBrightness(int value){
    QColor oldColor;
    QRgb newColor;
    int red, green, blue;

    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->lastImg.pixel(x,y));
            red = adjustValue(oldColor.red()+value);
            green = adjustValue(oldColor.green()+value);
            blue = adjustValue(oldColor.blue()+value);
            QRgb newColor = qRgb(red,green,blue);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

void Img::changeContrast(int value){
    QColor oldColor;
    QRgb newColor;
    int red, green, blue;
    float per = value*1.0/255; // MUDAR?
    cout << " per " << per;

    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->lastImg.pixel(x,y));
            red = adjustValue(oldColor.red()*per);
            green = adjustValue(oldColor.green()*per);
            blue = adjustValue(oldColor.blue()*per);
            QRgb newColor = qRgb(red,green,blue);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

void Img::negative(){
    QColor oldColor;
    QRgb newColor;
    int red, green, blue;

    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            oldColor = QColor(this->lastImg.pixel(x,y));
            red = 255-oldColor.red();
            green = 255-oldColor.green();
            blue = 255-oldColor.blue();
            QRgb newColor = qRgb(red,green,blue);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

vector<int> Img::greyHistogram(){
    int color;
    std::vector<int> hist(255, 0);

    //iterates over image pixels adding occurences
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            color = lastImg.pixelColor(x,y).red();
            hist[color] ++;
        }
    }
    return hist;
}

QMap<QRgb,long> Img::histogram(){
    QRgb color = 0;
    QMap<QRgb,long> hist;
    //iterates over image pixels and sees if the color is on the histogram
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            color = lastImg.pixel(x,y);
            if(!hist.contains(color)){
                hist.insert(color,0);
            }
        }
    }
    //iterates over image pixels adding occurences
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            color = lastImg.pixel(x,y);
            hist[color] = hist[color]+1;
        }
    }
    return hist;
}

int Img::adjustValue(int value){
    if (value<0){
        value = 0;
    }
    else if (value>255){
        value = 255;
    }
    return value;
}
