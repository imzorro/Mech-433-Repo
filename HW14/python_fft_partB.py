import csv
import numpy as np
import matplotlib.pyplot as plt

def read_csv(filename):
    t = []  # time column
    data = []  # data column
    with open(filename) as f:
        reader = csv.reader(f)
        for row in reader:
            t.append(float(row[0]))
            data.append(float(row[1]))
    return t, data

def calculate_sample_rate(t):
    total_time = t[-1] - t[0]
    num_points = len(t)
    sample_rate = num_points / total_time
    return sample_rate

def moving_average(data, X):
    filtered_data = []
    for i in range(len(data)):
        if i < X:
            filtered_data.append(np.mean(data[:i+1]))
        else:
            filtered_data.append(np.mean(data[i-X+1:i+1]))
    return filtered_data

def plot_signal_and_fft(t, data, filtered_data, sample_rate, title, X):
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
    ax1.plot(t, filtered_data, 'r', label=f'Filtered (X={X})')
    ax1.set_xlabel('Time [s]')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'Signal vs Time - {title} (X={X})')
    ax1.legend()

    ax2.plot(frq, abs(Y), 'k', label='Unfiltered')
    ax2.plot(frq, abs(Y_filtered), 'r', label=f'Filtered (X={X})')
    ax2.set_xlabel('Frequency [Hz]')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title(f'FFT - {title} (X={X})')
    ax2.legend()

    plt.tight_layout()
    plt.show()

# Parameters for moving average filter
X_values = {
    'sigA.csv': 10,
    'sigB.csv': 50,
    'sigC.csv': 10,
    'sigD.csv': 10
}

# Read and process each CSV file
for filename in ['sigA.csv','sigB.csv', 'sigC.csv', 'sigD.csv']:
    t, data = read_csv(filename)
    sample_rate = calculate_sample_rate(t)
    X = X_values[filename]
    filtered_data = moving_average(data, X)
    plot_signal_and_fft(t, data, filtered_data, sample_rate, filename, X)
