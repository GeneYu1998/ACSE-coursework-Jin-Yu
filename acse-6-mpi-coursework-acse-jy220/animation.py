import os
import loaddata
import glob
import sys
import numpy as np
from matplotlib.pyplot import LinearLocator
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib as mpl
import matplotlib.colors as colors

num_pics = len(glob.glob('./output/*'))

parameters = loaddata.read_parameters()
t_out = float(parameters['t_out'])
width = int(parameters['width'])
length = int(parameters['length'])
imax = int(parameters['imax'])
jmax = int(parameters['jmax'])
dt = float(parameters['dt'])
boundary_type = str(parameters['boundary_type'])

# set label and locator
ele_x = int(length / 5)
ele_y = int(width / 5)
ele_i = int(imax / 5)
ele_j = int(jmax / 5)
x_label = [ele_x * i for i in range(6)]
y_label = [ele_y * i for i in range(6)]
i_loc = [ele_i * i for i in range(6)]
j_loc = [ele_j * i for i in range(6)]
i_loc[-1] = i_loc[-1] - 0.5
j_loc[-1] = j_loc[-1] - 0.5

if(len(sys.argv) > 1 and str(sys.argv[1]) == '-s'):
      speedratio = int(sys.argv[2])
else:
   speedratio = 1

# set picture
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title(r'{0:} $\times$ {1:} grid on a {2:} $\times$ {3:} region, dt = {4:.3f} s'.format(imax, jmax, length, width, dt), fontsize=14)
ax.xaxis.set_major_locator(plt.FixedLocator(i_loc))
ax.xaxis.set_major_formatter(plt.FixedFormatter(x_label))
ax.yaxis.set_major_locator(plt.FixedLocator(j_loc))
ax.yaxis.set_major_formatter(plt.FixedFormatter(y_label))
text = plt.text(x = 0.95, y = 0.95, s='t = 0 s',fontdict=dict(fontsize=12, color='r',family='monospace',))

# first frame
arr = loaddata.load_binary_file(0).reshape((imax, jmax))
norm = mpl.colors.Normalize(vmin = np.min(arr), vmax = np.max(arr))
cax = ax.imshow(arr, interpolation='nearest', cmap = 'winter', norm = norm, origin = 'lower')
cb = fig.colorbar(cax)


def update_grid(picnum: int):
    text.set_text('t = {0:.2f} s'.format(picnum * t_out))
    arr = loaddata.load_binary_file(picnum).reshape((imax, jmax))
    vmax = np.max(arr)
    vmin = np.min(arr)
    cax.set_data(arr)
    cax.set_clim(vmin, vmax)
    return cax,
 

anim = animation.FuncAnimation(fig, update_grid, frames = np.arange(0, num_pics, speedratio), blit=True)

ani_name = './anime/%d * %d grid (Mesh size: %d * %d) dt = %f, boundary_type = %s.mp4' % (length, width, imax, jmax, dt, boundary_type )

gif_name = './anime/%d * %d grid (Mesh size: %d * %d) dt = %f, boundary_type = %s.gif' % (length, width, imax, jmax, dt, boundary_type )

if (sys.argv[-1] == '-save'):
   anim.save(ani_name,fps = int(1/t_out), dpi = 400)
elif(sys.argv[-1] == '-gif'):
   anim.save(gif_name,fps = int(1/t_out), dpi = 400)
else:
   plt.show()