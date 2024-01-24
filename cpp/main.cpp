#include "main.h"

int main(int argc, char const *argv[]) {
  fieldReal dispR(16, 16, 1., 1., 1.);
  fieldFour dispF(16, 16, 1., 1., 0.);
  io::writeReal("before.dat", dispR);
  dispF.fromFT(dispR);
  cout << dispF[0] << "\n";
  // dispF.at(2,2) = 0.5;
  dispR.fromFT(dispF);
  io::writeReal("after.dat", dispR);
  
  return 0;
}