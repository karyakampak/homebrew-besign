from setuptools import setup
from Cython.Build import cythonize
import numpy  # Remove this line if you are not using numpy
import os


setup(
    name='besign_helper',
    ext_modules = cythonize(
        "besign_helper.pyx",
        # language="c++",  # This tells Cython to generate C++ code
    ),
    include_dirs=[numpy.get_include()],  # Remove this line if you are not using numpy
    install_requires=[
        'fitz',  # PyMuPDF
        'pikepdf'
    ],
    zip_safe=False
)