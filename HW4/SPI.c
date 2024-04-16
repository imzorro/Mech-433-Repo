
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "math.h"

/* 
   Connections on Raspberry Pi Pico board

   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO 
   GPIO 17 (pin 22) Chip select -> CSB/!CS 
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK 
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI
   3.3v (pin 36) -> VCC
   GND (pin 38)  -> GND 
*/
// Constants
#define MAX_VAL 1023    // Max voltage

#ifdef PICO_DEFAULT_SPI_CSN_PIN    /// Needed
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN) ///Needed, need to be changed based on chip used
static void write_register(unsigned char chan, u_int16_t voltage) {
    uint8_t buf[2];
    uint8_t bit = 0;
    
    // Shift 'chan' to make the second least significant bit the most significant bit of 'bit'
    bit = bit | ((chan & 0b10) << 6);
    // Append the 111 bits to  bit
    bit = bit | (0b111 << 4);
    
    // Append the first 4 bits of 'voltage' to 'bit[0]'
    bit = bit | (voltage >> 6);
    
    buf[0] = bit;
    // Append the last 8 bits of 'voltage' to 'bit[1]'
    buf[1] = (voltage << 2) & 0xFF;
    
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
}

#endif

int main() {
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else

    printf("Hello! Reading raw data from registers via SPI...\n");

    // This example will use SPI0 at 0.5MHz: 500*1000
    spi_init(spi_default, 500 * 1000);   ///  Baudrate required in Hz - 12kHz max: 20Mhz
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Initializing blank variables
    float sine_time = 0;
    float triangle_time = 0;
    uint16_t sine_wave, triangle_wave;

    while (1) {

     // Generate sine wave (2Hz)
        sine_wave = (MAX_VAL / 2) * (1 + sin(2 * M_PI * 2 * sine_time));
        sine_time += 0.01; // Increment time for next sample

        // Generate triangle wave (1Hz)
        if (triangle_time < 0.5) {
            triangle_wave = (uint16_t)((2 * triangle_time) * MAX_VAL);
        } else {
            triangle_wave = (uint16_t)((2 - 2 * triangle_time) * MAX_VAL);
        }

        triangle_time += 0.01; // Increment time for next sample

        if (triangle_time >= 1.0) {
            triangle_time = 0; // Reset time to start over
        }
        
        // Output waveforms via SPI
        
        write_register('A', sine_wave); // Output to channel A
        write_register('B', triangle_wave); // Output to channel B

        sleep_ms(10); // Adjust the delay for 50x sampling frequency
    }
#endif
}
