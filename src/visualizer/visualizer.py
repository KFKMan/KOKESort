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
import argparse
from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score

def find_csv_file(directory, name):
    # İlk olarak belirtilen dizinde arama yap
    directory = os.path.abspath(directory)
    csv_files = glob.glob(os.path.join(directory, name))
    
    if not csv_files:
        print(f"No CSV files found in directory: {directory}. Searching in current directory...")
        
        # Eğer dosya bulunamazsa, kendi klasöründe ara
        current_directory = os.path.abspath(os.curdir)
        csv_files = glob.glob(os.path.join(current_directory, name))

        if not csv_files:
            print(f"No CSV files found in current directory: {current_directory}")
            return None
    
    return csv_files[0]

def evaluate(coeffs, best_model_name, n):
    if best_model_name == "O(n)":
        return coeffs[0] * n + coeffs[1]
    elif best_model_name == "O(n log n)":
        return coeffs[0] * n * np.log2(n) + coeffs[1]
    elif best_model_name == "O(n^2)":
        return coeffs[0] * n**2 + coeffs[1]
    else:
        raise ValueError(f"Unknown model name: {best_model_name}")

# Pretty-print the fit results
def print_fit_summary(algorithm_name, r2_scores, best_model_name, coeffs):
    model_equations = {
        "O(n)": lambda c: f"{c[0]:.6g} * n + {c[1]:.6g}",
        "O(n log n)": lambda c: f"{c[0]:.6g} * n * log(n) + {c[1]:.6g}",
        "O(n^2)": lambda c: f"{c[0]:.6g} * n^2 + {c[1]:.6g}",
    }

    print(f"\nAlgorithm: {algorithm_name}\n")
    print("Model Fits (R² Scores):")
    for name, (r2, _) in r2_scores.items():
        print(f"  {name:<10} => R² = {r2:.6f}")

    print(f"\nBest Fit: {best_model_name}")
    if best_model_name in model_equations:
        equation_str = model_equations[best_model_name](coeffs)
        print(f"   Approximated Equation:\n     y ≈ {equation_str}")


def analyze_fit(x, y, algorithm_name=None):
    x = np.array(x)
    y = np.array(y)

    def fit_and_score(x_transformed):
        x_reshaped = x_transformed.reshape(-1, 1)
        model = LinearRegression().fit(x_reshaped, y)
        y_pred = model.predict(x_reshaped)
        return r2_score(y, y_pred), model

    # Inputs
    x_linear = x
    x_nlogn = x * np.log2(x)
    x_quadratic = x ** 2

    # Calculating R² scores
    r2_linear, _ = fit_and_score(x_linear)
    r2_nlogn, _ = fit_and_score(x_nlogn)
    r2_quadratic, _ = fit_and_score(x_quadratic)

    # Best R²
    r2_scores = {
        "O(n)": (r2_linear, x_linear),
        "O(n log n)": (r2_nlogn, x_nlogn),
        "O(n^2)": (r2_quadratic, x_quadratic)
    }

    best_model_name = max(r2_scores, key=lambda k: r2_scores[k][0])
    best_r2, best_x = r2_scores[best_model_name]

    # Polinom Fit
    coeffs = np.polyfit(best_x, y, 1)
    fit_fn = np.poly1d(coeffs)

    if algorithm_name:
        print(f"\nAlgorithm: {algorithm_name}")
        for name, (r2, _) in r2_scores.items():
            print(f"{name:<10}: R² = {r2:.6f}")
        print(f"Best fit  : {best_model_name}")
        print_fit_summary(algorithm_name, r2_scores, best_model_name, coeffs)

    return fit_fn, best_x


