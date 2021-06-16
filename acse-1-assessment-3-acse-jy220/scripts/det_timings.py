from acse_la import gauss
from acse_la.det import det
import numpy as np
import numpy.linalg as la
import timeit
import copy


def gen_mat(list_dim):
    mat_list = []
    for dim in list_dim:
        mat = np.random.uniform(-10, 10, size=(dim, dim)).tolist()
        mat = copy.deepcopy(mat)
        mat_list.append(mat)

    mat_list = mat_list

    return mat_list


def test_mat(dim_list):
    data = []
    for x in range(len(dim_list)):
        str1 = "gauss(mat_list[" + str(x) + "],mat_list[" + str(x) + "])"
        str2 = "la.det(mat_list[" + str(x) + "])"
        str3 = "det(mat_list[" + str(x) + "])"
        t1 = timeit.timeit(stmt=str1, setup="from __main__ import gauss, mat_list", number=10)
        t2 = timeit.timeit(stmt=str2, setup="from __main__ import la, mat_list", number=10)
        t3 = timeit.timeit(stmt=str3, setup="from __main__ import det, mat_list", number=10)
        row_list = [dim_list[x], t1, t2, t3]
        data.append(row_list)

    return data


def writefile(data):
    putout = open("results/timings.txt", "w")
    putout.write(" size   gauss  numpy  my det\n")

    for row in data:
        putout.write("%5d" % row[0])
        for column in row[1:]:
            putout.write("%8.2g" % column)
        putout.write("\n")
    putout.close()


if __name__ == '__main__':
    t_list = [2, 4, 6, 10, 20, 50, 100, 250, 500, 1000]
    mat_list = gen_mat(t_list)
    data = test_mat(t_list)
    writefile(data)
