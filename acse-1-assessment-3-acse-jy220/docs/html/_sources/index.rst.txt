#######
acse_la
#######

A Gaussian Elimination routine
------------------------------

This package implements Gaussian elimination [1]_ for :obj:`numpy.ndarray` objects, along with hand-written matrix multiplication and a hand written Bareiss Algorithm [2]_ for computing determinants.

See :func:`acse_la.gauss`, :func:`acse_la.gauss.matmul` and :func:`acse_la.det.det` for more information.

.. automodule:: acse_la
  :members: gauss

.. automodule:: acse_la.gauss
  :members: matmul, zeromat
  :noindex: gauss

.. automodule:: acse_la.det
  :members: det
  :noindex: det
  
  
.. rubric:: References
.. [1] https://mathworld.wolfram.com/GaussianElimination.html
.. [2] http://informatika.stei.itb.ac.id/~rinaldi.munir/Matdis/2016-2017/Makalah2016/Makalah-Matdis-2016-051.pdf

