using FFTW, LinearAlgebra, DelimitedFiles

# You can switch between using Intel MKL's FFT and FFTW (default):
#FFTW.set_provider!("mkl")
#FFTW.set_provider!("fftw")
# MKL is ~30% faster on my Macbook!




function setup_memuse_tracker()
  tracker = Ref(0)
  function mem_use(tracker)
    finalizer(mem_use, tracker)
    println("# Memory used: ", Sys.maxrss()/2^20, " MiB")
    nothing
  end
  finalizer(mem_use, tracker)
  nothing
end
#setup_memuse_tracker()



function with_dummies(n::Int64 = 1024, write::Bool = false)
  nx = n
  ny = n
  nyHP1 = ny÷2+1
  nxHP1 = nx÷2+1
  dataRx = zeros(Float64, (nx, ny))
  dataRy = similar(dataRx)
  dataFx = zeros(ComplexF64,nxHP1,ny)
  dataFy = similar(dataFx)
  dummyF = similar(dataFx)
  dummyR = similar(dataRx)

  rfft = plan_rfft(dummyR)
  irfft = plan_irfft(dummyF, nx)
  for i = 0:(n/4 - 1)
    iq::Int64 = 1 + 2*i
    prefac::Float64 = 1/(iq*iq)
    if (i%2 == 0) 
      prefac = -prefac
    end
    dataFx[1,1+iq] = dataFx[1,1+iq] + im*prefac
    dataFx[1,1+ny-iq] = dataFx[1,1+ny-iq] - im*prefac
    dataFy[1+iq,1] = dataFy[1+iq,1] + im*prefac
    dummyF = dataFx
    dataRx = irfft * dummyF
    dummyF = dataFy
    dataRy = irfft * dummyF

    dataFx = rfft * dataRx
    dataFy = rfft * dataRy
  end

  if write
    writedlm("triangular_wave_x.dat", dataRx)
    writedlm("triangular_wave_y.dat", dataRy)
  end
end



function without_dummies(n::Int64 = 1024, write=false)
  nx = n
  ny = n
  nyHP1 = ny÷2+1
  nxHP1 = nx÷2+1
  dataRx = zeros(Float64, nx, ny)
  dataRy = similar(dataRx)
  dataFx = zeros(ComplexF64, nxHP1, ny)
  dataFy = similar(dataFx)

  rfft = plan_rfft(dataRx)
  irfft = plan_irfft(dataFx, nx)
  for i = 0:(n/4 - 1)
    iq::Int64 = 1 + 2*i
    prefac::Float64 = 1/(iq*iq)
    if (i%2 == 0) 
      prefac = -prefac
    end
    dataFx[1,1+iq] = dataFx[1,1+iq] + im*prefac
    dataFx[1,1+ny-iq] = dataFx[1,1+ny-iq] - im*prefac
    dataFy[1+iq,1] = dataFy[1+iq,1] + im*prefac
    dataRx = irfft * dataFx
    dataRy = irfft * dataFy
    dataFx = rfft * dataRx
    dataFy = rfft * dataRy
  end

  if write 
    writedlm("triangular_wave_x.dat", dataRx)
    writedlm("triangular_wave_y.dat", dataRy)
  end
end

# @time with_dummies(1024)

# time for multiple resolutions
# resols = (2*ones(7)) .^ LinRange(4,10,7)
# @time without_dummies(8)
# for i = 1 : 7
#   f = resols[i]
#   n::Int64 = trunc(f)
#   println(n)
#   @time without_dummies(n)
# end
