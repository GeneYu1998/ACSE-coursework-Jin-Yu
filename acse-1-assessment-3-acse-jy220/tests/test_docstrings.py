# Excuting doctests written in gauss.py.
# You can also type command
# 'python -m doctest -v acse_la/gauss.py'
# from the base folder to run it.
from acse_la import gauss


class MyClass(object):
    def stringtest():
        import doctest
        doctest.testmod(name=gauss, verbose=True)

    if __name__ == '__main__':
        stringtest()
