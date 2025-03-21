import numpy as np
import os
import argparse

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
        f.write("\n".join(" ".join(map(str, array)) for array in arrays) + "\n")
    print("Writed")

def main():
    parser = argparse.ArgumentParser(description="Generate and save unsorted arrays.")
    parser.add_argument("--num_arrays", type=int, default=3, help="Number of arrays to generate (default: 3)")
    parser.add_argument("--min_elements", type=int, default=1000000, help="Minimum number of elements in each array (default: 1000000)")
    parser.add_argument("--max_elements", type=int, default=1000000, help="Maximum number of elements in each array (default: 1000000)")
    parser.add_argument("--max_value", type=int, default=99999999, help="Maximum value of elements in the arrays (default: 99999999)")
    parser.add_argument("--output", type=str, default="testdata.txt", help="Output file name (default: testdata.txt)")
    
    args = parser.parse_args()

    if args.max_elements < args.min_elements:
        args.min_elements = args.max_elements
    
    arrays = generate_unsorted_arrays(
        num_arrays=args.num_arrays,
        min_elements=args.min_elements,
        max_elements=args.max_elements,
        max_value=args.max_value
    )
    
    save_arrays_to_file(arrays, args.output)
    print(f"Generated {len(arrays)} unsorted arrays and saved to '{args.output}'")

if __name__ == "__main__":
    main()
