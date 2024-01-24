#ifndef FIELDS_H
#define FIELDS_H

#include "header.h"
#include <fftw3.h>



class fieldReal;
class fieldFour;



class fieldReal {
private:
  std::vector<double> self;
  fftw_plan IFTplan;

public:
  const size_t size;
  const size_t nx, ny;
  const double Lx, Ly;

  inline size_t get_lin_idx(size_t ix, size_t iy) { return(ix*ny+iy); };
  inline size_t get_ix(size_t k) { return k/ny; }
  inline size_t get_iy(size_t k) { return k%ny; }

  inline double& at(size_t ix, size_t iy) {
    #ifdef DEBUG
      if (ix >= nx) cerr << "x-index out of range\n";
      if (iy >= ny) cerr << "y-index out of range\n";
    #endif
    return self[ix*ny + iy];
  }

  inline double& pbc(int32_t ix, int32_t iy) {
    ix = (ix + nx)%nx;
    iy = (iy + ny)%ny;
    return self[ix*ny + iy];
  }

  fieldReal(size_t nx, size_t ny, double Lx, double Ly);
  fieldReal(size_t nx, size_t ny, double Lx, double Ly, double val);
  
  double& operator[](size_t i);
  
  void fill(double val);
  
  double *data();
  
  std::vector<double>::iterator begin();
  std::vector<double>::iterator end();

  void fromIFT(fieldFour&);
};



class fieldFour {
private:
  std::vector<Complex> self;
  const size_t nyHP1;
  fftw_plan FTplan;
  vector<uint8_t> weight;

public:
  const size_t size;
  const size_t nx, ny;
  const double Lx, Ly;

  inline size_t get_lin_idx(size_t ix, size_t iy) { return(ix*nyHP1+iy); };
  inline size_t get_iqx(size_t k) { return k/nyHP1; }
  inline size_t get_iqy(size_t k) { return k%nyHP1; }
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

  inline Complex& pbc(int32_t ix, int32_t iy) {
    ix = (ix + nx)%nx;
    iy = (iy + nyHP1)%nyHP1;
    return self[ix*nyHP1 + iy];
  }

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