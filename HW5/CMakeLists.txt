add_executable(HW5
        I2CMCP23008.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(HW5 pico_stdlib hardware_i2c)
# enable usb output, disable uart output
pico_enable_stdio_usb(HW5 1)
pico_enable_stdio_uart(HW5 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW5)
