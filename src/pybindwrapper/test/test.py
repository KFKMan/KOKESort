import ctypes
import os

dll_path = os.path.join(os.getcwd(),"KOKESort.dll")

ctypes.CDLL(dll_path)


import KOKESortWrapper

arr = [5, 14, 7, 9, 16, 24, 215, 15, 46, 95, 23]

print(arr)

newArr = KOKESortWrapper.SortV1(arr)

print(newArr);