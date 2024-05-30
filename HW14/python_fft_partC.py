import csv
import numpy as np
import matplotlib.pyplot as plt

# Function to read CSV files
def read_csv(filename):
    t = []  # time column
    data = []  # data column
    with open(filename) as f:
        reader = csv.reader(f)
        for row in reader:
            t.append(float(row[0]))
            data.append(float(row[1]))
    return t, data

# Function to calculate the sample rate
def calculate_sample_rate(t):
    total_time = t[-1] - t[0]
    num_points = len(t)
    sample_rate = num_points / total_time
    return sample_rate

# Function to apply IIR filter
def iir_filter(data, A, B):
    filtered_data = []
    for i in range(len(data)):
        if i == 0:
            filtered_data.append(data[i])
        else:
            new_value = A * filtered_data[i-1] + B * data[i]
            filtered_data.append(new_value)
    return filtered_data

# Function to plot the signal and FFT
def plot_signal_and_fft(t, data, filtered_data, sample_rate, title, A, B):
    n = len(data)
    k = np.arange(n)
    T = n / sample_rate
    frq = k / T  # two sides frequency range
    frq = frq[range(n // 2)]  # one side frequency range

    Y = np.fft.fft(data) / n  # FFT for unfiltered data
    Y = Y[range(n // 2)]

    Y_filtered = np.fft.fft(filtered_data) / n  # FFT for filtered data
    Y_filtered = Y_filtered[range(n // 2)]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
    ax1.plot(t, data, 'k', label='Unfiltered')
    ax1.plot(t, filtered_data, 'r', label=f'Filtered (A={A}, B={B})')
    ax1.set_xlabel('Time [s]')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'Signal vs Time - {title} (A={A}, B={B})')
    ax1.legend()

    ax2.plot(frq, abs(Y), 'k', label='Unfiltered')
    ax2.plot(frq, abs(Y_filtered), 'r', label=f'Filtered (A={A}, B={B})')
    ax2.set_xlabel('Frequency [Hz]')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title(f'FFT - {title} (A={A}, B={B})')
    ax2.legend()

    plt.tight_layout()
    plt.show()

# Define filenames and best A and B values
best_AB_values = {
    'sigA.csv': (0.8, 0.2),
    'sigB.csv': (0.85, 0.15),
    'sigD.csv': (0.9, 0.1)
}

# Read and process each CSV file
for filename in best_AB_values.keys():
    t, data = read_csv(filename)
    sample_rate = calculate_sample_rate(t)
    A, B = best_AB_values[filename]
    filtered_data = iir_filter(data, A, B)
    plot_signal_and_fft(t, data, filtered_data, sample_rate, filename, A, B)
