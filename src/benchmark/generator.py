import numpy as np
import os

def generate_unsorted_arrays(num_arrays=10, min_elements=100, max_elements=100, max_value=1000):
    """
    Generates a list of unsorted arrays with random integers.

    :param num_arrays: Number of arrays to generate
    :param max_elements: Maximum number of elements in each array
    :param max_value: Maximum possible value of array elements
    :return: List of unsorted arrays
    """
    print("Creating...")
    arrays = [np.random.randint(0, max_value + 1, size=np.random.randint(min_elements, max_elements + 1)).tolist() for _ in range(num_arrays)]
    print("Created")
    return arrays

def save_arrays_to_file(arrays, filename="testdata.txt"):
    """
    Saves arrays to a file in a readable format.

    :param arrays: List of arrays to save
    :param filename: Name of the output file
    """
    print("Writing...")

    with open(filename, "w") as f:
        f.write("\n".join(" ".join(map(str, array)) + ";" for array in arrays) + "\n")
    print("Writed")

if __name__ == "__main__":
    arrays = generate_unsorted_arrays(num_arrays=500, min_elements = 5000, max_elements=5000, max_value=10000)
    save_arrays_to_file(arrays)
    print(f"Generated {len(arrays)} unsorted arrays and saved to 'testdata.txt'")
