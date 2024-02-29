import numpy as np

N = 2**14

arrayN = np.random.uniform(0,1,(N,))
arrayNx = arrayN.reshape((N,1))
arrayNy = arrayN.reshape((1,N))

def fftN(): return np.fft.rfft(arrayN)

def fftNx(): return np.fft.rfft2(arrayNx)

def fftNy(): return np.fft.rfft2(arrayNy)

def fft2D(array):
  if array.shape[0]==1: return np.fft.rfft(array.reshape(array.shape[1])).reshape((1, array.shape[1]//2+1))
  if array.shape[1]==1: return np.fft.rfft(array.reshape(array.shape[0])).reshape((array.shape[0]//2+1, 1))
  return np.fft.rfft2(array)
