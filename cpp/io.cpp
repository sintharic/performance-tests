#include "header.h"
#include "io.h"

fieldReal io::readReal(const string& filename, int column){ 

  ifstream configIn(filename);
  if (!configIn.is_open()) {
    cerr << "cannot open file: " << filename << "\n";
    terminate();
  }

  int fSizeUpScale = 0;

  uint32_t nx, ny;
  double dummy, value, Lx, Ly;
  string str;
  char dummyChar; //used to hold the "#" character in array.old
  configIn >> dummyChar >> nx >> ny;
  configIn >> dummyChar >> Lx >> Ly;

  fieldReal arrayR(nx, ny, Lx, Ly);
  for (uint32_t ix = 0; ix < nx; ix++){
    for (uint32_t iy = 0; iy < ny; iy++){
      for (int i = 1; i < column; ++i) configIn >> dummy; // skip other columns 
      configIn >> value; getline(configIn, str);
      arrayR.at(ix,iy) = value;
    }
    getline(configIn, str); // skip empty line
  }
  configIn.close();

  return arrayR;
}

void io::writeReal(const string& filename, fieldReal& arrayR, int xStep, int yStep) {
  //change: if input/output ever becomes advanced enough to not break completely 
  //        because of it, we should add column headers here as well.

  double dx = arrayR.Lx/arrayR.nx, dy = arrayR.Ly/arrayR.ny;
  ofstream output(filename);

  // create file header
  int nxNew = arrayR.nx/xStep, nyNew = arrayR.ny/yStep;
  output << "#" << nxNew << "\t" << nyNew << "\n";
  output << "#" << arrayR.Lx << "\t" << arrayR.Ly << "\n\n";

  // fields are 1D in y direction
  if(arrayR.nx==1) {
    for (uint32_t iy=0; iy <= arrayR.ny; iy+=yStep) {
      output << iy*dy; 
      output << "\t" << arrayR[iy];
      output << "\n"; 
    }
  } 
  
  // fields are 1D in x direction (this should not happen!)
  else if (arrayR.ny==1) {
    for (uint32_t ix=0; ix <= arrayR.nx; ix+=xStep) {
      output << ix*dx;
      output << "\t" << arrayR[ix];
      output << "\n"; 
    }
  } 
  
  // fields are 2D
  else {
    for (uint32_t ix=0; ix <= arrayR.nx; ix+=xStep) {
      for (uint32_t iy=0; iy <= arrayR.ny; iy+=yStep) {
        uint32_t k = (ix%arrayR.nx)*arrayR.ny + (iy%arrayR.ny);
        output << ix*dx << "\t" << iy*dy;
        output << "\t" << arrayR[k];
        output << "\n";
      } 
      output << "\n"; // add empty line
    }
  } 

  output.close();
}

void io::writeReal(const string& filename, fieldReal& arrayR) {
  io::writeReal(filename, arrayR, 1, 1);
}