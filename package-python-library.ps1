& "$PSScriptRoot/clean-packaging-outputs.ps1"

python -m pip install nanobind
python -m pip install build

$env:DEBUG = "1"

python setup.py build_ext --inplace
python setup.py bdist_wheel