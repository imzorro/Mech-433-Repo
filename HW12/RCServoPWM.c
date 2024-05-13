#include "pico/stdlib.h"
#include "hardware/pwm.h"
#define GPIO 1 // the built-in LED on the Pico

// Function to initialize PWM to 50Hz
void initialize_pwm() {
    // Get PWM slice number for GPIO
    uint slice_num = pwm_gpio_to_slice_num(GPIO);
    // Set clock divider for 1MHz base frequency
    pwm_set_clkdiv(slice_num, 125.0);
    // Set wrap value for 50Hz frequency
    pwm_set_wrap(slice_num, 20000); // 20ms period
    pwm_set_enabled(slice_num, true); // Turn on the PWM
}

// Function to convert angle in degrees to PWM duty cycle
uint16_t angle_to_duty_cycle(float angle) {
    // Convert angle to duty cycle (0.5ms to 2.5ms pulse width)
    float duty_cycle_ms = (angle / 180.0) * 2.0 + 0.5;
    // Convert duty cycle from milliseconds to PWM count
    uint16_t duty_cycle_count = (uint16_t)((duty_cycle_ms / 20.0) * 20000);
    return duty_cycle_count;
}

// Function to set the angle of the Servo
void set_servo_angle(uint16_t angle) {
    pwm_set_gpio_level(GPIO, angle_to_duty_cycle(angle));
}

int main() {
    // Initialize standard GPIO and PWM
    stdio_init_all();
    gpio_init(GPIO);
    gpio_set_function(GPIO, GPIO_FUNC_PWM);

    // Initialize PWM to 50Hz
    initialize_pwm();

    // Time taken to move from 0 to 180 and back in milliseconds
    uint32_t move_time_ms = 4000;
    // Total number of steps
    uint32_t total_steps = 180 / 5; // 5 degrees increment per step
    // Time taken for each step in milliseconds
    uint32_t step_time_ms = move_time_ms / (2 * total_steps); // Divide by 2 for forward and backward movement


    while (true) {
        // Move from 0 to 180 degrees
        for (uint16_t angle = 0; angle < 180; angle += 5) {
            set_servo_angle(angle);
            sleep_ms(step_time_ms);
        }

        // Move from 180 to 0 degrees
        for (uint16_t angle = 180; angle > 0; angle -= 5) {
            set_servo_angle(angle);
            sleep_ms(step_time_ms);
        }
    }

    return 0;
}
