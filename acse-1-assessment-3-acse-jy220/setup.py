try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

setup(name='acse_la',
      use_scm_version=True,
      setup_requires=['setuptools_scm'],
      description="Environment for completing ACSE-1 assessement 3.",
      long_description="""Environment for completing ACSE-1 assessement 3.""",
      url='https://github.com/acse-2020',
      author="Imperial College London",
      author_email='rhodri.nelson@imperial.ac.uk',
      packages=['acse_la'])
