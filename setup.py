import sys
from xml.etree.ElementInclude import include
from pybind11 import get_cmake_dir
from glob import glob
# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import build_ext
from setuptools import Extension as Pybind11Extension
from setuptools import setup

__version__ = "0.0.2"

ext_modules = [
    Pybind11Extension("gatherly",
                      ["src/gatherly.cpp", "src/pygatherly/main.cpp"] +
                      sorted(glob("lib/kmerDecoder/src/*cpp")) +
                      sorted(glob("lib/kmerDecoder/src/hashUtils/*cpp")) +
                      sorted(glob("lib/kmerDecoder/src/Utils/*cpp")),
                      library_dirs=['build/lib/kmerDecoder'],
                      libraries=['z'],
                      define_macros=[('VERSION_INFO', __version__)],
                      ),
]

setup(
    name="gatherly",
    version=__version__,
    author="Mohamed Abuelanin",
    author_email="mabuelanin@gmail.com",
    url="https://github.com/mr-eyes/gatherly",
    description="An ultrafast gather!",
    long_description="",
    ext_modules=ext_modules,
    include_dirs=["include", "lib/pybind11/include", "lib/parallel-hashmap", "lib/zstr/src", "lib/cpp-json",
                  "lib/kmerDecoder/include", "lib/kmerDecoder/lib/kseq/include", "lib/kmerDecoder/include/Utils", "lib/kmerDecoder/include/hashUtils"],
    extras_require={"test": "pytest"},
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
)
