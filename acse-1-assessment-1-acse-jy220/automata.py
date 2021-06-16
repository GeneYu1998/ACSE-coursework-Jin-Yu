"""Implementations of Wolfram's Rule 30 and Conway's
Game of Life on various meshes"""

import numpy as np
import scipy
import matplotlib.pyplot as plt

def rule_thirty(initial_state, nsteps, periodic=False):
    """
    Perform iterations of Wolfram's Rule 30 with specified boundary
    conditions.

    Parameters
    ----------
    initial_state : array_like or list
        Initial state of lattice in an array of booleans.
    nsteps : int
        Number of steps of Rule 30 to perform.
    periodic : bool, optional
        If true, then the lattice is assumed periodic.

    Returns
    -------

    numpy.ndarray
         Final state of lattice in array of booleans

    >>> rule_thirty([False, True, False], 1)
    array([True, True, True])

    >>> rule_thirty([False, False, True, False, False], 3)
    array([True, False, True, True, True])
    """
    init = np.array(initial_state)+0
    plot = init
    len_init = len(init)

   
    
    for n in range(nsteps):

        array_trans = np.zeros(len_init)

        if periodic == False:
             init = np.insert(init,0,0)
             init = np.append(init,0)
        else:
             init = np.insert(init,0,init[-1])
             init = np.append(init,init[1])
        
        prev_one = init[0]
        this_one = init[1]

        for i in range(1,len_init+1):
               next_one = init[i+1]
    
               if this_one == 1:
                  if prev_one == 1:
                      array_trans[i-1] = 1

               elif this_one == 0:
                  if prev_one!=next_one:
                      array_trans[i-1] = 1
    
               prev_one = this_one
               this_one = next_one


        init = np.abs(array_trans - np.delete(init,[0,-1]))
        plot = np.row_stack((init,plot))

    
   
          
       


    


    final_state = init > 0

    # write your code here to replace return statement
    return final_state   



def life(initial_state, nsteps):
    """
    Perform iterations of Conway’s Game of Life.
    Parameters
    ----------
    initial_state : array_like or list of lists
        Initial 2d state of grid in an array of booleans.
    nsteps : int
        Number of steps of Life to perform.
    Returns
    -------
    numpy.ndarray
         Final state of grid in array of booleans
    """
    init = np.array(initial_state)+0
    row_ori = init.shape[0]
    col_ori = init.shape[1]

    M = np.ones(9).reshape(3,3)
    M[1,1] = 0

    for n in range(nsteps):
       
 

       init =  np.column_stack((init,np.zeros(row_ori)))
       init =  np.column_stack((np.zeros(row_ori),init))
       init =  np.row_stack((np.zeros(col_ori+2),init))
       init =  np.row_stack((init,np.zeros(col_ori+2)))

       

       size = init.size
       row = init.shape[0]
       col = init.shape[1]

       trans_mat = np.zeros(size).reshape(row,col)


       for i in range(1,row-1):
          for j in range(1,col-1):
             env_mat = init[i-1:i+2,j-1:j+2]
             env_live = env_mat * M
             env_live_sum = np.sum(env_live)
             
             if init[i,j]==1:
                 if env_live_sum<2 or env_live_sum>3:
                      trans_mat[i,j] = 1
             if init[i,j]==0:
                 if env_live_sum==3:
                      trans_mat[i,j] = 1

       init = np.abs(trans_mat-init)
        
       init = init[1:-1,1:-1]
       plot_array(init)
     

    final_stage = init > 0 
    # write your code here to replace return statement
    return final_stage




def life_periodic(initial_state, nsteps):
    """
    Perform iterations of Conway's Game of Life on a doubly periodic mesh.

    Parameters
    ----------
    initial_state : array_like or list of lists
        Initial 2d state of grid in an array of booleans.
    nsteps : int
        Number of steps of Life to perform.

    Returns
    -------

    numpy.ndarray
         Final state of grid in array of booleans
    """
    init = np.array(initial_state)+0
    row_ori = init.shape[0]
    col_ori = init.shape[1]

    M = np.ones(9).reshape(3,3)
    M[1,1] = 0

    for n in range(nsteps):
       
     

       init =  np.column_stack((init,init[:,0]))
       init =  np.column_stack((init[:,-2],init))
       init =  np.row_stack((init[-1],init))
       init =  np.row_stack((init,init[1]))

  

       size = init.size
       row = init.shape[0]
       col = init.shape[1]

       trans_mat = np.zeros(size).reshape(row,col)


       for i in range(1,row-1):
          for j in range(1,col-1):
             env_mat = init[i-1:i+2,j-1:j+2]
             env_live = env_mat * M
             env_live_sum = np.sum(env_live)
           
             if init[i,j]==1:
                 if env_live_sum<2 or env_live_sum>3:
                      trans_mat[i,j] = 1
             if init[i,j]==0:
                 if env_live_sum==3:
                      trans_mat[i,j] = 1

       init = np.abs(trans_mat-init)
  
        
       init = init[1:-1,1:-1]
    
    final_stage = init > 0 
    # write your code here to replace return statement
    return final_stage




