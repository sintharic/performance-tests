#include "header.h"
#include "fields.h"

static vector<double> dummyR;
static vector<Complex> dummyF;



// ----- fieldReal ----- //

fieldReal::fieldReal(size_t nx, size_t ny, double Lx, double Ly, double val) : 
nx(nx), ny(ny), Lx(Lx), Ly(Ly) {
  self = std::vector<double>(nx*(ny+2), val);

  if (nx*(ny+2) > dummyR.size()) dummyR.resize(nx*(ny+2));
  if (nx*(ny/2+1) > dummyF.size()) dummyF.resize(nx*(ny/2+1));
  RIFFT = fftw_plan_dft_c2r_2d(nx, ny, (fftw_complex*) dummyF.data(), self.data(), FFTW_ESTIMATE); 
}

fieldReal::fieldReal(size_t nx, size_t ny, double Lx, double Ly) : 
  fieldReal(nx, ny, Lx, Ly, 0.) {};

size_t fieldReal::size() {return self.size();}
double* fieldReal::data() {return self.data();};
void fieldReal::fill(double val) {for (double& e : self) e = val;};
std::vector<double>::iterator fieldReal::begin() {return self.begin();};
std::vector<double>::iterator fieldReal::end() {return self.begin() + nx*ny;};

double& fieldReal::operator[](size_t i) {
  #ifdef DEBUG
    if (i >= nx*ny) cerr << "fieldReal linear index out of range\n";
  #endif
  return self[i];
}

void fieldReal::fromFT(fieldFour& arrayF) {
  #ifdef DEBUG
    if (arrayF.nx != nx) cerr << "Fourier array has wrong shape for FT\n";
    if (arrayF.ny != ny) cerr << "Fourier array has wrong shape for FT\n";
    if (size() > dummyR.size()) cerr << "real array larger than FFT dummy!!!";
    if (arrayF.size() > dummyF.size()) cerr << "Fourier array larger than FFT dummy!!!";
  #endif

  // cout << "fieldReal::fromFT() \n"; //DEBUG
  // cout << "  memcpy: \n"; //DEBUG
  memcpy((fftw_complex*) dummyF.data(), (fftw_complex*) arrayF.data(), arrayF.size()*sizeof(fftw_complex));
  // cout << "  fftw_execute: \n"; //DEBUG
  fftw_execute(RIFFT);
}



// ----- fieldFour ----- //

fieldFour::fieldFour(size_t nx, size_t ny, double Lx, double Ly, Complex val) : 
nx(nx), ny(ny), Lx(Lx), Ly(Ly), nyHP1(ny/2+1) {
  self = std::vector<Complex>(nx*nyHP1, val);

  if (nx*(ny+2) > dummyR.size()) dummyR.resize(nx*(ny+2));
  if (nx*nyHP1 > dummyF.size()) dummyF.resize(nx*nyHP1);

  RFFT = fftw_plan_dft_r2c_2d(nx, ny, dummyR.data(), (fftw_complex*) self.data(), FFTW_ESTIMATE);

  // initialize weights
  weight.resize(nx*nyHP1, 1);
  for (size_t k = 0; k < nx*nyHP1; ++k) {
    size_t iqy = qy_idx(k);
    weight[k] = 2;
    if ( (iqy==0) || (iqy==ny/2) ) weight[k] = 1;
  }
}

fieldFour::fieldFour(size_t nx, size_t ny, double Lx, double Ly) : 
  fieldFour(nx, ny, Lx, Ly, 0.) {};

size_t fieldFour::size() {return self.size();}
Complex *fieldFour::data() {return self.data();};
void fieldFour::fill(Complex val) {for (Complex& e : self) e = val;};
std::vector<Complex>::iterator fieldFour::begin() {return self.begin();};
std::vector<Complex>::iterator fieldFour::end() {return self.begin() + nx*nyHP1;};

Complex& fieldFour::operator[](size_t i) {
  #ifdef DEBUG
    if (i >= size()) cerr << "fieldFour linear index out of range\n";
  #endif
  return self[i];
}

void fieldFour::fromFT(fieldReal& arrayR) {
  #ifdef DEBUG
    if (arrayR.nx != nx) cerr << "real array has wrong shape for FT\n";
    if (arrayR.ny != ny) cerr << "real array has wrong shape for FT\n";
    if (arrayR.size() > dummyR.size()) cerr << "real array larger than FFT dummy!!!";
    if (size() > dummyF.size()) cerr << "Fourier array larger than FFT dummy!!!";
  #endif

  // cout << "fieldFour::fromFT() \n"; //DEBUG
  // cout << "  memcpy: \n"; //DEBUG
  memcpy(dummyR.data(), arrayR.data(), arrayR.nx*arrayR.ny*sizeof(double));
  // cout << "  fftw_execute: \n"; //DEBUG
  fftw_execute(RFFT);
  
  // normalize
  // cout << "  normalize: \n"; //DEBUG
  for (Complex& a : self) {
    a.real(a.real()/(nx*ny));
    a.imag(a.imag()/(nx*ny));
  }//*/
}