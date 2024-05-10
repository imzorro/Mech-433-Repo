add_executable(HW9
        uart_advanced.c

        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(HW9 pico_stdlib hardware_uart hardware_i2c)
# enable usb output, disable uart output
pico_enable_stdio_usb(HW9 1)
pico_enable_stdio_uart(HW9 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(HW9)

