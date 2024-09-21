from setuptools import setup
from Cython.Build import cythonize
import numpy  # Remove this line if you are not using numpy
import os

# Define a custom build directory if provided
build_dir = os.getenv('CYTHON_BUILD_DIR', 'cython_build')

setup(
    ext_modules = cythonize(
        "src/besign_helper.pyx",
        # language="c++",  # This tells Cython to generate C++ code
    ),
    include_dirs=[numpy.get_include()],  # Remove this line if you are not using numpy
    install_requires=[
        'fitz'  # PyMuPDF
    ],
    script_args=['build_ext', '--inplace'],
    options={'build_ext': {'build_lib': build_dir}}
)
