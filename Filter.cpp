#include "Filter.h"

float getIntensity(QColor color)
{
    return 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
}

QImage Filter::process(const QImage &img) const
{
    QImage result(img);

    for(int x = 0; x < img.width(); x++)
        for(int y = 0; y < img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }

    return result;
}

QColor InvertFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
    return color;

}

QColor MatrixFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;

    int size = mKernel.getSize();
    int radius = mKernel.getRadius();

    for(int i = -radius; i <= radius; i++)
        for(int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0), clamp(y + i, img.height() - 1, 0));

            returnR += color.red() * mKernel[idx];
            returnG += color.green() * mKernel[idx];
            returnB += color.blue() * mKernel[idx];
        }

    return QColor(clamp(returnR, 255.f, 0.f), clamp(returnG, 255.f, 0.f), clamp(returnB, 255.f, 0.f));
}

QColor SobelFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    float returnRX = 0;
    float returnGX = 0;
    float returnBX = 0;

    float returnRY = 0;
    float returnGY = 0;
    float returnBY = 0;


    int size = mKernel.getSize();
    int radius = mKernel.getRadius();

    for(int i = -radius; i <= radius; i++)
        for(int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;
            QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0), clamp(y + i, img.height() - 1, 0));

            returnRX += color.red() * kernelX[idx];
            returnGX += color.green() * kernelX[idx];
            returnBX += color.blue() * kernelX[idx];

            returnRY += color.red() * kernelY[idx];
            returnGY += color.green() * kernelY[idx];
            returnBY += color.blue() * kernelY[idx];
        }

    float returnR = sqrt(returnRX * returnRX + returnRY * returnRY);
    float returnG = sqrt(returnGX * returnGX + returnGY * returnGY);
    float returnB = sqrt(returnBX * returnBX + returnBY * returnBY);

    return QColor(clamp(returnR, 255.f, 0.f), clamp(returnG, 255.f, 0.f), clamp(returnB, 255.f, 0.f));
}

QColor GrayScaleFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(getIntensity(color), getIntensity(color), getIntensity(color));
    return color;
}

QColor SepiaFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRed(clamp(float(getIntensity(color) + 2 * k), 255.f, 0.f));
    color.setGreen(clamp(float(getIntensity(color) + 0.5 * k), 255.f, 0.f));
    color.setBlue(clamp(float(getIntensity(color) - 1 * k), 255.f, 0.f));
    return color;
}

QColor ContrastFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRed(clamp(color.red() + k, 255, 0));
    color.setGreen(clamp(color.green() + k, 255, 0));
    color.setBlue(clamp(color.blue() + k, 255, 0));
    return color;
}
