#include "img.h"
#include <typeinfo>

Img::Img(QString filePath)
{
    this->oriFilePath = filePath;

    oriImg.load(filePath);
    this->lastImg = this->oriImg.copy();
    //lastImg.load(filePath);

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
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            oldColor = QColor(img.pixel(x,y));
            int newValue = (oldColor.red()*0.299+oldColor.green()*0.587+oldColor.blue()*0.114);
            QRgb newColor = qRgb(newValue,newValue,newValue);
            img.setPixel(x,y,newColor);
        }
    }
    this->lastImg = img.copy();
    return this->lastImg;
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

void Img::changeContrast(float value){
    QColor oldColor;
    QRgb newColor;
    int red, green, blue;
    float per = value;
    //float per = value*1.0/255;

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

vector<int> Img::normalize(vector<int> hist){
    double max = *max_element(std::begin(hist), std::end(hist));
    vector <int> nhist;
    for (int x=0;x<hist.size();x++){
        nhist.push_back(round(hist[x]*1.0 *255/max));
    }
    return nhist;
}

vector<int> Img::histLab(vector<vector<double>> img){

    std::vector<int> hist(117, 0);
    int l;
    //iterates over image pixels adding occurences
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            l = round(img[x][y]);
            //qDebug() << img[x][y] << " " <<  round(img[x][y]);
            hist[l] ++;
        }
    }
    return hist;
}

QImage Img::labEqualization(QImage img){//Equalizes lastImg, turning grey, saves in lastImg

    vector<vector<vector<double>>> lab = fromImgToLab(img);
    vector<vector<double>> l (width, vector<double>(height, 0));

    for (int x=0;x<width;x++){
        for(int y =0;y<height;y++){
            l[x][y] = lab[x][y][0];
            /*if (x<20 && y<20){
                qDebug() << l[x][y];
            }*/
        }
    }
    vector<int> hist = histLab(l);
    double max = *max_element(std::begin(hist), std::end(hist));
    qDebug() << " max " << max <<" size " << hist.size();


    vector<int> histCum = histogramCumLAB(hist);
    double maxC = *max_element(std::begin(histCum), std::end(histCum));
    qDebug() << " max " << maxC <<" size " << histCum.size();

    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            lab[x][y][0] = histCum[round(l[x][y])];
            if (x<5 && y<5){
               qDebug() << round(l[x][y]) <<" " << histCum[round(l[x][y])] <<" " <<  lab[x][y][0];
            }

        }
    }
    QImage rgb = fromLabToImg(lab);


    return rgb;
}

QImage Img::fromLabToImg(vector<vector<vector<double>>> lab){
    vector<vector<vector<int>>> cRGB (width, vector<vector<int>>(height, vector<int>(3,0)));
    QImage img (QSize(width, height), this->lastImg.format());
    QRgb color;
    for (int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            cRGB[x][y] = lab2srgb(lab[x][y]);

//            if(x <10 && y <10){
//                qDebug() << "l " << lab[x][y][0] << " a " << lab[x][y][1] << " b "<< lab[x][y][2];
//                qDebug()<< cRGB[x][y][0] << " " << cRGB[x][y][1] << " " << cRGB[x][y][2];
//            }
            color= qRgb(cRGB[x][y][0],cRGB[x][y][1],cRGB[x][y][2]);
            lastImg.setPixel(x,y,color);
        }
    }
    return lastImg;
}

vector<vector<vector<double>>> Img::fromImgToLab(QImage img){
    vector<vector<vector<int>>> cRGB (width, vector<vector<int>>(height, vector<int>(3,0)));
    vector<vector<vector<double>>> lab (width, vector<vector<double>>(height, vector<double>(3,0)));

    for (int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            cRGB[x][y][0] = img.pixelColor(x,y).red();
            cRGB[x][y][1] = img.pixelColor(x,y).green();
            cRGB[x][y][2] = img.pixelColor(x,y).blue();

            lab[x][y] = srgb2lab(cRGB[x][y]);

//            if(x <20 && y <20){
//                qDebug() << "r " << cRGB[x][y][0] << " g " << cRGB[x][y][1] << " b "<< cRGB[x][y][2];
//                qDebug() << "l " << lab[x][y][0] << " a " << lab[x][y][1] << " b "<< lab[x][y][2];
//            }

        }
    }
    return lab;
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

