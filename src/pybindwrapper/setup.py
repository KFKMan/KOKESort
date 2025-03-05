from setuptools import setup, Extension
import sys
import os

module = Extension(
    'KOKESort',
    sources = ['wrapper.cpp'],
    include_dirs = ['/path/to/pybind11'],
    libraries = ['KOKESort'], 
    extra_compile_args=['-O3'],
)

setup(
    name='KOKESort',
    version='0.1',
    ext_modules=[module],
)