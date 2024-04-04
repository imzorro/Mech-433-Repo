#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// LED Pin is GPIO 15 check with opening definition of next line pico.h
uint LED_PIN = 15;

// Button Pin is GPIO 16 check with opening definition of next line pico.h
uint BUTTON_PIN = 16;
//initialize global variable
uint Status =0;


int main() {
    stdio_init_all();
    gpio_init(LED_PIN);    ///Initialize GPIO Pin
    gpio_set_dir(LED_PIN, GPIO_OUT); //// Setting GPIO pin as output(1) for LED
    gpio_init(BUTTON_PIN);    ///Initialize GPIO Pin
    gpio_set_dir(BUTTON_PIN, GPIO_IN); //// Setting GPIO pin as input(0) for LED
    // We are using the button to pull down to 0v when pressed, so ensure that when
    // unpressed, it uses internal pull ups. Otherwise when unpressed, the input will
    // be floating.
    gpio_pull_up(BUTTON_PIN);
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    while (true) {
        /// if statement for when button is pressed it turns off LED
        if (!gpio_get(BUTTON_PIN)) {
            gpio_put(LED_PIN, 0); // Button pressed! Turning off LED
            printf("Enter the number of analog samples (between 1 and 100): ");
            int num_samples;
            scanf("%d", &num_samples);
            if (num_samples < 1 || num_samples > 100) { //// if statement to check range of sample
                printf("Invalid number of samples. Please enter a number between 1 and 100.\n");
                }
                else {
                for (int i = 0; i < num_samples; i++) {
                    uint16_t raw_value = adc_read();     /// Read the 12 bit value
                    float voltage = raw_value * 3.3f / (1 << 12); // Convert raw ADC value to voltage
                    printf("Sample %d: Voltage = %.2fV\n", i + 1, voltage);
                    sleep_ms(10); // Wait for 10ms between samples or 100Hz
                }
            }
        }
         else {
            gpio_put(LED_PIN, 1); //Button released! Turning on LED
        }
    }
}