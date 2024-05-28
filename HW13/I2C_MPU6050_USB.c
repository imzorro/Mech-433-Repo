#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"

// device has default bus address of 0b1101000 (0x68)
#define ADDR _u(0x68)

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

// PICO LED Pin is GPIO 25
uint LED_PIN = PICO_DEFAULT_LED_PIN;

#ifdef i2c_default

// Function to read a byte from a specific register
uint8_t read_byte(uint8_t reg) {
    uint8_t buf;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, &buf, 1, false);
    return buf;
}

// Function to write a byte to a specific register
void write_byte(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

}

// Function to initialize the MPU6050
void mpu6050_init() {
    uint8_t who_am_i = read_byte(WHO_AM_I);
    printf("WHO_AM_I register: 0x%02X\n", who_am_i);
        // Wake up the MPU6050
    write_byte(PWR_MGMT_1, 0x00);
    printf("PWR_MGMT_1 written\n");
    // Set accelerometer range to ±2g
    write_byte(ACCEL_CONFIG, 0x00);
    printf("ACCEL_CONFIG written\n");
    // Set gyroscope range to ±2000°/s
    write_byte(GYRO_CONFIG, 0x18);
}

// Function to perform a burst read of 14 bytes
void read_burst_data(uint8_t start_reg, uint8_t* buffer, size_t length) {
    i2c_write_blocking(i2c_default, ADDR, &start_reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, length, false);
}

// Function to combine two bytes into a 16-bit signed integer
int16_t combine_bytes(uint8_t msb, uint8_t lsb) {
    return (int16_t)((msb << 8) | lsb);
}

// Function to read and print sensor data
void read_and_print_sensor_data() {
    uint8_t buffer[14];
    read_burst_data(ACCEL_XOUT_H, buffer, 14);

    int16_t accel_x = combine_bytes(buffer[0], buffer[1]);
    int16_t accel_y = combine_bytes(buffer[2], buffer[3]);
    int16_t accel_z = combine_bytes(buffer[4], buffer[5]);
    int16_t temp = combine_bytes(buffer[6], buffer[7]);
    int16_t gyro_x = combine_bytes(buffer[8], buffer[9]);
    int16_t gyro_y = combine_bytes(buffer[10], buffer[11]);
    int16_t gyro_z = combine_bytes(buffer[12], buffer[13]);

    float accel_x_g = accel_x * 0.000061;
    float accel_y_g = accel_y * 0.000061;
    float accel_z_g = accel_z * 0.000061;
    float temp_c = (temp / 340.0) + 36.53;
    float gyro_x_dps = gyro_x * 0.00763;
    float gyro_y_dps = gyro_y * 0.00763;
    float gyro_z_dps = gyro_z * 0.00763;

    printf("Accel: X=%.3fg, Y=%.3fg, Z=%.3fg\n", accel_x_g, accel_y_g, accel_z_g);
    printf("Temp: %.2f°C\n", temp_c);
    printf("Gyro: X=%.3f°/s, Y=%.3f°/s, Z=%.3f°/s\n", gyro_x_dps, gyro_y_dps, gyro_z_dps);
}
void read_accel_data(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z) {
    uint8_t buffer[6];

    i2c_write_blocking(i2c_default, ADDR, (uint8_t[]){ACCEL_XOUT_H}, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    *accel_x = (buffer[0] << 8) | buffer[1];
    *accel_y = (buffer[2] << 8) | buffer[3];
    *accel_z = (buffer[4] << 8) | buffer[5];
}
void send_hid_report(uint8_t report_id, int16_t delta_x, int16_t delta_y) {
    // HID mouse report structure
    uint8_t report[] = {0x01, delta_x & 0xFF, delta_x >> 8, delta_y & 0xFF, delta_y >> 8, 0, 0};
    tud_hid_n_report(0, report_id, report, sizeof(report));
}

void hid_task(void) {
    static uint32_t start_ms = 0;
    uint32_t interval_ms = 10;

    if (board_millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    int16_t accel_x, accel_y, accel_z;
    read_accel_data(&accel_x, &accel_y, &accel_z);

    int16_t delta_x = (accel_x / 8192); // Scale the accelerometer data
    int16_t delta_y = (accel_y / 8192); // Scale the accelerometer data

    send_hid_report(REPORT_ID_MOUSE, delta_x, delta_y);
}
#endif

int main() {
    stdio_init_all();
    board_init();
    tusb_init();
    sleep_ms(2000);  // Wait for the USB serial connection to establish
    printf("Initializing MPU6050...\n");

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c / mpu6050 example requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // Pico LED Pin set to GPIO 25
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize MPU6050
    mpu6050_init();

    while (1) {
        tud_task(); // tinyusb device task
        hid_task();
    }
#endif
}
