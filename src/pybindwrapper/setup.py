import os
import subprocess
import sys
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

GIT_REPO = "https://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@github.com/KFKMan/KOKESort.git"

# Repository directory name
REPO_DIR = "KOKESort"
# Path to CMakeLists.txt inside the cloned repo
CMAKE_LISTS = os.path.join(REPO_DIR, "src", "CMakeLists.txt")

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        # Ensure CMake is available
        try:
            subprocess.check_output(['cmake', '--version'])
        except Exception:
            raise RuntimeError("CMake is required to build the extension.")

        # Clone repo if needed
        if not os.path.exists(CMAKE_LISTS):
            subprocess.check_call([
                "git", "clone", "--depth", "1",
                GIT_REPO, REPO_DIR
            ])

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cfg = 'Debug' if self.debug else 'Release'
        build_temp = os.path.join(self.build_temp, ext.name)

        os.makedirs(build_temp, exist_ok=True)
        # Configure step
        subprocess.check_call([
            'cmake', ext.sourcedir,
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DCMAKE_BUILD_TYPE={cfg}',
            f'-DPYTHON_EXECUTABLE={sys.executable}',
            '-DPYBINDWRAPPER=ON',
            "-DUSE_TESTS=OFF"
        ], cwd=build_temp)
        # Build step
        subprocess.check_call([
            'cmake', '--build', '.', '--config', cfg
        ], cwd=build_temp)

setup(
    name='KOKESortWrapper',
    version='0.1.0',
    author='Kaan Faruk Kınalı, Mehmet Kerem Aslan, Emirhan Uçan',
    author_email='kaanfarukkinali@gmail.com',
    description='Python wrapper for KOKESort C library',
    long_description='',
    ext_modules=[
        CMakeExtension('KOKESortWrapper', sourcedir=os.path.join(REPO_DIR, 'src'))
    ],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    python_requires='>=3.7',
)