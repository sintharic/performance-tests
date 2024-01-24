#include "header.h"
#include "fields.h"

static vector<double> dummyR;
static vector<Complex> dummyF;



// ----- fieldReal ----- //

fieldReal::fieldReal(size_t nx, size_t ny, double Lx, double Ly, double val) : 
nx(nx), ny(ny), Lx(Lx), Ly(Ly), size(nx*ny) {
  self = std::vector<double>(nx*(ny+2), val);

  if (size > dummyR.size()) dummyR.resize(size);
  IFTplan = fftw_plan_dft_c2r_2d(nx, ny, (fftw_complex*) dummyF.data(), self.data(), FFTW_ESTIMATE); 
}

fieldReal::fieldReal(size_t nx, size_t ny, double Lx, double Ly) : 
  fieldReal(nx, ny, Lx, Ly, 0.) {};

double& fieldReal::operator[](size_t i) {
  #ifdef DEBUG
    if (i >= nx*ny) cerr << "linear index out of range\n";
  #endif
  return self[i];
}

void fieldReal::fill(double val) {for (double& e : self) e = val;};

double* fieldReal::data() {return self.data();};

std::vector<double>::iterator fieldReal::begin() {return self.begin();};
std::vector<double>::iterator fieldReal::end() {return self.begin() + nx*ny;};

void fieldReal::fromIFT(fieldFour& arrayF) {
  memcpy(dummyF.data(), arrayF.data(), arrayF.size*sizeof(Complex));
  fftw_execute(IFTplan);
}



// ----- fieldFour ----- //

fieldFour::fieldFour(size_t nx, size_t ny, double Lx, double Ly, Complex val) : 
nx(nx), ny(ny), Lx(Lx), Ly(Ly), nyHP1(ny/2+1), size(nx*(ny+2)) {
  self = std::vector<Complex>(size, val);

  if (size > dummyF.size()) dummyF.resize(nx*ny);

  FTplan = fftw_plan_dft_r2c_2d(nx, ny, dummyR.data(), (fftw_complex*) self.data(), FFTW_ESTIMATE);

  // initialize weights
  weight.resize(size);
  for (size_t k = 0; k < size; ++k) {
    size_t iqy = get_iqy(k);
    weight[k] = 2;
    if ( (iqy==0) || (iqy==ny/2) ) weight[k] = 1;
  }
}

fieldFour::fieldFour(size_t nx, size_t ny, double Lx, double Ly) : 
  fieldFour(nx, ny, Lx, Ly, 0.) {};

Complex& fieldFour::operator[](size_t i) {
  #ifdef DEBUG
    if (i >= size) cerr << "linear index out of range\n";
  #endif
  return self[i];
}

void fieldFour::fill(Complex val) {for (Complex& e : self) e = val;};

Complex *fieldFour::data() {return self.data();};

std::vector<Complex>::iterator fieldFour::begin() {return self.begin();};
std::vector<Complex>::iterator fieldFour::end() {return self.begin() + nx*nyHP1;};

void fieldFour::fromFT(fieldReal& arrayR) {
  memcpy(dummyR.data(), arrayR.data(), arrayR.size*sizeof(double));
  fftw_execute(FTplan);
  
  // normalize
  for (Complex& a : self) {
    a.real(a.real()/(nx*ny));
    a.imag(a.imag()/(nx*ny));
  }//*/
}