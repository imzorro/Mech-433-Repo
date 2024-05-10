/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"

/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define MAX_RX_BUFFER_SIZE 128 // Maximum size of the receive buffer

// 400 is usual, but often these can be overclocked to improve display response.
// Tested at 1000 on both 32 and 84 pixel height devices and it worked.
//#define SSD1306_I2C_CLK             400
#define SSD1306_I2C_CLK             1000

char rx_buffer[MAX_RX_BUFFER_SIZE]; // Global receive buffer
int rx_index = 0; // Index to track the position in the receive buffer

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        char received_char = uart_getc(UART_ID);
        if (received_char == '\r' || received_char == '\n') { // Check for carriage return or newline
            rx_buffer[rx_index] = 0; // Null-terminate the received string
            rx_index = 0; // Reset the buffer index
            printf("From Zero: %s \n", rx_buffer);
        }
        else{
            rx_buffer[rx_index] = received_char;
             rx_index++;
        }
    }
}

int main() {
    stdio_init_all();
    /// Initialize UART
    // Actually, we want a different speed
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);
    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

     while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    while (true){
    tight_loop_contents();
    int fromterm = 0;
    scanf("%d",&fromterm);
    printf("Computer Sent:  %d\n", fromterm);

    char text[100];
    sprintf(text,"%d\n",fromterm);
    uart_puts(UART_ID, text);

    sleep_ms(250);
    }
}
