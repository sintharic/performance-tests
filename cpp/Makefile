SHELL = /usr/bin/env bash
default_target: all

COMPILER := g++
CPPFLAGS := -O2 -std=c++11
#WFLAGS   := -Wno-unused-command-line-argument -Wno-shift-count-overflow -Wno-unused-result
OBJECTS  := fields.o fft.o io.o topo.o stats.o generate.o
opts     :=

# Current date (for backups)
TODAY    := $(shell date +%F)
FOLDER   := backup-$(TODAY)



# ------ Makefile info ------ #

# This is the reduced Makefile from src15 onward.
# Refer to src14f/Makefile for more legacy options.



# ------ 'seq' target info ------ #

# The 'seq' target sequentially compiles all objects as .o files and then
# links contMech.exe to them.
# However, (probably due to the use of global variables and namespace,)
# this can lead to untracable error messages on runtime if done incorrectly!



# ------ MinGW installation instructions ------ #

# (from http://www.fftw.org/install/windows.html)

# Download latest FFTW3 tar file from http://www.fftw.org/download.html
# Go to the Download folder, unpack, open a MinGW Terminal and execute:
# ./configure
# make
# make install



# ----- Determine correct FFTW paths ----- #

FFTW_PATH_Lnx := /usr/local
FFTW_PATH_Win := C:/MinGW/msys/1.0/local
FFTW_PATH_Vbx := ~/cp2k-master/tools/toolchain/install/fftw-3.3.8
FFTW_PATH_Ssh := ~/bin/fftw

ifneq "$(wildcard ${FFTW_PATH_Win} )" ""
  # If it exists, use the Windows path
  MSG = " Windows FFTW version"
  FFTW_PATH = $(FFTW_PATH_Win)
else ifneq "$(wildcard ${FFTW_PATH_Vbx} )" ""
  # If it exists, use the CP2K toolchain path
  MSG = " CP2K FFTW version"
  FFTW_PATH = $(FFTW_PATH_Vbx)
else ifneq "$(wildcard ${FFTW_PATH_Ssh} )" ""
  # If it exists, use the small cluster path
  MSG = " Cluster FFTW version"
  FFTW_PATH = $(FFTW_PATH_Ssh)
else
  # If none of those exist, use the default path
  MSG = " Linux/Mac default FFTW version"
  FFTW_PATH = $(FFTW_PATH_Lnx)
endif

FFTW_LIB = $(FFTW_PATH)/lib
FFTW_INC = $(FFTW_PATH)/include
LFFTW := -lfftw3 -L $(FFTW_LIB) -I $(FFTW_INC)



# ----- compilation rules ----- #

fields.o: header.h fields.cpp
	@echo " compiling fields.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) -c fields.cpp -o fields.o > error.log 2>&1

io.o: header.h io.cpp
	@echo " compiling io.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) -c io.cpp -o io.o > error.log 2>&1

fft.o: header.h fft.cpp
	@echo " ---$(MSG) ---"
	@echo " FFTW Paths: - $(FFTW_LIB)"
	@echo "             - $(FFTW_INC)"
	@echo " compiling fft.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) $(LFFTW) -c fft.cpp -o fft.o > error.log 2>&1

topo.o: header.h topo.cpp
	@echo " compiling topo.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) $(LFFTW) -c topo.cpp -o topo.o > error.log 2>&1

stats.o: header.h stats.cpp
	@echo " compiling stats.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) -c stats.cpp -o stats.o > error.log 2>&1

generate.o: header.h generate.cpp
	@echo " compiling generate.o ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) -c generate.cpp -o generate.o > error.log 2>&1

all: $(OBJECTS)
	@echo " compiling generate.exe ..."
	@$(COMPILER) $(CPPFLAGS) $(opts) $(OBJECTS) $(LFFTW) -o ../generate.exe > error.log 2>&1


cln:
	@rm -f *.o
	@rm -f *.exe

bak:
	@mkdir -p $(FOLDER)
	@cp *.cpp $(FOLDER)
	@cp *.h $(FOLDER)
	@#if [ -f "params.in" ]; then cp -f params.in $(FOLDER); fi
	@#if [ -f "equilPos0.in" ]; then cp -f equilPos0.in $(FOLDER); fi
	@#if [ -f "konfig0E.real" ]; then cp -f konfig0E.real $(FOLDER); fi
	@cp Makefile $(FOLDER)
	@echo " Source code backed up in $(FOLDER)."
