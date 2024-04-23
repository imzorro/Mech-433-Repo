
#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

 // device has default bus address of 0x80 0100 0000
#define ADDR _u(0x20)

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)
// PICO LED Pin is GPIO 25
uint LED_PIN = PICO_DEFAULT_LED_PIN;

#ifdef i2c_default
void chip_init() {
    uint8_t buf[2];
    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111; //// Setting GPIO7 as an output for LED
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}
void setPin (unsigned char address, unsigned char registers, unsigned char value) {
    uint8_t buf[2];
    // send register number followed by its corresponding value
    buf[0] = registers;
    buf[1] = value<<7; // Only GPIO7
    i2c_write_blocking(i2c_default, address, buf, 2, false);
}

unsigned char readPin(unsigned char address, unsigned char registers) {
    uint8_t buf[1];
    uint8_t reg = registers; /// Setting Read register for GPIO
    i2c_write_blocking(i2c_default, address, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, address, buf, 1, false);  // false - finished with bus
    if ((buf[0] & 0b1) == 1) {
    return 1;
    }
    else {
        return 0;
    }
}

#endif
int main() {
    stdio_init_all();
    printf("Hello, now connecting to MCP23008...\n");
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c / bmp280_i2c example requires a board with I2C pins
        puts("Default I2C pins were not defined");
    return 0;
#else
    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // configure MCP23008
    chip_init();
    // Pico LED Pin set to GPIO 25
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    sleep_ms(250); // sleep so that data polling and register update don't collide
    
    while (1) {
    //Blink PICO LED
    gpio_put(LED_PIN, 1);
    sleep_ms(200);
    gpio_put(LED_PIN, 0);
    sleep_ms(200);
    /// if statement for when button is pressed it turns on LED
    int status= readPin(ADDR, REG_GPIO); /// Read GPIO 0
        if (status==1) {
            setPin (ADDR, REG_OLAT, 1); // Button pressed! Turning on LED
            printf("Buttom is being Pressed %d\n", status);
        }
         else {
           setPin (ADDR, REG_OLAT, 0); //Button released! Turning off LED
           printf("Buttom is being Released  %d\n", status);
        }
        sleep_ms(500);
    }
#endif
}
