import sys
#sys.path.append(r"C:\Users\oyunm\AppData\Local\Programs\Python\Python313\Lib\site-packages\Release")

try:
    import KOKESortWrapper
except ImportError as e:
    import traceback
    traceback.print_exc()

arr = [5, 14, 7, 9, 16, 24, 215, 15, 46, 95, 23]

def comparer(a, b):
    if a < b:
        return -1
    elif a > b:
        return 1
    else:
        return 0

print(arr)

newArr = KOKESortWrapper.KOKESortV1(arr, comparer)

print(newArr);