import numpy as np
import cmutils as cm

data_x_cpp = cm.readConfig("cpp/triangular_wave_x.dat")
data_x_jl = cm.readImg("julia/triangular_wave_x.dat")*(1024**2)
data_x_py = cm.readImg("python/triangular_wave_x.dat")*(1024**2)

data_y_cpp = cm.readConfig("cpp/triangular_wave_y.dat")
data_y_jl = cm.readImg("julia/triangular_wave_y.dat")*(1024**2)
data_y_py = cm.readImg("python/triangular_wave_y.dat")*(1024**2)

ax_cpp = cm.plotImg(np.rot90(data_x_cpp) - data_y_cpp)
ax_cpp.set_title("cpp difference between x and y")
ax_jl = cm.plotImg(np.rot90(data_x_jl) - data_y_jl)
ax_jl.set_title("jl difference between x and y")
ax_py = cm.plotImg(np.rot90(data_x_py) - data_y_py)
ax_py.set_title("py difference between x and y")

ax1 = cm.plotImg(data_x_cpp - data_x_jl)
ax1.set_title("cpp vs jl")
ax2 = cm.plotImg(data_x_cpp - data_x_py)
ax2.set_title("cpp vs py")
ax3 = cm.plotImg(data_x_jl - data_x_py)
ax3.set_title("jl vs py")