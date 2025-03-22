import matplotlib.pyplot as plt
import pandas as pd
import glob
import os
import re
import numpy as np
from numpy.polynomial.polynomial import Polynomial

def plot_time_complexity(directory):
    directory = os.path.abspath(directory)
    csv_files = glob.glob(os.path.join(directory, 'result*.csv'))
    
    if not csv_files:
        print(f"No CSV files found in directory: {directory}")
        return

    plt.figure(figsize=(10, 6))

    algorithm_data = {}

    for file in csv_files:
        df = pd.read_csv(file, header=None)
        
        algorithm_name = os.path.basename(file).split('.')[0][6:]
        
        algorithm_name = re.sub(r'\d+$', '', algorithm_name)
        
        x = df[0]
        y = df[1]
        
        if algorithm_name not in algorithm_data:
            algorithm_data[algorithm_name] = {'x': [], 'y': []}
        
        algorithm_data[algorithm_name]['x'].extend(x)
        algorithm_data[algorithm_name]['y'].extend(y)

    for algorithm_name, data in algorithm_data.items():
        x = np.array(data['x'])
        y = np.array(data['y'])

        unique_x = np.unique(x)
        averaged_y = []
        for ux in unique_x:
            averaged_y.append(np.mean(y[x == ux]))

        poly_coeffs = np.polyfit(unique_x, averaged_y, 2)
        p = np.poly1d(poly_coeffs)
        
        plt.plot(unique_x, averaged_y, 'o', label=f'{algorithm_name} Data')
        plt.plot(unique_x, p(unique_x), '--', label=f'{algorithm_name} Fit')

    plt.xlabel('Parameter Size (Array Size)')
    plt.ylabel('Elapsed Time')
    plt.title('Time Complexity Comparison with Polynomial Fit')
    plt.legend()
    plt.grid(True)
    plt.show()

directory = r"..\benchres"
plot_time_complexity(directory)
