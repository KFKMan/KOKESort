try:
    import KOKESort
except:
    print("KOKESort module not found. Please ensure it is installed correctly.")
try:
    import KOKESortWrapper
except:
    print("KOKESortWrapper module not found. Please ensure it is installed correctly.")
try:
    import kokesortwrapper
except:
    print("kokesortwrapper module not found. Please ensure it is installed correctly.")

arr = [5, 14, 7, 9, 16, 24, 215, 15, 46, 95, 23]

print(arr)

newArr = KOKESort(arr)

print(newArr);