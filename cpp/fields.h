#ifndef FIELDS_H
#define FIELDS_H

#include "header.h"
#include <fftw3.h>



class fieldReal;
class fieldFour;



class fieldReal {
private:
  std::vector<double> self;
  fftw_plan RIFFT;

public:
  const size_t nx, ny;
  const double Lx, Ly;

  inline size_t lin_idx(size_t ix, size_t iy) { return(ix*ny+iy); };
  inline size_t x_idx(size_t k) { return k/ny; }
  inline size_t y_idx(size_t k) { return k%ny; }

  inline double& at(size_t ix, size_t iy) {
    #ifdef DEBUG
      if (ix >= nx) cerr << "x-index out of range\n";
      if (iy >= ny) cerr << "y-index out of range\n";
    #endif
    return self[ix*ny + iy];
  }

  inline double& pbc(int64_t ix, int64_t iy) {
    #ifdef DEBUG
      if (ix < (int64_t) -nx) cerr << "x-index out of range\n";
      if (iy < (int64_t) -ny) cerr << "y-index out of range\n";
    #endif
    ix = (ix + nx)%nx;
    iy = (iy + ny)%ny;
    return self[ix*ny + iy];
  }

  size_t size();
  fieldReal(size_t nx, size_t ny, double Lx, double Ly);
  fieldReal(size_t nx, size_t ny, double Lx, double Ly, double val);
  
  double& operator[](size_t i);
  
  void fill(double val);
  
  double *data();
  
  std::vector<double>::iterator begin();
  std::vector<double>::iterator end();

  void fromFT(fieldFour&);
};



class fieldFour {
private:
  std::vector<Complex> self;
  const size_t nyHP1;
  fftw_plan RFFT;
  vector<uint8_t> weight;

public:
  const size_t nx, ny;
  const double Lx, Ly;

  inline size_t lin_idx(size_t ix, size_t iy) { return(ix*nyHP1+iy); };
  inline size_t qx_idx(size_t k) { return k/nyHP1; }
  inline size_t qy_idx(size_t k) { return k%nyHP1; }
  inline size_t get_abs_iqx(size_t k){ 
    size_t jqx = k/nyHP1;
    if (jqx > nx/2) jqx = nx - jqx;
    return jqx; 
  }
  inline size_t get_abs_iqy(size_t k) { return k%nyHP1; }
  

  inline Complex& at(size_t ix, size_t iy) {
    #ifdef DEBUG
      if (ix >= nx) cerr << "x-index out of range\n";
      if (iy >= nyHP1) cerr << "y-index out of range\n";
    #endif
    return self[ix*nyHP1 + iy];
  }

  inline Complex& pbc(int64_t ix, int64_t iy) {
    #ifdef DEBUG
      if (ix < (int64_t) -nx/2) cerr << "x-index out of range\n";
      if (iy < (int64_t) -ny/2) cerr << "y-index out of range\n";
    #endif
    ix = (ix + nx)%nx;
    iy = (iy + nyHP1)%nyHP1;
    return self[ix*nyHP1 + iy];
  }

  size_t size();
  fieldFour(size_t nx, size_t ny, double Lx, double Ly);
  fieldFour(size_t nx, size_t ny, double Lx, double Ly, Complex val);
  
  Complex& operator[](size_t i);

  void fill(Complex val);
  
  Complex *data();

  std::vector<Complex>::iterator begin();
  std::vector<Complex>::iterator end();

  void fromFT(fieldReal&);
};

#endif //FIELDS_H