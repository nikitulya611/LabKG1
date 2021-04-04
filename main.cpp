#include <iostream>
#include <QDebug>
#include <QImage>
#include "Filter.h"

using namespace std;

int main(int argc, char* argv[])
{
    //cout << argc << endl;
    string s;
    QImage img;
    for(int i = 2; i < argc; i++)
    {
        cout << "argv = " << argv[i];
        s = argv[i];
        cout << img.load(QString(s.c_str()));

        InvertFilter invert;
        cout << invert.process(img).save("Images/Invert" + QString::fromStdString(argv[i]));

        BlurFilter blur;
        blur.process(img).save("Images/Blur" + QString::fromStdString(argv[i]));

        GaussianFilter gaussBlur;
        gaussBlur.process(img).save("Images/GaussBlur" + QString::fromStdString(argv[i]));

        GrayScaleFilter grayScale;
        grayScale.process(img).save("Images/GrayScale" + QString::fromStdString(argv[i]));

        SepiaFilter sepia;
        sepia.process(img).save("Images/Sepia" + QString::fromStdString(argv[i]));

        ContrastFilter contrast;
        contrast.process(img).save("Images/Contrast" + QString::fromStdString(argv[i]));

        SobelFilter sobel;
        sobel.process(img).save("Images/Sobel" + QString::fromStdString(argv[i]));

        SharplessFilter sharpless;
        sharpless.process(img).save("Images/Sharpless" + QString::fromStdString(argv[i]));

    }

    cout << "Success";
    system("pause");
    return 0;
}