vector<int> Img::histogramCumLAB(vector<int> hist){ //receves coloured image, but returns its grey histogram
    float scale = 116.0/this->numPixels;

    //vector<int> hist = this->greyHistogram(img);
    vector<float> histCum (hist.size(), 0);
    vector <int> histCum2 (hist.size(), 0);

    histCum[0] = scale * hist[0];

    for (int x=1;x<hist.size();x++){
        histCum[x] = histCum[x-1] + scale *hist[x];
        //qDebug() << x<< "," << histCum[x] <<";" << "s*h "<< scale *hist[x];
    }

    for (int x=0;x<hist.size();x++){
        histCum2[x] = static_cast<int>(round(histCum[x]));
        //qDebug() <<histCum[x]<< "," << histCum2[x] <<";";
    }
    return histCum2;
}

vector<int> Img::histogramCum(vector<int> hist){ //receves coloured image, but returns its grey histogram
    float scale = 255.0/this->numPixels;

    //vector<int> hist = this->greyHistogram(img);
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

void Img::colorImageEqualization(){//Equalizes lastImg, turning grey, saves in lastImg
    QRgb newColor ;
    QImage img = this->lastImg;
    vector<int> hist = greyHistogram(img);
    vector<int> histCum = this->histogramCum(hist);
    int sR,sG,sB;
    int nsR,nsG,nsB;


    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            sR = QColor(img.pixel(x,y)).red();
            nsR = histCum[sR];

            sG = QColor(img.pixel(x,y)).green();
            nsG = histCum[sG];

            sB = QColor(img.pixel(x,y)).blue();
            nsB = histCum[sB];

            newColor = qRgb(nsR,nsG,nsB);
            lastImg.setPixel(x,y,newColor);
        }
    }
}

void Img::greyImageEqualization(){//Equalizes lastImg, turning grey, saves in lastImg
    QRgb newColor ;
    QImage img = this->convertToGreyScale(this->lastImg);
    vector<int> hist = greyHistogram(img);
    vector<int> histCum = this->histogramCum(hist);
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

    int minDif = abs(histCumMatch[0]- search);
    int shadeMinDif = 0;
    int newDif;
    for (int x=1; x<=255;x++){ //iterates over shades of image finding the closest
        newDif = abs(histCumMatch[x]-search);
        if (newDif< minDif){
            minDif = newDif;            
            shadeMinDif = x;
        }
    }
    qDebug() << "search" << search << " value "<< histCumMatch[shadeMinDif]<< " shademindif "<< shadeMinDif << " minDif "<<minDif ;


    return shadeMinDif;
    //return histCumMatch[shadeMinDif];
}



