import numpy as np
import re

def load_binary_file(i: int):
   picpath = './output/output_%d' %i
   data = np.fromfile(picpath)

   return data

def load_array(i: int):
    picpath = './output/output_%d.dat' %i
    data = np.loadtxt(picpath)

    return data

def read_parameters():
    f = open('./parameters.ini', 'r')
    lines = f.readlines()
    # create a dicitionary to store the parameters
    list_p = {}
 
    for line in lines[1:]:
        line = line.strip('\n')
        ss = re.split(' =', line)
        list_p[ss[0]] = ss[-1]
    f.close()
    
    return list_p