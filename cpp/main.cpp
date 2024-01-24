#include "main.h"
#include <iomanip>
#include <bitset>
#include <array>

void check_fftw_types();

int main(int argc, char const *argv[]) {
  fieldReal dispR(16, 16, 1., 1., 1.);
  fieldFour dispF(16, 16, 1., 1.);
  io::writeReal("before.dat", dispR);
  dispF.fromFT(dispR);
  cout << dispF[0] << "\n";
  dispR.fromFT(dispF);
  io::writeReal("after.dat", dispR);
  
  check_fftw_types();

  return 0;
}

// from https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
// assumes little-endian
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

void check_fftw_types() {
  cout << "\n------------------\n";
  cout << " check_fftw_types \n";
  cout << "------------------\n";
  Complex val1 = Complex(1.2, 3.14);
  fftw_complex val2 = {1.2, 3.14};
  cout << val1 << " as complex<double,double>*:\n";
  printBits(sizeof(Complex), &val1);
  cout << val1 << " as fftw_complex:\n";
  printBits(sizeof(double[2]), val2);
  cout << "------------------\n\n";
}