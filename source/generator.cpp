#include "generator.h"
#include <cmath>

Generator::Generator(int kernel): kernel_(kernel)
{
}

Generator::~Generator()
{
}

double Generator::Rnd()
{
  int h = kernel_ / Q;
  kernel_ = A * (kernel_ - Q * h) - R * h;
  if (kernel_ < 0)
    kernel_ = kernel_ + static_cast<int>(M);
  return kernel_/M;
}

double Generator::RndZeroOne(double p)
{
  double k = Rnd();

  if (k < p)
    return 1;
  else
    return 0;
}

double Generator::RandMinMax(double min, double max)
{
  return Rnd() * (max - min) + min;
}

double Generator::RndExp(double lambda)
{
  double k = Rnd();
  return -1 * (1.0 / lambda) * log(k);
}
