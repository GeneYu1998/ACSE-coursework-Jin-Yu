import numpy as np
import loaddata
import sys
from matplotlib.pyplot import LinearLocator
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib as mpl
import matplotlib.colors as colors

parameters = loaddata.read_parameters()
t_out = float(parameters['t_out'])
width = int(parameters['width'])
length = int(parameters['length'])
imax = int(parameters['imax'])
jmax = int(parameters['jmax'])
dt = float(parameters['dt'])
boundary_type = str(parameters['boundary_type'])

if(len(sys.argv) > 1):
      picnum = int(sys.argv[1])
else:
   picnum = 0

data = loaddata.load_binary_file(picnum).reshape((imax, jmax))

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

t = t_out * picnum

# set picture
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title(r'{0:} $\times$ {1:} grid on a {2:} $\times$ {3:} region, dt = {4:.3f} s'.format(imax, jmax, length, width, dt), fontsize=14)
ax.xaxis.set_major_locator(plt.FixedLocator(i_loc))
ax.xaxis.set_major_formatter(plt.FixedFormatter(x_label))
ax.yaxis.set_major_locator(plt.FixedLocator(j_loc))
ax.yaxis.set_major_formatter(plt.FixedFormatter(y_label))
text = plt.text(x = 0.95, y = 0.95, s='t = %.3f s' % (t),fontdict=dict(fontsize=12, color='r',family='monospace',))


arr = data
norm = mpl.colors.Normalize(vmin = np.min(arr), vmax = np.max(arr))
cax = ax.imshow(arr, interpolation='nearest', cmap = 'winter', norm = norm, origin = 'lower')
cb = fig.colorbar(cax)

pic_name = './pic/%d * %d grid (Mesh size: %d * %d) t = %f, boundary_type = %s.png' % (length, width, imax, jmax, t, boundary_type)

if (sys.argv[-1] == '-save'):
   plt.savefig(pic_name, dpi = 400)
else:
   plt.show()