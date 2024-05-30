import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin, lfilter, freqz

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

# Function to apply FIR filter
def apply_fir_filter(data, fir_coefficients):
    filtered_data = lfilter(fir_coefficients, 1.0, data)
    return filtered_data

# Function to plot the signal and FFT
def plot_signal_and_fft(t, data, filtered_data, sample_rate, title, fir_info):
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
    ax1.plot(t, filtered_data, 'r', label=f'Filtered ({fir_info})')
    ax1.set_xlabel('Time [s]')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'Signal vs Time - {title} ({fir_info})')
    ax1.legend()

    ax2.plot(frq, abs(Y), 'k', label='Unfiltered')
    ax2.plot(frq, abs(Y_filtered), 'r', label=f'Filtered ({fir_info})')
    ax2.set_xlabel('Frequency [Hz]')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title(f'FFT - {title} ({fir_info})')
    ax2.legend()

    plt.tight_layout()
    plt.show()

# FIR filter design parameters
cutoff_frequencies = [0.1, 0.2]  # Example cutoff frequencies in normalized form (0 to 1, where 1 is Nyquist frequency)
num_taps = 101  # Number of filter coefficients (taps)
window_types = ['hamming', 'hann', 'blackman']

# Define filenames and read data
filenames = ['sigA.csv', 'sigB.csv', 'sigD.csv']

# Read and process each CSV file
for filename in filenames:
    t, data = read_csv(filename)
    sample_rate = calculate_sample_rate(t)
    
    for cutoff in cutoff_frequencies:
        for window in window_types:
            # Generate FIR filter coefficients
            fir_coefficients = firwin(num_taps, cutoff, window=window, pass_zero='lowpass')
            
            # Apply FIR filter
            filtered_data = apply_fir_filter(data, fir_coefficients)
            
            # Plot the signal and FFT
            fir_info = f'FIR {window}, cutoff={cutoff}, taps={num_taps}'
            plot_signal_and_fft(t, data, filtered_data, sample_rate, filename, fir_info)
