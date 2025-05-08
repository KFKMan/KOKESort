import sys
import os
import traceback

def log_exception(exc_type, exc_value, exc_tb):
    print("Uncaught exception:", exc_type, exc_value)

sys.excepthook = log_exception

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
        
    def __repr__(self):
        return f"{self.name} ({self.age})"

def my_comparer(a, b) -> int:
    try:
        print("my_comparer called")
        print("Comparing:", a, b)
        return a.age - b.age
    except Exception as e:
        print("Error in my_comparer:")
        print(e)
        traceback.print_exc()
        raise

try:
    import KOKESortWrapper

    peoples = [
    Person("Alice", 30),
    Person("Bob", 25),
    Person("Charlie", 35)
    ]


    print("Input array:", peoples)
    KOKESortWrapper.Test()
    print("Test function called successfully.")
    KOKESortWrapper.TestStatic()
    print("Test Static function called successfully.")
    newArr = KOKESortWrapper.SortV1(peoples, my_comparer, log = True)
    print("Sort function called successfully.")

    print("Sorted array:", newArr)

except Exception as e:
    print("Error:")
    print(e)
    traceback.print_exc()
