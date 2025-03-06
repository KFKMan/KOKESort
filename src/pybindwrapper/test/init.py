import os
import glob
import shutil

parent_dir = os.path.dirname(os.path.dirname(os.getcwd()))

build_dir = os.path.join(parent_dir, 'build')

lib_dir = build_dir

debug_dir = os.path.join(build_dir, "Debug")
release_dir = os.path.join(build_dir, "Release")

if os.path.exists(release_dir):
    lib_dir = release_dir
elif os.path.exists(debug_dir):
    lib_dir = debug_dir

print("Library path setted to " + lib_dir)

shutil.copytree(lib_dir, os.getcwd(), dirs_exist_ok=True)