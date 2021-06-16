import numpy as np
import pytest

from acse_la import gauss, matmul, zeromat


class TestGauss(object):
    """
    Class for testing the Gaussian elimination algorithm Gauss
    and its associated functions.
    """

    @pytest.mark.parametrize('a, b, dete, xe', [
        ([[2, 9, 4], [7, 5, 3], [6, 1, 8]],
         [[1, 0, 0], [0, 1, 0], [0, 0, 1]], -360.0,
         [[-0.10277777777777776, 0.18888888888888888, -0.019444444444444438],
          [0.10555555555555554, 0.02222222222222223, -0.061111111111111116],
          [0.0638888888888889, -0.14444444444444446, 0.14722222222222223]])

    ])
    def test_gauss_1(self, a, b, dete, xe):
        """ Test the gauss function --example 1 """
        det, x = gauss(a, b)

        assert np.isclose(det, dete)
        assert np.allclose(x, xe)

    @pytest.mark.parametrize('c, d, detf, xf', [
        ([[7/9, 16/5, 11/4], [4/3, 13/7, 17/19], [23/8, -43/9, -51/23]],
         [[23/6, -5/3, 18/11], [-17/8, -25/16, 21/13], [-27/5, 14/3, -15/2]],
         -14.387117981144485,
         [[-1.77663255,  0.0897174, -0.50909328],
         [-1.78059359, -1.36907147,  2.00116624],
         [3.96838477, 0.96166613, -1.5896025]])

    ])
    def test_gauss_2(self, c, d, detf, xf):
        """ Test the gauss function --example 2 """
        det2, x2 = gauss(c, d)

        assert np.isclose(det2, detf)
        assert np.allclose(x2, xf)

    @pytest.mark.parametrize('mat1, mat2, mule', [
        ([[5, -3, 2], [-4, 6, -1]],
         [[7, -9], [11, 8], [-3, -10]],
         [[-4, -89], [41, 94]])
    ])
    def test_matmul(self, mat1, mat2, mule):
        """ Test the matmul function """
        mul = matmul(mat1, mat2)

        assert np.allclose(mul, mule)

    @pytest.mark.parametrize('p, q, n, m, z1, z2', [
        (1, 2, 3, 4, [[0], [0]], [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]])
    ])
    def test_zeromat(self, p, q, n, m, z1, z2):
        """ Test the zeromat function """
        zm1 = zeromat(p, q)
        zm2 = zeromat(n, m)

        assert np.allclose(zm1, z1)
        assert np.allclose(zm2, z2)
