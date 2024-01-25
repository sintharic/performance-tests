#include "header.h"
#include "io.h"

fieldReal io::readReal(const string& filename, int column){ 

  ifstream configIn(filename);
  if (!configIn.is_open()) {
    cerr << "cannot open file: " << filename << "\n";
    terminate();
  }

  size_t nx, ny;
  double dummy, value, Lx, Ly;
  string str;

  // Read Lx and Ly from the last line: 
  configIn.seekg(-1, std::ios_base::end);
  while (configIn.peek() == '\n') configIn.seekg(-1, std::ios_base::cur);
  while (configIn.peek() != '\n') configIn.seekg(-1, std::ios_base::cur);
  configIn >> Lx >> Ly;
  // cout << "---\n" << Lx << "\t" << Ly << configIn.rdbuf() << "---\n";//DEBUG
  configIn.seekg(0);


  char dummyChar; //used to hold the "#" character in array.old
  configIn >> dummyChar >> nx >> ny;

  fieldReal arrayR(nx, ny, Lx, Ly);
  for (size_t ix = 0; ix < nx; ix++){
    for (size_t iy = 0; iy < ny; iy++){
      for (int i = 0; i < column; ++i) configIn >> dummy; // skip other columns 
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
  output << "\n";

  // fields are 1D in y direction
  if(arrayR.nx==1) {
    for (size_t iy=0; iy <= arrayR.ny; iy+=yStep) {
      output << iy*dy; 
      output << "\t" << arrayR[iy];
      output << "\n"; 
    }
  } 
  
  // fields are 1D in x direction (this should not happen!)
  else if (arrayR.ny==1) {
    for (size_t ix=0; ix <= arrayR.nx; ix+=xStep) {
      output << ix*dx;
      output << "\t" << arrayR[ix];
      output << "\n"; 
    }
  } 
  
  // fields are 2D
  else {
    for (size_t ix=0; ix <= arrayR.nx; ix+=xStep) {
      for (size_t iy=0; iy <= arrayR.ny; iy+=yStep) {
        output << ix*dx << "\t" << iy*dy;
        output << "\t" << arrayR.pbc(ix,iy);
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