/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/adc.h"

/* Example code to talk to an SSD1306-based OLED display

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on display
   board
   GPIO PICO_DEFAULT_I2C_SCL_PIN (on Pico this is GP5 (pin 7)) -> SCL on
   display board
   3.3v (pin 36) -> VCC on display board
   GND (pin 38)  -> GND on display board
*/

// 400 is usual, but often these can be overclocked to improve display response.
// Tested at 1000 on both 32 and 84 pixel height devices and it worked.
//#define SSD1306_I2C_CLK             400
#define SSD1306_I2C_CLK             1000

// PICO LED Pin is GPIO 25
uint LED_PIN = PICO_DEFAULT_LED_PIN;

void drawVoltageAndFPS() {
    // Get ADC0 value
    uint16_t raw_value = adc_read();     /// Read the 12 bit value
    // Convert ADC value to voltage (3.3V reference)
    float voltage = raw_value * 3.3f / (1 << 12); // Convert raw ADC value to voltage
   
    // Calculate frames per second (fps)
    static unsigned int prevTime = 0;
    static unsigned int frames = 0;
    unsigned int currentTime = to_us_since_boot(get_absolute_time());
     // Increment frame counter
    frames++;
    if (currentTime - prevTime >= 1000000) { // 1 second has passed
        // Calculate fps
        float fps = frames / ((currentTime - prevTime) / 1000000.0f); // Cast one operand to float
        // Debug print
        printf("Frames: %d, Current Time: %d, Previous Time: %d, FPS: %.2f\n", frames, currentTime, prevTime, fps);
        // Draw fps at the bottom-left corner of the screen
        drawString(0, 3, "FPS:");
        char fpsStr[10];
        sprintf(fpsStr, "%.2f", fps); // Format with 2 decimal places
        drawString(5, 3, fpsStr);
        // Reset frame counter and update previous time
        frames = 0;
        prevTime = currentTime;
    }
    // Draw voltage at the top-left corner of the screen
    drawString(0, 0, "Volts:");
    char voltageStr[10];
    sprintf(voltageStr, "%.2f", voltage);
    drawString(9, 0, voltageStr);

    // Update the display
    ssd1306_update();
}

int main() {
///// Initializations /////
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Hello, SSD1306 OLED display! Look at my raspberries..\n");
    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // Initializing PICO LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0
    // run through the complete initialization process
    ssd1306_setup();
    while(1){
    /// blink both a LED and a pixel at 1Hz test
        // ssd1306_drawPixel(10,10,1);
        // ssd1306_update();
        // gpio_put(LED_PIN, 1);   // Setting pin to high
        // printf("On\n");
        // sleep_ms(1000);
        // ssd1306_drawPixel(10,10,0);
        // ssd1306_update();
        // gpio_put(LED_PIN, 0);  // Setting pin to low
        // printf("Off\n");
        // sleep_ms(1000);
    ///////////////////////////////////////////////////////
    // // Test drawString function
    // drawString(0, 0, "Hello, World!");
    // // Update OLED display
    // ssd1306_update();
    //// Print the voltage from the Potentiometer/////
        drawVoltageAndFPS();
    }
}