QImage Img::greyHistMatching(QImage matchImg){//original = src, match=target, last= HM_src
    QImage resImg (this->getOriImg().size(), this->getOriImg().format());

    vector<int> histOri = this->greyHistogram(this->getOriImg());
    vector<int> histMatch = this->greyHistogram(matchImg);
    vector<int> histCumOri = this->histogramCum(histOri); //F1
    vector<int> histCumMatch = this->histogramCum(histMatch); //F2

    vector<int> match (256, 0);
    int shade, newShade;
    QRgb newColor;
    for (int x=0; x<=255;x++){
        match[x] = findClosestShade(histCumOri[x], histCumMatch);  //find G2 in which F1(G1) = F2(G2)
    }

    for (int x=0; x<=255;x++){
        qDebug() << "match[" <<x << "] " << " " << match[x];
    }
    for(int x = 0; x<this->width; x++){
        for(int y = 0; y<this->height; y++){
            shade = QColor(this->oriImg.pixel(x,y)).red();
            newShade = match[shade];
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

    QRgb newColor;
    QImage img (QSize(timesx, timesy), this->lastImg.format());

    int xrec, yrec;

    for (int x = 0; x < timesx; x++) {
        for (int y = 0; y < timesy; y++) {

            //se é o ultimo pixel da imagem nova e a imagem não deu divisao correta
            if ((x == timesx - 1 && width % sx != 0) && (y == timesy - 1 && height % sy != 0)) {
                xrec = width % sx;
                yrec = height % sy;
            }
            else if(y == timesy - 1 && height % sy != 0){
                xrec = sx;
                yrec = height % sy;
            }
            else if(x == timesx - 1 && width % sx != 0){
                xrec = width % sx;
                yrec = sy;
            }
            else { //retangulo normal
              xrec = sx;
              yrec = sy;
            }
            newColor = calcColor(xrec, yrec,x*sx,y*sy);
            img.setPixel(x,y,newColor);
        }
    }

    this->lastImg = img.copy();
    this->width = this->lastImg.width();
    this->height = this->lastImg.height();
    return this->lastImg;

}

QRgb Img::calcColor(int pix, int piy, int x, int y){
    int addR =0, addG =0, addB =0, pixels = pix*piy;
    QRgb newColor;
    QColor oldColor;

    for (int i = x; i < pix+x; i++) {
        for (int j = y; j < piy+y; j++) {
            oldColor = QColor(this->lastImg.pixel(i,j));
            addR+= oldColor.red();
            addG+= oldColor.green();
            addB+= oldColor.blue();

        }
    }
    addR = addR*1.0/pixels;
    addG = addG*1.0/pixels;
    addB = addB*1.0/pixels;
    newColor = qRgb(addR,addG,addB);
    return newColor;

}

QRgb Img::calcColorZoom(QImage img, int x, int y, int isLateral){
    int red=0;
    int green=0;
    int blue=0;
    QColor color;

    if (isLateral){
        color = QColor(img.pixel(x,y-1));
        red+= color.red();
        green+= color.green();
        blue+= color.blue();

        color = QColor(img.pixel(x,y+1));
        red+= color.red();
        green+= color.green();
        blue+= color.blue();

    }else{
        color = QColor(img.pixel(x-1,y));
        red+= color.red();
        green+= color.green();
        blue+= color.blue();

        color = QColor(img.pixel(x+1,y));
        red+= color.red();
        green+= color.green();
        blue+= color.blue();
    }

    return qRgb(red/2,green/2,blue/2);
}

QImage Img::zoomIn(){
    int width = this->lastImg.width();
    int height = this->lastImg.height();
    int newx = width*2-1, newy = height*2-1;
    QRgb oldPixel, newPixel;
    QImage img (QSize(newx, newy), this->lastImg.format());

    //just the original pixels
    for(int x=0;x<newx;x++){
        for (int y =0;y<newy;y++){
            if (x%2==0 && y%2==0){
                oldPixel = this->lastImg.pixel(x/2,y/2);
                img.setPixel(x,y,oldPixel);
            }
        }
    }
    for(int x=0;x<newx;x++){
        for (int y =0;y<newy;y++){
            if (x%2==0 && y%2!=0 ){//even rows
                newPixel = calcColorZoom(img, x, y, 1);
                img.setPixel(x,y,newPixel);

            }
            else if(x%2!=0 && y%2==0){//even columns
                newPixel = calcColorZoom(img, x, y, 0);
                img.setPixel(x,y,newPixel);
            }
        }
    }
    for(int x=0;x<newx;x++){
        for (int y =0;y<newy;y++){
            if (x%2!=0 && y%2!=0){
                newPixel = calcColorZoom(img, x, y, 0);
                img.setPixel(x,y,newPixel);
            }
        }
    }

    this->lastImg = img.copy();
    this->width = this->lastImg.width();
    this->height = this->lastImg.height();
    return this->lastImg;
}

QImage Img::rotate(int right){

    int nwidth = height, nheight = this->width;
    QImage img (QSize(nwidth, nheight), this->lastImg.format());

    QRgb pix;
    for (int x=0;x<this->width;x++){
        for(int y=0;y<this->height;y++){
            pix = this->lastImg.pixel(x,y);
            if (right){
                img.setPixel(this->height-y-1,x,pix);
            }else{
                img.setPixel(y,this->width-x-1,pix);
            }
        }
    }
    this->lastImg = img.copy();
    this->width = this->lastImg.width();
    this->height = this->lastImg.height();
    return this->lastImg;
}

void rotateArray(float arr[3][3]){
    float copy [3][3];
    copy[0][0] = arr[2][0];
    copy[0][1] = arr[2][1];
    copy[0][2] = arr[2][2];
    copy[2][0] = arr[0][0];
    copy[2][1] = arr[0][1];
    copy[2][2] = arr[0][2];

    copy[1][1] = arr[1][1];
    copy[1][0] = arr[1][2];
    copy[1][2] = arr[1][0];
    for(int x=0;x<3;x++){
        for(int y=0;y<3;y++){
            arr[x][y]=copy[x][y];
        }
    }

}

QImage Img::convolution(float kernel[3][3], int tipo){
    rotateArray(kernel);
    float sumR, sumG, sumB;
    int value;
    QImage img (QSize(width, height), this->lastImg.format());

    for (int x=1;x<width-1;x++){
        for(int y=1;y<height-1;y++){
            sumR =0;
            sumG =0;
            sumB =0;
            for (int k=-1; k<=1;k++){
                for(int j=-1;j<=1;j++){
                    //red
                    value = QColor(this->lastImg.pixel(x+j,y+k)).red();
                    sumR += kernel[j+1][k+1]* value;
                    //green
                    value = QColor(this->lastImg.pixel(x+j,y+k)).green();
                    sumG += kernel[j+1][k+1]* value;
                    //blue
                    value = QColor(this->lastImg.pixel(x+j,y+k)).blue();
                    sumB += kernel[j+1][k+1]* value;
                }
            }
            if (tipo){
                sumR = adjustValue(sumR);
                sumG = adjustValue(sumG);
                sumB = adjustValue(sumB);
            }
            else{
                sumR = adjustValue(sumR+127);
                sumG = adjustValue(sumG+127);
                sumB = adjustValue(sumB+127);
            }

            img.setPixel(x,y,qRgb(sumR,sumG,sumB));
        }
    }
    this->lastImg = img.copy();
    return this->lastImg;
}

vector<double> Img::RBGintToDouble(vector<int> values){
    vector<double> nvalues;
    for (int x=0;x<3;x++){
        nvalues.push_back(values[x]*1.0/255.0);
    }
    return nvalues;
}

vector<double> Img::RGBtoXYZ(vector<double> color){

    double mat[3][3] = {{0.4124564, 0.3575761, 0.1804375,},{0.2126729, 0.7151522, 0.0721750}, {0.0193339, 0.1191920, 0.9503041}};
    vector<double> xyz(3,0);
    for (int x=0;x<3;x++){
        if(color[x]<=0.04045){
            color[x]=color[x]/12.92;
        }
        else{
            color[x]=pow((color[x]+0.055)/1.055,2.4);
        }
        //color[x]=color[x]*100;
    }

    xyz[0]= color[0]*mat[0][0]+color[1]*mat[0][1]+color[2]*mat[0][2];
    xyz[1]= color[0]*mat[1][0]+color[1]*mat[1][1]+color[2]*mat[1][2];
    xyz[2]= color[0]*mat[2][0]+color[1]*mat[2][1]+color[2]*mat[2][2];
    return xyz;
}

vector<double> Img::XYZtoLAB(vector<double> xyz){
    double white[3] = {0.9505, 1.0000, 1.0890}; //primeiro
    //double white[3] =   {0.9481, 1.0000, 1.0730};
    //double white[3] = {0.95047, 1.0000, 1.08883};

    float k=903.3, e=0.008856;
    double xyzr[3], fxyz[3];
    vector<double> lab(3,0);
    xyzr[0] = xyz[0]/white[0];
    xyzr[1] = xyz[1]/white[1];
    xyzr[2] = xyz[2]/white[2];

    for (int x=0;x<3;x++){
        if(xyzr[x]>e){
            fxyz[x]= cbrt(xyzr[x]);
        }
        else{
            fxyz[x]= (k*xyzr[x]+16)/116;
        }
    }
    lab[0]=116*fxyz[1]-16;
    lab[1]=500*(fxyz[0]-fxyz[1]);
    lab[2]=200*(fxyz[1]-fxyz[2]);
    return lab;
}

vector<double> Img::srgb2lab (vector<int> cRGB){
    //qDebug()<<" abiru srgb2lab";
    vector<double> rgb = RBGintToDouble(cRGB);
    vector<double> xyz = RGBtoXYZ(rgb);
    vector<double> lab = XYZtoLAB(xyz);
    return lab;
}

vector<double> Img::LABtoXYZ(vector<double> lab){
    vector<double> xyz (3,0);
    double white[3] = {0.9505, 1.0000, 1.0890};
    float k=903.3, e=0.008856;
    double fx, fy, fz, xr, yr,zr;

    fy = (lab[0]+16)/116;
    fx = (lab[1]/500)+fy;
    fz= fy-(lab[2]/200);

    if (pow(fx,3)>e){
        xr = pow(fx,3);
    }else{
        xr = (116*fx-16)/k;
    }

    if (lab[0]>k*e){
        yr = pow((lab[0]+16)/116,3);
    }else{
        yr = lab[0]/k;
    }

    if (pow(fz,3)>e){
        zr = pow(fz,3);
    }else{
        zr = (116*fz-16)/k;
    }
    xyz[0] = xr*white[0];
    xyz[1] = yr*white[1];
    xyz[2] = zr*white[2];
    return xyz;
}

vector<int> Img::XYZtoRGB(vector<double> xyz){
    double mati[3][3] = {{3.24045, -1.53713, -0.49853},{-0.96926, 1.87601, 0.04155}, {0.05564, -0.20402, 1.05722}};
    vector<double> rgb(3,0);
    vector<int> rgbi(3,0);

    rgb[0] = xyz[0]*mati[0][0]+ xyz[1]*mati[0][1]+xyz[2]*mati[0][2];
    rgb[1] = xyz[0]*mati[1][0]+ xyz[1]*mati[1][1]+xyz[2]*mati[1][2];
    rgb[2] = xyz[0]*mati[2][0]+ xyz[1]*mati[2][1]+xyz[2]*mati[2][2];


    for (int x=0;x<3;x++){
        if (rgb[x]<=0.0031308){
            rgb[x] = 12.92*rgb[x];
        }
        else{
            rgb[x] = 1.055 * pow(rgb[x], (1/2.4))-0.055;
        }
        rgbi[x]= round(rgb[x]*255);
        //cout << rgb[x]*255;
    }
    return rgbi;
}

vector<int> Img::lab2srgb (vector<double> lab){
    vector<double> xyz = LABtoXYZ(lab);

    //cout << "xyz ";
//    for(int x=0;x<3;x++){
//        cout << " " << xyz[x];
//    }
    vector<int> rgb = XYZtoRGB(xyz);

//    cout << "\nrgb ";
//    for(int x=0;x<3;x++){
//        cout << " " << rgb[x];
//    }
    return rgb;
}
