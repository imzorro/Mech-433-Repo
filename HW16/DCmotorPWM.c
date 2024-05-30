#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GPIO pins for the motors
const uint MOTOR_A_PWM_PIN = 14;
const uint MOTOR_A_DIR_PIN = 15;
const uint MOTOR_B_PWM_PIN = 16;
const uint MOTOR_B_DIR_PIN = 17;

// Constants
const float Kp = 1.0;  // Proportional gain
const uint MAX_PWM_LEVEL = 65535;  // Max PWM level (100% duty cycle)

void set_motor_speed(uint pwm_pin, uint dir_pin, float speed) {
    uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
    if (speed < 0) {
        gpio_put(dir_pin, 1);  // Set direction to reverse
        speed = -speed;  // Convert speed to positive value
    } else {
        gpio_put(dir_pin, 0);  // Set direction to forward
    }
    uint16_t level = (uint16_t)(speed * MAX_PWM_LEVEL);
    pwm_set_gpio_level(pwm_pin, level);
}

int main() {
    stdio_init_all();

    // Initialize GPIO for motor direction pins
    gpio_init(MOTOR_A_DIR_PIN);
    gpio_set_dir(MOTOR_A_DIR_PIN, GPIO_OUT);
    gpio_init(MOTOR_B_DIR_PIN);
    gpio_set_dir(MOTOR_B_DIR_PIN, GPIO_OUT);

    // Initialize PWM for motor PWM pins
    gpio_set_function(MOTOR_A_PWM_PIN, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR_B_PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num_a = pwm_gpio_to_slice_num(MOTOR_A_PWM_PIN);
    uint slice_num_b = pwm_gpio_to_slice_num(MOTOR_B_PWM_PIN);

    pwm_set_wrap(slice_num_a, MAX_PWM_LEVEL);
    pwm_set_wrap(slice_num_b, MAX_PWM_LEVEL);
    pwm_set_enabled(slice_num_a, true);
    pwm_set_enabled(slice_num_b, true);

    while (1) {
        printf("Enter line position (0 to 100): ");
        int line_position;
        if (scanf("%d", &line_position) == 1) {
            // Ensure the input is within the valid range
            if (line_position < 0) line_position = 0;
            if (line_position > 100) line_position = 100;

            // Calculate motor speeds based on line position
            int error = line_position - 50;
            float motor_a_speed = 0.5f + Kp * error / 100.0f;
            float motor_b_speed = 0.5f - Kp * error / 100.0f;

            // Clamp speeds to range [0, 1]
            motor_a_speed = motor_a_speed < -1 ? -1 : (motor_a_speed > 1 ? 1 : motor_a_speed);
            motor_b_speed = motor_b_speed < -1 ? -1 : (motor_b_speed > 1 ? 1 : motor_b_speed);

            // Set motor speeds
            set_motor_speed(MOTOR_A_PWM_PIN, MOTOR_A_DIR_PIN, motor_a_speed);
            set_motor_speed(MOTOR_B_PWM_PIN, MOTOR_B_DIR_PIN, motor_b_speed);

            printf("Line Position: %d, Motor A Speed: %.2f, Motor B Speed: %.2f\n",
                   line_position, motor_a_speed, motor_b_speed);
        } else {
            // Clear the input buffer if the input is invalid
            while (getchar() != '\n');
        }
    }

    return 0;
}
