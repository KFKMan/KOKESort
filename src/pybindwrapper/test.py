import sys
sys.path.append("C:/Users/kaan/Desktop/Search/src/build/pybindwrapper/Debug")

from KOKESortWrapper import *

arr = [5, 14, 7, 9, 16, 24, 215, 15, 46, 95, 23]

print(arr)

newArr = SortV1(arr)

print(newArr);