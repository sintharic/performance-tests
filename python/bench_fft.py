import os
import linecache
import numpy as np
import tracemalloc



# from https://stackoverflow.com/questions/552744/how-do-i-profile-memory-usage-in-python
def display_top(snapshot, key_type='lineno', limit=3):
  snapshot = snapshot.filter_traces((
    tracemalloc.Filter(False, "<frozen importlib._bootstrap>"),
    tracemalloc.Filter(False, "<unknown>"),
  ))
  top_stats = snapshot.statistics(key_type)

  print("Top %s lines" % limit)
  for index, stat in enumerate(top_stats[:limit], 1):
    frame = stat.traceback[0]
    # replace "/path/to/module/file.py" with "module/file.py"
    filename = os.sep.join(frame.filename.split(os.sep)[-2:])
    print("#%s: %s:%s: %.1f KiB"
          % (index, filename, frame.lineno, stat.size / 1024))
    line = linecache.getline(frame.filename, frame.lineno).strip()
    if line: print('    %s' % line)

  other = top_stats[limit:]
  if other:
    size = sum(stat.size for stat in other)
    print("%s other: %.1f KiB" % (len(other), size / 1024))
  total = sum(stat.size for stat in top_stats)
  print("Total allocated size: %.1f KiB" % (total / 1024))



def main(n = 1024, trace=False, export=False):
  if trace: tracemalloc.start()

  nx = n
  ny = n
  nyHP1 = ny//2+1
  nxHP1 = nx//2+1
  dataRx = np.zeros((nx, ny))
  dataRy = np.zeros_like(dataRx)
  dataFx = np.zeros((nx,nyHP1), dtype=np.complex64)
  dataFy = np.zeros_like(dataFx)

  for i in range(0, (n//4 - 1)):
    iq = 1 + 2*i
    prefac = 1./(iq*iq)
    if (i%2 == 0): prefac = -prefac
    dataFx[iq,0] = dataFx[iq,0] + 1j*prefac
    dataFx[-iq,0] = dataFx[-iq,0] - 1j*prefac
    dataFy[0,iq] = dataFy[0,iq] + 1j*prefac
    dataRx = np.fft.irfft2(dataFx)
    dataRy = np.fft.irfft2(dataFy)
    dataFx = np.fft.rfft2(dataRx)
    dataFy = np.fft.rfft2(dataRy)

  if trace:
    snapshot = tracemalloc.take_snapshot()
    display_top(snapshot)

  if export:
    np.savetxt("triangular_wave_x.dat", dataRx.transpose(), fmt="%g")
    np.savetxt("triangular_wave_y.dat", dataRy.transpose(), fmt="%g")

  

if __name__ == '__main__': main()