def plot_time_complexity(directory, degree):
    directory = os.path.abspath(directory)
    csv_files = [find_csv_file(directory, "benchmark_results.csv")]

    plt.figure(figsize=(12, 6))

    algorithm_data = {}

    for file in csv_files:
        df = pd.read_csv(file)
        
        # Burada tüm satırlardaki verileri işliyoruz
        for index, row in df.iterrows():
            algorithm_name = row['Algorithm']
            dataset_size = row['Size']
            time_value = row['Time']

            if algorithm_name not in algorithm_data:
                algorithm_data[algorithm_name] = {'sizes': [], 'times': []}
            
            # Verileri algoritmaya göre ekliyoruz
            algorithm_data[algorithm_name]['sizes'].append(dataset_size)
            algorithm_data[algorithm_name]['times'].append(time_value)

    for algorithm_name, data in algorithm_data.items():
        sizes = data['sizes']
        times = data['times']

        min_times = []
        unique_sizes = np.unique(sizes)

        for size in unique_sizes:
            # Aynı boyut için ilgili zamanları alıyoruz
            corresponding_times = [times[i] for i in range(len(sizes)) if sizes[i] == size]
            # Minimi alıyoruz
            min_times.append(min(corresponding_times))

        # Her algoritma için farklı çizim
        plt.plot(sizes, min_times, label=algorithm_name)

    global_min = 1000000000 * 1000 
    global_max = -1

    algFuncs = ""

    for algorithm_name, data in algorithm_data.items():
        x = np.array(data['sizes'])
        y = np.array(data['times'])

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

        line, = plt.plot(filtered_x, filtered_y, 'o', label=f'{algorithm_name} Data')
        
        '''
        log_filtered_x = np.log(filtered_x)
        log_filtered_y = np.log(filtered_y)

        poly_coeffs = np.polyfit(log_filtered_x, log_filtered_y, degree)
        p = np.poly1d(poly_coeffs)

        fitline, = plt.plot(log_filtered_x, p(log_filtered_x), '--', label=f'{algorithm_name} Fit')
        '''

        '''
        # Filtrelenmiş verilerle polinom fit yap
        poly_coeffs = np.polyfit(filtered_x, filtered_y, degree)
        p = np.poly1d(poly_coeffs)

        # Veriyi ve fit çizgisini çiz
        fitline, = plt.plot(filtered_x, p(filtered_x), '--', label=f'{algorithm_name} Fit')
        '''

        fit_fn, fit_x = analyze_fit(filtered_x, filtered_y, algorithm_name)
        fitline, = plt.plot(filtered_x, fit_fn(fit_x), '--', label="quick best fit")

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
        #algFuncs += f"f(x) = {poly_coeffs[0]} * x^2 + {poly_coeffs[1]} * x + {poly_coeffs[2]}\n\n"

        '''
        print(poly_coeffs)

        for i in range(degree, -1, -1):
            coef = poly_coeffs[degree - i]
    
            if coef == 0:
                continue  # Katsayı sıfırsa eklemeyelim
    
            term = f"{abs(coef):.20f}"  # Mutlak değeri alarak katsayıyı yazdır (ondalık hassasiyeti ayarlanabilir)
    
            if i > 0:
                term += f" * x^{i}"
    
            if i == degree:  
                # İlk terim ise işareti kontrol etmeye gerek yok
                algFuncs += term
            else:
                # İşarete göre + veya - ekleyelim
                sign = " + " if coef > 0 else " - "
                algFuncs += sign + term

        algFuncs += "\n\n"
        '''


    print("Global Min:", global_min)
    print("Global Max:",global_max)

    plt.ylim(global_min * 0.7, global_max * 1.3)
    plt.xlabel('Parameter Size (Array Size)')
    plt.ylabel('Elapsed Time')
    plt.title('Time Complexity Comparison with Polynomial Fit')
    #plt.legend()

    plt.legend(loc='upper left', bbox_to_anchor=(1, 1), title='Algorithms', fontsize='x-small')
    plt.tight_layout()

    plt.grid(True)
    plt.show()

directory = r"..\benchres"

parser = argparse.ArgumentParser(description="Generate and save unsorted arrays.")
parser.add_argument("--degree", type=int, default=2, help="Polynom fit degree")

args = parser.parse_args();

plot_time_complexity(directory, args.degree)
