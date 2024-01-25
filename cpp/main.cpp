#include "main.h"

void bench_fft();

int main(int argc, char const *argv[]) {
  #ifdef DEBUG
    cerr << "!!! RUNNING IN DEBUG MODE !!!\n";
  #endif
  
  bench_fft();

  return 0;
}

void bench_fft() {
  const size_t n = 1024;
  fieldReal dataRx(n,n,1.,1.);
  fieldFour dataFx(n,n,1.,1.);
  fieldReal dataRy(n,n,1.,1.);
  fieldFour dataFy(n,n,1.,1.);

  for (int i = 0; i < n/4; ++i) {
    int iq = 1 + 2*i;
    // cout << iq << "\n";
    double prefac = 1./(iq*iq);
    if (i%2 == 0) prefac *= -1;
    dataFx.at(iq,0) += Complex(0, prefac);
    dataFx.pbc(-iq,0) -= Complex(0, prefac);
    dataFy.at(0,iq) += Complex(0, prefac);
    dataRx.fromFT(dataFx);
    dataRy.fromFT(dataFy);
    dataFx.fromFT(dataRx);
    dataFy.fromFT(dataRy);
  }
  // io::writeReal("triangular_wave_x.dat", dataRx);
  // io::writeReal("triangular_wave_y.dat", dataRy);
}