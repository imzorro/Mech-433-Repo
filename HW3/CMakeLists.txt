add_executable(HW3
        print.c       # Name of c file
        )

# pull in common dependencies
target_link_libraries(HW3 pico_stdlib hardware_adc)  # First name of c file + pico_stdlib
# enable usb output, disable uart output
pico_enable_stdio_usb(HW3 1)
pico_enable_stdio_uart(HW3 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(HW3)
