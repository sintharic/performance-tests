#include "tests.h"

void check_ft_norm();
void check_fftw_types();
void check_io();

int main(int argc, char const *argv[]) {
  #ifdef DEBUG
    cerr << "!!! RUNNING IN DEBUG MODE !!!\n";
  #endif
  
  check_ft_norm();
  check_io();
  check_fftw_types();

  return 0;
}

// from https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
// this solution assumes little-endian
void printBits(size_t const size, void const * const ptr) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  
  for (i = size-1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
    printf(" ");
  }
  puts("");
}

void check_ft_norm() {
  fieldReal dispR(16, 16, 2., 2., 1.);
  fieldFour dispF(16, 16, 2., 2.);
  io::writeReal("before.dat", dispR);
  dispF.fromFT(dispR);
  cout << dispF[0] << "\n";
  dispR.fromFT(dispF);
  io::writeReal("after.dat", dispR);
}

void check_io() {
  cout << "\n----------\n";
  cout << " check_io \n";
  cout << "----------\n";
  fieldReal a(16, 16, 2., 3.);
  for (size_t i=0; i<a.nx*a.ny; ++i) a[i] = i*i;
  io::writeReal("test.dat", a);
  fieldReal b = io::readReal("test.dat", 2);
  if (a.Lx != b.Lx) cout << "Lx went wrong!\n";
  if (a.Ly != b.Ly) cout << "Ly went wrong!\n";
  for (size_t i=0; i<a.nx*a.ny; ++i) {
    if (a[i] != b[i]) cout << "index " << i << " went wrong!\n";
  }
  cout << "Done.\n";
  cout << "----------\n\n";
}

void check_fftw_types() {
  cout << "\n------------------\n";
  cout << " check_fftw_types \n";
  cout << "------------------\n";
  Complex val1 = Complex(1.2, 3.14);
  fftw_complex val2 = {1.2, 3.14};
  cout << val1 << " read from complex<double,double>*:\n";
  printBits(sizeof(Complex), &val1);
  cout << val1 << " read from fftw_complex:\n";
  printBits(sizeof(double[2]), val2);
  cout << "------------------\n\n";
}