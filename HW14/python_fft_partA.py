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

def plot_signal_and_fft(t, data, sample_rate, title):
    n = len(data)
    k = np.arange(n)
    T = n / sample_rate
    frq = k / T  # two sides frequency range
    frq = frq[range(n // 2)]  # one side frequency range
    Y = np.fft.fft(data) / n  # fft computing and normalization
    Y = Y[range(n // 2)]

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t, data, 'b')
    ax1.set_xlabel('Time [s]')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'Signal vs Time - {title}')
    
    ax2.plot(frq, abs(Y), 'b')
    ax2.set_xlabel('Frequency [Hz]')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title(f'FFT - {title}')
    
    plt.tight_layout()
    plt.show()

# Read and process each CSV file
for filename in ['sigA.csv', 'sigB.csv', 'sigC.csv','sigD.csv']:
    t, data = read_csv(filename)
    sample_rate = calculate_sample_rate(t)
    plot_signal_and_fft(t, data, sample_rate, filename)
