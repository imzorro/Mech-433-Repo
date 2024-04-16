add_executable(HW4
        SPI.c       # Name of c file
        )

# pull in common dependencies
target_link_libraries(HW4 pico_stdlib hardware_adc hardware_spi)  # First name of c file + pico_stdlib
# enable usb output, disable uart output
pico_enable_stdio_usb(HW4 1)
pico_enable_stdio_uart(HW4 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(HW4)
