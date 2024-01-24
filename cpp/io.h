#ifndef IO_H
#define IO_H

#include "header.h"
#include "fields.h"

namespace io {
  fieldReal readReal(const string&, int);
  void writeReal(const string&, fieldReal&);
  void writeReal(const string&, fieldReal&, int, int);
};

#endif //IO_H