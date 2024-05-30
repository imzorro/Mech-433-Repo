add_executable(HW16
        DCmotorPWM.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(HW16 pico_stdlib hardware_pwm hardware_uart)
# enable usb output, disable uart output
pico_enable_stdio_usb(HW16 1)
pico_enable_stdio_uart(HW16 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW16)

