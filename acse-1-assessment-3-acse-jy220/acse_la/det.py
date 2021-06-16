import copy

# This method is called "Bareiss Algorithm".
# Got inspriation from Felix Limanta,
# "Efficiently Calculating the Determinant of a Matrix",
# Page.4, 2016.
# The following code of implementing the algorithm is written by myself.


def det(a):
    """
    An "Bareiss Algorithm" to compute the determinant of a square matrix a.

    Parameters
    ----------
    a : np.array or list of lists
        'n x n' array

    Notes
    -----
    See reference[2] in index.html for further details.
    """
    a = copy.deepcopy(a)
    n = len(a)
    det = 1
    com_k = 1
    for k in range(n-1):
        step = 1

        while a[k][k] == 0:
            a[k+step], a[k] = a[k], a[k+step]
            det = -det
            step += 1
        mul = a[k][k]

        for i in range(k+1, n):
            for j in range(k+1, n):
                a[i][j] *= mul
                a[i][j] -= a[i][k] * a[k][j]
                a[i][j] /= com_k

        com_k = mul

    det = det * a[-1][-1]

    return det
