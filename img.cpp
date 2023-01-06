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
    return this->numPixels;
}

void Img::resetImage(){
    this->lastImg = this->oriImg;
}

QImage Img::convertToGreyScale(QImage img){
    QColor oldColor;
    int width= img.width();
    int height = img.height();
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            oldColor = QColor(img.pixel(x,y));
            int newValue = (oldColor.red()*0.299+oldColor.green()*0.587+oldColor.blue()*0.114);
            QRgb newColor = qRgb(newValue,newValue,newValue);
            img.setPixel(x,y,newColor);
        }
    }
    return img;
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
            newColor = qRgb(red,green,blue);
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
            newColor = qRgb(red,green,blue);
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
            newColor = qRgb(red,green,blue);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

vector<int> Img::greyHistogram(QImage img){
    int color;
    std::vector<int> hist(256, 0);
    int width= img.width();
    int height = img.height();

    //iterates over image pixels adding occurences
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            color = img.pixelColor(x,y).red();
            hist[color] ++;
        }
    }
    return hist;
}

vector<int> Img::greyHistogramCum(QImage img){
    float scale = 255.0/this->numPixels;

    vector<int> hist = this->greyHistogram(img);
    vector<float> histCum (256, 0);
    vector <int> histCum2 (256, 0);

    histCum[0] = scale * hist[0];

    qDebug() << "tste";
    qDebug() << "sacle " << scale;
    qDebug() << "pixels  ";
    qDebug() << this->numPixels;

    for (int x=1;x<=255;x++){
        histCum[x] = histCum[x-1] + scale *hist[x];
        qDebug() << x<< "," << histCum[x] <<";" << "s*h "<< scale *hist[x];
    }

    for (int x=0;x<=255;x++){
        histCum2[x] = static_cast<int>(round(histCum[x]));
        qDebug() <<histCum[x]<< "," << histCum2[x] <<";";
    }
    return histCum2;
}

void Img::greyImageEqualization(){//Equalizes lastImg, turning grey, saves in lastImg
    QRgb newColor ;
    QImage img = this->convertToGreyScale(this->lastImg);
    vector<int> histCum = this->greyHistogramCum(img);
    int shade, newShade;


    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            shade = QColor(img.pixel(x,y)).red();
            newShade = histCum[shade];
            newColor = qRgb(newShade,newShade,newShade);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

int findClosestShade(int search, std::vector<int> histCumMatch){
    int minDif = histCumMatch[0]- search;
    int shadeMinDif = 0;
    int newDif;
    for (int x=1; x<=255;x++){
        newDif = histCumMatch[x]-search;
        if (newDif< minDif){
            minDif = newDif;
            shadeMinDif = x;
        }
    }
    return shadeMinDif;
}

QImage Img::greyHistMatching(QImage matchImg){//original = src, match=target, last= HM_src
    QImage resImg;
    vector<int> histOri = this->greyHistogramCum(this->getOriImg());
    vector<int> histMatch = this->greyHistogramCum(this->getOriImg());
    vector<int> histCumOri = this->greyHistogramCum(matchImg); //F1
    vector<int> histCumMatch = this->greyHistogramCum(matchImg); //F2

    vector<int> match (256, 0);
    int shade, newShade;
    QRgb newColor;

    for (int x=0; x<=255;x++){
        match[x] = findClosestShade(histCumOri[x], histCumMatch);  //find F1(G1) = F2(G2)
    }
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            shade = QColor(this->oriImg.pixel(x,y)).red();
            newShade = match[shade];
            newColor = qRgb(newShade,newShade,newShade);
            lastImg.setPixel(x,y,newColor);
        }
    }

    return resImg;
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
