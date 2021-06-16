import numpy as np
import copy


def gauss(a, b):
    """
    Given two matrices, `a` and `b`, with `a` square, the determinant
    of `a` and a matrix `x` such that a*x = b are returned.
    If `b` is the identity, then `x` is the inverse of `a`.

    Parameters
    ----------
    a : np.array or list of lists
        'n x n' array
    b : np. array or list of lists
        'm x n' array

    Examples
    --------
    >>> a = [[2, 0, -1], [0, 5, 6], [0, -1, 1]]
    >>> b = [[2], [1], [2]]
    >>> det, x = gauss(a, b)
    >>> det
    22.0
    >>> x
    [[1.5], [-1.0], [1.0]]
    >>> A = [[1, 0, -1], [-2, 3, 0], [1, -3, 2]]
    >>> I = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
    >>> Det, Ainv = gauss(A, I)
    >>> Det
    3.0
    >>> Ainv #doctest:+NORMALIZE_WHITESPACE
    [[2.0, 1.0, 1.0],
    [1.3333333333333333, 1.0, 0.6666666666666666],
    [1.0, 1.0, 1.0]]

    Notes
    -----
    See https://en.wikipedia.org/wiki/Gaussian_elimination for further details.
    """
    a = copy.deepcopy(a)
    b = copy.deepcopy(b)
    n = len(a)
    p = len(b[0])
    det = np.float64(1)
    for i in range(n - 1):
        k = i
        for j in range(i + 1, n):
            if abs(a[j][i]) > abs(a[k][i]):
                k = j
        if k != i:
            a[i], a[k] = a[k], a[i]
            b[i], b[k] = b[k], b[i]
            det = -det

        for j in range(i + 1, n):
            t = a[j][i]/a[i][i]
            for k in range(i + 1, n):
                a[j][k] -= t*a[i][k]
            for k in range(p):
                b[j][k] -= t*b[i][k]

    for i in range(n - 1, -1, -1):
        for j in range(i + 1, n):
            t = a[i][j]
            for k in range(p):
                b[i][k] -= t*b[j][k]
        t = 1/a[i][i]
        det *= a[i][i]
        for j in range(p):
            b[i][j] *= t
    return det, b


def matmul(a, b):
    """
    Given `a` an n x m matrix and `b` an m x l matrix, the product of a and b
    is returned, as an n x l matrix.

    Parameters
    ----------
    a : np.array or list of lists
        'n x m' array
    b : np. array or list of lists
        'm x l' array

    Examples
    --------
    >>> a = [[1, 2], [3, 4]]
    >>> b = [[5],[6]]
    >>> mul_1 = matmul(a, b)
    >>> mul_1
    [[17], [39]]
    >>> c = [[5, 1], [6, 2]]
    >>> mul_2 = matmul(a, c)
    >>> mul_2
    [[17, 5], [39, 11]]
    """
    # from IPython import embed; embed()
    n, p = len(a), len(a[0])
    p1, q = len(b), len(b[0])
    if p != p1:
        raise ValueError("Incompatible dimensions")
    c = zeromat(n, q)
    for i in range(n):
        for j in range(q):
            c[i][j] = sum(a[i][k]*b[k][j] for k in range(p))
    return c


def zeromat(p, q):
    """
    Create an p x q matrix with all its entries be 0.

    Parameters
    ----------
    p : integer
    q : integer

    Examples
    --------
    >>> p = 5
    >>> q = 6
    >>> z_mat = zeromat(p, q)
    >>> z_mat #doctest:+NORMALIZE_WHITESPACE
    [[0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0]]
    """
    return [[0]*q for i in range(p)]
