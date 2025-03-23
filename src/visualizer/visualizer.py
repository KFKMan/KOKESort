import matplotlib.pyplot as plt
import pandas as pd
import glob
import os
import re
import numpy as np
from numpy.polynomial.polynomial import Polynomial
import mplcursors
from matplotlib.animation import FuncAnimation
import sys

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

    global_min = 1000000000 * 1000 
    global_max = -1

    algFuncs = ""

    for algorithm_name, data in algorithm_data.items():
        x = np.array(data['x'])
        y = np.array(data['y'])

        unique_x = np.unique(x)
        averaged_y = []
        for ux in unique_x:
            #averaged_y.append(np.mean(y[x == ux]))
            indices = np.where(np.isclose(x, ux))
            averaged_y.append(np.mean(y[indices]))

        averaged_y = np.array(averaged_y)

        mean_y = np.mean(averaged_y)
        std_y = np.std(averaged_y)

        # Verilerin çoğu 1 veya 2 standart sapma içinde yer alır, bu sınırları kullanabiliriz
        threshold_low = mean_y - 2 * std_y  # Ortalamadan 2 standart sapma düşük
        threshold_high = mean_y + 2 * std_y  # Ortalamadan 2 standart sapma yüksek

        # Filtreleme, yani aşırı uç Y değerlerini çıkart
        valid_indices = (averaged_y >= threshold_low) & (averaged_y <= threshold_high)
        filtered_x = unique_x[valid_indices]
        filtered_y = averaged_y[valid_indices]

        global_min = min(global_min, max(min(filtered_y), 0))
        global_max = max(global_max, max(filtered_y))

        # Filtrelenmiş verilerle polinom fit yap
        poly_coeffs = np.polyfit(filtered_x, filtered_y, 2)
        p = np.poly1d(poly_coeffs)

        # Veriyi ve fit çizgisini çiz
        line, = plt.plot(filtered_x, filtered_y, 'o', label=f'{algorithm_name} Data')
        fitline, = plt.plot(filtered_x, p(filtered_x), '--', label=f'{algorithm_name} Fit')
        
        mplcursors.cursor(line, hover=True).connect(
            "add", lambda sel, algo=algorithm_name: sel.annotation.set_text(f'{algo} Data')
        )
        
        mplcursors.cursor(fitline, hover=True).connect(
            "add", lambda sel, algo=algorithm_name: sel.annotation.set_text(f'{algo} Fit')
        )

        print("Algorithm:",algorithm_name)
        print("Unique X:", unique_x)
        print("Filtered X:", filtered_x)
        print("Average Y:", averaged_y)
        print("Y Value Samples:", averaged_y[:10])
        print("Filtered Y:",filtered_y)

        algFuncs += f"Polynomial fit for {algorithm_name}:\n"
        algFuncs += f"f(x) = {poly_coeffs[0]} * x^2 + {poly_coeffs[1]} * x + {poly_coeffs[2]}\n\n"


    print("Global Min:", global_min)
    print("Global Max:",global_max)

    print("Functions of Algorithms")
    print(algFuncs)
    plt.ylim(global_min * 0.7, global_max * 1.3)
    plt.xlabel('Parameter Size (Array Size)')
    plt.ylabel('Elapsed Time')
    plt.title('Time Complexity Comparison with Polynomial Fit')
    plt.legend()
    plt.grid(True)
    plt.show()

directory = r"..\benchres"
plot_time_complexity(directory)
