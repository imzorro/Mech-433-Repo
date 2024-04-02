
#include "pico/stdlib.h"

// LED Pin is GPIO 25 check with opening definition of next line pico.h
uint LED_PIN = PICO_DEFAULT_LED_PIN;

int main(){
    
    gpio_init(LED_PIN);    ///Initialize GPIO Pin
    gpio_set_dir(LED_PIN, GPIO_OUT); //// Setting GPIO pin as output for LED
    while (true) {
        gpio_put(LED_PIN, 1);   // Setting pin to high
        sleep_ms(100);          //// Sleep
        gpio_put(LED_PIN, 0);  // Setting pin to low
        sleep_ms(100);          //// Sleep
    }
}
