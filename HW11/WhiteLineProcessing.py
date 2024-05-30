from PIL import Image
import matplotlib.pyplot as plt

# Load the image
img_path = "CameraPic.jpg"  # Update this path if needed
img_data = Image.open(img_path)
img_data = img_data.convert('RGB')

# Get the image size
width, height = img_data.size

# Set IIR filter parameters
alpha = 0.95
beta = 0.05

# Initialize lists to store the centers of the line
centers = []

# Loop through every pixel in every row to find the line
for row in range(height):
    brightness = []  # The brightness of a pixel
    smoothed_brightness = []  # The smoothed brightness of a pixel
    brightness_derivative = []  # The derivative of the brightness of a pixel
    
    for i in range(width):
        r, g, b = img_data.getpixel((i, row))  # Get the raw color of a pixel
        brightness.append(r + g + b)  # Calculate the brightness
        
        if i == 0:
            smoothed_brightness.append(brightness[-1])
            brightness_derivative.append(0)
        else:
            smoothed_brightness.append(smoothed_brightness[-1] * alpha + brightness[-1] * beta)  # Smooth out the brightness with IIR
            brightness_derivative.append(smoothed_brightness[-1] - smoothed_brightness[-2])  # Calculate the derivative

    # Find the edges, where the derivative is most positive and negative
    posleft = brightness_derivative.index(max(brightness_derivative))
    posright = brightness_derivative.index(min(brightness_derivative))

    # Calculate the center of the line
    center = int(posleft + (posright - posleft) / 2)
    centers.append(center)

    # Draw green dots where we think the edge is
    img_data.putpixel((posleft, row), (0, 255, 0))
    img_data.putpixel((posright, row), (0, 255, 0))
    # Draw a red dot where we think the center of the line is
    img_data.putpixel((center, row), (255, 0, 0))

# Save the image with marked edges and center
output_img_path = "line_marked.jpg"  # Update this path if needed
img_data.save(output_img_path)

# Plot the centers of the line
plt.figure(figsize=(10, 6))
plt.plot(range(height), centers, 'r-', label='Center of the Line')
plt.xlabel('Row')
plt.ylabel('Center Position')
plt.title('Center of the White Line')
plt.legend()
plt.gca().invert_yaxis()  # Invert y-axis to match image coordinates
plt.show()

# Save the plot
plot_path = "line_center_plot.tiff"  # Update this path if needed
plt.savefig(plot_path)
