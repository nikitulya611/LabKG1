#ifndef FILTER_H
#define FILTER_H

#include <QImage>
#include <cmath>
#include <memory>

template<class T>
T clamp(T value, T max, T min)
{
    if(value > max)
        return max;
    if(value < min)
        return min;
    return value;
}

class Kernel
{
protected:
    std::unique_ptr<float []> data; //данные ядра
    size_t radius; //радиус ядра
    size_t getLen() const {return getSize() * getSize();} //размер памяти ядра
public:
    Kernel(size_t m_radius) : radius(m_radius)
    {
        data = std::make_unique<float []>(getLen()); //выделение памяти

    }

    Kernel(const Kernel& right) : Kernel(right.radius)
    {
        std::copy(right.data.get(), right.data.get() + getLen(), data.get());
    }

    //аксессоры
    size_t  getRadius() const {return radius;}
    size_t getSize() const {return 2 * radius + 1;}
    float operator[](size_t id) const {return data[id];}
    float& operator[](size_t id) {return data[id];}
};

class BlurKernel : public Kernel
{
public:
    using Kernel::Kernel;
    BlurKernel(size_t radius = 2) : Kernel(radius)
    {
        for(size_t i = 0; i < getLen(); i++)
            data[i] = 1.0f / getLen();
    }
};

class GaussianKernel : public Kernel
{
public:
    using Kernel::Kernel;
    GaussianKernel(size_t radius = 2, float sigma = 3.f) : Kernel(radius)
    {
        float norm = 0;
        int signed_radius = static_cast<int>(radius);

        for(int x = -signed_radius; x <= signed_radius; x++)
            for(int y = -signed_radius; y <= signed_radius; y++)
            {
                size_t idx = (x + radius) * getSize() + (y + radius);
                data[idx] = exp( -(x * x + y * y) / (sigma * sigma) );
                norm += data[idx];
            }

        for(size_t i = 0; i < getLen(); i++)
            data[i] /= norm;
    }
};

class SobelKernelX : public Kernel
{
    using Kernel::Kernel;

public:
    SobelKernelX() : Kernel(1)
    {
        data[0] = data[6] = -1;
        data[1] = data[4] = data[7] = 0;
        data[2] = data[8] = 1;
        data[3] = -2;
        data[5] = 2;
    }
};

class SobelKernelY : public Kernel
{
    using Kernel::Kernel;

public:
    SobelKernelY() : Kernel(1)
    {
        data[3] = data[4] = data[5] = 0;
        data[0] = data[2] = -1;
        data[1] = -2;
        data[6] = data[8] = 1;
        data[7] = 2;
    }
};

class SharplessKernel : public Kernel
{
    using Kernel::Kernel;

public:
    SharplessKernel() : Kernel(1)
    {
        data[0] = data[2] = data[6] = data[8] = 0;
        data[1] = data[3] = data[5] = data[7] = -1;
        data[4] = 5;
    }
};

class Filter
{

protected:
    virtual QColor calcNewPixelColor(const QImage& img, int x, int y) const = 0;

public:
   virtual ~Filter() = default;
   virtual QImage process(const QImage& img) const;

};

class InvertFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class MatrixFilter : public Filter
{
protected:
    Kernel mKernel;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

public:
    MatrixFilter(const Kernel& kernel) : mKernel(kernel) {}
    virtual ~MatrixFilter() override = default;
};

class BlurFilter : public MatrixFilter
{
public:
    BlurFilter(size_t radius = 1) : MatrixFilter(BlurKernel(radius)) {}
};

class GaussianFilter : public MatrixFilter
{
public:
    GaussianFilter(size_t radius = 1) : MatrixFilter(GaussianKernel(radius)) {}
};

class GrayScaleFilter : public Filter
{
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class SepiaFilter : public Filter
{
    int k = 21;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class ContrastFilter : public Filter
{
    int k = 69;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class SobelFilter : public MatrixFilter
{
    SobelKernelX kernelX;
    SobelKernelY kernelY;
    QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
    SobelFilter() : MatrixFilter(SobelKernelX()) {}
};

class SharplessFilter : public MatrixFilter
{
public:
    SharplessFilter() : MatrixFilter(SharplessKernel()) {}
};



#endif // FILTER_H
