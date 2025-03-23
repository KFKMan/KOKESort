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
    arrays = [np.random.randint(0, max_value + 1, size=np.random.randint(min_elements, max_elements + 1), dtype=np.int64).tolist() for _ in range(num_arrays)]
    print("Created")
    return arrays

def save_arrays_to_file(arrays, filename="testdata.txt"):
    """
    Saves arrays to a file in a readable format.

    :param arrays: List of arrays to save
    :param filename: Name of the output file
    """
    print("Writing...")

    output_dir = "gen"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Dosya yolunu belirle
    file_path = os.path.join(output_dir, filename)

    with open(file_path, "w") as f:
        f.write("\n".join(" ".join(map(str, array)) for array in arrays) + "\n")

    print(f"Written to {file_path}")

def main():
    parser = argparse.ArgumentParser(description="Generate and save unsorted arrays.")
    parser.add_argument("--num_arrays", type=int, default=3, help="Number of arrays to generate (default: 3)")
    parser.add_argument("--array_sizes", type=str, default="10,20,50", help="Comma-separated sizes of arrays (default: '10,20,50')")
    parser.add_argument("--max_value", type=int, default=9223372036854775807, help="Maximum value of elements in the arrays (default: 9223372036854775807)")
    parser.add_argument("--output_base_name", type=str, default="test", help="Base name for output files (default: test)")
    
    args = parser.parse_args()

    #Weird parameter and variable name. it's need to be changed.
    array_sizes = list(map(int, args.array_sizes.split(',')))

    for i, size in enumerate(array_sizes, 1):
        arrays = generate_unsorted_arrays(
            num_arrays=args.num_arrays,
            min_elements=size,
            max_elements=size,
            max_value=args.max_value
        )
        output_filename = f"{args.output_base_name}{size}.csv"
        save_arrays_to_file(arrays, output_filename)

        print(f"Generated {len(arrays)} unsorted arrays and saved to '{output_filename}'")

if __name__ == "__main__":
    main()