def lifehex(initial_state, nsteps):
    """
    Perform iterations of Conway's Game of Life on
    a hexagonal tessellation.

    Parameters
    ----------
    initial_state : list of lists
        Initial state of grid on hexagons.
    nsteps : int
        Number of steps of Life to perform.

    Returns
    -------

    list of lists
         Final state of tessellation.
    """

    nrow = len(initial_state)
 
    for i in range(nrow):
        initial_state[i] = np.array(initial_state[i]) + 0
        
    if len(initial_state[0]) < len(initial_state[1]):
        slength = len(initial_state[0])
    else:
        slength = len(initial_state[1])
    
    edge_1= np.array([0] * (slength + 2))
    
    
    
     
    for n in range(nsteps):
        iterate =[0]* (nrow + 2)
        for i in range(nrow):
            initial_state[i] = np.insert(initial_state[i],0,0)
            initial_state[i] = np.append(initial_state[i],0)
            
        initial_state.insert(0,edge_1)
        initial_state.append(edge_1)
        
        for i in range(len(initial_state)):
             iterate[i] = initial_state[i].copy()


        for i in range(1,nrow+1):
           
            if len(initial_state[i]) == slength + 2:
 
                for j in range(1,len(initial_state[i])-1):
                   
                   env_live = initial_state[i][j-1] + initial_state[i][j+1] + initial_state[i-1][j] + initial_state[i-1][j+1] +initial_state[i+1][j] +initial_state[i+1][j+1]

                   if initial_state[i][j] == 1:
                       if env_live == 3 or env_live == 5:
                           iterate[i][j] = 1
                       else:
                           iterate[i][j] = 0
                   elif initial_state[i][j] == 0:
                        if env_live == 2:
                            iterate[i][j] = 1
                        else:
                            iterate[i][j] = 0

            elif len(initial_state[i]) == slength +3:

                for j in range(1,len(initial_state[i])-1):
                   
                   env_live = initial_state[i][j-1] + initial_state[i][j+1] + initial_state[i-1][j-1] + initial_state[i-1][j] +initial_state[i+1][j-1] +initial_state[i+1][j]

                   if initial_state[i][j] == 1:
                       if env_live == 3 or env_live == 5:
                           iterate[i][j] = 1
                       else:
                           iterate[i][j] = 0
                   elif initial_state[i][j] == 0:
                        if env_live == 2:
                            iterate[i][j] = 1
                        else:
                            iterate[i][j] = 0
            
        
   
        del iterate[0]
        del iterate[-1]
        del initial_state[0]
        del initial_state[-1]

        for i in range(nrow):
            iterate[i]= np.delete(iterate[i], [0,-1])
            initial_state[i]= np.delete(initial_state[i], [0,-1])


    
    
        for i in range(nrow):
             initial_state[i] = iterate[i].copy()
   


        
       
    final_state = [0] * nrow  
    for i in range(len(initial_state)):
        final_state[i] = initial_state[i].copy() > 0 

        
    # write your code here to replace return statement
    return final_state


def life_generic(matrix, initial_state, nsteps, environment, fertility):
    """
    Perform iterations of Conway’s Game of Life for an arbitrary
    collection of cells.
    Parameters
    ----------
    matrix : 2d array of ints (o or 1)
        a boolean matrix with rows indicating neighbours for each cell
    initial_state : 1d array of bools
        Initial state vectr.
    nsteps : int
        Number of steps of Life to perform.
    environment : set of ints
        neighbour counts for which live cells survive.
    fertility : set of ints
        neighbour counts for which dead cells turn on.
    Returns
    -------
    array
         Final state.
    """
    init = np.array(initial_state) + 0
    M = np.array(matrix)


    for n in range(nsteps):
        trans_array = np.zeros(len(init))
        for i in range(len(init)):
             env_live = np.dot(init,M[:,i])
            
             if init[i] == 1:
                if not env_live in environment:
                     trans_array[i] = 1
             elif init[i] == 0:
                if env_live in fertility:
                     trans_array[i] = 1
     
        init = np.abs(trans_array - init)
    
    final = init > 0
    # write your code here to replace return statement
    return final


# Remaining routines are for plotting

def plot_array(data, show_axis=False,
               cmap=plt.get_cmap('bone'), **kwargs):
    """Plot a 1D/2D array in an appropriate format.

    Mostly just a naive wrapper around pcolormesh.

    Parameters
    ----------

    data : array_like
        array to plot
    show_axis: bool, optional
        show axis numbers if true
    cmap : pyplot.colormap or str
        colormap

    Other Parameters
    ----------------

    **kwargs
        Additional arguments passed straight to pyplot.pcolormesh
    """
    plt.pcolormesh(data, edgecolor='b', cmap=cmap, **kwargs)

    plt.axis('equal')
    if show_axis:
        plt.axis('on')
    else:
        plt.axis('off')


def plot_hexes(data, **kwargs):
    """Plot a hexagonal array.

    Parameters
    ----------
    data : list of lists
        The state of the Game of Life system.

    This uses pyplot.fill to plot closed hexagons.
    """

    colors = ['w', 'r']
    for i, row in enumerate(data):
        for j, val in enumerate(row):
            plot_one_hex((i, j), edgecolor='k',
                         facecolor=colors[1*val], **kwargs)


def plot_one_hex(coord, **kwargs):
    """Plot a hexagon, optionally filled.

    Parameters
    ----------
    coord : array_like
       The 2d coordinates of the centre of the hexagon

    This uses pyplot.fill to plot closed hexagons.
    """
    x_c = np.array([1, 0, -1, -1, 0, 1])
    y_c = np.array([0.5, 1.0, 0.5, -0.5, -1.0, -0.5])

    plt.fill(np.sqrt(3.0)*(coord[1]-0.5*(coord[0] % 2))+x_c,
             -1.5*coord[0]+y_c, **kwargs)
