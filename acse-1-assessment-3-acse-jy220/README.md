# ACSE-1-assessment-3

This repository contains all material required to get you started with the
third, and final, assessment of ACSE-1. Details of the assessment and the
associated marking scheme can be found in the file `assessment.pdf`
in the documentation folder of this repository.

To complete this assignment, while not a requirement, it is recommended that
you create a new virtual environment and install the required `Python` packages
within your virtual environment.

If you wish to create an `Anaconda` environment, an `environment.yml` file has
been provided from which you can create the `acse_la` environment
by running
```bash
conda env create -f environment.yml
```
from within the base folder of this repository. If you wish to use some other virtual environment solution (or none at all),
when in your environment ensure that all requirements are satisfied via
```bash
pip install -r requirements.txt
```

Whichever solution you decide upon, the package `acse_la` should then be installed
by running
```bash
pip install -e .
```
again from within the base folder of this repository.
