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
    this->lastImg = this->oriImg.copy();
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

vector<int> Img::greyHistogramCum(QImage img){ //receves coloured image, but returns its grey histogram
    float scale = 255.0/this->numPixels;

    vector<int> hist = this->greyHistogram(img);
    vector<float> histCum (256, 0);
    vector <int> histCum2 (256, 0);

    histCum[0] = scale * hist[0];

    for (int x=1;x<=255;x++){
        histCum[x] = histCum[x-1] + scale *hist[x];
        //qDebug() << x<< "," << histCum[x] <<";" << "s*h "<< scale *hist[x];
    }

    for (int x=0;x<=255;x++){
        histCum2[x] = static_cast<int>(round(histCum[x]));
        //qDebug() <<histCum[x]<< "," << histCum2[x] <<";";
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
    qDebug() << "search" << search;
    int minDif = abs(histCumMatch[0]- search);
    qDebug() << "minDif" << minDif;
    int shadeMinDif = 0;
    int newDif;
    for (int x=1; x<=255;x++){
        newDif = abs(histCumMatch[x]-search);
        if (newDif< minDif){
            minDif = newDif;            
            shadeMinDif = x;
            qDebug() << "minDif" << minDif;
            qDebug() << "shademinDif" << shadeMinDif;
        }
    }
    return shadeMinDif;
}

QImage Img::greyHistMatching(QImage matchImg){//original = src, match=target, last= HM_src
    QImage resImg (this->getOriImg().size(), this->getOriImg().format());

    vector<int> histOri = this->greyHistogram(this->getOriImg());
    vector<int> histMatch = this->greyHistogram(this->getOriImg());
    vector<int> histCumOri = this->greyHistogramCum(matchImg); //F1
    vector<int> histCumMatch = this->greyHistogramCum(matchImg); //F2

    vector<int> match (256, 0);
    int shade, newShade;
    QRgb newColor;
    qDebug() << "teste";
    for (int x=0; x<=255;x++){
        match[x] = findClosestShade(histCumOri[x], histCumMatch);  //find F1(G1) = F2(G2)
        //qDebug() << match[x];
    }
    qDebug() << "\nspace\n";
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            shade = QColor(this->oriImg.pixel(x,y)).red();
            newShade = match[shade];
            //qDebug() << newShade;
            newColor = qRgb(newShade,newShade,newShade);
            resImg.setPixel(x,y,newColor);
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

QImage Img::zoomOut(int sx, int sy){
    int width = this->lastImg.width();
    int height = this->lastImg.height();

    int timesx = static_cast<int>(ceil(width*1.0/sx));
    int timesy = static_cast<int>(ceil(height*1.0/sy));

    QColor oldColor;
    QRgb newColor;
    QImage img (QSize(timesx, timesy), this->lastImg.format());

    int pix=sx, piy=sy;

    qDebug() << "old size " << width << " x " << height;
    qDebug() << "new size" << timesx << " x " << timesy;

    for (int x = 0; x < timesx; x++) {
        for (int y = 0; y < timesy; y++) {
            //cout << " x " << x << " y " << y << "\n";
            if ((x == timesx - 1 && width % sx != 0) ||  (y == timesy - 1 && height % sy != 0)) {
              if((y == timesy - 1) && (x == timesx - 1)){
                pix = width % sx;
                piy = height % sy;
              }
              else if (y == timesy - 1)  {
                pix = width % sx;
                piy = sy;
              } else  {
                pix = sx;
                piy = height % sy;
              }

            } else {
          pix = sx;
          piy = sy;
        }

            newColor = calcColor(pix, piy,x*pix,y*piy);

            img.setPixel(x,y,newColor);


        }
    }
    qDebug() << "size img " << img.size();
    qDebug() << "size lastImg" << this->lastImg.size();
    this->lastImg = img.copy();
    qDebug() << "size lastImg" << this->lastImg.size() << "\n";
    this->width = this->lastImg.width();
    this->height = this->lastImg.height();
    return this->lastImg;

}
QRgb Img::calcColor(int pix, int piy, int x, int y){
    int addR =0, addG =0, addB =0, pixels = pix*piy;
    QRgb newColor;
    QColor oldColor;

    if (x==158 && y ==216){
        qDebug() << "x "<< x << " y "<< y;
    }
    //qDebug() << "pixels " << pixels;

    for (int i = 0; i < pix; i++) {
        for (int j = 0; j < piy; j++) {
            oldColor = QColor(this->lastImg.pixel(i+x,j+y));
            addR+= oldColor.red();
            addG+= oldColor.green();
            addB+= oldColor.blue();
            if (x==158 && y ==216){
                qDebug() <<" r " << addR << " g " << addG << " b " << addB;
            }

        }
    }
    addR = addR*1.0/pixels;
    addG = addG*1.0/pixels;
    addB = addB*1.0/pixels;
    newColor = qRgb(addR,addG,addB);
    if (x==158 && y ==216){
        qDebug() <<"soma r " << addR << " g " << addG << " b " << addB;
    }
    //qDebug () << "\n\n break\n\n";
    return newColor;

}
