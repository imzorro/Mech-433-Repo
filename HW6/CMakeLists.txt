add_executable(HW6
        ssd1306_i2c.c
        ssd1306fuctions.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(HW6 pico_stdlib hardware_i2c hardware_adc)
# enable usb output, disable uart output
pico_enable_stdio_usb(HW6 1)
pico_enable_stdio_uart(HW6 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW6)

