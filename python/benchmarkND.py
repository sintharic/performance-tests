import numpy as np

def FFT(array, ndim):
  return np.fft.rfftn(array, axes=range(array.ndim-ndim, array.ndim))

def IFFT(array, ndim):
  return np.fft.irfftn(array, axes=range(array.ndim-ndim, array.ndim))


# for testing
N = 2**8

arrayR = np.random.uniform(0,1, (3,N,N))
arrayF1 = FFT(arrayR, 1) # arrayF1 = np.fft.rfftn(arrayR, axes=[2])
arrayF2 = FFT(arrayR, 2) # arrayF2 = np.fft.rfftn(arrayR, axes=(1,2))

for i in range(3):
  print((arrayF1[i,0,:] == np.fft.rfft(arrayR[i,0,:])).mean()) 
  print((arrayF2[i,:,:] == np.fft.rfft2(arrayR[i,:,:])).mean()) 

arrayM1 = IFFT(arrayF1, 1) # arrayM1 = np.fft.irfftn(arrayF1, axes=[2])
arrayM2 = IFFT(arrayF2, 2) # arrayM2 = np.fft.irfftn(arrayF2, axes=(1,2))
for i in range(3):
  print((arrayM1[i,0,:] == np.fft.irfft(arrayF1[i,0,:])).mean()) 
  print((arrayM2[i,:,:] == np.fft.irfft2(arrayF2[i,:,:])).mean()) 