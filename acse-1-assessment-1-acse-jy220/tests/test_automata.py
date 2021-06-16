import os

import numpy as np

import automata

BASE_PATH = os.path.dirname(__file__)


def test_rule_thirty():
    ¨¨¨Test rule 30 implementation¨¨¨
    initial128 = np.load(os.sep.join((BASE_PATH, 'rule30_128_init.npy')))

    # non-perodic test

    final128 = np.load(os.sep.join((BASE_PATH, 'rule30_128_final.npy')))
    assert (automata.rule_thirty(initial128, 100) == final128).all()

    # periodic test

    final128_periodic = np.load(os.sep.join((BASE_PATH,
                                'rule30_128_periodic_final.npy')))

    assert (automata.rule_thirty(initial128, 100,
                                 periodic=true) == final128_periodic).all()
