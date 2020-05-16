#ifndef GENERATOR_H_
#define GENERATOR_H_

class Generator
{
public:
  Generator(int kernel);
  ~Generator();

  double Rnd();
  double RndZeroOne(double p);
  double RandMinMax(double min, double max);
  double RndExp(double lambda);

  int return_kernel() { return kernel_; };
  int set_kernel(int k) { kernel_ = k; };
private:

  double p = 0.8;

  int kernel_;
  const double M = 2147483647;
  static const int A = 16807;
  static const int Q = 127773;
  static const int R = 2836;

};

#endif // GENERATOR_H_