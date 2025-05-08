import sys
import os
import traceback

def log_exception(exc_type, exc_value, exc_tb):
    print("Uncaught exception:", exc_type, exc_value)

sys.excepthook = log_exception

def my_comparer(a, b) -> int:
    try:
        print("my_comparer called")
        print("Comparing:", a, b)

        if(a > b):
            return 1
        elif(a == b):
            return 0
        else:
            return -1
    except Exception as e:
        print("Error in my_comparer:")
        print(e)
        traceback.print_exc()
        raise

try:
    import KOKESortWrapper

    arr = [5, 14, 7, 9, 16, 24, 215, 15, 46, 95, 23]


    print("Input array:", arr)
    KOKESortWrapper.Test()
    print("Test function called successfully.")
    KOKESortWrapper.TestStatic()
    print("Test Static function called successfully.")
    newArr = KOKESortWrapper.SortV1(arr, my_comparer, log = True)
    print("Sort function called successfully.")

    print("Sorted array:", newArr)

except Exception as e:
    print("Error:")
    print(e)
    traceback.print_exc()
